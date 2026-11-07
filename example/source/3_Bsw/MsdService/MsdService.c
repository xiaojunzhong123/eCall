/*******************************************************
  * Name    :MsdService.c
  * Author  :xiaojunzhong
  * Function:BSW层MSD串口组帧与CRC；不编码标准MSD，不管理呼叫或睡眠。
  * Version :V1.0.0
  * Data    :2026.11.06
*******************************************************/
#include "MsdService.h"
#include "MsdService_Cfg.h"
#include "Uart.h"
#include <string.h>

#define MSD_SERVICE_PAYLOAD_LENGTH    48U
#define MSD_SERVICE_HEADER_LENGTH     8U
#define MSD_SERVICE_FRAME_LENGTH      58U
#define MSD_SERVICE_TRIGGER_ALL       0x07U
#define MSD_SERVICE_TRIGGER_AUTO      0x06U

/* 初始化归启动上下文，运行期仅由EcallTask读写，不在ISR或其他任务调用。
 * UART发送为同步忙等，缓存调用后可复用；未来改异步须重新定义缓存所有权。 */
typedef struct
{
    uint32_t u32SessionId; /* 当前发送所关联的完整事件号，零表示尚无发送。 */
    uint16_t u16NextSeq;   /* 本次启动递增序号，一至六万五千五百三十五，回绕跳过零。 */
    uint16_t u16ActiveSeq; /* 当前发送的序号，供后续ACK解析关联事件；不是事件号。 */
    uint8_t au8Frame[MSD_SERVICE_FRAME_LENGTH]; /* 五十八字节小端线缆帧，静态生命周期。 */
} MsdServiceRuntime_t;
static MsdServiceRuntime_t s_msdSvcRun; /* EcallTask独占，无堆分配或跨任务写者。 */

/*******************************************************
  * Name    :MsdService_WriteU16
  * Author  :xiaojunzhong
  * Function:按低字节在前编码十六位值，屏蔽结构体对齐。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :目标至少两字节及待写数值。
  * Output  :无返回值。
  * Caller  :服务私有组帧路径。
  * Precond :目标地址由固定帧偏移保证有效。
  * Failure :不接受外部缓冲区，不单独检查长度。
  * Effect  :写入两个目标字节。
*******************************************************/
static void MsdService_WriteU16(uint8_t *pu8Target, uint16_t u16Value)
{
    pu8Target[0] = (uint8_t)u16Value;
    pu8Target[1] = (uint8_t)(u16Value >> 8U);
}

/*******************************************************
  * Name    :MsdService_WriteU32
  * Author  :xiaojunzhong
  * Function:按小端编码位置占位值，不将占位解释为有效位置。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :目标至少四字节及待写数值。
  * Output  :无返回值。
  * Caller  :服务私有组帧路径。
  * Precond :目标为帧内固定四字节字段。
  * Failure :不接受外部缓冲区，不单独检查长度。
  * Effect  :写入四个目标字节。
*******************************************************/
static void MsdService_WriteU32(uint8_t *pu8Target, uint32_t u32Value)
{
    pu8Target[0] = (uint8_t)u32Value;
    pu8Target[1] = (uint8_t)(u32Value >> 8U);
    pu8Target[2] = (uint8_t)(u32Value >> 16U);
    pu8Target[3] = (uint8_t)(u32Value >> 24U);
}

/*******************************************************
  * Name    :MsdService_CalculateCrc16
  * Author  :xiaojunzhong
  * Function:计算CRC-16/MODBUS；初值FFFF，右移多项式A001，最终异或零。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :指向负载字节及字节数，不含帧头和帧尾。
  * Output  :返回十六位CRC值，发送时另按小端编码。
  * Caller  :服务组帧路径，后续可仅替换此算法。
  * Precond :内部调用保证非空输入且长度为四十八。
  * Failure :算法不提供加密或身份认证。
  * Effect  :无全局状态修改，无硬件访问。
*******************************************************/
static uint16_t MsdService_CalculateCrc16(const uint8_t *pu8Payload, uint8_t u8Length)
{
    uint16_t u16Crc = 0xFFFFU; /* 当前累计CRC，初始化后逐字节及逐位更新。 */
    uint8_t u8ByteIndex; /* 本次输入字节索引，范围零至长度减一。 */
    uint8_t u8BitIndex;  /* 当前字节的八个移位步骤。 */

    for (u8ByteIndex = 0U; u8ByteIndex < u8Length; u8ByteIndex++)
    {
        u16Crc ^= pu8Payload[u8ByteIndex];
        for (u8BitIndex = 0U; u8BitIndex < 8U; u8BitIndex++)
        {
            if ((u16Crc & 1U) != 0U)
            {
                u16Crc = (uint16_t)((u16Crc >> 1U) ^ 0xA001U);
            }
            else
            {
                u16Crc >>= 1U;
            }
        }
    }
    return u16Crc;
}

