/*******************************************************************************
 * @file  MonitoringTask.c
 * @brief RTOS context reserved for IMU and automatic trigger monitoring.
 ******************************************************************************/
#include "cmsis_os.h"
#include "Hmi.h"

static void MonitoringTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("MonitoringTask");

    for (;;)
    {
        /* P1 placeholder: future IMU collision and rollover judgement. */
        osDelay(20U);
    }
}

void MonitoringTask_Create(void)
{
    osThreadDef(MonitoringTask, MonitoringTask_Entry, osPriorityAboveNormal, 0U, 384U);
    (void)osThreadCreate(osThread(MonitoringTask), NULL);
}
