/*******************************************************
  * Name    :EcallDiagService.c
  * Author  :xiaojunzhong
  * Function:只适配已存在的模组响应及 CAN/GNSS 数据，保留未覆盖检查。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#include "EcallDiagService.h"
#include "RteSys_Diag.h"
#include "RteSys.h"
#include "cmsis_os.h"
#include <string.h>
typedef struct
{
    uint32_t u32LastSrsMs; /* 已消费的最近 SRS 来源时刻，毫秒。 */
    uint32_t u32LastNavigationMs; /* 已消费的最近定位来源时刻，毫秒。 */
    uint8_t u8SrsSeen; /* 是否已观察过真实 SRS 样本。 */
    uint8_t u8NavigationSeen; /* 是否已观察过有效导航样本。 */
    RteSysDiagToken_t astTokens[RTESYS_DIAG_COUNT]; /* 检查开始前捕获的固定来源令牌。 */
} EcallDiagServiceRuntime_t;
static EcallDiagServiceRuntime_t s_dSvcRun; /* AppTask 的诊断来源适配过程。 */
/*******************************************************
  * Name    :EcallDiagService_Publish
  * Author  :xiaojunzhong
  * Function:使用预先捕获的来源令牌发布局部证据，不伪造完整覆盖。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void EcallDiagService_Publish(RteSysDiagItem_t enItem, RteSysDiagState_t enState, uint16_t u16Reason, uint32_t u32ObservedMs)
{
    RteSysDiagObservation_t stObservation; /* 某项诊断的来源观测副本。 */
    (void)memset(&stObservation, 0, sizeof(stObservation));
    stObservation.stToken = s_dSvcRun.astTokens[enItem];
    stObservation.enSource = RteSys_DiagOwner(enItem);
    stObservation.enState = enState;
    stObservation.u16Reason = u16Reason;
    stObservation.u32ObservedMs = u32ObservedMs;
    if (u16Reason == RTE_DIAG_REASON_MODEM_UNAVAILABLE)
    {
        /* 共同来源标识仅表示模组服务不可用，不推断 SIM 或天线已损坏。 */
        stObservation.u16CommonCause = 1U;
    }
    (void)RteSys_DiagPublish(enItem, &stObservation);
}
/*******************************************************
  * Name    :EcallDiagService_Init
  * Author  :xiaojunzhong
  * Function:为十类检查显式发布能力缺口，避免初始化默认通过。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void EcallDiagService_Init(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    (void)memset(&s_dSvcRun, 0, sizeof(s_dSvcRun));
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        (void)RteSys_DiagReadToken((RteSysDiagItem_t)u8Index, &s_dSvcRun.astTokens[u8Index]);
        EcallDiagService_Publish((RteSysDiagItem_t)u8Index, RTE_DIAG_UNSUPPORTED, RTE_DIAG_REASON_CAPABILITY, osKernelSysTick());
    }
}
/*******************************************************
  * Name    :EcallDiagService_Process
  * Author  :xiaojunzhong
  * Function:只在来源事实变化时发布，重复读取不刷新样本年龄。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void EcallDiagService_Process(void)
{
    RteSysMsdSnapshot_t stSnapshot; /* 仅复用本机 CAN 和导航观测。 */
    uint8_t u8Read; /* 本次读取是否取得有效副本。 */
    /* 模组自检结果须由自定义协议回传；没有结果时保持能力缺口。
     * 不再通过 AT 型号/版本响应推断模组、SIM 或天线健康。 */
    u8Read = RteSys_ReadMsdSnapshot(&stSnapshot);
    if (u8Read == 0U)
    {
        return;
    }
    if ((stSnapshot.stSrs.u8Received != 0U)
        && ((s_dSvcRun.u8SrsSeen == 0U) || (stSnapshot.stSrs.u32UpdateMs != s_dSvcRun.u32LastSrsMs)))
    {
        s_dSvcRun.u8SrsSeen = 1U;
        s_dSvcRun.u32LastSrsMs = stSnapshot.stSrs.u32UpdateMs;
        /* 当前缺完整计数器/校验策略，收到碰撞位或零值都不能证明 ECU 健康。 */
        EcallDiagService_Publish(RTE_DIAG_CRASH_ECU, RTE_DIAG_INDETERMINATE, RTE_DIAG_REASON_PARTIAL_COVERAGE, stSnapshot.stSrs.u32UpdateMs);
    }
    if ((stSnapshot.stNavigation.u8PositionValid != 0U)
        && ((s_dSvcRun.u8NavigationSeen == 0U) || (stSnapshot.stNavigation.u32UpdateMs != s_dSvcRun.u32LastNavigationMs)))
    {
        s_dSvcRun.u8NavigationSeen = 1U;
        s_dSvcRun.u32LastNavigationMs = stSnapshot.stNavigation.u32UpdateMs;
        /* 有坐标不证明天线连接或接收机完整自检，不能写 PASS。 */
        EcallDiagService_Publish(RTE_DIAG_GNSS_RECEIVER, RTE_DIAG_INDETERMINATE, RTE_DIAG_REASON_PARTIAL_COVERAGE, stSnapshot.stNavigation.u32UpdateMs);
    }
}
