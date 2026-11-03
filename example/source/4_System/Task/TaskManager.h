/*******************************************************
  * Name    :TaskManager.h
  * Author  :xiaojunzhong
  * Function:按服务初始化、共享状态建立、任务创建的顺序启动 MCU 业务。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__
#include <stdint.h>
#include "Ecall.h"
typedef enum
{
    ECALL_TASK_EVT_TRIGGER_PENDING = 0U, /* 仅用于唤醒，来源保存在独立锁存位。 */
    ECALL_TASK_EVT_MANUAL_TRIGGER,      /* 兼容手动触发提交入口。 */
    ECALL_TASK_EVT_MANUAL_CANCEL,       /* 取消意图单独锁存，队列满不会丢失。 */
    ECALL_TASK_EVT_AUTO_TRIGGER,        /* 预留本机碰撞生产者入口。 */
    ECALL_TASK_EVT_CAN_TRIGGER          /* CAN 碰撞服务入口。 */
} EcallTaskEvent_t;
/* 所有服务初始化结束后再创建任务，建立明确的并发起点。 */
void TaskManager_Start(void);
/* 任务上下文提交事件，不阻塞；取消与触发均有独立锁存保护。 */
void EcallTask_PostEvent(EcallTaskEvent_t enEvent);
/* 合并触发来源并尝试唤醒；任务周期还会检查锁存位。 */
void EcallTask_RequestTrigger(EcallTrigger_t enTrigger);
#endif
