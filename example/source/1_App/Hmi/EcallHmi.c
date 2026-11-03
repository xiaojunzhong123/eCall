/*******************************************************
  * Name    :EcallHmi.c
  * Author  :xiaojunzhong
  * Function:自检与 MCU 事件提示仲裁，不接管模组通话状态。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
/* 流程阅读说明：
 * 读取诊断和 MCU 事件快照 -> 生成提示意图 -> 输出服务保存 -> 等待真实交付确认。
 * 默认无真实提示通道；保存请求或 RTT 日志不能当作乘员已经看到。
 * 通话由模组管理，本模块只展示 MCU 数据事件阶段及自检信息。
 */

#include "EcallHmi.h"
#include "EcallDiag.h"
#include "EcallDiag_Cfg.h"
#include "Ecall.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <string.h>
typedef enum
{
    EC_HMI_WAIT_CHANNEL = 0U, /* 等待真实乘员提示通道。 */
    EC_HMI_ACTIVE /* 输出请求并监督真实交付。 */
} EcallHmiPhase_t;
typedef struct
{
    EcallHmiPhase_t enPhase; /* 当前私有处理阶段。 */
    OutputServiceEcallRequest_t stLastIntent; /* 上次已发布的提示意图，用于避免重复提交。 */
    uint32_t u32SubmittedMs; /* 本次提示请求提交时刻，毫秒。 */
    uint8_t u8HasIntent; /* 是否已经保存过提示意图。 */
    uint8_t u8ForceSubmit; /* 失败或超时后下一周期重申最新提示。 */
} EcallHmiRuntime_t;
/* AppTask 拥有仲裁和请求；其他任务仅通过快照查询稳定交付结论。 */
static EcallHmiRuntime_t s_eHmiRun; /* AppTask 独占的提示仲裁过程。 */
static EcallHmiStatus_t s_eHmiSts; /* 对外可查询的提示交付结论。 */
/*******************************************************
  * Name    :Ecall_HmiGetText
  * Author  :xiaojunzhong
  * Function:返回只读提示资源，缺少已评审阿拉伯语时明确返回空指针。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
const char *Ecall_HmiGetText(OutputServiceEcallMessage_t enMessage, uint8_t u8Language)
{
    const char *pcText = NULL;
    if (u8Language == 0U)
    {
        switch (enMessage)
        {
            case OUTPUT_ECALL_MESSAGE_FAULT:
                pcText = "eCall system fault. Service required.";
                break;
            case OUTPUT_ECALL_MESSAGE_INCOMPLETE:
                pcText = "eCall self-test incomplete. Service required.";
                break;
            case OUTPUT_ECALL_MESSAGE_CHECKING:
                pcText = "eCall self-test in progress.";
                break;
            case OUTPUT_ECALL_MESSAGE_NONE:
                pcText = "";
                break;
            default:
                break;
        }
    }
    else if ((u8Language == 1U) && (ECALL_HMI_ARABIC_REVIEWED != 0U))
    {
        switch (enMessage)
        {
            case OUTPUT_ECALL_MESSAGE_FAULT:
                pcText = ECALL_HMI_FAULT_AR;
                break;
            case OUTPUT_ECALL_MESSAGE_INCOMPLETE:
                pcText = ECALL_HMI_INCOMPLETE_AR;
                break;
            case OUTPUT_ECALL_MESSAGE_CHECKING:
                pcText = ECALL_HMI_CHECKING_AR;
                break;
            case OUTPUT_ECALL_MESSAGE_NONE:
                pcText = "";
                break;
            default:
                break;
        }
        if ((pcText != NULL) && (pcText[0] == '\0') && (enMessage != OUTPUT_ECALL_MESSAGE_NONE))
        {
            pcText = NULL;
        }
    }
    else
    {
        /* 未评审的翻译不作为可用乘员资源。 */
    }
    return pcText;
}
/*******************************************************
  * Name    :Ecall_HmiEnterWaitChannel
  * Author  :xiaojunzhong
  * Function:进入等待真实提示通道阶段，保留当前故障请求。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiEnterWaitChannel(void)
{
    s_eHmiRun.enPhase = EC_HMI_WAIT_CHANNEL;
    s_eHmiRun.u32SubmittedMs = osKernelSysTick();
}
/*******************************************************
  * Name    :Ecall_HmiEnterActive
  * Author  :xiaojunzhong
  * Function:通道恢复后准备补发最新组合提示，尚不宣称已显示。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiEnterActive(void)
{
    s_eHmiRun.enPhase = EC_HMI_ACTIVE;
    s_eHmiRun.u8ForceSubmit = 1U;
    s_eHmiRun.u32SubmittedMs = osKernelSysTick();
}
/*******************************************************
  * Name    :Ecall_HmiUpdateIntent
  * Author  :xiaojunzhong
  * Function:组合诊断及通话意图，变化或点火再激活时发布新请求。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiUpdateIntent(void)
{
    EcallDiagStatus_t stDiagnostic; /* 本周期读取的诊断快照。 */
    EcallStatus_t stCall; /* MCU 事件状态副本，历史变量名不表示通话所有权。 */
    OutputServiceEcallRequest_t stIntent; /* 本周期待发布的诊断和 MCU 事件组合提示。 */
    uint32_t u32RequestId; /* 非零请求号，用于过滤迟到输出确认。 */
    uint8_t u8Accepted; /* 接口是否接受本次请求，不能等同硬件执行成功。 */
    (void)Ecall_DiagReadStatus(&stDiagnostic);
    (void)Ecall_ReadStatus(&stCall);
    (void)memset(&stIntent, 0, sizeof(stIntent));
    stIntent.u32Generation = stDiagnostic.u32Generation;
    stIntent.u32ReminderRevision = stDiagnostic.u32ReminderRevision;
    stIntent.u16ActiveFaultMask = stDiagnostic.u16ActiveFaultMask;
    stIntent.u16IncompleteMask = stDiagnostic.u16IncompleteMask;
    stIntent.u8EcallState = (uint8_t)stCall.enState;
    if (stDiagnostic.u16ActiveFaultMask != 0U)
    {
        stIntent.enDiagnosticMessage = OUTPUT_ECALL_MESSAGE_FAULT;
    }
    else if (stDiagnostic.u16IncompleteMask != 0U)
    {
        stIntent.enDiagnosticMessage = OUTPUT_ECALL_MESSAGE_INCOMPLETE;
    }
    else if (stDiagnostic.enConclusion == ECALL_DIAG_CHECKING)
    {
        stIntent.enDiagnosticMessage = OUTPUT_ECALL_MESSAGE_CHECKING;
    }
    else if (stDiagnostic.enConclusion != ECALL_DIAG_HEALTHY)
    {
        stIntent.enDiagnosticMessage = OUTPUT_ECALL_MESSAGE_INCOMPLETE;
    }
    else
    {
        stIntent.enDiagnosticMessage = OUTPUT_ECALL_MESSAGE_NONE;
    }
    stIntent.u8PersistentWarning = ((stIntent.u16ActiveFaultMask != 0U) || (stIntent.u16IncompleteMask != 0U)) ? 1U : 0U;
    stIntent.u8EnglishAvailable = (Ecall_HmiGetText(stIntent.enDiagnosticMessage, 0U) != NULL) ? 1U : 0U;
    stIntent.u8ArabicAvailable = (Ecall_HmiGetText(stIntent.enDiagnosticMessage, 1U) != NULL) ? 1U : 0U;
    if ((s_eHmiRun.u8HasIntent == 0U) || (s_eHmiRun.u8ForceSubmit != 0U)
        || (memcmp(&stIntent, &s_eHmiRun.stLastIntent, sizeof(stIntent)) != 0))
    {
        u8Accepted = OutputService_PublishEcallIndication(&stIntent, &u32RequestId);
        if (u8Accepted != 0U)
        {
            s_eHmiRun.stLastIntent = stIntent;
            s_eHmiRun.u8HasIntent = 1U;
            s_eHmiRun.u8ForceSubmit = 0U;
            s_eHmiRun.u32SubmittedMs = osKernelSysTick();
            portENTER_CRITICAL();
            s_eHmiSts.u32RequestId = u32RequestId;
            portEXIT_CRITICAL();
        }
    }
    portENTER_CRITICAL();
    s_eHmiSts.u8PersistentWarning = stIntent.u8PersistentWarning;
    s_eHmiSts.u8LanguageComplete = ((stIntent.u8EnglishAvailable != 0U) && (stIntent.u8ArabicAvailable != 0U)) ? 1U : 0U;
    portEXIT_CRITICAL();
}
/*******************************************************
  * Name    :Ecall_HmiObserveDelivery
  * Author  :xiaojunzhong
  * Function:读取输出服务真实交付结果，日志不能替代确认。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiObserveDelivery(OutputServiceEcallStatus_t *pstOutput)
{
    (void)OutputService_ReadEcallIndicationStatus(pstOutput);
    portENTER_CRITICAL();
    if (pstOutput->u32RequestId != s_eHmiSts.u32RequestId)
    {
        /* 通道重建可能已使旧请求失效；下周期重申最新意图再认领交付。 */
        s_eHmiRun.u8ForceSubmit = 1U;
        s_eHmiSts.enDelivery = OUTPUT_ECALL_PENDING;
    }
    else
    {
        s_eHmiSts.enDelivery = pstOutput->enDelivery;
        if (pstOutput->enDelivery == OUTPUT_ECALL_DELIVERED)
        {
            s_eHmiSts.u8DeliveryTimedOut = 0U;
        }
    }
    portEXIT_CRITICAL();
}
/*******************************************************
  * Name    :Ecall_HmiProcessWaitChannel
  * Author  :xiaojunzhong
  * Function:无通道时保存最新意图，不抢占现有指示灯。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiProcessWaitChannel(void)
{
    OutputServiceEcallStatus_t stOutput; /* 实际输出通道及交付状态副本。 */
    Ecall_HmiUpdateIntent();
    Ecall_HmiObserveDelivery(&stOutput);
    if (stOutput.u8ChannelAvailable != 0U)
    {
        Ecall_HmiEnterActive();
    }
}
/*******************************************************
  * Name    :Ecall_HmiProcessActive
  * Author  :xiaojunzhong
  * Function:监督输出确认和有界重试节拍，不改变诊断故障状态。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
static void Ecall_HmiProcessActive(void)
{
    OutputServiceEcallStatus_t stOutput; /* 实际输出通道及交付状态副本。 */
    uint32_t u32AgeMs; /* 从来源或请求起点经过的毫秒数。 */
    Ecall_HmiUpdateIntent();
    Ecall_HmiObserveDelivery(&stOutput);
    if (stOutput.u8ChannelAvailable == 0U)
    {
        Ecall_HmiEnterWaitChannel();
        return;
    }
    u32AgeMs = osKernelSysTick() - s_eHmiRun.u32SubmittedMs;
    if ((stOutput.enDelivery == OUTPUT_ECALL_FAILED) && (u32AgeMs >= ECALL_HMI_RETRY_MS))
    {
        s_eHmiRun.u8ForceSubmit = 1U;
    }
    else if ((stOutput.enDelivery == OUTPUT_ECALL_PENDING) && (u32AgeMs >= ECALL_HMI_ACK_TIMEOUT_MS))
    {
        portENTER_CRITICAL();
        s_eHmiSts.u8DeliveryTimedOut = 1U;
        portEXIT_CRITICAL();
        s_eHmiRun.u8ForceSubmit = 1U;
    }
    else
    {
        /* 已确认的实际输出不周期重发；缺通道不影响活动故障。 */
    }
}
/*******************************************************
  * Name    :Ecall_HmiInit
  * Author  :xiaojunzhong
  * Function:清零提示运行态和结果，默认等待实际通道。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void Ecall_HmiInit(void)
{
    (void)memset(&s_eHmiRun, 0, sizeof(s_eHmiRun));
    (void)memset(&s_eHmiSts, 0, sizeof(s_eHmiSts));
    Ecall_HmiEnterWaitChannel();
}
/*******************************************************
  * Name    :Ecall_HmiProcess
  * Author  :xiaojunzhong
  * Function:按当前提示阶段分发，进入的新阶段下周期执行。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
void Ecall_HmiProcess(void)
{
    switch (s_eHmiRun.enPhase)
    {
        case EC_HMI_WAIT_CHANNEL:
            Ecall_HmiProcessWaitChannel();
            break;
        case EC_HMI_ACTIVE:
            Ecall_HmiProcessActive();
            break;
        default:
            break;
    }
}
/*******************************************************
  * Name    :Ecall_HmiReadStatus
  * Author  :xiaojunzhong
  * Function:按值读取稳定交付和语言状态，不清除故障。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
uint8_t Ecall_HmiReadStatus(EcallHmiStatus_t *pstStatus)
{
    if (pstStatus == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstStatus = s_eHmiSts;
    portEXIT_CRITICAL();
    return 1U;
}
