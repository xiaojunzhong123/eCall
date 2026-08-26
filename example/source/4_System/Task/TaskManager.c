/*******************************************************
  * Name    :TaskManager.c
  * Author  :LoH
  * Function:TBOX P1 RTOS service initialization and task creation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "CanService.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "InputService.h"
#include "ModemService.h"
#include "PowerService.h"
#include "TaskManager.h"

extern void EcallTask_Create(void);
extern void CanTask_Create(void);
extern void ModemTask_Create(void);
extern void GnssTask_Create(void);
extern void MonitoringTask_Create(void);
extern void AppTask_Create(void);
extern void PowTask_Create(void);

/*******************************************************
  * Name    :TaskManager_Start
  * Author  :LoH
  * Function:Initialize services and create business tasks.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void TaskManager_Start(void)
{
    Hmi_Init();
    PowerService_Init();
    GnssService_Init();
    ModemService_Init();
    CanService_Init();
    InputService_Init();
    Ecall_Init();

    PowTask_Create();
    GnssTask_Create();
    ModemTask_Create();
    CanTask_Create();
    MonitoringTask_Create();
    AppTask_Create();
    EcallTask_Create();
}
