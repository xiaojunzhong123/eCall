/*******************************************************
  * Name    :RteSys_Diag.c
  * Author  :xiaojunzhong
  * Function:共享信号缓存和值复制契约；生产者更新，使用者获取独立副本。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#include "RteSys_Diag.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <string.h>
typedef struct
{
    uint32_t u32Generation; /* 本次系统上电诊断代次。 */
    uint32_t u32NextSequence; /* 下一次发布的样本序号，跳过零。 */
    RteSysDiagObservation_t astItems[RTESYS_DIAG_COUNT]; /* 按十类诊断索引保存的单项记录。 */
} RteSysDiagRuntime_t;
/* 服务单写各自责任项；所有任务读写均经短临界区，禁止 ISR 调用。 */
static RteSysDiagRuntime_t s_rDiagRun; /* 诊断观测存储及令牌状态，临界区保护。 */
static const RteSysDiagSource_t s_aeDiagOwner[RTESYS_DIAG_COUNT] =
{
    RTE_DIAG_SOURCE_SYSTEM, RTE_DIAG_SOURCE_MODEM, RTE_DIAG_SOURCE_MODEM,
    RTE_DIAG_SOURCE_GNSS, RTE_DIAG_SOURCE_GNSS, RTE_DIAG_SOURCE_CAN,
    RTE_DIAG_SOURCE_SYSTEM, RTE_DIAG_SOURCE_MODEM, RTE_DIAG_SOURCE_POWER, RTE_DIAG_SOURCE_POWER
};
/*******************************************************
  * Name    :RteSys_DiagOwner
  * Author  :xiaojunzhong
  * Function:返回诊断项固定责任来源，不推断实际检测能力。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
RteSysDiagSource_t RteSys_DiagOwner(RteSysDiagItem_t enItem)
{
    if ((uint32_t)enItem >= RTESYS_DIAG_COUNT)
    {
        return (RteSysDiagSource_t)0U;
    }
    return s_aeDiagOwner[enItem];
}
/*******************************************************
  * Name    :RteSys_InitDiagnostics
  * Author  :xiaojunzhong
  * Function:系统启动时清零全部诊断交换数据。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void RteSys_InitDiagnostics(void)
{
    (void)memset(&s_rDiagRun, 0, sizeof(s_rDiagRun));
}
/*******************************************************
  * Name    :RteSys_DiagBeginCycle
  * Author  :xiaojunzhong
  * Function:建立本次启动诊断代次，所有结果重新从未测开始。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint32_t RteSys_DiagBeginCycle(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    uint32_t u32Generation; /* 本次系统上电诊断代次。 */
    portENTER_CRITICAL();
    s_rDiagRun.u32Generation++;
    if (s_rDiagRun.u32Generation == 0U)
    {
        s_rDiagRun.u32Generation = 1U;
    }
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        (void)memset(&s_rDiagRun.astItems[u8Index], 0, sizeof(s_rDiagRun.astItems[u8Index]));
        s_rDiagRun.astItems[u8Index].stToken.u32Generation = s_rDiagRun.u32Generation;
        s_rDiagRun.astItems[u8Index].stToken.u32SourceEpoch = 1U;
        s_rDiagRun.astItems[u8Index].enSource = s_aeDiagOwner[u8Index];
    }
    u32Generation = s_rDiagRun.u32Generation;
    portEXIT_CRITICAL();
    return u32Generation;
}
/*******************************************************
  * Name    :RteSys_DiagReadToken
  * Author  :xiaojunzhong
  * Function:复制检查开始时的代次和来源生命周期令牌。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t RteSys_DiagReadToken(RteSysDiagItem_t enItem, RteSysDiagToken_t *pstToken)
{
    if (((uint32_t)enItem >= RTESYS_DIAG_COUNT) || (pstToken == NULL))
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstToken = s_rDiagRun.astItems[enItem].stToken;
    portEXIT_CRITICAL();
    return (pstToken->u32Generation != 0U) ? 1U : 0U;
}
/*******************************************************
  * Name    :RteSys_DiagPublish
  * Author  :xiaojunzhong
  * Function:校验来源、代次和单调采样时刻后原子保存新观测。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t RteSys_DiagPublish(RteSysDiagItem_t enItem, const RteSysDiagObservation_t *pstObservation)
{
    RteSysDiagObservation_t *pstCurrent;
    uint8_t u8Accepted = 0U; /* 接口是否接受本次请求，不能等同硬件执行成功。 */
    uint32_t u32Now = osKernelSysTick(); /* 当前单调毫秒计数。 */
    if (((uint32_t)enItem >= RTESYS_DIAG_COUNT) || (pstObservation == NULL))
    {
        return 0U;
    }
    if (((uint32_t)pstObservation->enState > RTE_DIAG_NOT_APPLICABLE)
        || (pstObservation->enSource != s_aeDiagOwner[enItem])
        || ((int32_t)(u32Now - pstObservation->u32ObservedMs) < 0))
    {
        return 0U;
    }
    portENTER_CRITICAL();
    pstCurrent = &s_rDiagRun.astItems[enItem];
    if ((pstObservation->stToken.u32Generation != 0U)
        && (pstObservation->stToken.u32Generation == pstCurrent->stToken.u32Generation)
        && (pstObservation->stToken.u32SourceEpoch == pstCurrent->stToken.u32SourceEpoch)
        && ((pstCurrent->u32Sequence == 0U) || ((int32_t)(pstObservation->u32ObservedMs - pstCurrent->u32ObservedMs) > 0)))
    {
        *pstCurrent = *pstObservation;
        s_rDiagRun.u32NextSequence++;
        if (s_rDiagRun.u32NextSequence == 0U)
        {
            s_rDiagRun.u32NextSequence = 1U;
        }
        pstCurrent->u32Sequence = s_rDiagRun.u32NextSequence;
        u8Accepted = 1U;
    }
    portEXIT_CRITICAL();
    return u8Accepted;
}
/*******************************************************
  * Name    :RteSys_DiagInvalidateSource
  * Author  :xiaojunzhong
  * Function:来源重启时失效旧令牌，保留全局诊断代次。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void RteSys_DiagInvalidateSource(RteSysDiagSource_t enSource)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    portENTER_CRITICAL();
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        if (s_aeDiagOwner[u8Index] == enSource)
        {
            s_rDiagRun.astItems[u8Index].stToken.u32SourceEpoch++;
            if (s_rDiagRun.astItems[u8Index].stToken.u32SourceEpoch == 0U)
            {
                s_rDiagRun.astItems[u8Index].stToken.u32SourceEpoch = 1U;
            }
            s_rDiagRun.astItems[u8Index].u32Sequence = 0U;
            s_rDiagRun.astItems[u8Index].enState = RTE_DIAG_NOT_RUN;
            s_rDiagRun.astItems[u8Index].u16Reason = RTE_DIAG_REASON_SOURCE_RESET;
        }
    }
    portEXIT_CRITICAL();
}
/*******************************************************
  * Name    :RteSys_DiagReadItem
  * Author  :xiaojunzhong
  * Function:按值复制单项观测，避免读取刷新数据年龄。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t RteSys_DiagReadItem(RteSysDiagItem_t enItem, RteSysDiagObservation_t *pstObservation)
{
    if (((uint32_t)enItem >= RTESYS_DIAG_COUNT) || (pstObservation == NULL))
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstObservation = s_rDiagRun.astItems[enItem];
    portEXIT_CRITICAL();
    return 1U;
}
