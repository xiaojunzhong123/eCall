/*******************************************************
  * Name    :EcallDiag.c
  * Author  :xiaojunzhong
  * Function:上电诊断和持续监测，来源观测、故障确认及恢复分别处理。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
/* 流程阅读说明：
 * 执行路径：服务发布带来源令牌的观测 -> RTE -> 本模块逐项确认/恢复/过期 -> 聚合。
 * 新样本数量与采样时间同时满足才确认；重复读取不刷新来源年龄。
 * 未测、过期、不支持与物理故障分开。缺少模组协议结果时不自动判健康。
 */

#include "EcallDiag.h"
#include "EcallDiag_Cfg.h"
#include "Ecall.h"
#include "RteSys.h"
#include "Hmi.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <string.h>
typedef enum
{
    EC_DIAG_PHASE_STARTUP = 0U, /* 上电准备和首次检查阶段。 */
    EC_DIAG_PHASE_MONITOR /* 上电检查结束后的持续监测阶段。 */
} EcallDiagPhase_t;
typedef struct
{
    uint32_t u32LastSequence; /* 已消费的来源样本序号，防止重复样本累计确认。 */
    uint32_t u32SourceEpoch; /* 来源重启代次，变化时原令牌失效。 */
    uint32_t u32PreparationStartMs; /* 本项开始准备的单调毫秒时刻。 */
    uint32_t u32CandidateStartMs; /* 连续确认或恢复候选的首样本时刻，毫秒。 */
    uint32_t u32LastObservedMs; /* 上次真实观测的来源时刻，毫秒。 */
    uint16_t u16CandidateSamples; /* 新的有效候选样本个数，重复查询不累加。 */
    RteSysDiagState_t enCandidate; /* 正在确认的候选诊断状态。 */
} EcallDiagItemRuntime_t;
typedef struct
{
    EcallDiagPhase_t enPhase; /* 当前私有处理阶段。 */
    uint8_t u8LastIgnitionOn; /* 上周期点火状态，用于识别再次点火。 */
    EcallDiagItemRuntime_t astItems[RTESYS_DIAG_COUNT]; /* 按十类诊断索引保存的单项记录。 */
} EcallDiagRuntime_t;
/* AppTask 单写私有过程数据；稳定结果读写以短临界区复制。 */
static EcallDiagRuntime_t s_eDiagRun; /* AppTask 独占的诊断确认过程。 */
static EcallDiagStatus_t s_eDiagSts; /* 可跨任务复制的诊断聚合结论。 */
/*******************************************************
  * Name    :Ecall_DiagEnterMonitor
  * Author  :xiaojunzhong
  * Function:进入持续监测阶段，不清除已有活动故障。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagEnterMonitor(void)
{
    s_eDiagRun.enPhase = EC_DIAG_PHASE_MONITOR;
}
/*******************************************************
  * Name    :Ecall_DiagResetCandidate
  * Author  :xiaojunzhong
  * Function:仅复位确认过程，不改变已确认活动故障。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagResetCandidate(EcallDiagItemRuntime_t *pstRun)
{
    pstRun->enCandidate = RTE_DIAG_NOT_RUN;
    pstRun->u16CandidateSamples = 0U;
    pstRun->u32CandidateStartMs = 0U;
}
/*******************************************************
  * Name    :Ecall_DiagConfirm
  * Author  :xiaojunzhong
  * Function:仅用新的有效样本累计确认次数和真实采样时间。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static uint8_t Ecall_DiagConfirm(EcallDiagItemRuntime_t *pstRun, const RteSysDiagObservation_t *pstObservation, uint32_t u32DurationMs)
{
    if (pstRun->enCandidate != pstObservation->enState)
    {
        pstRun->enCandidate = pstObservation->enState;
        pstRun->u32CandidateStartMs = pstObservation->u32ObservedMs;
        pstRun->u16CandidateSamples = 0U;
    }
    if (pstRun->u16CandidateSamples < ECALL_DIAG_CONFIRM_SAMPLES)
    {
        pstRun->u16CandidateSamples++;
    }
    return ((pstRun->u16CandidateSamples >= ECALL_DIAG_CONFIRM_SAMPLES)
        && ((pstObservation->u32ObservedMs - pstRun->u32CandidateStartMs) >= u32DurationMs)) ? 1U : 0U;
}
/*******************************************************
  * Name    :Ecall_DiagLogFault
  * Author  :xiaojunzhong
  * Function:仅在故障确认或恢复时记录检查编号，不包含车辆或位置数据。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagLogFault(uint8_t u8Index, uint8_t u8Active)
{
    char acMessage[32];
    (void)strcpy(acMessage, (u8Active != 0U) ? "DIAG_ITEM_0_ACTIVE\r\n" : "DIAG_ITEM_0_RECOVERED\r\n");
    acMessage[10] = (char)('0' + u8Index);
    HMI_LOG_ECALL(acMessage);
}

/*******************************************************
  * Name    :Ecall_DiagEvaluateFresh
  * Author  :xiaojunzhong
  * Function:按完整覆盖、配置资格和确认恢复条件处理新观测。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagEvaluateFresh(uint8_t u8Index, EcallDiagItemRuntime_t *pstRun, EcallDiagItemStatus_t *pstItem, const RteSysDiagObservation_t *pstObservation)
{
    uint8_t u8Confirmed; /* 确认时间和新样本数是否同时满足。 */
    if ((pstObservation->enState == RTE_DIAG_PASS) || (pstObservation->enState == RTE_DIAG_FAIL))
    {
        if (ECALL_DIAG_POLICY_CONFIRMED == 0U)
        {
            pstItem->enState = RTE_DIAG_INDETERMINATE;
            pstItem->u16Reason = RTE_DIAG_REASON_POLICY_UNCONFIRMED;
            Ecall_DiagResetCandidate(pstRun);
            return;
        }
    }
    switch (pstObservation->enState)
    {
        case RTE_DIAG_FAIL:
            u8Confirmed = Ecall_DiagConfirm(pstRun, pstObservation, ECALL_DIAG_FAIL_CONFIRM_MS);
            if ((u8Confirmed != 0U) || (pstItem->u8ActiveFault != 0U))
            {
                if (pstItem->u8ActiveFault == 0U)
                {
                    Ecall_DiagLogFault(u8Index, 1U);
                }
                pstItem->u8ActiveFault = 1U;
                pstItem->u16ActiveFaultReason = pstObservation->u16Reason;
                pstItem->enState = RTE_DIAG_FAIL;
            }
            else
            {
                pstItem->enState = RTE_DIAG_INDETERMINATE;
                pstItem->u16Reason = RTE_DIAG_REASON_CONFIRMING;
            }
            break;
        case RTE_DIAG_PASS:
            if (pstObservation->u8CoverageComplete == 0U)
            {
                pstItem->enState = RTE_DIAG_INDETERMINATE;
                pstItem->u16Reason = RTE_DIAG_REASON_PARTIAL_COVERAGE;
                Ecall_DiagResetCandidate(pstRun);
            }
            else if (pstItem->u8ActiveFault != 0U)
            {
                u8Confirmed = Ecall_DiagConfirm(pstRun, pstObservation, ECALL_DIAG_RECOVER_CONFIRM_MS);
                if (u8Confirmed != 0U)
                {
                    pstItem->u8ActiveFault = 0U;
                    pstItem->u16ActiveFaultReason = 0U;
                    pstItem->enState = RTE_DIAG_PASS;
                    Ecall_DiagResetCandidate(pstRun);
                    Ecall_DiagLogFault(u8Index, 0U);
                }
                else
                {
                    pstItem->enState = RTE_DIAG_INDETERMINATE;
                    pstItem->u16Reason = RTE_DIAG_REASON_RECOVERING;
                }
            }
            else
            {
                pstItem->enState = RTE_DIAG_PASS;
                Ecall_DiagResetCandidate(pstRun);
            }
            break;
        case RTE_DIAG_NOT_APPLICABLE:
            Ecall_DiagResetCandidate(pstRun);
            if ((ECALL_DIAG_POLICY_CONFIRMED != 0U)
                && ((ECALL_DIAG_NOT_APPLICABLE_MASK & (1U << u8Index)) != 0U)
                && (pstObservation->u16ApplicabilityBasis != 0U))
            {
                pstItem->enState = RTE_DIAG_NOT_APPLICABLE;
            }
            else
            {
                pstItem->enState = RTE_DIAG_INDETERMINATE;
                pstItem->u16Reason = RTE_DIAG_REASON_INVALID_APPLICABILITY;
            }
            break;
        default:
            Ecall_DiagResetCandidate(pstRun);
            pstItem->enState = pstObservation->enState;
            break;
    }
}
/*******************************************************
  * Name    :Ecall_DiagUpdateItem
  * Author  :xiaojunzhong
  * Function:处理单项启动期限、来源重启和过期，保留未恢复故障。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagUpdateItem(uint8_t u8Index, uint32_t u32NowMs, EcallDiagStatus_t *pstNext)
{
    RteSysDiagObservation_t stObservation; /* 某项诊断的来源观测副本。 */
    EcallDiagItemRuntime_t *pstRun = &s_eDiagRun.astItems[u8Index];
    EcallDiagItemStatus_t *pstItem = &pstNext->astItems[u8Index];
    (void)RteSys_DiagReadItem((RteSysDiagItem_t)u8Index, &stObservation);
    if (stObservation.stToken.u32SourceEpoch != pstRun->u32SourceEpoch)
    {
        pstRun->u32SourceEpoch = stObservation.stToken.u32SourceEpoch;
        pstRun->u32LastSequence = 0U;
        pstRun->u32PreparationStartMs = u32NowMs;
        Ecall_DiagResetCandidate(pstRun);
    }
    pstItem->u32SourceEpoch = pstRun->u32SourceEpoch;
    pstItem->enObservedState = stObservation.enState;
    if ((stObservation.enState == RTE_DIAG_NOT_RUN) || (stObservation.enState == RTE_DIAG_RUNNING))
    {
        Ecall_DiagResetCandidate(pstRun);
        if ((u32NowMs - pstRun->u32PreparationStartMs) < ECALL_DIAG_PREPARATION_MS)
        {
            pstItem->enState = RTE_DIAG_RUNNING;
            pstItem->u16Reason = stObservation.u16Reason;
        }
        else
        {
            pstItem->enState = RTE_DIAG_INDETERMINATE;
            pstItem->u16Reason = RTE_DIAG_REASON_PREPARATION_TIMEOUT;
        }
        return;
    }
    if ((stObservation.enState != RTE_DIAG_UNSUPPORTED)
        && (stObservation.enState != RTE_DIAG_NOT_APPLICABLE)
        && ((u32NowMs - stObservation.u32ObservedMs) >= ECALL_DIAG_SAMPLE_MAX_AGE_MS))
    {
        pstItem->enState = RTE_DIAG_INDETERMINATE;
        pstItem->u16Reason = RTE_DIAG_REASON_STALE;
        Ecall_DiagResetCandidate(pstRun);
        return;
    }
    if (stObservation.u32Sequence != pstRun->u32LastSequence)
    {
        if ((stObservation.u32ObservedMs - pstRun->u32LastObservedMs) >= ECALL_DIAG_SAMPLE_MAX_AGE_MS)
        {
            Ecall_DiagResetCandidate(pstRun);
        }
        pstRun->u32LastObservedMs = stObservation.u32ObservedMs;
        pstRun->u32LastSequence = stObservation.u32Sequence;
        pstItem->u32ObservedMs = stObservation.u32ObservedMs;
        pstItem->u16Reason = stObservation.u16Reason;
        pstItem->u16CommonCause = stObservation.u16CommonCause;
        Ecall_DiagEvaluateFresh(u8Index, pstRun, pstItem, &stObservation);
    }
}
/*******************************************************
  * Name    :Ecall_DiagAggregate
  * Author  :xiaojunzhong
  * Function:按活动故障、检查中、未完成和完整健康顺序形成结论。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagAggregate(EcallDiagStatus_t *pstNext)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    uint16_t u16Bit; /* 当前诊断项对应的位掩码。 */
    uint8_t u8AllQualified = 1U; /* 所有必需检查是否具有合格结论。 */
    pstNext->u16ActiveFaultMask = 0U;
    pstNext->u16IncompleteMask = 0U;
    pstNext->u16CheckingMask = 0U;
    pstNext->u16UnsupportedMask = 0U;
    pstNext->u16NotApplicableMask = 0U;
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        u16Bit = (uint16_t)(1U << u8Index);
        if (pstNext->astItems[u8Index].u8ActiveFault != 0U)
        {
            pstNext->u16ActiveFaultMask |= u16Bit;
        }
        switch (pstNext->astItems[u8Index].enState)
        {
            case RTE_DIAG_PASS:
                break;
            case RTE_DIAG_FAIL:
                u8AllQualified = 0U;
                break;
            case RTE_DIAG_NOT_APPLICABLE:
                pstNext->u16NotApplicableMask |= u16Bit;
                break;
            case RTE_DIAG_RUNNING:
            case RTE_DIAG_NOT_RUN:
                pstNext->u16CheckingMask |= u16Bit;
                u8AllQualified = 0U;
                break;
            case RTE_DIAG_UNSUPPORTED:
                pstNext->u16UnsupportedMask |= u16Bit;
                pstNext->u16IncompleteMask |= u16Bit;
                u8AllQualified = 0U;
                break;
            default:
                pstNext->u16IncompleteMask |= u16Bit;
                u8AllQualified = 0U;
                break;
        }
    }
    pstNext->u8StartupComplete = (pstNext->u16CheckingMask == 0U) ? 1U : 0U;
    if (pstNext->u16ActiveFaultMask != 0U)
    {
        pstNext->enConclusion = ECALL_DIAG_FAULT;
    }
    else if (pstNext->u16CheckingMask != 0U)
    {
        pstNext->enConclusion = ECALL_DIAG_CHECKING;
    }
    else if ((u8AllQualified != 0U) && (ECALL_DIAG_POLICY_CONFIRMED != 0U))
    {
        pstNext->enConclusion = ECALL_DIAG_HEALTHY;
    }
    else
    {
        pstNext->enConclusion = ECALL_DIAG_INCOMPLETE;
    }
}
/*******************************************************
  * Name    :Ecall_DiagUpdate
  * Author  :xiaojunzhong
  * Function:读取十项观测及点火状态，在短临界区发布业务结果。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagUpdate(void)
{
    EcallDiagStatus_t stNext; /* 计算完毕后一次发布的新状态副本。 */
    RteSwitchState_t enIgnition = RteSys_GetSwitchState(RTESYS_SWITCH_KL15); /* 归一化 KL15 点火输入。 */
    uint8_t u8Index; /* 当前数组或通道索引。 */
    uint8_t u8IgnitionOn = (enIgnition == RTE_SWITCH_ACTIVE) ? 1U : 0U; /* 点火逻辑值，一为接通。 */
    uint32_t u32NowMs = osKernelSysTick(); /* 当前单调毫秒计数，用无符号差值处理回绕。 */
    stNext = s_eDiagSts;
    stNext.u8EmergencyPriority = Ecall_IsSessionActive();
    if ((u8IgnitionOn != 0U) && (s_eDiagRun.u8LastIgnitionOn == 0U))
    {
        stNext.u32ReminderRevision++;
    }
    s_eDiagRun.u8LastIgnitionOn = u8IgnitionOn;
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        Ecall_DiagUpdateItem(u8Index, u32NowMs, &stNext);
    }
    Ecall_DiagAggregate(&stNext);
    if (memcmp(&stNext, &s_eDiagSts, sizeof(stNext)) != 0)
    {
        stNext.u32Revision++;
        portENTER_CRITICAL();
        s_eDiagSts = stNext;
        portEXIT_CRITICAL();
    }
}
/*******************************************************
  * Name    :Ecall_DiagProcessStartup
  * Author  :xiaojunzhong
  * Function:执行有界启动检查，全部项离开准备期后转持续监测。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagProcessStartup(void)
{
    Ecall_DiagUpdate();
    if (s_eDiagSts.u8StartupComplete != 0U)
    {
        Ecall_DiagEnterMonitor();
    }
}
/*******************************************************
  * Name    :Ecall_DiagProcessMonitor
  * Author  :xiaojunzhong
  * Function:持续消费新观测，不发起任何冲突检查或硬件动作。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_DiagProcessMonitor(void)
{
    Ecall_DiagUpdate();
}
/*******************************************************
  * Name    :Ecall_DiagInit
  * Author  :xiaojunzhong
  * Function:系统启动时集中清零并建立新的诊断代次，初值未测。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void Ecall_DiagInit(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    (void)memset(&s_eDiagRun, 0, sizeof(s_eDiagRun));
    (void)memset(&s_eDiagSts, 0, sizeof(s_eDiagSts));
    s_eDiagSts.u32Generation = RteSys_DiagBeginCycle();
    s_eDiagSts.u8PolicyConfirmed = ECALL_DIAG_POLICY_CONFIRMED;
    s_eDiagSts.u16CheckingMask = RTESYS_DIAG_ALL_MASK;
    s_eDiagRun.enPhase = EC_DIAG_PHASE_STARTUP;
    for (u8Index = 0U; u8Index < RTESYS_DIAG_COUNT; u8Index++)
    {
        s_eDiagRun.astItems[u8Index].u32SourceEpoch = 1U;
        s_eDiagRun.astItems[u8Index].u32PreparationStartMs = osKernelSysTick();
    }
}
/*******************************************************
  * Name    :Ecall_DiagProcess
  * Author  :xiaojunzhong
  * Function:按顶层阶段分发本周期自检和运行监测。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void Ecall_DiagProcess(void)
{
    switch (s_eDiagRun.enPhase)
    {
        case EC_DIAG_PHASE_STARTUP:
            Ecall_DiagProcessStartup();
            break;
        case EC_DIAG_PHASE_MONITOR:
            Ecall_DiagProcessMonitor();
            break;
        default:
            break;
    }
}
/*******************************************************
  * Name    :Ecall_DiagReadStatus
  * Author  :xiaojunzhong
  * Function:复制完整稳定结果，查询不会清故障或推进确认。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t Ecall_DiagReadStatus(EcallDiagStatus_t *pstStatus)
{
    if (pstStatus == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstStatus = s_eDiagSts;
    portEXIT_CRITICAL();
    return 1U;
}
