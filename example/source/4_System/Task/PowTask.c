/*******************************************************
  * Name    :PowTask.c
  * Author  :LoH
  * Function:Power service RTOS task entry and creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "PowerService.h"

/*******************************************************
  * Name    :PowTask_Entry
  * Author  :LoH
  * Function:Execute periodic power service processing.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
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

/*******************************************************
  * Name    :PowTask_Create
  * Author  :LoH
  * Function:Create PowTask with below-normal priority.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowTask_Create(void)
{
    osThreadDef(PowTask, PowTask_Entry, osPriorityBelowNormal, 0U, 256U);
    (void)osThreadCreate(osThread(PowTask), NULL);
}
