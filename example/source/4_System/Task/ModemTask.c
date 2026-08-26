/*******************************************************************************
 * @file  ModemTask.c
 * @brief RTOS context for modem call service processing.
 ******************************************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "ModemService.h"
#include "TaskManager.h"

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

void ModemTask_Create(void)
{
    osThreadDef(ModemTask, ModemTask_Entry, osPriorityAboveNormal, 0U, 512U);
    (void)osThreadCreate(osThread(ModemTask), NULL);
}