/*******************************************************
  * Name    :MsdService_BuildPayload
  * Author  :xiaojunzhong
  * Function:将首次冻结车辆数据编码为四十八字节负载，定位等按约定占位。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :冻结快照指针及合法来源掩码。
  * Output  :无返回值，负载写入静态帧。
  * Caller  :EcallTask中的服务发送路径。
  * Precond :输入已检查；帧已预填FF，无效车辆来源保留FF。
  * Failure :固定FFFF掩码不证明字段真实有效；定位和翻滚占位由模组解释。
  * Effect  :只读快照，写静态帧，不刷新数据或产生实测值。
*******************************************************/
static void MsdService_BuildPayload(const RteSysVehicleSnapshot_t *pstVehicle, uint8_t u8TriggerMask)
{
    uint8_t *pu8Payload = &s_msdSvcRun.au8Frame[MSD_SERVICE_HEADER_LENGTH]; /* 固定四十八字节负载起点。 */

    MsdService_WriteU16(&pu8Payload[0], MSD_SERVICE_VALID_MASK);
    pu8Payload[2] = MSD_SERVICE_MSD_VERSION;
    /* 自动和CAN来源优先；位置可信位为零，交由模组按实际定位补齐。 */
    pu8Payload[3] = ((u8TriggerMask & MSD_SERVICE_TRIGGER_AUTO) != 0U) ? 1U : 0U;
    pu8Payload[3] |= (uint8_t)(MSD_SERVICE_TEST_CALL << 1U);
    if (pstVehicle->stVehicle.u8VehicleTypeValid != 0U)
    {
        pu8Payload[4] = pstVehicle->stVehicle.u8VehicleType;
    }
    if (pstVehicle->stVehicle.u8VinValid != 0U)
    {
        (void)memcpy(&pu8Payload[5], pstVehicle->stVehicle.au8Vin, RTESYS_MSD_VIN_LENGTH);
    }
    if (pstVehicle->u8PropulsionValid != 0U)
    {
        pu8Payload[22] = pstVehicle->u8PropulsionMask;
    }
    MsdService_WriteU32(&pu8Payload[23], MSD_SERVICE_POSITION_PLACEHOLDER);
    MsdService_WriteU32(&pu8Payload[27], MSD_SERVICE_POSITION_PLACEHOLDER);
    pu8Payload[31] = MSD_SERVICE_DIRECTION_PLACEHOLDER;
    MsdService_WriteU16(&pu8Payload[32], MSD_SERVICE_DELTA_PLACEHOLDER);
    MsdService_WriteU16(&pu8Payload[34], MSD_SERVICE_DELTA_PLACEHOLDER);
    MsdService_WriteU16(&pu8Payload[36], MSD_SERVICE_DELTA_PLACEHOLDER);
    MsdService_WriteU16(&pu8Payload[38], MSD_SERVICE_DELTA_PLACEHOLDER);
    /* 人数来自有效载重估算；无来源保留FF，不能填默认一人。 */
    if (pstVehicle->stLoad.u8OccupantsValid != 0U)
    {
        pu8Payload[40] = pstVehicle->stLoad.u8Occupants;
    }
    pu8Payload[41] = MSD_SERVICE_IMPACT_PLACEHOLDER;
    pu8Payload[42] = MSD_SERVICE_ROLLOVER_PLACEHOLDER;
    pu8Payload[43] = MSD_SERVICE_RANGE_PLACEHOLDER;
    MsdService_WriteU16(&pu8Payload[44], MSD_SERVICE_DELTA_V_PLACEHOLDER);
    MsdService_WriteU16(&pu8Payload[46], MSD_SERVICE_DELTA_V_PLACEHOLDER);
}

