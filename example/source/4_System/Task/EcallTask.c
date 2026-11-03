/*******************************************************
  * Name    :EcallTask.c
  * Author  :xiaojunzhong
  * Function:唯一执行 eCall 应用状态机；事件队列只负责唤醒，不承载事故数据。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Ecall.h"
#include "Hmi.h"
#include "TaskManager.h"

/* 八个轻量通知槽；完整事故数据保存在应用私有静态实例中。
 * 队列满时通知可丢，但来源位和取消位不丢；最多二十毫秒后再次处理。
 * 下列锁存变量由任务生产者写、EcallTask 消费，均在临界区内访问。 */
osMessageQDef(EcallTaskEventQueue, 8U, uint32_t);
static osMessageQId s_ecTaskQueue;       /* 内核创建的通知队列句柄。 */
static uint8_t s_ecTaskPendingMask;      /* 尚未合并至应用的三路来源。 */
static uint8_t s_ecTaskCancelPending;    /* 尚未处理的手动取消意图。 */

/*******************************************************
  * Name    :EcallTask_GetTriggerMask
  * Author  :xiaojunzhong
  * Function:检查来源枚举，非法输入返回零，不生成触发。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static uint8_t EcallTask_GetTriggerMask(EcallTrigger_t enTrigger)
{
    switch (enTrigger)
    {
        case ECALL_TRIGGER_MANUAL:
            return ECALL_TRIGGER_MASK_MANUAL;
        case ECALL_TRIGGER_AUTO:
            return ECALL_TRIGGER_MASK_AUTO;
        case ECALL_TRIGGER_CAN:
            return ECALL_TRIGGER_MASK_CAN;
        default:
            return 0U;
    }
}

/*******************************************************
  * Name    :EcallTask_ProcessPending
  * Author  :xiaojunzhong
  * Function:原子取走锁存意图；先处理触发，再取消，保证自动碰撞优先。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void EcallTask_ProcessPending(void)
{
    uint8_t u8TriggerMask; /* 本周期合并的来源，不依赖队列通知数量。 */
    uint8_t u8Cancel;      /* 本周期是否请求取消。 */
    portENTER_CRITICAL();
    u8TriggerMask = s_ecTaskPendingMask;
    u8Cancel = s_ecTaskCancelPending;
    s_ecTaskPendingMask = 0U;
    s_ecTaskCancelPending = 0U;
    portEXIT_CRITICAL();
    if (u8TriggerMask != 0U)
    {
        Ecall_OnTriggerMask(u8TriggerMask);
    }
    if (u8Cancel != 0U)
    {
        Ecall_OnManualCancel();
    }
}

/*******************************************************
  * Name    :EcallTask_Entry
  * Author  :xiaojunzhong
  * Function:高优先级任务至多等待二十毫秒，再处理意图和当前应用阶段。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void EcallTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("EcallTask");
    for (;;)
    {
        (void)osMessageGet(s_ecTaskQueue, 20U);
        EcallTask_ProcessPending();
        Ecall_Process();
    }
}

/*******************************************************
  * Name    :EcallTask_Create
  * Author  :xiaojunzhong
  * Function:创建通知队列和原有五百一十二字节任务栈；失败时记录并停止创建。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void EcallTask_Create(void)
{
    s_ecTaskPendingMask = 0U;
    s_ecTaskCancelPending = 0U;
    s_ecTaskQueue = osMessageCreate(osMessageQ(EcallTaskEventQueue), NULL);
    if (s_ecTaskQueue == NULL)
    {
        HMI_LOG_ECALL("EVENT_QUEUE_CREATE_FAILED\r\n");
        return;
    }
    osThreadDef(EcallTask, EcallTask_Entry, osPriorityHigh, 0U, 512U);
    if (osThreadCreate(osThread(EcallTask), NULL) == NULL)
    {
        HMI_LOG_ECALL("TASK_CREATE_FAILED\r\n");
    }
}

/*******************************************************
  * Name    :EcallTask_RequestTrigger
  * Author  :xiaojunzhong
  * Function:任务上下文锁存合法来源后非阻塞唤醒；ISR 应先交给下层缓冲。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void EcallTask_RequestTrigger(EcallTrigger_t enTrigger)
{
    uint8_t u8Mask = EcallTask_GetTriggerMask(enTrigger); /* 合法配置或来源的位集合。 */
    if (u8Mask == 0U)
    {
        return;
    }
    portENTER_CRITICAL();
    s_ecTaskPendingMask |= u8Mask;
    portEXIT_CRITICAL();
    if (s_ecTaskQueue != NULL)
    {
        (void)osMessagePut(s_ecTaskQueue, ECALL_TASK_EVT_TRIGGER_PENDING, 0U);
    }
}

/*******************************************************
  * Name    :EcallTask_PostEvent
  * Author  :xiaojunzhong
  * Function:将兼容事件转换为可靠锁存意图；不再接受定位和通话通知。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void EcallTask_PostEvent(EcallTaskEvent_t enEvent)
{
    switch (enEvent)
    {
        case ECALL_TASK_EVT_MANUAL_TRIGGER:
            EcallTask_RequestTrigger(ECALL_TRIGGER_MANUAL);
            break;
        case ECALL_TASK_EVT_AUTO_TRIGGER:
            EcallTask_RequestTrigger(ECALL_TRIGGER_AUTO);
            break;
        case ECALL_TASK_EVT_CAN_TRIGGER:
            EcallTask_RequestTrigger(ECALL_TRIGGER_CAN);
            break;
        case ECALL_TASK_EVT_MANUAL_CANCEL:
            portENTER_CRITICAL();
            s_ecTaskCancelPending = 1U;
            portEXIT_CRITICAL();
            if (s_ecTaskQueue != NULL)
            {
                (void)osMessagePut(s_ecTaskQueue, ECALL_TASK_EVT_MANUAL_CANCEL, 0U);
            }
            break;
        default:
            break;
    }
}
