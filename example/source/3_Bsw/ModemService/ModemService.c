/*******************************************************
  * Name    :ModemService.c
  * Author  :LoH
  * Function:Modem emergency call service implementation.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#include "ModemService.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_NETWORK_PROCESS_PERIOD_MS     100U
#define MODEM_NETWORK_SLEEP_HOLD_MS         300000UL
#define MODEM_TCP_COMMAND_LENGTH             80U

typedef enum
{
    MODEM_AT_SYNC_STEP_QUEUE = 0U,
    MODEM_AT_SYNC_STEP_WAIT
} ModemAtSyncStep_t;

typedef enum
{
    MODEM_PDP_STATE_STEP_QUEUE = 0U,
    MODEM_PDP_STATE_STEP_WAIT,
    MODEM_PDP_OPEN_STEP_QUEUE,
    MODEM_PDP_OPEN_STEP_WAIT,
    MODEM_PDP_IPADDR_STEP_QUEUE,
    MODEM_PDP_IPADDR_STEP_WAIT
} ModemPdpOpenStep_t;

typedef enum
{
    MODEM_TCP_KEEPALIVE_STEP_QUEUE = 0U,
    MODEM_TCP_KEEPALIVE_STEP_WAIT,
    MODEM_TCP_DATA_ECHO_STEP_QUEUE,
    MODEM_TCP_DATA_ECHO_STEP_WAIT,
    MODEM_TCP_OPEN_STEP_QUEUE,
    MODEM_TCP_OPEN_STEP_WAIT
} ModemTcpConnectStep_t;

typedef struct
{
    ModemAtSyncStep_t enAtSyncStep;
    ModemPdpOpenStep_t enPdpOpenStep;
    ModemTcpConnectStep_t enTcpConnectStep;
    uint16_t u16NetworkProcessCount;
    uint16_t u16CallProcessCount;
    uint8_t u8EmergencyRequestPending;
    uint8_t u8NetworkSleepRequested;
    uint32_t u32NetworkSleepHoldMs;
    char acTcpOpenCommand[MODEM_TCP_COMMAND_LENGTH];
    EcallMsd_t stLastMsd;
} ModemServiceRuntime_t;

typedef struct
{
    ModemCallState_t enCallState;
    ModemNetworkState_t enNetworkState;
} ModemServiceStatus_t;

static ModemServiceRuntime_t s_mSvcRun;
static ModemServiceStatus_t s_mSvcSts;

static void ModemService_EnterNetworkSleep(void);

/*******************************************************
  * Name    :ModemService_IsNetworkRegistered
  * Author  :LoH
  * Function:Return whether a CEREG response reports home or roaming registration.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static uint8_t ModemService_IsNetworkRegistered(const char *pcResponse)
{
    uint8_t u8Index = 0U;

    if (pcResponse == 0)
    {
        return 0U;
    }

    while (pcResponse[u8Index] != '\0')
    {
        if ((pcResponse[u8Index] == ',')
            && ((pcResponse[u8Index + 1U] == '1') || (pcResponse[u8Index + 1U] == '5')))
        {
            return 1U;
        }
        u8Index++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemService_IsPdpOpened
  * Author  :LoH
  * Function:Return whether NETOPEN query reports an active PDP context.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static uint8_t ModemService_IsPdpOpened(const char *pcResponse)
{
    uint8_t u8Index = 0U;

    if (pcResponse == 0)
    {
        return 0U;
    }

    while (pcResponse[u8Index] != '\0')
    {
        if ((pcResponse[u8Index] == ':')
            && (pcResponse[u8Index + 1U] == ' ')
            && (pcResponse[u8Index + 2U] == '1'))
        {
            return 1U;
        }
        u8Index++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemService_IsTcpOpened
  * Author  :LoH
  * Function:Return whether CIPOPEN reports a successful connection on link zero.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static uint8_t ModemService_IsTcpOpened(const char *pcResponse)
{
    return ((pcResponse != 0) && (strstr(pcResponse, "+CIPOPEN: 0,0") != 0)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_EnterAtSync
  * Author  :LoH
  * Function:Enter the AC7600 basic modem, SIM and network probe phase.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_EnterAtSync(void)
{
    s_mSvcSts.enNetworkState = MODEM_NETWORK_AT_SYNC;
    s_mSvcRun.enAtSyncStep = MODEM_AT_SYNC_STEP_QUEUE;
    s_mSvcRun.u16NetworkProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemService_EnterPdpOpening
  * Author  :LoH
  * Function:Enter PDP activation after successful LTE registration.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_EnterPdpOpening(void)
{
    s_mSvcSts.enNetworkState = MODEM_NETWORK_PDP_OPENING;
    s_mSvcRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_QUEUE;
    s_mSvcRun.u16NetworkProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemService_EnterTcpConnecting
  * Author  :LoH
  * Function:Enter the AC7600 TCP parameter and connection phase.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_EnterTcpConnecting(void)
{
    s_mSvcSts.enNetworkState = MODEM_NETWORK_TCP_CONNECTING;
    s_mSvcRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_QUEUE;
    s_mSvcRun.u16NetworkProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemService_EnterNetworkSleep
  * Author  :LoH
  * Function:Stop current network work and report a pending eCall failure.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_EnterNetworkSleep(void)
{
    s_mSvcSts.enNetworkState = MODEM_NETWORK_SLEEP;
    s_mSvcRun.enAtSyncStep = MODEM_AT_SYNC_STEP_QUEUE;
    s_mSvcRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_QUEUE;
    s_mSvcRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_QUEUE;
    s_mSvcRun.u16NetworkProcessCount = 0U;

    if (s_mSvcRun.u8EmergencyRequestPending != 0U)
    {
        s_mSvcSts.enCallState = MODEM_CALL_FAILED;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessSleep
  * Author  :LoH
  * Function:Wait for KL15 or eCall to start the network workflow.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessSleep(void)
{
    /* Requests enter AT_SYNC through the public service APIs. */
}

