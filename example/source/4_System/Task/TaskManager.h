/*******************************************************
  * Name    :TaskManager.h
  * Author  :LoH
  * Function:RTOS task creation and eCall event interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include <stdint.h>
#include "Ecall.h"

typedef enum
{
    ECALL_TASK_EVT_TRIGGER_PENDING = 0U,
    ECALL_TASK_EVT_MANUAL_TRIGGER,
    ECALL_TASK_EVT_MANUAL_CANCEL,
    ECALL_TASK_EVT_AUTO_TRIGGER,
    ECALL_TASK_EVT_CAN_TRIGGER,
    ECALL_TASK_EVT_GNSS_READY,
    ECALL_TASK_EVT_GNSS_TIMEOUT,
    ECALL_TASK_EVT_CALL_ACTIVE,
    ECALL_TASK_EVT_CALL_ENDED,
    ECALL_TASK_EVT_CALL_FAILED
} EcallTaskEvent_t;

/* Initialize services and create all TBOX business tasks. */
void TaskManager_Start(void);
/* Post a non-trigger eCall task event. */
void EcallTask_PostEvent(EcallTaskEvent_t enEvent);
/* Latch one manual, automatic or CAN trigger in task context. */
void EcallTask_RequestTrigger(EcallTrigger_t enTrigger);

#endif
