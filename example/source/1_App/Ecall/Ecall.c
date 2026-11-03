/*******************************************************
  * Name    :Ecall.c
  * Author  :xiaojunzhong
  * Function:负责触发合并、手动取消、事故快照及 MCU 数据交付生命周期。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#include "Ecall.h"
#include "PowerService.h"
#include "Hmi.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <string.h>

/* EcallTask 独占过程数据；跨任务共享的请求及交付标志统一由临界区保护。
 * 不等待 AT、不解析通话状态、不编码 MSD，也不使用动态内存。
 * 阅读顺序：OnTriggerMask -> ManualWindow -> DataPending -> Standby。 */
typedef struct
{
    EcallRequest_t stRequest;    /* 首次触发冻结的快照，后续触发仅合并来源。 */
    uint32_t u32NextSessionId;   /* 本次启动中的递增事件号，跳过零。 */
    uint32_t u32PhaseStartMs;    /* 单调毫秒计时起点，不受 UTC 校时影响。 */
    uint8_t u8DeliveryReady;     /* 协议服务已回报交付结果，应用尚未消费。 */
    uint8_t u8DeliverySuccess;   /* 一为已交付，零为被明确拒绝。 */
} EcallRuntime_t;
static EcallRuntime_t s_ecAppRun; /* 事件过程和有界交付邮箱。 */
static EcallStatus_t s_ecAppSts;  /* 对外发布的稳定业务结论。 */

/*******************************************************
  * Name    :Ecall_SetState
  * Author  :xiaojunzhong
  * Function:发布阶段并记录一次日志；临界区外输出日志，避免延长关中断时间。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_SetState(EcallState_t enState)
{
    portENTER_CRITICAL();
    s_ecAppSts.enState = enState;
    portEXIT_CRITICAL();
    s_ecAppRun.u32PhaseStartMs = osKernelSysTick();
    Hmi_ShowEcallState(enState);
}

/*******************************************************
  * Name    :Ecall_EnterStandby
  * Author  :xiaojunzhong
  * Function:清理本次事件并释放数据处理电源锁；不代表模组允许整机休眠。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_EnterStandby(void)
{
    portENTER_CRITICAL();
    (void)memset(&s_ecAppRun.stRequest, 0, sizeof(s_ecAppRun.stRequest));
    s_ecAppRun.u8DeliveryReady = 0U;
    s_ecAppRun.u8DeliverySuccess = 0U;
    s_ecAppSts.u8SessionActive = 0U;
    portEXIT_CRITICAL();
    Hmi_SetEcallCancelWindow(0U);
    PowerService_SetEcallLock(0U);
    Ecall_SetState(ECALL_STATE_STANDBY);
}

/*******************************************************
  * Name    :Ecall_EnterManualWindow
  * Author  :xiaojunzhong
  * Function:只为纯手动事件开启十秒取消窗口；重复按键不重置起点。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_EnterManualWindow(void)
{
    Hmi_SetEcallCancelWindow(1U);
    Ecall_SetState(ECALL_STATE_MANUAL_CANCEL_WINDOW);
}

/*******************************************************
  * Name    :Ecall_EnterDataPending
  * Author  :xiaojunzhong
  * Function:关闭取消窗口并公开待交付数据；协议未接入时停留此阶段。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_EnterDataPending(void)
{
    Hmi_SetEcallCancelWindow(0U);
    Ecall_SetState(ECALL_STATE_DATA_PENDING);
}

/*******************************************************
  * Name    :Ecall_EnterComplete
  * Author  :xiaojunzhong
  * Function:协议报告已交付后记录结果；不推断模组通话是否建立。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_EnterComplete(void)
{
    Ecall_SetState(ECALL_STATE_COMPLETE);
}

/*******************************************************
  * Name    :Ecall_EnterFailure
  * Author  :xiaojunzhong
  * Function:协议明确拒绝时保存原因；不重拨、不复位模组。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_EnterFailure(void)
{
    portENTER_CRITICAL();
    s_ecAppSts.u16FailureReason = 1U;
    portEXIT_CRITICAL();
    Ecall_SetState(ECALL_STATE_FAILED);
}

/*******************************************************
  * Name    :Ecall_Init
  * Author  :xiaojunzhong
  * Function:任务创建前清零事件及交付邮箱，初始状态为待机。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_Init(void)
{
    (void)memset(&s_ecAppRun, 0, sizeof(s_ecAppRun));
    (void)memset(&s_ecAppSts, 0, sizeof(s_ecAppSts));
    PowerService_SetEcallLock(0U);
}

/*******************************************************
  * Name    :Ecall_OnTriggerMask
  * Author  :xiaojunzhong
  * Function:接受并合并触发；自动/CAN 抢占取消窗口，但不刷新首次事故快照。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_OnTriggerMask(uint8_t u8TriggerMask)
{
    uint8_t u8Automatic; /* 自动来源非零时不允许手动取消。 */
    u8TriggerMask &= ECALL_TRIGGER_MASK_ALL;
    if (u8TriggerMask == 0U)
    {
        return;
    }
    u8Automatic = u8TriggerMask & (ECALL_TRIGGER_MASK_AUTO | ECALL_TRIGGER_MASK_CAN);
    if (s_ecAppSts.u8SessionActive == 0U)
    {
        s_ecAppRun.u32NextSessionId++;
        if (s_ecAppRun.u32NextSessionId == 0U)
        {
            s_ecAppRun.u32NextSessionId = 1U;
        }
        /* 非空目标必可复制；字段是否有效由快照中的有效标志独立描述。
         * 不以缺少 VIN/定位为理由丢弃触发，模组策略由协议接入后处理。 */
        (void)RteSys_ReadMsdSnapshot(&s_ecAppRun.stRequest.stSnapshot);
        portENTER_CRITICAL();
        s_ecAppRun.stRequest.u32SessionId = s_ecAppRun.u32NextSessionId;
        s_ecAppRun.stRequest.u8TriggerMask = u8TriggerMask;
        s_ecAppRun.u8DeliveryReady = 0U;
        s_ecAppSts.u32SessionId = s_ecAppRun.u32NextSessionId;
        s_ecAppSts.u8TriggerMask = u8TriggerMask;
        s_ecAppSts.u8SessionActive = 1U;
        s_ecAppSts.u16FailureReason = 0U;
        portEXIT_CRITICAL();
        PowerService_SetEcallLock(1U);
        if (u8Automatic != 0U)
        {
            Ecall_EnterDataPending();
        }
        else
        {
            Ecall_EnterManualWindow();
        }
    }
    else
    {
        portENTER_CRITICAL();
        s_ecAppSts.u8TriggerMask |= u8TriggerMask;
        s_ecAppRun.stRequest.u8TriggerMask |= u8TriggerMask;
        portEXIT_CRITICAL();
        if ((s_ecAppSts.enState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
            && (u8Automatic != 0U))
        {
            Ecall_EnterDataPending();
        }
    }
}

