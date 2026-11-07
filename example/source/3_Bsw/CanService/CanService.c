/*******************************************************
  * Name    :CanService.c
  * Author  :xiaojunzhong
  * Function:CAN碰撞接收、车辆数据发布及总线错误恢复。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
/* 流程阅读说明：
 * 执行路径：CAN ISR -> 驱动环形队列 -> CanTask -> ProcessRx -> 矩阵三报文回调 -> RTE。
 * 碰撞位上升沿锁存后由 CanTask 转交 EcallTask；ISR 不执行应用状态机。
 * 当前只路由已接入的三项数据，其他诊断帧路由和完整性校验仍待独立完善。
 * 已删除 MCU 通话状态计时及旧 0x21C 周期发送，不编造模组呼叫状态。
 */

#include "CanService.h"
#include "Can.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Hmi.h"
#include "CanMatrix.h"
#include "RteSys.h"
#include <string.h>

#define CAN_SERVICE_SRS_INFO_ID                0x10CU
#define CAN_SERVICE_FRAME_LENGTH              8U
#define CAN_SERVICE_BUS_OFF_RECOVERY_MS       1000U
#define CAN_SERVICE_TX_ERROR_REARM_MS          1000U

typedef struct
{
    uint32_t u32BusOffDetectedMs; /* 本次总线关闭故障的检测时刻，毫秒。 */
    uint8_t u8CrashArmed; /* 收到零后置一，碰撞上升沿锁存后清零。 */
    uint8_t u8TriggerPending; /* 尚未由 CanTask 取走的碰撞触发位。 */
    uint8_t u8BusOffRecoveryPending; /* 是否等待任务上下文恢复 CAN。 */
} CanServiceRuntime_t;

typedef struct
{
    uint32_t u32LastRxOverflowCount; /* 上次已经报告的接收溢出计数。 */
    uint32_t u32LastTxErrorCount; /* 上次已经报告的发送错误计数。 */
    uint32_t u32LastTxErrorMs; /* 最近一次发送错误日志时刻，毫秒。 */
    uint8_t u8TxErrorReported; /* 发送错误日志的抑制标志。 */
} CanServiceStatus_t;


static CanServiceRuntime_t s_mCanSvcRun; /* CanTask 拥有的接收和恢复过程。 */
static CanServiceStatus_t s_mCanSvcSts; /* 总线错误及发送统计结论。 */

