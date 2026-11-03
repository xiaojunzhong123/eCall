/*******************************************************
  * Name    :MonitoringTask.c
  * Author  :xiaojunzhong
  * Function:本机碰撞算法调度预留，不伪造自动触发。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"

/*******************************************************
  * Name    :MonitoringTask_Entry
  * Author  :xiaojunzhong
  * Function:保留二十毫秒碰撞监测调度入口；当前没有 IMU 判定实现。
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
  * Author  :xiaojunzhong
  * Function:创建高于普通级的监测任务，不自动生成碰撞事件。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void MonitoringTask_Create(void)
{
    osThreadDef(MonitoringTask, MonitoringTask_Entry, osPriorityAboveNormal, 0U, 384U);
    (void)osThreadCreate(osThread(MonitoringTask), NULL);
}