/*******************************************************
  * Name    :MsdService_Init
  * Author  :xiaojunzhong
  * Function:清空服务运行态并设置非零的下一帧序号。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :无。
  * Output  :无返回值。
  * Caller  :Ecall_Init在创建任务前调用一次。
  * Precond :UART由既有启动流程初始化，运行期禁止重复初始化。
  * Failure :不检查模组状态，不表示串口链路已经可用。
  * Effect  :清空静态缓存及事务关联，不操作硬件或堆。
*******************************************************/
void MsdService_Init(void)
{
    (void)memset(&s_msdSvcRun, 0, sizeof(s_msdSvcRun));
    s_msdSvcRun.u16NextSeq = 1U;
}

/*******************************************************
  * Name    :MsdService_Send
  * Author  :xiaojunzhong
  * Function:生成五十八字节小端帧并复用既有UART同步发送一次。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :冻结车辆快照、来源位及非零完整事件号。
  * Output  :一仅表示UART接口返回TRUE，零为输入或发送失败。
  * Caller  :EcallTask独占，禁止ISR或多任务并行调用。
  * Precond :服务和UART已初始化；输入在本次同步调用期间稳定。
  * Failure :无效输入不发送；底层忙等无超时，不保证模组收到。
  * Effect  :分配序号并保存事件映射，临界区外发送，不释放电源锁。
*******************************************************/
uint8_t MsdService_Send(const RteSysVehicleSnapshot_t *pstVehicle, uint8_t u8TriggerMask, uint32_t u32SessionId)
{
    uint16_t u16Crc = 0;

    if ((pstVehicle == NULL) || (u32SessionId == 0U) || (u8TriggerMask == 0U)
        || ((u8TriggerMask & (uint8_t)(~MSD_SERVICE_TRIGGER_ALL)) != 0U))
    {
        return 0U;
    }
    s_msdSvcRun.u32SessionId = u32SessionId;
    s_msdSvcRun.u16ActiveSeq = s_msdSvcRun.u16NextSeq;
    s_msdSvcRun.u16NextSeq++;
    if (s_msdSvcRun.u16NextSeq == 0U)
    {
        s_msdSvcRun.u16NextSeq = 1U;
    }
    (void)memset(s_msdSvcRun.au8Frame, 0xFF, sizeof(s_msdSvcRun.au8Frame));
    s_msdSvcRun.au8Frame[0] = 0xAAU;
    s_msdSvcRun.au8Frame[1] = 0x55U;
    s_msdSvcRun.au8Frame[2] = 0x01U;
    s_msdSvcRun.au8Frame[3] = 0x01U;
    MsdService_WriteU16(&s_msdSvcRun.au8Frame[4], s_msdSvcRun.u16ActiveSeq);
    MsdService_WriteU16(&s_msdSvcRun.au8Frame[6], MSD_SERVICE_PAYLOAD_LENGTH);
    MsdService_BuildPayload(pstVehicle, u8TriggerMask);
    u16Crc = MsdService_CalculateCrc16(&s_msdSvcRun.au8Frame[MSD_SERVICE_HEADER_LENGTH], MSD_SERVICE_PAYLOAD_LENGTH);
    MsdService_WriteU16(&s_msdSvcRun.au8Frame[56], u16Crc);
    return (Uart_TxData(s_msdSvcRun.au8Frame, MSD_SERVICE_FRAME_LENGTH) == TRUE) ? 1U : 0U;
}

/*******************************************************
  * Name    :MsdService_ReadAck
  * Author  :xiaojunzhong
  * Function:ACK读取预留接口，未接入前始终没有交付结果。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :调用方结果缓冲区，可为空。
  * Output  :当前始终返回零且不修改输出。
  * Caller  :EcallTask在DATA_PENDING阶段每周期查询。
  * Precond :后续仅由本服务消费UART接收字节并关联当前序号和完整事件号。
  * Failure :未实现解析、校验、重试、超时和跨重启去重，不能伪造成功。
  * Effect  :当前不消费串口，不清邮箱，不修改事件锁。
*******************************************************/
uint8_t MsdService_ReadAck(MsdServiceAck_t *pstAck)
{
    /* 后续在此补充非阻塞拆帧和ACK校验；忙或待重试不返回最终拒绝。
     * 匹配u16ActiveSeq后映射u32SessionId，迟到旧帧不得完成新事件。 */
    (void)pstAck;
    return 0U;
}