/*******************************************************
  * Name    :CanService_IsSrsInfoFrame
  * Author  :xiaojunzhong
  * Function:确认 0x10C 为标准经典 CAN 八字节帧，拒绝格式不符的碰撞输入。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static uint8_t CanService_IsSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    if ((pstFrame == NULL)
        || (pstFrame->u32Id != CAN_SERVICE_SRS_INFO_ID)
        || (pstFrame->u8Ide != 0U)
        || (pstFrame->u8Fdf != 0U)
        || (pstFrame->u8Brs != 0U)
        || (pstFrame->u8Length != CAN_SERVICE_FRAME_LENGTH))
    {
        return 0U;
    }
    return 1U;
}

/*******************************************************
  * Name    :CanService_HandleSrsInfoFrame
  * Author  :xiaojunzhong
  * Function:校验并发布完整SRS观测，再按碰撞上升沿锁存一次紧急触发。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :CanTask中的CanService_ProcessRx，禁止ISR直接调用。
  * Input   :完整接收帧指针；必须为标准经典CAN八字节且BRS关闭。
  * Output  :无返回值；合法帧发布一次SRS观测，合格上升沿锁存触发。
  * Precond :共享RTE及CAN服务已初始化，CanTask独占本接收路径。
  * Failure :空指针或帧格式错误不更新RTE、不触发；CRC尚未验证。
  * Effect  :先更新采样时刻、气囊和校验字节观测，再处理碰撞锁存。
*******************************************************/
static void CanService_HandleSrsInfoFrame(const CanRxFrame_t *pstFrame)
{
    CanServiceSrsInfoFrame_t stSrsInfo; /* 八字节碰撞报文的本地解析视图。 */
    RteSysMsdSrs_t stSignal; /* 本帧完整SRS观测；先发布到RTE，再锁存触发，防止首次快照缺数据。 */
    uint8_t u8Crash; /* 碰撞观测位，置位本身不表示传感器故障。 */
    uint8_t u8TriggerLatched = 0U; /* 本帧是否刚锁存一次碰撞，用于一次性日志。 */

    if (CanService_IsSrsInfoFrame(pstFrame) == 0U)
    {
        HMI_LOG_CAN("RX_10C_FORMAT_REJECTED\r\n");
        return;
    }

    (void)memcpy(stSrsInfo.U, pstFrame->au8Data, CAN_SERVICE_FRAME_LENGTH);
    u8Crash = (stSrsInfo.B.u8CrashOutputStatus != 0U) ? 1U : 0U;
    /* 0x10C只由CanTask消费：气囊警告为字节零高两位，计数器为字节六低四位。
     * 保留原矩阵字段编码，尚未验证CRC和计数连续性，不能将完整性标志置一。
     */
    (void)memset(&stSignal, 0, sizeof(stSignal));
    stSignal.u32UpdateMs = osKernelSysTick();
    stSignal.u8Received = 1U;
    stSignal.u8Crash = u8Crash;
    stSignal.u8AirbagWarning = (uint8_t)((pstFrame->au8Data[0] >> 6U) & 0x03U);
    stSignal.u8Counter = (uint8_t)(pstFrame->au8Data[6] & 0x0FU);
    stSignal.u8Checksum = pstFrame->au8Data[7];
    RteSys_UpdateMsdSrs(&stSignal);
    portENTER_CRITICAL();
    /* SRS_Info defines only the crash state; MSD impact fields remain unknown. */

    if ((u8Crash != 0U) && (s_mCanSvcRun.u8CrashArmed != 0U))
    {
        s_mCanSvcRun.u8CrashArmed = 0U;
        s_mCanSvcRun.u8TriggerPending = 1U;
        u8TriggerLatched = 1U;
    }
    else if (u8Crash == 0U)
    {
        s_mCanSvcRun.u8CrashArmed = 1U;
    }
    portEXIT_CRITICAL();

    if (u8TriggerLatched != 0U)
    {
        HMI_LOG_CAN("RX_10C_ECALL_TRIGGER\r\n");
    }
}

/*******************************************************
  * Name    :CanService_ProcessRx
  * Author  :xiaojunzhong
  * Function:唯一消费硬件接收队列，SRS由本服务发布，其他两项继续经矩阵回调。
  * Version :V1.2.0
  * Data    :2026.11.07
  * Caller  :CanTask周期调度，运行期独占接收缓存。
  * Input   :无；从驱动环形队列复制完整接收帧。
  * Output  :无返回值；合法观测发布到RTE，碰撞意图留待CanTask取走。
  * Precond :RTE、矩阵及CAN服务初始化完成，硬件队列由本任务唯一消费。
  * Failure :扩展帧、非法经典CAN的BRS或不匹配帧不进入业务发布。
  * Effect  :每帧先发布数据再锁存事件；持续碰撞不重复触发，不新增队列或任务。
*******************************************************/
static void CanService_ProcessRx(void)
{
    CanRxFrame_t stFrame; /* 从 ISR 环形队列复制出来的完整帧。 */

    while (Can_TakeRxFrame(&stFrame) != 0U)
    {
        if ((stFrame.u8Ide != 0U) || ((stFrame.u8Fdf == 0U) && (stFrame.u8Brs != 0U)))
        {
            continue;
        }
        if (CanMatrix_ProcessMsdFrame(stFrame.u32Id, stFrame.u8Fdf, stFrame.au8Data, stFrame.u8Length) != 0U)
        {
            if (stFrame.u32Id == CAN_SERVICE_SRS_INFO_ID)
            {
                /* 矩阵仅匹配并缓存SRS帧；本服务负责唯一一次RTE发布及上升沿锁存。 */
                CanService_HandleSrsInfoFrame(&stFrame);
            }
        }
    }
}

