/*******************************************************************************
 * @file  TaskManager.c
 * @brief RTOS task creation for the TBOX P1 skeleton.
 ******************************************************************************/
#include "CanService.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
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

void TaskManager_Start(void)
{
    Hmi_Init();
    PowerService_Init();
    GnssService_Init();
    ModemService_Init();
    CanService_Init();
    Ecall_Init();

    PowTask_Create();
    GnssTask_Create();
    ModemTask_Create();
    CanTask_Create();
    MonitoringTask_Create();
    AppTask_Create();
    EcallTask_Create();
}
