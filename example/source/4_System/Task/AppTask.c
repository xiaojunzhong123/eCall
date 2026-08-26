/*******************************************************
  * Name    :AppTask.c
  * Author  :LoH
  * Function:HMI processing and P1 manual eCall trigger task.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :AppTask_Entry
  * Author  :LoH
  * Function:Execute HMI processing and P1 application logic.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void AppTask_Entry(void const *argument)
{
    uint8_t u8MockTriggerSent = 0U;
    uint16_t u16ElapsedMs = 0U;

    (void)argument;
    Hmi_ShowTaskStarted("AppTask");

    for (;;)
    {
        Hmi_Process();

        if ((u8MockTriggerSent == 0U) && (u16ElapsedMs >= 2000U))
        {
            EcallTask_PostEvent(ECALL_TASK_EVT_MANUAL_TRIGGER);
            u8MockTriggerSent = 1U;
        }

        /* P1 placeholder: future SOS key, UART command, LED and watchdog. */
        osDelay(10U);
        u16ElapsedMs += 10U;
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
