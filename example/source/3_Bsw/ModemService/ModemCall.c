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
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_CALL_PROCESS_PERIOD_MS       100U
#define MODEM_CALL_COMMAND_LENGTH          32U

typedef enum
{
    MODEM_CALL_STEP_IDLE = 0U,
    MODEM_CALL_STEP_QUEUE,
    MODEM_CALL_STEP_WAIT_COMMAND,
    MODEM_CALL_STEP_WAIT_VOICE,
    MODEM_CALL_STEP_CEER_QUEUE,
    MODEM_CALL_STEP_CEER_WAIT,
    MODEM_CALL_STEP_RETRY_WAIT
} ModemCallStep_t;

typedef struct
{
    ModemCallStep_t enStep;
    uint8_t u8VoiceProgressObserved;
    uint8_t u8RetryCount;
    uint32_t u32VoiceWaitMs;
    uint32_t u32RetryHoldMs;
    uint32_t u32RetryElapsedMs;
    char acDialCommand[MODEM_CALL_COMMAND_LENGTH];
} ModemCallRuntime_t;

static ModemCallRuntime_t s_mCallRun;

/*******************************************************
  * Name    :ModemCall_GetCeerCause
  * Author  :LoH
  * Function:Extract the numerical AC7600 release cause from one +CEER response.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemCall_GetCeerCause(const char *pcResponse, uint16_t *pu16Cause)
{
    const char *pcCause;
    uint16_t u16Cause;

    if ((pcResponse == 0) || (pu16Cause == 0))
    {
        return 0U;
    }

    pcCause = strchr(pcResponse, ':');
    if (pcCause == 0)
    {
        return 0U;
    }

    pcCause++;
    while ((*pcCause == ' ') || (*pcCause == '\"'))
    {
        pcCause++;
    }

    if ((*pcCause < '0') || (*pcCause > '9'))
    {
        return 0U;
    }

    u16Cause = 0U;
    while ((*pcCause >= '0') && (*pcCause <= '9'))
    {
        u16Cause = (uint16_t)((u16Cause * 10U) + (uint16_t)(*pcCause - '0'));
        pcCause++;
    }

    *pu16Cause = u16Cause;
    return 1U;
}

/*******************************************************
  * Name    :ModemCall_EnterCeerQuery
  * Author  :LoH
  * Function:Defer terminal classification until AC7600 reports the last release cause.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemCall_EnterCeerQuery(ModemCallState_t *penCallState)
{
    *penCallState = MODEM_CALL_DIALING;
    s_mCallRun.enStep = MODEM_CALL_STEP_CEER_QUEUE;
}

/*******************************************************
  * Name    :ModemCall_GetRetryDelayMs
  * Author  :LoH
  * Function:Return the SRS retry delay for one completed voice connection failure.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint32_t ModemCall_GetRetryDelayMs(uint8_t u8RetryCount)
{
    if (u8RetryCount == 1U)
    {
        return MODEM_AT_CFG_VOICE_RETRY_FIRST_MS;
    }
    else if (u8RetryCount == 2U)
    {
        return MODEM_AT_CFG_VOICE_RETRY_SECOND_MS;
    }
    else
    {
        return MODEM_AT_CFG_VOICE_RETRY_LATER_MS;
    }
}

/*******************************************************
  * Name    :ModemCall_EnterRetryWait
  * Author  :LoH
  * Function:Schedule one independent voice connection retry within the SRS window.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemCall_EnterRetryWait(ModemCallState_t *penCallState)
{
    if (MODEM_AT_CFG_VOICE_RETRY_ENABLED == 0U)
    {
        *penCallState = MODEM_CALL_FAILED;
        s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
        HMI_LOG_MODEM("VOICE_RETRY_DISABLED\r\n");
    }
    else if (s_mCallRun.u32RetryElapsedMs >= MODEM_AT_CFG_VOICE_RETRY_WINDOW_MS)
    {
        *penCallState = MODEM_CALL_FAILED;
        s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
        HMI_LOG_MODEM("VOICE_RETRY_WINDOW_EXPIRED\r\n");
    }
    else
    {
        s_mCallRun.u8RetryCount++;
        s_mCallRun.u32RetryHoldMs = 0U;
        s_mCallRun.u8VoiceProgressObserved = 0U;
        s_mCallRun.enStep = MODEM_CALL_STEP_RETRY_WAIT;
        HMI_LOG_MODEM("VOICE_RETRY_WAIT\r\n");
    }
}

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
        s_mCallRun.u32RetryHoldMs = 0U;
        s_mCallRun.u32RetryElapsedMs = 0U;
        s_mCallRun.u8RetryCount = 0U;
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
    uint16_t u16CeerCause;

    if (penCallState == 0)
    {
        return;
    }

    if (*penCallState == MODEM_CALL_ACTIVE)
    {
        enVoiceEvent = ModemPort_TakeVoiceEvent();
        if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_DISCONNECTED)
        {
            /* Wait for the following VOICE CALL: END or failure URC. */
        }
        else if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_ENDED)
        {
            ModemCall_EnterCeerQuery(penCallState);
        }
        else if (enVoiceEvent == MODEM_PORT_VOICE_EVENT_FAILED)
        {
            ModemCall_EnterCeerQuery(penCallState);
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

    if (s_mCallRun.u32RetryElapsedMs < MODEM_AT_CFG_VOICE_RETRY_WINDOW_MS)
    {
        s_mCallRun.u32RetryElapsedMs += MODEM_CALL_PROCESS_PERIOD_MS;
    }

    if (s_mCallRun.u32RetryElapsedMs >= MODEM_AT_CFG_VOICE_RETRY_WINDOW_MS)
    {
        *penCallState = MODEM_CALL_FAILED;
        s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
        HMI_LOG_MODEM("VOICE_RETRY_WINDOW_EXPIRED\r\n");
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
                ModemCall_EnterRetryWait(penCallState);
            }
            else
            {
                s_mCallRun.enStep = MODEM_CALL_STEP_WAIT_COMMAND;
            }
            break;

        case MODEM_CALL_STEP_WAIT_COMMAND:
            if (ModemPort_HasFailure() != 0U)
            {
                ModemCall_EnterRetryWait(penCallState);
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
                     || (enVoiceEvent == MODEM_PORT_VOICE_EVENT_DISCONNECTED)
                     || (enVoiceEvent == MODEM_PORT_VOICE_EVENT_FAILED))
            {
                ModemCall_EnterCeerQuery(penCallState);
            }
            else
            {
                if (s_mCallRun.u8VoiceProgressObserved == 0U)
                {
                    s_mCallRun.u32VoiceWaitMs += MODEM_CALL_PROCESS_PERIOD_MS;
                    if (s_mCallRun.u32VoiceWaitMs >= MODEM_AT_CFG_DIAL_TIMEOUT_MS)
                    {
                        ModemCall_EnterRetryWait(penCallState);
                    }
                }
                else
                {
                    /* An observed dialing or alerting state owns completion until a terminal URC arrives. */
                }
            }
            break;

        case MODEM_CALL_STEP_CEER_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(MODEM_AT_CMD_EXTENDED_ERROR_REPORT,
                                       MODEM_AT_RSP_EXTENDED_ERROR_REPORT,
                                       MODEM_AT_CFG_EXTENDED_ERROR_TIMEOUT_MS) == 0U)
            {
                ModemCall_EnterRetryWait(penCallState);
            }
            else
            {
                s_mCallRun.enStep = MODEM_CALL_STEP_CEER_WAIT;
            }
            break;

        case MODEM_CALL_STEP_CEER_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                ModemCall_EnterRetryWait(penCallState);
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if ((ModemCall_GetCeerCause(ModemPort_GetLastMatchedResponse(), &u16CeerCause) != 0U)
                    && ((u16CeerCause == 16U) || (u16CeerCause == 31U)))
                {
                    *penCallState = MODEM_CALL_ENDED;
                    s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
                    HMI_LOG_MODEM("VOICE_RELEASE_NORMAL\r\n");
                }
                else
                {
                    HMI_LOG_MODEM("VOICE_RELEASE_RETRY\r\n");
                    ModemCall_EnterRetryWait(penCallState);
                }
            }
            else
            {
                /* Wait for the AC7600 extended release report. */
            }
            break;

        case MODEM_CALL_STEP_RETRY_WAIT:
            s_mCallRun.u32RetryHoldMs += MODEM_CALL_PROCESS_PERIOD_MS;
            if (s_mCallRun.u32RetryHoldMs >= ModemCall_GetRetryDelayMs(s_mCallRun.u8RetryCount))
            {
                s_mCallRun.u32VoiceWaitMs = 0U;
                s_mCallRun.u8VoiceProgressObserved = 0U;
                s_mCallRun.enStep = MODEM_CALL_STEP_QUEUE;
                HMI_LOG_MODEM("VOICE_RETRY_START\r\n");
            }
            else
            {
                /* Wait for the next independent voice connection attempt. */
            }
            break;

        default:
            *penCallState = MODEM_CALL_FAILED;
            s_mCallRun.enStep = MODEM_CALL_STEP_IDLE;
            break;
    }
}
