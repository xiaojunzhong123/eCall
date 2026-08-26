/*******************************************************
  * Name    :ModemTask.c
  * Author  :LoH
  * Function:Modem call RTOS task entry and creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "ModemService.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :ModemTask_Entry
  * Author  :LoH
  * Function:Process modem service and report call states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void ModemTask_Entry(void const *argument)
{
    ModemCallState_t enLastState = MODEM_CALL_IDLE;
    ModemCallState_t enCurrentState;

    (void)argument;
    Hmi_ShowTaskStarted("ModemTask");

    for (;;)
    {
        ModemService_Process();
        enCurrentState = ModemService_GetCallState();

        if (enCurrentState != enLastState)
        {
            if (enCurrentState == MODEM_CALL_ACTIVE)
            {
                EcallTask_PostEvent(ECALL_TASK_EVT_CALL_ACTIVE);
            }
            else if (enCurrentState == MODEM_CALL_ENDED)
            {
                EcallTask_PostEvent(ECALL_TASK_EVT_CALL_ENDED);
            }
            else if (enCurrentState == MODEM_CALL_FAILED)
            {
                EcallTask_PostEvent(ECALL_TASK_EVT_CALL_FAILED);
            }
            else
            {
                /* No eCall event is associated with this state. */
            }
            enLastState = enCurrentState;
        }
        osDelay(100U);
    }
}

/*******************************************************
  * Name    :ModemTask_Create
  * Author  :LoH
  * Function:Create ModemTask with above-normal priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void ModemTask_Create(void)
{
    osThreadDef(ModemTask, ModemTask_Entry, osPriorityAboveNormal, 0U, 512U);
    (void)osThreadCreate(osThread(ModemTask), NULL);
}