/*******************************************************
  * Name    :ModemService_ProcessAtSync
  * Author  :LoH
  * Function:Queue and supervise AC7600, SIM, signal and LTE registration probes.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessAtSync(void)
{
    switch (s_mSvcRun.enAtSyncStep)
    {
        case MODEM_AT_SYNC_STEP_QUEUE:
            ModemPort_ClearFailure();
            if ((ModemPort_QueueCommand(MODEM_AT_CMD_BASIC, MODEM_AT_RSP_OK, MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_ECHO_OFF, MODEM_AT_RSP_OK, MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIM_STATUS, MODEM_AT_RSP_SIM_READY, MODEM_AT_CFG_SIM_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIGNAL_QUALITY, MODEM_AT_RSP_SIGNAL_QUALITY, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_CIRCUIT_REGISTRATION, MODEM_AT_RSP_NETWORK_REGISTRATION, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U))
            {
                s_mSvcRun.enAtSyncStep = MODEM_AT_SYNC_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("AT_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_AT_SYNC_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("NETWORK_PROBE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsNetworkRegistered(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("NETWORK_REGISTERED\r\n");
                    ModemService_EnterPdpOpening();
                }
                else
                {
                    HMI_LOG_MODEM("NETWORK_NOT_REGISTERED\r\n");
                    ModemService_EnterNetworkSleep();
                }
            }
            else
            {
                /* ModemPort owns transmission, response parsing and timeout. */
            }
            break;

        default:
            HMI_LOG_MODEM("AT_SYNC_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPdpOpening
  * Author  :LoH
  * Function:Activate PDP and wait for the NETOPEN asynchronous result.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessPdpOpening(void)
{
    switch (s_mSvcRun.enPdpOpenStep)
    {
        case MODEM_PDP_STATE_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_STATE, MODEM_AT_RSP_PDP_STATE, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_STATE_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PDP_STATE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_STATE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsPdpOpened(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("PDP_ALREADY_OPEN\r\n");
                    s_mSvcRun.enPdpOpenStep = MODEM_PDP_IPADDR_STEP_QUEUE;
                }
                else
                {
                    s_mSvcRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_QUEUE;
                }
            }
            else
            {
                /* Wait for +NETOPEN: <state> or a ModemPort timeout. */
            }
            break;

        case MODEM_PDP_OPEN_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_OPEN, MODEM_AT_RSP_PDP_OPENED, MODEM_AT_CFG_PDP_OPEN_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_OPEN_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PDP_OPEN_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_OPEN_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_OPENED\r\n");
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_IPADDR_STEP_QUEUE;
            }
            else
            {
                /* Wait for +NETOPEN: 0 or a ModemPort timeout. */
            }
            break;

        case MODEM_PDP_IPADDR_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_IPADDR, MODEM_AT_RSP_OK, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_IPADDR_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_IPADDR_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PDP_IPADDR_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_IPADDR_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_IPADDR_READY\r\n");
                s_mSvcSts.enNetworkState = MODEM_NETWORK_PDP_ACTIVE;
            }
            else
            {
                /* Wait for AT+IPADDR response and final OK. */
            }
            break;

        default:
            HMI_LOG_MODEM("PDP_OPEN_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPdpActive
  * Author  :LoH
  * Function:Hold verified PDP state until TCP parameters are available.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessPdpActive(void)
{
    HMI_LOG_MODEM("PDP_ACTIVE\r\n");
    ModemService_EnterTcpConnecting();
}

/*******************************************************
  * Name    :ModemService_ProcessTcpConnecting
  * Author  :LoH
  * Function:Reserve TCP connection phase for CIPCFG and CIPOPEN processing.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessTcpConnecting(void)
{
    int i32Length;

    switch (s_mSvcRun.enTcpConnectStep)
    {
        case MODEM_TCP_KEEPALIVE_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_KEEPALIVE, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_TCP_KEEPALIVE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_READY\r\n");
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_DATA_ECHO_STEP_QUEUE;
            }
            else
            {
                /* Wait for CTCPKA final OK. */
            }
            break;

        case MODEM_TCP_DATA_ECHO_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_DATA_ECHO_OFF, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_DATA_ECHO_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_CFG_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_TCP_DATA_ECHO_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_CFG_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_CFG_READY\r\n");
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_QUEUE;
            }
            else
            {
                /* Wait for CIPCFG final OK. */
            }
            break;

        case MODEM_TCP_OPEN_STEP_QUEUE:
            i32Length = snprintf(s_mSvcRun.acTcpOpenCommand, sizeof(s_mSvcRun.acTcpOpenCommand), MODEM_AT_CMD_TCP_OPEN_FORMAT,
                                 MODEM_PLATFORM_LINK_ID, MODEM_PLATFORM_SERVER_IP, MODEM_PLATFORM_SERVER_PORT);
            if ((i32Length > 0)
                && ((uint32_t)i32Length < sizeof(s_mSvcRun.acTcpOpenCommand))
                && (ModemPort_QueueCommand(s_mSvcRun.acTcpOpenCommand, MODEM_AT_RSP_TCP_OPEN, MODEM_AT_CFG_TCP_OPEN_TIMEOUT_MS) != 0U))
            {
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_OPEN_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_TCP_OPEN_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_OPEN_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsTcpOpened(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("TCP_CONNECTED\r\n");
                    s_mSvcSts.enNetworkState = MODEM_NETWORK_PLATFORM_REGISTERING;
                }
                else
                {
                    HMI_LOG_MODEM("TCP_OPEN_RESULT_FAILED\r\n");
                    ModemService_EnterNetworkSleep();
                }
            }
            else
            {
                /* Wait for +CIPOPEN: 0,0 or a ModemPort timeout. */
            }
            break;

        default:
            HMI_LOG_MODEM("TCP_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPlatformRegistering
  * Author  :LoH
  * Function:Reserve platform registration phase for confirmed protocol frames.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessPlatformRegistering(void)
{
    /* Future sequence: CIPSEND prompt, registration payload and confirmation. */
}

/*******************************************************
  * Name    :ModemService_ProcessOnline
  * Author  :LoH
  * Function:Maintain online state and apply the KL15 delayed sleep policy.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessOnline(void)
{
    if (s_mSvcRun.u8EmergencyRequestPending != 0U)
    {
        s_mSvcSts.enNetworkState = MODEM_NETWORK_MSD_UPLOADING;
        s_mSvcRun.u16NetworkProcessCount = 0U;
    }
    else if ((s_mSvcRun.u8NetworkSleepRequested != 0U)
             && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
                 || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
                 || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        if (s_mSvcRun.u32NetworkSleepHoldMs < MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            s_mSvcRun.u32NetworkSleepHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
        }

        if (s_mSvcRun.u32NetworkSleepHoldMs >= MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            HMI_LOG_MODEM("KL15_OFF_SLEEP\r\n");
            ModemService_EnterNetworkSleep();
            s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        }
    }
    else
    {
        /* Future AC7600 heartbeat is driven by the ModemTask period. */
    }
}

