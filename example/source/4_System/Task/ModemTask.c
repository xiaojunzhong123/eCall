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
  * Name    :ModemTask_ShowNetworkState
  * Author  :LoH
  * Function:Show modem network state changes through HMI.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void ModemTask_ShowNetworkState(ModemNetworkState_t enNetworkState)
{
    if (enNetworkState == MODEM_NETWORK_SLEEP)
    {
        HMI_LOG_MODEM("SLEEP\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_AT_SYNC)
    {
        HMI_LOG_MODEM("AT_SYNC\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_SIM_CHECK)
    {
        HMI_LOG_MODEM("SIM_CHECK\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_REGISTERING)
    {
        HMI_LOG_MODEM("NETWORK_REGISTERING\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_TCP_CONNECTING)
    {
        HMI_LOG_MODEM("TCP_CONNECTING\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_PLATFORM_REGISTERING)
    {
        HMI_LOG_MODEM("PLATFORM_REGISTERING\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_ONLINE)
    {
        HMI_LOG_MODEM("ONLINE\r\n");
    }
    else if (enNetworkState == MODEM_NETWORK_MSD_UPLOADING)
    {
        HMI_LOG_MODEM("MSD_UPLOADING\r\n");
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
    ModemNetworkState_t enLastNetworkState = MODEM_NETWORK_SLEEP;
    ModemNetworkState_t enCurrentNetworkState;

    (void)argument;
    Hmi_ShowTaskStarted("ModemTask");

    for (;;)
    {
        ModemService_Process();
        enCurrentState = ModemService_GetCallState();
        enCurrentNetworkState = ModemService_GetNetworkState();

        if (enCurrentNetworkState != enLastNetworkState)
        {
            ModemTask_ShowNetworkState(enCurrentNetworkState);
            enLastNetworkState = enCurrentNetworkState;
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
