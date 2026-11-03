/*******************************************************
  * Name    :OutputService_Ecall.c
  * Author  :xiaojunzhong
  * Function:逻辑输出到物理引脚的映射，及可确认的乘员提示接口。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#include "OutputService_Ecall.h"
#include "FreeRTOS.h"
#include <string.h>
typedef struct
{
    OutputServiceEcallRequest_t stRequest; /* 已经复制到本模块的完整业务请求。 */
    uint32_t u32NextRequestId; /* 下一提示请求编号，跳过零。 */
    uint8_t u8HasRequest; /* 是否已有可查询提示请求。 */
} OutputServiceEcallRuntime_t;
/* AppTask 单写请求，未来显示任务写交付；共享数据均经短临界区访问。 */
static OutputServiceEcallRuntime_t s_oEcRun; /* 提示请求存储和编号过程。 */
static OutputServiceEcallStatus_t s_oEcSts; /* 实际显示通道及交付状态。 */
/*******************************************************
  * Name    :OutputService_InitEcallIndication
  * Author  :xiaojunzhong
  * Function:清零请求和交付状态，默认实际通道不可用。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void OutputService_InitEcallIndication(void)
{
    (void)memset(&s_oEcRun, 0, sizeof(s_oEcRun));
    (void)memset(&s_oEcSts, 0, sizeof(s_oEcSts));
}
/*******************************************************
  * Name    :OutputService_PublishEcallIndication
  * Author  :xiaojunzhong
  * Function:原子保存最新提示并分配请求号，不执行硬件输出。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t OutputService_PublishEcallIndication(const OutputServiceEcallRequest_t *pstRequest, uint32_t *pu32RequestId)
{
    if ((pstRequest == NULL) || (pu32RequestId == NULL)
        || ((uint32_t)pstRequest->enDiagnosticMessage > OUTPUT_ECALL_MESSAGE_FAULT))
    {
        return 0U;
    }
    portENTER_CRITICAL();
    s_oEcRun.u32NextRequestId++;
    if (s_oEcRun.u32NextRequestId == 0U)
    {
        s_oEcRun.u32NextRequestId = 1U;
    }
    s_oEcRun.stRequest = *pstRequest;
    s_oEcRun.stRequest.u32RequestId = s_oEcRun.u32NextRequestId;
    s_oEcRun.u8HasRequest = 1U;
    s_oEcSts.u32RequestId = s_oEcRun.u32NextRequestId;
    s_oEcSts.enDelivery = (s_oEcSts.u8ChannelAvailable != 0U) ? OUTPUT_ECALL_PENDING : OUTPUT_ECALL_UNAVAILABLE;
    *pu32RequestId = s_oEcSts.u32RequestId;
    portEXIT_CRITICAL();
    return 1U;
}
/*******************************************************
  * Name    :OutputService_ReadEcallIndication
  * Author  :xiaojunzhong
  * Function:复制最新提示意图，读取不会清除持久故障请求。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t OutputService_ReadEcallIndication(OutputServiceEcallRequest_t *pstRequest)
{
    uint8_t u8Ready; /* 是否有可供消费的新结果。 */
    if (pstRequest == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstRequest = s_oEcRun.stRequest;
    u8Ready = s_oEcRun.u8HasRequest;
    portEXIT_CRITICAL();
    return u8Ready;
}
/*******************************************************
  * Name    :OutputService_ReadEcallIndicationStatus
  * Author  :xiaojunzhong
  * Function:复制实际通道和交付状态，供提示仲裁查询。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t OutputService_ReadEcallIndicationStatus(OutputServiceEcallStatus_t *pstStatus)
{
    if (pstStatus == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstStatus = s_oEcSts;
    portEXIT_CRITICAL();
    return 1U;
}
/*******************************************************
  * Name    :OutputService_SetEcallIndicationAvailable
  * Author  :xiaojunzhong
  * Function:由实际输出服务报告能力变化，重新可用后仍需输出确认。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void OutputService_SetEcallIndicationAvailable(uint8_t u8Available)
{
    uint8_t u8Next = (u8Available != 0U) ? 1U : 0U; /* 归一化后的新逻辑值。 */
    portENTER_CRITICAL();
    if (s_oEcSts.u8ChannelAvailable != u8Next)
    {
        s_oEcSts.u8ChannelAvailable = u8Next;
        if (u8Next == 0U)
        {
            s_oEcSts.enDelivery = OUTPUT_ECALL_UNAVAILABLE;
        }
        else if (s_oEcRun.u8HasRequest != 0U)
        {
            /* 通道生命周期变化后立即失效旧确认，不等待 APP 下一次调度。 */
            s_oEcRun.u32NextRequestId++;
            if (s_oEcRun.u32NextRequestId == 0U)
            {
                s_oEcRun.u32NextRequestId = 1U;
            }
            s_oEcRun.stRequest.u32RequestId = s_oEcRun.u32NextRequestId;
            s_oEcSts.u32RequestId = s_oEcRun.u32NextRequestId;
            s_oEcSts.enDelivery = OUTPUT_ECALL_PENDING;
        }
    }
    portEXIT_CRITICAL();
}
/*******************************************************
  * Name    :OutputService_ReportEcallIndication
  * Author  :xiaojunzhong
  * Function:按当前请求号记录实际输出成败，拒绝迟到旧确认。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t OutputService_ReportEcallIndication(uint32_t u32RequestId, uint8_t u8Success)
{
    uint8_t u8Accepted = 0U; /* 接口是否接受本次请求，不能等同硬件执行成功。 */
    portENTER_CRITICAL();
    if ((s_oEcSts.u8ChannelAvailable != 0U) && (s_oEcRun.u8HasRequest != 0U)
        && (u32RequestId != 0U) && (u32RequestId == s_oEcSts.u32RequestId))
    {
        s_oEcSts.enDelivery = (u8Success != 0U) ? OUTPUT_ECALL_DELIVERED : OUTPUT_ECALL_FAILED;
        u8Accepted = 1U;
    }
    portEXIT_CRITICAL();
    return u8Accepted;
}
