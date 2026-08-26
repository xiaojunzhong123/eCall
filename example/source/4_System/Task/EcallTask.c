/*******************************************************************************
 * @file  EcallTask.c
 * @brief RTOS context for the TBOX eCall state machine.
 ******************************************************************************/
#include "cmsis_os.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "TaskManager.h"

osMessageQDef(EcallTaskEventQueue, 8U, uint32_t);
static osMessageQId m_xEcallEventQueue;

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
                case ECALL_TASK_EVT_MANUAL_TRIGGER:
                    Ecall_OnTrigger(ECALL_TRIGGER_MANUAL);
                    break;
                case ECALL_TASK_EVT_AUTO_TRIGGER:
                    Ecall_OnTrigger(ECALL_TRIGGER_AUTO);
                    break;
                case ECALL_TASK_EVT_CAN_TRIGGER:
                    Ecall_OnTrigger(ECALL_TRIGGER_CAN);
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
        }
    }
}

void EcallTask_Create(void)
{
    m_xEcallEventQueue = osMessageCreate(osMessageQ(EcallTaskEventQueue), NULL);
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
