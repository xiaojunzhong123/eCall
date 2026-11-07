/*******************************************************
  * Name    :Ecall.c
  * Author  :xiaojunzhong
  * Function:负责触发合并、手动取消、事故快照及 MCU 数据交付生命周期。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#include "Ecall.h"
#include "EcallSelfCheck.h"
#include "MsdService.h"
#include "PowerService.h"
#include "Hmi.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <string.h>

/* EcallTask 独占过程数据；跨任务共享的请求及交付标志统一由临界区保护。
 * 只冻结车辆数据，不管理模组定位、标准数据编码和平台上传，也不使用动态内存。
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
  * Name    :Ecall_SendMSDModule
  * Author  :xiaojunzhong
  * Function:进入待交付阶段并发送一次首次事故快照；不等待模组ACK。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :无，使用EcallTask持有的冻结请求。
  * Output  :无返回值；发送失败记录一次日志，仍保留事件。
  * Caller  :仅EcallTask的三条阶段入口。
  * Precond :请求已冻结且事件电源锁已建立，服务已初始化。
  * Failure :UART返回不证明交付；失败无自动重试，ACK未接入时一直等待。
  * Effect  :关闭取消窗口，进入DATA_PENDING，临界区外调用服务发送。
*******************************************************/
static void Ecall_SendMSDModule(void)
{
    Hmi_SetEcallCancelWindow(0U);
    Ecall_SetState(ECALL_STATE_DATA_PENDING);
    if (MsdService_Send(&s_ecAppRun.stRequest.stVehicleData, s_ecAppRun.stRequest.u8TriggerMask, s_ecAppRun.stRequest.u32SessionId) == 0U)
    {
        HMI_LOG_ECALL("MSD UART send failed; delivery remains pending");
    }
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
  * Function:上电初始化事件、MSD发送服务和交付邮箱，并初始化精简自检。
  * Version :V1.3.0
  * Data    :2026.11.05
  * Input   :无；由 TaskManager_Start 在任务创建前调用一次。
  * Output  :无返回值；事件待机，自检默认正常且全部来源待接入。
  * Caller  :main 启动上下文，不在运行期用于清除自检故障。
  * Precond :电源锁、日志和输出服务已初始化。
  * Failure :未知检查不阻塞紧急事件，不宣称真实自检通过。
  * Effect  :释放事件锁并提交 D29 熄灭请求，不改变模组供电。
*******************************************************/
void Ecall_Init(void)
{
    (void)memset(&s_ecAppRun, 0, sizeof(s_ecAppRun));
    (void)memset(&s_ecAppSts, 0, sizeof(s_ecAppSts));
    PowerService_SetEcallLock(0U);
    MsdService_Init();
    Ecall_SelfCheckInit(); //上电自检初始化
}

/*******************************************************
  * Name    :Ecall_OnTriggerMask
  * Author  :xiaojunzhong
  * Function:触发报警，冻结车辆数据并进入事件状态机
  		自动报警优先级高于手动报警，不会进入10s取消窗口
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_OnTriggerMask(uint8_t u8TriggerMask)
{
    uint8_t u8Automatic = 0; /* 自动报警标志，非零时不允许手动取消。 */
	
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
         * 不以缺少车辆配置为理由丢弃触发，模组策略由协议接入后处理。 */
        (void)RteSys_ReadVehicleSnapshot(&s_ecAppRun.stRequest.stVehicleData); //冻结车辆数据
        portENTER_CRITICAL();
        s_ecAppRun.stRequest.u32SessionId = s_ecAppRun.u32NextSessionId;
        s_ecAppRun.stRequest.u8TriggerMask = u8TriggerMask; //触发源
        s_ecAppRun.u8DeliveryReady = 0U;
        s_ecAppSts.u32SessionId = s_ecAppRun.u32NextSessionId;
        s_ecAppSts.u8TriggerMask = u8TriggerMask;
        s_ecAppSts.u8SessionActive = 1U;
        s_ecAppSts.u16FailureReason = 0U;
        portEXIT_CRITICAL();
        PowerService_SetEcallLock(1U);
        if (u8Automatic != 0U)
        {
        	//这里已经准备好车辆数据
            Ecall_SendMSDModule();
        }
        else
        {
            Ecall_EnterManualWindow();
        }
    }
    else //10s内又收到自动/CAN碰撞报警，立即结束取消窗口，提前进入车辆数据交付状态
    {
        portENTER_CRITICAL();
        s_ecAppSts.u8TriggerMask |= u8TriggerMask;
        s_ecAppRun.stRequest.u8TriggerMask |= u8TriggerMask;
        portEXIT_CRITICAL();
        if ((s_ecAppSts.enState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
            && (u8Automatic != 0U))
        {
            Ecall_SendMSDModule();
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
  * Function:手动取消报警
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
  * Function:检查十秒手动报警取消窗口
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void Ecall_ProcessManualWindow(void)
{
    if ((osKernelSysTick() - s_ecAppRun.u32PhaseStartMs) >= ECALL_MANUAL_CANCEL_WINDOW_MS)
    {
        Ecall_SendMSDModule();
    }
}

/*******************************************************
  * Name    :Ecall_ProcessDataPending
  * Author  :xiaojunzhong
  * Function:非阻塞查询ACK预留口并消费既有交付邮箱，未收到结果持续等待。
  * Version :V1.0.0
  * Data    :2026.11.06
  * Input   :无。
  * Output  :无返回值。
  * Caller  :EcallTask当前阶段处理。
  * Precond :ACK服务返回结果时已完成序号校验及事件号关联。
  * Failure :接口当前返回零；旧号和非法结果由交付邮箱拒绝，不自动超时。
  * Effect  :消费真实结果后进入完成或失败，当前周期不处理新阶段。
*******************************************************/
static void Ecall_ProcessDataPending(void)
{
    MsdServiceAck_t stAck; /* 后续服务提供的最终交付结果，零返回时禁止读取。 */
    uint8_t u8Ready;   /* 是否存在尚未消费的结果。 */
    uint8_t u8Success; /* 只表示 MCU 到模组的数据交付。 */
    if (MsdService_ReadAck(&stAck) != 0U)
    {
        (void)Ecall_ReportDelivery(stAck.u32SessionId, stAck.u8Success);
    }
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
  * Function:eCall状态机周期处理入口
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void Ecall_Process(void)
{
    switch (s_ecAppSts.enState)
    {
        case ECALL_STATE_MANUAL_CANCEL_WINDOW://手动窗口判断是否满10S
            Ecall_ProcessManualWindow();
            break;
        case ECALL_STATE_DATA_PENDING://检查是否交付车辆数据
            Ecall_ProcessDataPending();
            break;
        case ECALL_STATE_COMPLETE://清理本次会话，回到待机
        case ECALL_STATE_FAILED://清理会话，记录失败原因
            Ecall_EnterStandby();
            break;
        case ECALL_STATE_STANDBY: //待机，等待新的触发请求
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
