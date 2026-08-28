/*******************************************************
  * Name    :ModemTask.c
  * Author  :LoH
  * Function:Modem call RTOS task entry and creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "ModemPort.h"
#include "ModemService.h"
#include "RteSys.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :ModemTask_ShowServiceState
  * Author  :LoH
  * Function:Show modem service lifecycle state changes through HMI.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void ModemTask_ShowServiceState(ModemServiceState_t enServiceState)
{
    if (enServiceState == MODEM_SERVICE_SLEEP)
    {
        HMI_LOG_MODEM("SLEEP\r\n");
    }
    else if (enServiceState == MODEM_SERVICE_NETWORK_STARTUP)
    {
        HMI_LOG_MODEM("NETWORK_STARTUP\r\n");
    }
    else if (enServiceState == MODEM_SERVICE_PLATFORM_ACTIVE)
    {
        HMI_LOG_MODEM("PLATFORM_ACTIVE\r\n");
    }
    else
    {
        HMI_LOG_MODEM("NETWORK_UNKNOWN\r\n");
    }
}

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
    ModemServiceState_t enLastServiceState = MODEM_SERVICE_SLEEP;
    ModemServiceState_t enCurrentServiceState;
    RteKl15State_t enLastKl15State = RTE_KL15_OFF;
    RteKl15State_t enCurrentKl15State;

    (void)argument;
    Hmi_ShowTaskStarted("ModemTask");

    for (;;)
    {
        ModemPort_Process();
        ModemService_Process();
        enCurrentState = ModemService_GetCallState();
        enCurrentServiceState = ModemService_GetState();
        enCurrentKl15State = (RteKl15State_t)RteSys_GetU8Sig(RTESYS_U8_KL15_STATE);

        if (enCurrentKl15State != enLastKl15State)
        {
            if (enCurrentKl15State == RTE_KL15_ON)
            {
                HMI_LOG_MODEM("KL15_ON\r\n");
                ModemService_RequestNetworkOnline();
            }
            else
            {
                HMI_LOG_MODEM("KL15_OFF\r\n");
                ModemService_RequestNetworkSleep();
            }
            enLastKl15State = enCurrentKl15State;
        }

        if (enCurrentServiceState != enLastServiceState)
        {
            ModemTask_ShowServiceState(enCurrentServiceState);
            enLastServiceState = enCurrentServiceState;
        }

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
