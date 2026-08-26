/*******************************************************
  * Name    :AppTask.c
  * Author  :LoH
  * Function:HMI processing and P1 manual eCall trigger task.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "InputService.h"
#include "RteSys.h"
#include "TaskManager.h"

#define APP_TASK_PERIOD_MS                 10U
#define APP_TASK_SOS_TRIGGER_HOLD_MS       2000U
#define APP_TASK_SOS_CANCEL_HOLD_MS        1000U

/*******************************************************
  * Name    :AppTask_Entry
  * Author  :LoH
  * Function:Execute HMI processing and P1 application logic.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void AppTask_Entry(void const *argument)
{
    RteSwitchState_t enCurrentSosState;
    EcallState_t enEcallState;
    uint16_t u16SosTriggerHoldMs = 0U;
    uint16_t u16SosCancelHoldMs = 0U;
    uint8_t u8ManualTriggerSent = 0U;
    uint8_t u8ManualCancelArmed = 0U;

    (void)argument;
    Hmi_ShowTaskStarted("AppTask");

    for (;;)
    {
        Hmi_Process();
        InputService_Process10ms();
        enCurrentSosState = (RteSwitchState_t)RteSys_GetU8Sig(RTESYS_U8_SOS_SWITCH_STATE);
        enEcallState = Ecall_GetState();

        if (enEcallState == ECALL_STATE_STANDBY)
        {
            u16SosCancelHoldMs = 0U;
            u8ManualCancelArmed = 0U;

            if (enCurrentSosState == RTE_SWITCH_RELEASED)
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

            if (enCurrentSosState == RTE_SWITCH_RELEASED)
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
  * Author  :LoH
  * Function:Create AppTask with normal RTOS priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void AppTask_Create(void)
{
    osThreadDef(AppTask, AppTask_Entry, osPriorityNormal, 0U, 384U);
    (void)osThreadCreate(osThread(AppTask), NULL);
}
