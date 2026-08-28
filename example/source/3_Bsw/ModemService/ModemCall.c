/*******************************************************
  * Name    :ModemCall.c
  * Author  :LoH
  * Function:Private modem call state processing implementation.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#include "ModemCall.h"
#include <stdio.h>
#include <string.h>
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_CALL_PROCESS_PERIOD_MS       100U
#define MODEM_CALL_COMMAND_LENGTH          32U

typedef enum
{
    MODEM_CALL_STEP_IDLE = 0U,
    MODEM_CALL_STEP_QUEUE,
    MODEM_CALL_STEP_WAIT_COMMAND,
    MODEM_CALL_STEP_WAIT_VOICE
} ModemCallStep_t;

typedef struct
{
    ModemCallStep_t enStep;
    uint8_t u8VoiceProgressObserved;
    uint32_t u32VoiceWaitMs;
    char acDialCommand[MODEM_CALL_COMMAND_LENGTH];
} ModemCallRuntime_t;

static ModemCallRuntime_t s_mCallRun;

/*******************************************************
  * Name    :ModemCall_Init
  * Author  :LoH
  * Function:Initialize the private call processing runtime.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Init(void)
{
    (void)memset(&s_mCallRun, 0, sizeof(s_mCallRun));
}

/*******************************************************
  * Name    :ModemCall_Reset
  * Author  :LoH
  * Function:Clear the private call delay counter for one new call request.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Reset(void)
{
    (void)memset(&s_mCallRun, 0, sizeof(s_mCallRun));
}

/*******************************************************
  * Name    :ModemCall_Start
  * Author  :LoH
  * Function:Start one AC7600 voice-call transaction after MSD confirmation.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Start(ModemCallState_t *penCallState)
{
    if (penCallState != 0)
    {
        s_mCallRun.enStep = MODEM_CALL_STEP_QUEUE;
        s_mCallRun.u8VoiceProgressObserved = 0U;
        s_mCallRun.u32VoiceWaitMs = 0U;
        *penCallState = MODEM_CALL_DIALING;
    }
}

/*******************************************************
  * Name    :ModemCall_Process
  * Author  :LoH
  * Function:Send ATD and classify AC7600 voice-call command and URC results.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemCall_Process(ModemCallState_t *penCallState)
{
    ModemPortVoiceEvent_t enVoiceEvent;
    int i32Length;

    if (penCallState == 0)
    {
        return;
    }

    if (*penCallState == MODEM_CALL_ACTIVE)
    {
        enVoiceEvent = ModemPort_TakeVoiceEvent();
        if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_ENDED)
        {
            *penCallState = MODEM_CALL_ENDED;
        }
        else if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_FAILED)
        {
            *penCallState = MODEM_CALL_FAILED;
        }
        else
        {
            /* Keep the active voice call until AC7600 reports its result. */
        }
        return;
    }

    if (*penCallState != MODEM_CALL_DIALING)
    {
        return;
    }

    switch (s_mCallRun.enStep)
    {
        case MODEM_CALL_STEP_QUEUE:
            ModemPort_ClearFailure();
            i32Length = snprintf(s_mCallRun.acDialCommand,
                                 sizeof(s_mCallRun.acDialCommand),
                                 MODEM_AT_CMD_DIAL_FORMAT,
                                 MODEM_PLATFORM_DIAL_NUMBER);
            if ((i32Length <= 0)
                || ((uint32_t)i32Length >= sizeof(s_mCallRun.acDialCommand))
                || (ModemPort_QueueCommand(s_mCallRun.acDialCommand,
                                           MODEM_AT_RSP_OK,
                                           MODEM_AT_CFG_DIAL_TIMEOUT_MS) == 0U))
            {
                *penCallState = MODEM_CALL_FAILED;
                s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            }
            else
            {
                s_mCallRun.enStep = MODEM_CALL_STEP_WAIT_COMMAND;
            }
            break;

        case MODEM_CALL_STEP_WAIT_COMMAND:
            if (ModemPort_HasFailure() != 0U)
            {
                *penCallState = MODEM_CALL_FAILED;
                s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mCallRun.u32VoiceWaitMs = 0U;
                s_mCallRun.enStep = MODEM_CALL_STEP_WAIT_VOICE;
            }
            else
            {
                /* Wait for the ATD command response. */
            }
            break;

        case MODEM_CALL_STEP_WAIT_VOICE:
            enVoiceEvent = ModemPort_TakeVoiceEvent();
            if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_PROGRESS)
            {
                s_mCallRun.u8VoiceProgressObserved = 1U;
            }
            else if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_CONNECTED)
            {
                *penCallState = MODEM_CALL_ACTIVE;
                s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            }
            else if ((enVoiceEvent == MODEM_PORT_VOICE_EVENT_ENDED)
                     || (enVoiceEvent == MODEM_PORT_VOICE_EVENT_FAILED))
            {
                *penCallState = MODEM_CALL_FAILED;
                s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            }
            else
            {
                if (s_mCallRun.u8VoiceProgressObserved == 0U)
                {
                    s_mCallRun.u32VoiceWaitMs += MODEM_CALL_PROCESS_PERIOD_MS;
                    if (s_mCallRun.u32VoiceWaitMs >= MODEM_AT_CFG_DIAL_TIMEOUT_MS)
                    {
                        *penCallState = MODEM_CALL_FAILED;
                        s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
                    }
                }
                else
                {
                    /* An observed dialing or alerting state owns completion until a terminal URC arrives. */
                }
            }
            break;

        default:
            *penCallState = MODEM_CALL_FAILED;
            s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            break;
    }
}