/*******************************************************
  * Name    :CanService_ProcessErrors
  * Author  :xiaojunzhong
  * Function:在任务上下文记录溢出/发送错误，执行既有 Bus-Off 延迟恢复。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
static void CanService_ProcessErrors(void)
{
    uint32_t u32NowMs; /* 当前单调毫秒计数，用无符号差值处理回绕。 */
    uint32_t u32RxOverflowCount; /* 底层累计接收溢出数量。 */
    uint32_t u32TxErrorCount; /* 底层累计发送错误数量。 */

    u32NowMs = osKernelSysTick();
    u32RxOverflowCount = Can_GetRxOverflowCount();
    u32TxErrorCount = Can_GetTxErrorCount();

    if (u32RxOverflowCount != s_mCanSvcSts.u32LastRxOverflowCount)
    {
        s_mCanSvcSts.u32LastRxOverflowCount = u32RxOverflowCount;
        HMI_LOG_CAN("RX_OVERFLOW\r\n");
    }
    if (u32TxErrorCount != s_mCanSvcSts.u32LastTxErrorCount)
    {
        s_mCanSvcSts.u32LastTxErrorCount = u32TxErrorCount;
        s_mCanSvcSts.u32LastTxErrorMs = u32NowMs;
        if (s_mCanSvcSts.u8TxErrorReported == 0U)
        {
            s_mCanSvcSts.u8TxErrorReported = 1U;
            HMI_LOG_CAN("TX_ERROR\r\n");
        }
    }
    else if ((s_mCanSvcSts.u8TxErrorReported != 0U)
             && ((u32NowMs - s_mCanSvcSts.u32LastTxErrorMs) >= CAN_SERVICE_TX_ERROR_REARM_MS))
    {
        s_mCanSvcSts.u8TxErrorReported = 0U;
    }

    if ((Can_GetBusOffFlag() != 0U) && (s_mCanSvcRun.u8BusOffRecoveryPending == 0U))
    {
        s_mCanSvcRun.u8BusOffRecoveryPending = 1U;
        s_mCanSvcRun.u32BusOffDetectedMs = u32NowMs;
        HMI_LOG_CAN("BUS_OFF\r\n");
    }
    if ((s_mCanSvcRun.u8BusOffRecoveryPending != 0U)
        && ((u32NowMs - s_mCanSvcRun.u32BusOffDetectedMs) >= CAN_SERVICE_BUS_OFF_RECOVERY_MS))
    {
        Net_Can_DeInit();
        Can_Init();
        s_mCanSvcRun.u8BusOffRecoveryPending = 0U;
        s_mCanSvcSts.u32LastRxOverflowCount = 0U;
        s_mCanSvcSts.u32LastTxErrorCount = 0U;
        s_mCanSvcSts.u8TxErrorReported = 0U;
        HMI_LOG_CAN("BUS_OFF_RECOVERY\r\n");
    }
}

/*******************************************************
  * Name    :CanService_Init
  * Author  :xiaojunzhong
  * Function:初始化 CAN 服务私有缓存和碰撞重新使能标志，启动已有 CAN2 配置。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Init(void)
{
    (void)memset(&s_mCanSvcRun, 0, sizeof(s_mCanSvcRun));
    (void)memset(&s_mCanSvcSts, 0, sizeof(s_mCanSvcSts));
    s_mCanSvcRun.u8CrashArmed = 1U;
    Can_Init();
    HMI_LOG_CAN("CAN2_FD_500K_2M_INIT\r\n");
}

/*******************************************************
  * Name    :CanService_Process
  * Author  :xiaojunzhong
  * Function:按接收和错误恢复顺序处理，不采集或发送模组定位时间。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void CanService_Process(void)
{
    CanService_ProcessRx();
    CanService_ProcessErrors();
}

/*******************************************************
  * Name    :CanService_TakeEcallTrigger
  * Author  :xiaojunzhong
  * Function:原子取走一次碰撞锁存位；由 CanTask 转交 EcallTask。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
uint8_t CanService_TakeEcallTrigger(void)
{
    uint8_t u8Trigger; /* 本周期取走的一次碰撞意图。 */

    portENTER_CRITICAL();
    u8Trigger = s_mCanSvcRun.u8TriggerPending;
    s_mCanSvcRun.u8TriggerPending = 0U;
    portEXIT_CRITICAL();
    return u8Trigger;
}
