/*******************************************************
  * Name    :GnssTask.c
  * Author  :LoH
  * Function:GNSS snapshot RTOS task entry and creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :GnssTask_Entry
  * Author  :LoH
  * Function:Update GNSS data and report eCall GNSS result.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void GnssTask_Entry(void const *argument)
{
    uint8_t u8RequestReported = 0U;

    (void)argument;
    Hmi_ShowTaskStarted("GnssTask");

    for (;;)
    {
        GnssService_Process();

        if ((Ecall_GetState() == ECALL_STATE_WAIT_GNSS) && (u8RequestReported == 0U))
        {
            EcallTask_PostEvent(ECALL_TASK_EVT_GNSS_READY);
            u8RequestReported = 1U;
        }
        else if (Ecall_IsSessionActive() == 0U)
        {
            u8RequestReported = 0U;
        }
        else
        {
            /* GNSS request was already reported for this session. */
        }
        osDelay(100U);
    }
}

/*******************************************************
  * Name    :GnssTask_Create
  * Author  :LoH
  * Function:Create GnssTask with below-normal priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void GnssTask_Create(void)
{
    osThreadDef(GnssTask, GnssTask_Entry, osPriorityBelowNormal, 0U, 384U);
    (void)osThreadCreate(osThread(GnssTask), NULL);
}