/*******************************************************
  * Name    :Ecall_OnTrigger
  * Author  :xiaojunzhong
  * Function:将合法的单个来源转换为位掩码；非法枚举不产生任何触发。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_OnTrigger(EcallTrigger_t enTrigger)
{
    switch (enTrigger)
    {
        case ECALL_TRIGGER_MANUAL:
            Ecall_OnTriggerMask(ECALL_TRIGGER_MASK_MANUAL);
            break;
        case ECALL_TRIGGER_AUTO:
            Ecall_OnTriggerMask(ECALL_TRIGGER_MASK_AUTO);
            break;
        case ECALL_TRIGGER_CAN:
            Ecall_OnTriggerMask(ECALL_TRIGGER_MASK_CAN);
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :Ecall_OnManualCancel
  * Author  :xiaojunzhong
  * Function:仅撤销尚未开始交付的纯手动事件；自动事件及待交付事件不取消。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_OnManualCancel(void)
{
    if (s_ecAppSts.enState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
    {
        Ecall_EnterStandby();
    }
}

/*******************************************************
  * Name    :Ecall_ProcessManualWindow
  * Author  :xiaojunzhong
  * Function:通过无符号时间差检查十秒窗口，兼容毫秒计数回绕。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_ProcessManualWindow(void)
{
    if ((osKernelSysTick() - s_ecAppRun.u32PhaseStartMs) >= ECALL_MANUAL_CANCEL_WINDOW_MS)
    {
        Ecall_EnterDataPending();
    }
}

/*******************************************************
  * Name    :Ecall_ProcessDataPending
  * Author  :xiaojunzhong
  * Function:消费一次协议交付结果；没有真实回报时不伪造完成或自动释放电源锁。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_ProcessDataPending(void)
{
    uint8_t u8Ready;   /* 是否存在尚未消费的结果。 */
    uint8_t u8Success; /* 只表示 MCU 到模组的数据交付。 */
    portENTER_CRITICAL();
    u8Ready = s_ecAppRun.u8DeliveryReady;
    u8Success = s_ecAppRun.u8DeliverySuccess;
    portEXIT_CRITICAL();
    if (u8Ready != 0U)
    {
        if (u8Success != 0U)
        {
            Ecall_EnterComplete();
        }
        else
        {
            Ecall_EnterFailure();
        }
    }
}

