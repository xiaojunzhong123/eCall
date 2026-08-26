/*******************************************************************************
 * @file  PowTask.c
 * @brief RTOS context for power service processing.
 ******************************************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "PowerService.h"

static void PowTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("PowTask");

    for (;;)
    {
        PowerService_Process();
        osDelay(100U);
    }
}

void PowTask_Create(void)
{
    osThreadDef(PowTask, PowTask_Entry, osPriorityBelowNormal, 0U, 256U);
    (void)osThreadCreate(osThread(PowTask), NULL);
}
