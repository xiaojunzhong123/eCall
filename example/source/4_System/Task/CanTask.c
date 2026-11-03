/*******************************************************
  * Name    :CanTask.c
  * Author  :xiaojunzhong
  * Function:十毫秒调度 CAN 服务并转发碰撞锁存。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "CanService.h"
#include "Hmi.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :CanTask_Entry
  * Author  :xiaojunzhong
  * Function:每十毫秒调度 CAN 服务并将碰撞锁存转交可靠触发入口。
  * Version :V1.1.0
  * Data    :2026.11.01
*******************************************************/
static void CanTask_Entry(void const *argument)
{
    uint8_t u8Trigger; /* 本周期取走的一次碰撞意图。 */

    (void)argument;
    Hmi_ShowTaskStarted("CanTask");

    for (;;)
    {
        CanService_Process();
        u8Trigger = CanService_TakeEcallTrigger();
        if (u8Trigger != 0U)
        {
            EcallTask_RequestTrigger(ECALL_TRIGGER_CAN);
        }
        osDelay(10U);
    }
}

/*******************************************************
  * Name    :CanTask_Create
  * Author  :xiaojunzhong
  * Function:创建实时优先级 CAN 任务，沿用原栈配置。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void CanTask_Create(void)
{
    osThreadDef(CanTask, CanTask_Entry, osPriorityRealtime, 0U, 512U);
    (void)osThreadCreate(osThread(CanTask), NULL);
}
