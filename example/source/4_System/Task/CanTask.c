/*******************************************************************************
 * @file  CanTask.c
 * @brief RTOS context for CAN service processing.
 ******************************************************************************/
#include "cmsis_os.h"
#include "CanService.h"
#include "Hmi.h"
#include "TaskManager.h"

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

void CanTask_Create(void)
{
    osThreadDef(CanTask, CanTask_Entry, osPriorityRealtime, 0U, 512U);
    (void)osThreadCreate(osThread(CanTask), NULL);
}
