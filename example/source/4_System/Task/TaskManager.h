/*******************************************************************************
 * @file  TaskManager.h
 * @brief RTOS task creation and eCall task event interface.
 ******************************************************************************/
#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <stdint.h>

typedef enum
{
    ECALL_TASK_EVT_MANUAL_TRIGGER = 0U,
    ECALL_TASK_EVT_AUTO_TRIGGER,
    ECALL_TASK_EVT_CAN_TRIGGER,
    ECALL_TASK_EVT_GNSS_READY,
    ECALL_TASK_EVT_GNSS_TIMEOUT,
    ECALL_TASK_EVT_CALL_ACTIVE,
    ECALL_TASK_EVT_CALL_ENDED,
    ECALL_TASK_EVT_CALL_FAILED
} EcallTaskEvent_t;

void TaskManager_Start(void);
void EcallTask_PostEvent(EcallTaskEvent_t enEvent);

#endif
