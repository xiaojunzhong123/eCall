/*******************************************************************************
 * @file  GnssTask.c
 * @brief RTOS context for GNSS snapshot service processing.
 ******************************************************************************/
#include "cmsis_os.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "TaskManager.h"

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

void GnssTask_Create(void)
{
    osThreadDef(GnssTask, GnssTask_Entry, osPriorityBelowNormal, 0U, 384U);
    (void)osThreadCreate(osThread(GnssTask), NULL);
}
