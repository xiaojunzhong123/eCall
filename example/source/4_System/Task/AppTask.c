/*******************************************************
  * Name    :AppTask.c
  * Author  :xiaojunzhong
  * Function:任务层周期调度日志、输入、精简自检、通用输出及 SOS 判定。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
/* 流程阅读说明：
 * 每十毫秒：日志 -> 输入消抖 -> 点火有效时自检 -> 通用输出 -> SOS 意图。
 * 待机时 SOS 按住两秒产生手动触发；松开后再次按住一秒可在十秒窗口内取消。
 * 自动/CAN 触发不受手动取消控制；最终仲裁均由 EcallTask 执行。
 */

#include "cmsis_os.h"
#include "Hmi.h"
#include "EcallSelfCheck.h"
#include "InputService.h"
#include "OutputService.h"
#include "RteSys.h"
#include "TaskManager.h"

#define APP_TASK_PERIOD_MS                 10U
#define APP_TASK_SOS_TRIGGER_HOLD_MS       2000U
#define APP_TASK_SOS_CANCEL_HOLD_MS        1000U

/*******************************************************
  * Name    :AppTask_Entry
  * Author  :xiaojunzhong
  * Function:调度日志和输入，点火有效时处理自检，继续通用输出和 SOS 逻辑。
  * Version :V1.4.0
  * Data    :2026.11.05
  * Input   :argument 未使用；由调度器进入。
  * Output  :无返回值，任务持续运行。
  * Caller  :AppTask，普通优先级任务上下文。
  * Precond :输入、输出及事件服务已初始化。
  * Failure :未点火暂停自检消费并保留故障；未知来源仍保留待接入标记。
  * Effect  :点火有效才消费自检邮箱；灯请求保持，原有事件逻辑和延时不变。
*******************************************************/
static void AppTask_Entry(void const *argument)
{
    RteSwitchState_t enCurrentSosState; /* 本周期消抖后的 SOS 输入。 */
    EcallState_t enEcallState; /* 本周期 MCU 事件阶段。 */
    uint16_t u16SosTriggerHoldMs = 0U; /* SOS 连续按住累计时间，毫秒，达到两秒触发。 */
    uint16_t u16SosCancelHoldMs = 0U; /* 取消用的再次长按时间，毫秒，达到一秒取消。 */
    uint8_t u8ManualTriggerSent = 0U; /* 本次按住已发送触发，松开后才重新使能。 */
    uint8_t u8ManualCancelArmed = 0U; /* 触发按键松开后才允许再次按下取消。 */

    (void)argument;
    Hmi_ShowTaskStarted("AppTask");

    for (;;)
    {
        Hmi_Process();
        InputService_Process10ms();
        /* 点火有效后才消费自检结果；熄火不清除已有故障和灯请求。 */
        if (RteSys_GetSwitchState(RTESYS_SWITCH_KL15) == RTE_SWITCH_ACTIVE)
        {
            Ecall_SelfCheckProcess();
        }
        OutputService_Process10ms();
        enCurrentSosState = RteSys_GetSwitchState(RTESYS_SWITCH_SOS);
        enEcallState = Ecall_GetState();

        if (enEcallState == ECALL_STATE_STANDBY)
        {
            u16SosCancelHoldMs = 0U;
            u8ManualCancelArmed = 0U;

            if (enCurrentSosState == RTE_SWITCH_INACTIVE)
            {
                u16SosTriggerHoldMs = 0U;
                u8ManualTriggerSent = 0U;
            }
            else if (u8ManualTriggerSent == 0U)
            {
                if (u16SosTriggerHoldMs < APP_TASK_SOS_TRIGGER_HOLD_MS)
                {
                    u16SosTriggerHoldMs += APP_TASK_PERIOD_MS;
                }

                if (u16SosTriggerHoldMs >= APP_TASK_SOS_TRIGGER_HOLD_MS)
                {
                    HMI_LOG_APP("SOS manual trigger\r\n");
                    EcallTask_RequestTrigger(ECALL_TRIGGER_MANUAL); //SOS手动报警
                    u8ManualTriggerSent = 1U;
                }
            }
            else
            {
                /* Wait for SOS release before allowing another manual trigger. */
            }
        }
        else if (enEcallState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
        {
            u16SosTriggerHoldMs = 0U;

            if (enCurrentSosState == RTE_SWITCH_INACTIVE)
            {
                u8ManualCancelArmed = 1U;
                u16SosCancelHoldMs = 0U;
            }
            else if (u8ManualCancelArmed != 0U)
            {
                if (u16SosCancelHoldMs < APP_TASK_SOS_CANCEL_HOLD_MS)
                {
                    u16SosCancelHoldMs += APP_TASK_PERIOD_MS;
                }

                if (u16SosCancelHoldMs >= APP_TASK_SOS_CANCEL_HOLD_MS)
                {
                    HMI_LOG_APP("SOS manual cancel\r\n");
                    EcallTask_PostEvent(ECALL_TASK_EVT_MANUAL_CANCEL);
                    u8ManualCancelArmed = 0U;
                    u16SosCancelHoldMs = 0U;
                }
            }
            else
            {
                /* Initial trigger press must be released before cancel is armed. */
            }
        }
        else
        {
            u16SosTriggerHoldMs = 0U;
            u16SosCancelHoldMs = 0U;
            u8ManualCancelArmed = 0U;
        }

        /* P1 placeholder: future UART command, LED and watchdog. */
        osDelay(APP_TASK_PERIOD_MS);
    }
}

/*******************************************************
  * Name    :AppTask_Create
  * Author  :xiaojunzhong
  * Function:创建普通优先级输入及通用输出任务，沿用原栈配置。
  * Version :V1.1.0
  * Data    :2026.11.05
  * Input   :无；启动阶段调用一次。
  * Output  :无返回值，沿用现有任务创建方式。
  * Caller  :TaskManager_Start，内核启动前。
  * Precond :服务初值已建立，RTOS 堆可用。
  * Failure :任务创建失败行为沿用现有实现，本轮不扩展处理。
  * Effect  :创建 AppTask，不新增任务、ISR 或缓冲区。
*******************************************************/
void AppTask_Create(void)
{
    osThreadDef(AppTask, AppTask_Entry, osPriorityNormal, 0U, 384U);
    (void)osThreadCreate(osThread(AppTask), NULL);
}
