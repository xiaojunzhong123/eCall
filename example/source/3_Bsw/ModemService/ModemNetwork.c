/*******************************************************
  * Name    :ModemNetwork.c
  * Author  :LoH
  * Function:AC7600 AT synchronization, PDP and TCP connection flow.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#include "ModemNetwork.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_NETWORK_PROCESS_PERIOD_MS     100U
#define MODEM_TCP_COMMAND_LENGTH             80U
#define MODEM_TCP_OPEN_RETRY_MAX             3U
#define MODEM_TCP_OPEN_RETRY_HOLD_MS         5000UL

typedef enum
{
    MODEM_NETWORK_STEP_IDLE = 0U,
    MODEM_NETWORK_STEP_AT_SYNC,
    MODEM_NETWORK_STEP_PDP_OPENING,
    MODEM_NETWORK_STEP_PDP_ACTIVE,
    MODEM_NETWORK_STEP_TCP_CONNECTING
} ModemNetworkStep_t;

typedef enum
{
    MODEM_AT_SYNC_STEP_QUEUE = 0U,
    MODEM_AT_SYNC_STEP_WAIT
} ModemAtSyncStep_t;

typedef enum
{
    MODEM_PDP_STATE_STEP_QUEUE = 0U,
    MODEM_PDP_STATE_STEP_WAIT,
    MODEM_PDP_CLOSE_STEP_QUEUE,
    MODEM_PDP_CLOSE_STEP_WAIT,
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
    MODEM_TCP_RX_MODE_STEP_QUEUE,
    MODEM_TCP_RX_MODE_STEP_WAIT,
    MODEM_TCP_OPEN_STEP_QUEUE,
    MODEM_TCP_OPEN_STEP_WAIT,
    MODEM_TCP_OPEN_RETRY_WAIT
} ModemTcpConnectStep_t;

typedef struct
{
    ModemAtSyncStep_t enAtSyncStep;
    ModemPdpOpenStep_t enPdpOpenStep;
    ModemTcpConnectStep_t enTcpConnectStep;
    uint8_t u8TcpOpenRetryCount;
    uint32_t u32TcpOpenRetryHoldMs;
    char acTcpOpenCommand[MODEM_TCP_COMMAND_LENGTH];
} ModemNetworkRuntime_t;

typedef struct
{
    ModemNetworkStep_t enNetworkStep;
    uint8_t u8TcpConnectedEvent;
    uint8_t u8FailureEvent;
} ModemNetworkStatus_t;

static ModemNetworkRuntime_t s_mNetRun;
static ModemNetworkStatus_t s_mNetSts;

static void ModemNetwork_EnterFailure(void);

/*******************************************************
  * Name    :ModemNetwork_IsNetworkRegistered
  * Author  :LoH
  * Function:Return whether a CEREG response reports home or roaming registration.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemNetwork_IsNetworkRegistered(const char *pcResponse)
{
    uint8_t u8Index = 0U;

    if (pcResponse == 0)
    {
        return 0U;
    }

    while (pcResponse[u8Index] != '\0')
    {
        if ((pcResponse[u8Index] == ',') && ((pcResponse[u8Index + 1U] == '1') || (pcResponse[u8Index + 1U] == '5')))
        {
            return 1U;
        }
        u8Index++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemNetwork_IsPdpOpened
  * Author  :LoH
  * Function:Return whether NETOPEN query reports an active PDP context.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemNetwork_IsPdpOpened(const char *pcResponse)
{
    uint8_t u8Index = 0U;

    if (pcResponse == 0)
    {
        return 0U;
    }

    while (pcResponse[u8Index] != '\0')
    {
        if ((pcResponse[u8Index] == ':') && (pcResponse[u8Index + 1U] == ' ') && (pcResponse[u8Index + 2U] == '1'))
        {
            return 1U;
        }
        u8Index++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemNetwork_IsTcpOpened
  * Author  :LoH
  * Function:Return whether CIPOPEN reports a successful connection on link zero.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemNetwork_IsTcpOpened(const char *pcResponse)
{
    return ((pcResponse != 0) && (strstr(pcResponse, "+CIPOPEN: 0,0") != 0)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemNetwork_EnterTcpOpenRetry
  * Author  :LoH
  * Function:Schedule one bounded retry after a TCP connection failure.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemNetwork_EnterTcpOpenRetry(void)
{
    if (s_mNetRun.u8TcpOpenRetryCount < MODEM_TCP_OPEN_RETRY_MAX)
    {
        s_mNetRun.u8TcpOpenRetryCount++;
        s_mNetRun.u32TcpOpenRetryHoldMs = 0U;
        s_mNetRun.enTcpConnectStep = MODEM_TCP_OPEN_RETRY_WAIT;
        ModemPort_ClearFailure();
        HMI_LOG_MODEM("TCP_OPEN_RETRY\r\n");
    }
    else
    {
        HMI_LOG_MODEM("TCP_OPEN_RETRY_EXHAUSTED\r\n");
        ModemNetwork_EnterFailure();
    }
}

/*******************************************************
  * Name    :ModemNetwork_EnterFailure
  * Author  :LoH
  * Function:Finish the network workflow with one consumable failure event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemNetwork_EnterFailure(void)
{
    s_mNetSts.u8FailureEvent = 1U;
}

/*******************************************************
  * Name    :ModemNetwork_ProcessAtSync
  * Author  :LoH
  * Function:Queue and supervise AC7600, SIM, signal and LTE registration probes.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemNetwork_ProcessAtSync(void)
{
    switch (s_mNetRun.enAtSyncStep)
    {
        case MODEM_AT_SYNC_STEP_QUEUE:
            ModemPort_ClearFailure();
            if ((ModemPort_QueueCommand(MODEM_AT_CMD_BASIC, MODEM_AT_RSP_OK, MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_ECHO_OFF, MODEM_AT_RSP_OK, MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIM_STATUS, MODEM_AT_RSP_SIM_READY, MODEM_AT_CFG_SIM_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIGNAL_QUALITY, MODEM_AT_RSP_SIGNAL_QUALITY, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_CIRCUIT_REGISTRATION, MODEM_AT_RSP_NETWORK_REGISTRATION, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U))
            {
                s_mNetRun.enAtSyncStep = MODEM_AT_SYNC_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("AT_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;

        case MODEM_AT_SYNC_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("NETWORK_PROBE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemNetwork_IsNetworkRegistered(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("NETWORK_REGISTERED\r\n");
                    HMI_LOG_MODEM("PDP_OPENING\r\n");
                    s_mNetSts.enNetworkStep = MODEM_NETWORK_STEP_PDP_OPENING;
                    s_mNetRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_QUEUE;
                }
                else
                {
                    HMI_LOG_MODEM("NETWORK_NOT_REGISTERED\r\n");
                    ModemNetwork_EnterFailure();
                }
            }
            else
            {
                /* ModemPort owns transmission, response parsing and timeout. */
            }
            break;

        default:
            HMI_LOG_MODEM("AT_SYNC_STEP_UNKNOWN\r\n");
            ModemNetwork_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemNetwork_ProcessPdpOpening
  * Author  :LoH
  * Function:Activate PDP and wait for the NETOPEN asynchronous result.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemNetwork_ProcessPdpOpening(void)
{
    switch (s_mNetRun.enPdpOpenStep)
    {
        case MODEM_PDP_STATE_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_STATE, MODEM_AT_RSP_PDP_STATE, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_STATE_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_PDP_STATE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_STATE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemNetwork_IsPdpOpened(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("PDP_CLEANUP\r\n");
                    s_mNetRun.enPdpOpenStep = MODEM_PDP_CLOSE_STEP_QUEUE;
                }
                else
                {
                    s_mNetRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_QUEUE;
                }
            }
            else
            {
            }
            break;
        case MODEM_PDP_CLOSE_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_CLOSE, MODEM_AT_RSP_PDP_CLOSED, MODEM_AT_CFG_PDP_OPEN_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enPdpOpenStep = MODEM_PDP_CLOSE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_CLOSE_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_PDP_CLOSE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_CLOSE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_CLOSED\r\n");
                s_mNetRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_QUEUE;
            }
            else
            {
            }
            break;
        case MODEM_PDP_OPEN_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_OPEN, MODEM_AT_RSP_PDP_OPENED, MODEM_AT_CFG_PDP_OPEN_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_OPEN_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_PDP_OPEN_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_OPEN_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_OPENED\r\n");
                s_mNetRun.enPdpOpenStep = MODEM_PDP_IPADDR_STEP_QUEUE;
            }
            else
            {
            }
            break;
        case MODEM_PDP_IPADDR_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_IPADDR, MODEM_AT_RSP_OK, MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enPdpOpenStep = MODEM_PDP_IPADDR_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_IPADDR_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_PDP_IPADDR_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_IPADDR_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_IPADDR_READY\r\n");
                s_mNetSts.enNetworkStep = MODEM_NETWORK_STEP_PDP_ACTIVE;
            }
            else
            {
            }
            break;
        default:
            HMI_LOG_MODEM("PDP_OPEN_STEP_UNKNOWN\r\n");
            ModemNetwork_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemNetwork_ProcessTcpConnecting
  * Author  :LoH
  * Function:Configure TCP, connect to the platform and apply bounded connection retries.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemNetwork_ProcessTcpConnecting(void)
{
    int i32Length;

    switch (s_mNetRun.enTcpConnectStep)
    {
        case MODEM_TCP_KEEPALIVE_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_KEEPALIVE, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_TCP_KEEPALIVE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_KEEPALIVE_READY\r\n");
                s_mNetRun.enTcpConnectStep = MODEM_TCP_DATA_ECHO_STEP_QUEUE;
            }
            else
            {
            }
            break;
        case MODEM_TCP_DATA_ECHO_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_DATA_ECHO_OFF, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enTcpConnectStep = MODEM_TCP_DATA_ECHO_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_CFG_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_TCP_DATA_ECHO_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_CFG_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_CFG_READY\r\n");
                s_mNetRun.enTcpConnectStep = MODEM_TCP_RX_MODE_STEP_QUEUE;
            }
            else
            {
            }
            break;
        case MODEM_TCP_RX_MODE_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_RX_MODE, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mNetRun.enTcpConnectStep = MODEM_TCP_RX_MODE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_RX_MODE_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_TCP_RX_MODE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_RX_MODE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_RX_MODE_READY\r\n");
                s_mNetRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_QUEUE;
            }
            else
            {
            }
            break;
        case MODEM_TCP_OPEN_STEP_QUEUE:
            i32Length = snprintf(s_mNetRun.acTcpOpenCommand, sizeof(s_mNetRun.acTcpOpenCommand), MODEM_AT_CMD_TCP_OPEN_FORMAT, MODEM_PLATFORM_LINK_ID, MODEM_PLATFORM_SERVER_IP, MODEM_PLATFORM_SERVER_PORT);
            if ((i32Length > 0) && ((uint32_t)i32Length < sizeof(s_mNetRun.acTcpOpenCommand)) && (ModemPort_QueueCommand(s_mNetRun.acTcpOpenCommand, MODEM_AT_RSP_TCP_OPEN, MODEM_AT_CFG_TCP_OPEN_TIMEOUT_MS) != 0U))
            {
                s_mNetRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_OPEN_QUEUE_FAILED\r\n");
                ModemNetwork_EnterFailure();
            }
            break;
        case MODEM_TCP_OPEN_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_OPEN_FAILED\r\n");
                ModemNetwork_EnterTcpOpenRetry();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemNetwork_IsTcpOpened(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("TCP_CONNECTED\r\n");
                    s_mNetSts.u8TcpConnectedEvent = 1U;
                }
                else
                {
                    HMI_LOG_MODEM("TCP_OPEN_RESULT_FAILED\r\n");
                    ModemNetwork_EnterTcpOpenRetry();
                }
            }
            else
            {
            }
            break;
        case MODEM_TCP_OPEN_RETRY_WAIT:
            if (s_mNetRun.u32TcpOpenRetryHoldMs < MODEM_TCP_OPEN_RETRY_HOLD_MS)
            {
                s_mNetRun.u32TcpOpenRetryHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
            }
            if (s_mNetRun.u32TcpOpenRetryHoldMs >= MODEM_TCP_OPEN_RETRY_HOLD_MS)
            {
                s_mNetRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_QUEUE;
            }
            break;
        default:
            HMI_LOG_MODEM("TCP_STEP_UNKNOWN\r\n");
            ModemNetwork_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemNetwork_Init
  * Author  :LoH
  * Function:Initialize private network runtime and status data.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemNetwork_Init(void)
{
    (void)memset(&s_mNetRun, 0, sizeof(s_mNetRun));
    (void)memset(&s_mNetSts, 0, sizeof(s_mNetSts));
    s_mNetSts.enNetworkStep = MODEM_NETWORK_STEP_IDLE;
    s_mNetRun.enAtSyncStep = MODEM_AT_SYNC_STEP_QUEUE;
    s_mNetRun.enPdpOpenStep = MODEM_PDP_STATE_STEP_QUEUE;
    s_mNetRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_QUEUE;
}