/*******************************************************
  * Name    :ModemService_ProcessMsdUploading
  * Author  :LoH
  * Function:Reserve MSD upload handling until CIPSEND is implemented.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessMsdUploading(void)
{
    /* Future sequence: CIPSEND prompt, binary MSD payload and confirmation. */
}

/*******************************************************
  * Name    :ModemService_ProcessCallState
  * Author  :LoH
  * Function:Reserve call timing until AC7600 dial and retry handling is added.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemService_ProcessCallState(void)
{
    if (s_mSvcSts.enCallState == MODEM_CALL_DIALING)
    {
        s_mSvcRun.u16CallProcessCount++;
        if (s_mSvcRun.u16CallProcessCount >= 3U)
        {
            s_mSvcRun.u16CallProcessCount = 0U;
            s_mSvcSts.enCallState = MODEM_CALL_ACTIVE;
        }
    }
    else if (s_mSvcSts.enCallState == MODEM_CALL_ACTIVE)
    {
        s_mSvcRun.u16CallProcessCount++;
        if (s_mSvcRun.u16CallProcessCount >= 10U)
        {
            s_mSvcRun.u16CallProcessCount = 0U;
            s_mSvcSts.enCallState = MODEM_CALL_ENDED;
        }
    }
    else
    {
        /* No processing is required for other call states. */
    }
}

