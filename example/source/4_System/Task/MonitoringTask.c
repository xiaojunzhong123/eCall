/*******************************************************
  * Name    :MonitoringTask.c
  * Author  :LoH
  * Function:Reserved IMU monitoring and automatic trigger RTOS task.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"

/*******************************************************
  * Name    :MonitoringTask_Entry
  * Author  :LoH
  * Function:Reserve monitoring cycle for collision judgement.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
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

/*******************************************************
  * Name    :MonitoringTask_Create
  * Author  :LoH
  * Function:Create MonitoringTask with above-normal priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void MonitoringTask_Create(void)
{
    osThreadDef(MonitoringTask, MonitoringTask_Entry, osPriorityAboveNormal, 0U, 384U);
    (void)osThreadCreate(osThread(MonitoringTask), NULL);
}
