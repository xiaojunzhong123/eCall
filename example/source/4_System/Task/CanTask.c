/*******************************************************
  * Name    :CanTask.c
  * Author  :LoH
  * Function:CAN service RTOS task entry and creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "CanService.h"
#include "Hmi.h"
#include "TaskManager.h"

/*******************************************************
  * Name    :CanTask_Entry
  * Author  :LoH
  * Function:Process CAN service and forward eCall triggers.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void CanTask_Entry(void const *argument)
{
    (void)argument;
    Hmi_ShowTaskStarted("CanTask");

    for (;;)
    {
        CanService_Process();
        if (CanService_GetMockEcallTrigger() != 0U)
        {
            EcallTask_PostEvent(ECALL_TASK_EVT_CAN_TRIGGER);
        }
        osDelay(10U);
    }
}

/*******************************************************
  * Name    :CanTask_Create
  * Author  :LoH
  * Function:Create CanTask with real-time RTOS priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void CanTask_Create(void)
{
    osThreadDef(CanTask, CanTask_Entry, osPriorityRealtime, 0U, 512U);
    (void)osThreadCreate(osThread(CanTask), NULL);
}