/*******************************************************
  * Name    :ModemService_Init
  * Author  :LoH
  * Function:Reset modem network and call states before task scheduling.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_Init(void)
{
    (void)memset(&s_mSvcRun, 0, sizeof(s_mSvcRun));
    s_mSvcSts.enCallState = MODEM_CALL_IDLE;
    s_mSvcSts.enNetworkState = MODEM_NETWORK_SLEEP;
    s_mSvcRun.enAtSyncStep = MODEM_AT_SYNC_STEP_QUEUE;
    s_mSvcRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_QUEUE;
    s_mSvcRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_QUEUE;
    ModemPort_Init();
}

/*******************************************************
  * Name    :ModemService_RequestEmergencyCall
  * Author  :LoH
  * Function:Latch MSD and request emergency network service.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd)
{
    if ((pstMsd != 0)
        && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        s_mSvcRun.stLastMsd = *pstMsd;
        s_mSvcRun.u16CallProcessCount = 0U;
        s_mSvcRun.u8EmergencyRequestPending = 1U;
        s_mSvcRun.u8NetworkSleepRequested = 0U;
        if (s_mSvcSts.enNetworkState == MODEM_NETWORK_ONLINE)
        {
            s_mSvcSts.enNetworkState = MODEM_NETWORK_MSD_UPLOADING;
            s_mSvcRun.u16NetworkProcessCount = 0U;
        }
        else if (s_mSvcSts.enNetworkState == MODEM_NETWORK_SLEEP)
        {
            ModemService_EnterAtSync();
        }
        else
        {
            /* Existing network work continues before MSD upload. */
        }
        s_mSvcSts.enCallState = MODEM_CALL_PREPARING;
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkOnline
  * Author  :LoH
  * Function:Request normal KL15-controlled network online state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkOnline(void)
{
    s_mSvcRun.u8NetworkSleepRequested = 0U;
    s_mSvcRun.u32NetworkSleepHoldMs = 0U;

    if (s_mSvcSts.enNetworkState == MODEM_NETWORK_SLEEP)
    {
        ModemService_EnterAtSync();
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkSleep
  * Author  :LoH
  * Function:Request normal network sleep after pending emergency work.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkSleep(void)
{
    if (s_mSvcRun.u8NetworkSleepRequested == 0U)
    {
        s_mSvcRun.u8NetworkSleepRequested = 1U;
        s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        HMI_LOG_MODEM("KL15_OFF_HOLD\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_Process
  * Author  :LoH
  * Function:Dispatch one modem network phase and one independent call phase.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_Process(void)
{
    switch (s_mSvcSts.enNetworkState)
    {
        case MODEM_NETWORK_SLEEP:
            ModemService_ProcessSleep();
            break;

        case MODEM_NETWORK_AT_SYNC:
            ModemService_ProcessAtSync();
            break;

        case MODEM_NETWORK_PDP_OPENING:
            ModemService_ProcessPdpOpening();
            break;

        case MODEM_NETWORK_PDP_ACTIVE:
            ModemService_ProcessPdpActive();
            break;

        case MODEM_NETWORK_TCP_CONNECTING:
            ModemService_ProcessTcpConnecting();
            break;

        case MODEM_NETWORK_PLATFORM_REGISTERING:
            ModemService_ProcessPlatformRegistering();
            break;

        case MODEM_NETWORK_ONLINE:
            ModemService_ProcessOnline();
            break;

        case MODEM_NETWORK_MSD_UPLOADING:
            ModemService_ProcessMsdUploading();
            break;

        default:
            HMI_LOG_MODEM("NETWORK_STATE_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }

    ModemService_ProcessCallState();
}

/*******************************************************
  * Name    :ModemService_GetCallState
  * Author  :LoH
  * Function:Return modem call state for EcallTask notification.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemCallState_t ModemService_GetCallState(void)
{
    return s_mSvcSts.enCallState;
}

/*******************************************************
  * Name    :ModemService_GetNetworkState
  * Author  :LoH
  * Function:Return current modem network state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemNetworkState_t ModemService_GetNetworkState(void)
{
    return s_mSvcSts.enNetworkState;
}