/*******************************************************
  * Name    :Ecall_Process
  * Author  :xiaojunzhong
  * Function:每周期处理当前大阶段；新进入的阶段在下一周期执行。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_Process(void)
{
    switch (s_ecAppSts.enState)
    {
        case ECALL_STATE_MANUAL_CANCEL_WINDOW:
            Ecall_ProcessManualWindow();
            break;
        case ECALL_STATE_DATA_PENDING:
            Ecall_ProcessDataPending();
            break;
        case ECALL_STATE_COMPLETE:
        case ECALL_STATE_FAILED:
            Ecall_EnterStandby();
            break;
        case ECALL_STATE_STANDBY:
        default:
            break;
    }
}

/*******************************************************
  * Name    :Ecall_ReadPendingRequest
  * Author  :xiaojunzhong
  * Function:任务上下文复制完整事件；读取不消费数据，调用方必须遵守逐字段有效性。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
uint8_t Ecall_ReadPendingRequest(EcallRequest_t *pstRequest)
{
    uint8_t u8Available = 0U; /* 一为有可供复制的待交付事件。 */
    if (pstRequest == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    if ((s_ecAppSts.enState == ECALL_STATE_DATA_PENDING)
        && (s_ecAppRun.u8DeliveryReady == 0U))
    {
        *pstRequest = s_ecAppRun.stRequest;
        u8Available = 1U;
    }
    portEXIT_CRITICAL();
    return u8Available;
}

/*******************************************************
  * Name    :Ecall_ReportDelivery
  * Author  :xiaojunzhong
  * Function:协议服务单写交付邮箱；旧事件号、重复结果和非法布尔值均被拒绝。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
uint8_t Ecall_ReportDelivery(uint32_t u32SessionId, uint8_t u8Success)
{
    uint8_t u8Accepted = 0U; /* 接口是否接受本次请求，不能等同硬件执行成功。 */
    if ((u32SessionId == 0U) || (u8Success > 1U))
    {
        return 0U;
    }
    portENTER_CRITICAL();
    if ((s_ecAppSts.enState == ECALL_STATE_DATA_PENDING)
        && (s_ecAppSts.u32SessionId == u32SessionId)
        && (s_ecAppRun.u8DeliveryReady == 0U))
    {
        s_ecAppRun.u8DeliverySuccess = u8Success;
        s_ecAppRun.u8DeliveryReady = 1U;
        u8Accepted = 1U;
    }
    portEXIT_CRITICAL();
    return u8Accepted;
}

/*******************************************************
  * Name    :Ecall_ReadStatus
  * Author  :xiaojunzhong
  * Function:以短临界区复制稳定业务状态，不暴露文件私有实例。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
uint8_t Ecall_ReadStatus(EcallStatus_t *pstStatus)
{
    if (pstStatus == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    *pstStatus = s_ecAppSts;
    portEXIT_CRITICAL();
    return 1U;
}

/*******************************************************
  * Name    :Ecall_GetState
  * Author  :xiaojunzhong
  * Function:向输入任务提供只读阶段，用于决定长按触发或取消。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
EcallState_t Ecall_GetState(void)
{
    EcallStatus_t stStatus; /* 按值副本，避免任务间直接访问私有变量。 */
    (void)Ecall_ReadStatus(&stStatus);
    return stStatus.enState;
}

/*******************************************************
  * Name    :Ecall_IsSessionActive
  * Author  :xiaojunzhong
  * Function:向诊断和电源业务提供事件活动结论，不代表模组正在通话。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
uint8_t Ecall_IsSessionActive(void)
{
    EcallStatus_t stStatus; /* 调用者持有的稳定状态副本。 */
    (void)Ecall_ReadStatus(&stStatus);
    return stStatus.u8SessionActive;
}
