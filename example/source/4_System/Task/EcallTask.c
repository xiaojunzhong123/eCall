/*******************************************************************************
 * @file  EcallTask.c
 * @brief RTOS context for the TBOX eCall state machine.
 ******************************************************************************/
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "TaskManager.h"

osMessageQDef(EcallTaskEventQueue, 8U, uint32_t);
static osMessageQId m_xEcallEventQueue;
static uint8_t m_u8PendingTriggerMask = 0U;
static uint8_t m_u8TriggerWakePending = 0U;

static uint8_t EcallTask_GetTriggerMask(EcallTrigger_t enTrigger)
{
    if (enTrigger == ECALL_TRIGGER_MANUAL)
    {
        return ECALL_TRIGGER_MASK_MANUAL;
    }
    else if (enTrigger == ECALL_TRIGGER_AUTO)
    {
        return ECALL_TRIGGER_MASK_AUTO;
    }
    else if (enTrigger == ECALL_TRIGGER_CAN)
    {
        return ECALL_TRIGGER_MASK_CAN;
    }
    else
    {
        return 0U;
    }
}

static void EcallTask_ProcessPendingTriggers(void)
{
    uint8_t u8TriggerMask;

    portENTER_CRITICAL();
    u8TriggerMask = m_u8PendingTriggerMask;
    m_u8PendingTriggerMask = 0U;
    m_u8TriggerWakePending = 0U;
    portEXIT_CRITICAL();

    if (u8TriggerMask != 0U)
    {
        Ecall_OnTriggerMask(u8TriggerMask);
    }
}

static void EcallTask_Entry(void const *argument)
{
    osEvent xEvent;
    EcallMsd_t stMsd;

    (void)argument;
    Hmi_ShowTaskStarted("EcallTask");

    for (;;)
    {
        xEvent = osMessageGet(m_xEcallEventQueue, osWaitForever);
        if (xEvent.status == osEventMessage)
        {
            switch ((EcallTaskEvent_t)xEvent.value.v)
            {
                case ECALL_TASK_EVT_TRIGGER_PENDING:
                    break;
                case ECALL_TASK_EVT_MANUAL_TRIGGER:
                    EcallTask_RequestTrigger(ECALL_TRIGGER_MANUAL);
                    break;
                case ECALL_TASK_EVT_AUTO_TRIGGER:
                    EcallTask_RequestTrigger(ECALL_TRIGGER_AUTO);
                    break;
                case ECALL_TASK_EVT_CAN_TRIGGER:
                    EcallTask_RequestTrigger(ECALL_TRIGGER_CAN);
                    break;
                case ECALL_TASK_EVT_GNSS_READY:
                    (void)Ecall_OnGnssResult(&stMsd, GnssService_GetLatest(&stMsd));
                    break;
                case ECALL_TASK_EVT_GNSS_TIMEOUT:
                    Ecall_OnGnssTimeout();
                    break;
                case ECALL_TASK_EVT_CALL_ACTIVE:
                    Ecall_OnCallActive();
                    break;
                case ECALL_TASK_EVT_CALL_ENDED:
                    Ecall_OnCallEnded();
                    break;
                case ECALL_TASK_EVT_CALL_FAILED:
                    Ecall_OnCallFailed();
                    break;
                default:
                    break;
            }
            EcallTask_ProcessPendingTriggers();
        }
    }
}

void EcallTask_Create(void)
{
    m_xEcallEventQueue = osMessageCreate(osMessageQ(EcallTaskEventQueue), NULL);
    m_u8PendingTriggerMask = 0U;
    m_u8TriggerWakePending = 0U;
    osThreadDef(EcallTask, EcallTask_Entry, osPriorityHigh, 0U, 512U);
    (void)osThreadCreate(osThread(EcallTask), NULL);
}

void EcallTask_PostEvent(EcallTaskEvent_t enEvent)
{
    if (m_xEcallEventQueue != NULL)
    {
        (void)osMessagePut(m_xEcallEventQueue, (uint32_t)enEvent, 0U);
    }
}

void EcallTask_RequestTrigger(EcallTrigger_t enTrigger)
{
    uint8_t u8TriggerMask;
    uint8_t u8NeedWake = 0U;

    u8TriggerMask = EcallTask_GetTriggerMask(enTrigger);
    if (u8TriggerMask == 0U)
    {
        return;
    }

    portENTER_CRITICAL();
    m_u8PendingTriggerMask |= u8TriggerMask;
    if (m_u8TriggerWakePending == 0U)
    {
        m_u8TriggerWakePending = 1U;
        u8NeedWake = 1U;
    }
    portEXIT_CRITICAL();

    if ((u8NeedWake != 0U) && (m_xEcallEventQueue != NULL))
    {
        (void)osMessagePut(m_xEcallEventQueue, ECALL_TASK_EVT_TRIGGER_PENDING, 0U);
    }
}
