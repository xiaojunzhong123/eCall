/*******************************************************
  * Name    :AppTask.c
  * Author  :xiaojunzhong
  * Function:十毫秒调度输入、自检、提示及 SOS 长按判定。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
/* 流程阅读说明：
 * 每十毫秒：日志 -> 输入消抖 -> 自检证据 -> 故障确认 -> 提示仲裁 -> 物理输出。
 * 待机时 SOS 按住两秒产生手动触发；松开后再次按住一秒可在十秒窗口内取消。
 * 自动/CAN 触发不受手动取消控制；最终仲裁均由 EcallTask 执行。
 */

#include "cmsis_os.h"
#include "Hmi.h"
#include "EcallDiag.h"
#include "EcallDiagService.h"
#include "EcallHmi.h"
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
  * Function:调度输入、无冲突诊断、提示仲裁和既有 SOS 逻辑，不等待 AT 或显示完成。
  * Version :V1.1.0
  * Data    :2026.11.01
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
        EcallDiagService_Process();
        Ecall_DiagProcess();
        Ecall_HmiProcess();
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
                    EcallTask_RequestTrigger(ECALL_TRIGGER_MANUAL);
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
  * Function:创建普通优先级输入、自检及提示任务，沿用原栈配置。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void AppTask_Create(void)
{
    osThreadDef(AppTask, AppTask_Entry, osPriorityNormal, 0U, 384U);
    (void)osThreadCreate(osThread(AppTask), NULL);
}