/*******************************************************
  * Name    :ModemNetwork_Reset
  * Author  :LoH
  * Function:Stop and clear the private network connection workflow.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemNetwork_Reset(void)
{
    ModemNetwork_Init();
}

/*******************************************************
  * Name    :ModemNetwork_Start
  * Author  :LoH
  * Function:Start a fresh AT synchronization and TCP connection workflow.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemNetwork_Start(void)
{
    ModemNetwork_Init();
    s_mNetSts.enNetworkStep = MODEM_NETWORK_STEP_AT_SYNC;
    HMI_LOG_MODEM("AT_SYNC\r\n");
}

/*******************************************************
  * Name    :ModemNetwork_Process
  * Author  :LoH
  * Function:Dispatch one private modem network phase.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemNetwork_Process(void)
{
    switch (s_mNetSts.enNetworkStep)
    {
        case MODEM_NETWORK_STEP_AT_SYNC:
            ModemNetwork_ProcessAtSync();
            break;
        case MODEM_NETWORK_STEP_PDP_OPENING:
            ModemNetwork_ProcessPdpOpening();
            break;
        case MODEM_NETWORK_STEP_PDP_ACTIVE:
            HMI_LOG_MODEM("PDP_ACTIVE\r\n");
            HMI_LOG_MODEM("TCP_CONNECTING\r\n");
            s_mNetSts.enNetworkStep = MODEM_NETWORK_STEP_TCP_CONNECTING;
            s_mNetRun.enTcpConnectStep = MODEM_TCP_KEEPALIVE_STEP_QUEUE;
            break;
        case MODEM_NETWORK_STEP_TCP_CONNECTING:
            ModemNetwork_ProcessTcpConnecting();
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :ModemNetwork_TakeTcpConnected
  * Author  :LoH
  * Function:Return and clear the private TCP-connected event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemNetwork_TakeTcpConnected(void)
{
    uint8_t u8Result = s_mNetSts.u8TcpConnectedEvent;
    s_mNetSts.u8TcpConnectedEvent = 0U;
    return u8Result;
}

/*******************************************************
  * Name    :ModemNetwork_TakeFailure
  * Author  :LoH
  * Function:Return and clear the private network-failed event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemNetwork_TakeFailure(void)
{
    uint8_t u8Result = s_mNetSts.u8FailureEvent;
    s_mNetSts.u8FailureEvent = 0U;
    return u8Result;
}
