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
#define MODEM_TCP_OPEN_RETRY_MAX             3U
#define MODEM_TCP_OPEN_RETRY_HOLD_MS         5000UL
#define MODEM_PLATFORM_JSON_LENGTH           64U
#define MODEM_PLATFORM_FRAME_LENGTH          (MODEM_PLATFORM_JSON_LENGTH + 5U)
#define MODEM_PLATFORM_COMMAND_LENGTH        32U
#define MODEM_PLATFORM_RX_MAX_LENGTH          750U
#define MODEM_PLATFORM_REPLY_TYPE_LENGTH       32U
#define MODEM_PLATFORM_REPLY_LOG_LENGTH        56U

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

typedef enum
{
    MODEM_PLATFORM_REGISTER_STEP_PREPARE = 0U,
    MODEM_PLATFORM_REGISTER_STEP_QUEUE,
    MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_REGISTER_STEP_SEND,
    MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT
} ModemPlatformRegisterStep_t;

typedef enum
{
    MODEM_PLATFORM_RX_STEP_IDLE = 0U,
    MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE,
    MODEM_PLATFORM_RX_STEP_LENGTH_WAIT,
    MODEM_PLATFORM_RX_STEP_DATA_QUEUE,
    MODEM_PLATFORM_RX_STEP_DATA_WAIT
} ModemPlatformReceiveStep_t;

typedef enum
{
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT = 0U,
    MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE,
    MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE,
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_HEARTBEAT_STEP_SEND,
    MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT
} ModemPlatformHeartbeatStep_t;

typedef struct
{
    ModemAtSyncStep_t enAtSyncStep;
    ModemPdpOpenStep_t enPdpOpenStep;
    ModemTcpConnectStep_t enTcpConnectStep;
    ModemPlatformRegisterStep_t enPlatformRegisterStep;
    ModemPlatformReceiveStep_t enPlatformReceiveStep;
    ModemPlatformHeartbeatStep_t enPlatformHeartbeatStep;
    uint16_t u16NetworkProcessCount;
    uint16_t u16CallProcessCount;
    uint8_t u8EmergencyRequestPending;
    uint8_t u8NetworkSleepRequested;
    uint8_t u8TcpOpenRetryCount;
    uint32_t u32NetworkSleepHoldMs;
    uint32_t u32TcpOpenRetryHoldMs;
    uint32_t u32HeartbeatHoldMs;
    char acTcpOpenCommand[MODEM_TCP_COMMAND_LENGTH];
    char acRegisterCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acHeartbeatCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveLengthCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveDataCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    uint8_t au8RegisterFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8HeartbeatFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8PlatformReceiveFrame[MODEM_PLATFORM_RX_MAX_LENGTH];
    uint16_t u16RegisterFrameLength;
    uint16_t u16HeartbeatFrameLength;
    uint16_t u16PlatformReceiveLength;
    uint16_t u16PlatformReceiveFrameLength;
    uint8_t u8PlatformReceiveLinkId;
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
  * Name    :ModemService_EnterPlatformRegistering
  * Author  :LoH
  * Function:Enter the candidate AECS platform registration phase.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_EnterPlatformRegistering(void)
{
    s_mSvcSts.enNetworkState = MODEM_NETWORK_PLATFORM_REGISTERING;
    s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mSvcRun.u32HeartbeatHoldMs = 0U;
    s_mSvcRun.u16NetworkProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemService_EnterTcpOpenRetry
  * Author  :LoH
  * Function:Schedule one bounded retry after a TCP connection failure.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_EnterTcpOpenRetry(void)
{
    if (s_mSvcRun.u8TcpOpenRetryCount < MODEM_TCP_OPEN_RETRY_MAX)
    {
        s_mSvcRun.u8TcpOpenRetryCount++;
        s_mSvcRun.u32TcpOpenRetryHoldMs = 0U;
        s_mSvcRun.enTcpConnectStep = MODEM_TCP_OPEN_RETRY_WAIT;
        ModemPort_ClearFailure();
        HMI_LOG_MODEM("TCP_OPEN_RETRY\r\n");
    }
    else
    {
        HMI_LOG_MODEM("TCP_OPEN_RETRY_EXHAUSTED\r\n");
        ModemService_EnterNetworkSleep();
    }
}

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
  * Name    :ModemService_IsDataSendComplete
  * Author  :LoH
  * Function:Return whether CIPSEND confirms every requested registration byte.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_IsDataSendComplete(const char *pcResponse, uint16_t u16ExpectedLength)
{
    unsigned int u32LinkId;
    unsigned int u32RequestedLength;
    unsigned int u32ConfirmedLength;

    if ((pcResponse == 0)
        || (sscanf(pcResponse, "+CIPSEND: %u,%u,%u", &u32LinkId, &u32RequestedLength, &u32ConfirmedLength) != 3))
    {
        return 0U;
    }

    return ((u32LinkId == MODEM_PLATFORM_LINK_ID)
            && (u32RequestedLength == u16ExpectedLength)
            && (u32ConfirmedLength == u16ExpectedLength)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_PrepareRegisterFrame
  * Author  :LoH
  * Function:Build the candidate AECS registration frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_PrepareRegisterFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mSvcRun.au8RegisterFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                              MODEM_PLATFORM_REGISTER_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mSvcRun.au8RegisterFrame[0] = MODEM_PLATFORM_MSG_REGISTER;
    s_mSvcRun.au8RegisterFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mSvcRun.au8RegisterFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mSvcRun.au8RegisterFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mSvcRun.au8RegisterFrame[4] = (uint8_t)u32JsonLength;
    s_mSvcRun.u16RegisterFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mSvcRun.acRegisterCommand, sizeof(s_mSvcRun.acRegisterCommand), MODEM_AT_CMD_TCP_SEND_FORMAT,
                                 MODEM_PLATFORM_LINK_ID, s_mSvcRun.u16RegisterFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mSvcRun.acRegisterCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_PrepareHeartbeatFrame
  * Author  :LoH
  * Function:Build the candidate AECS heartbeat frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_PrepareHeartbeatFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mSvcRun.au8HeartbeatFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                              MODEM_PLATFORM_HEARTBEAT_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mSvcRun.au8HeartbeatFrame[0] = MODEM_PLATFORM_MSG_HEARTBEAT;
    s_mSvcRun.au8HeartbeatFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mSvcRun.au8HeartbeatFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mSvcRun.au8HeartbeatFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mSvcRun.au8HeartbeatFrame[4] = (uint8_t)u32JsonLength;
    s_mSvcRun.u16HeartbeatFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mSvcRun.acHeartbeatCommand, sizeof(s_mSvcRun.acHeartbeatCommand), MODEM_AT_CMD_TCP_SEND_FORMAT,
                                 MODEM_PLATFORM_LINK_ID, s_mSvcRun.u16HeartbeatFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mSvcRun.acHeartbeatCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_ParseReceiveLength
  * Author  :LoH
  * Function:Read the buffered-data length returned by CIPRXGET mode four.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_ParseReceiveLength(const char *pcResponse, uint8_t u8LinkId, uint16_t *pu16Length)
{
    unsigned int u32ResponseLinkId;
    unsigned int u32Length;

    if ((pcResponse == 0)
        || (pu16Length == 0)
        || (sscanf(pcResponse, "+CIPRXGET: 4,%u,%u", &u32ResponseLinkId, &u32Length) != 2)
        || (u32ResponseLinkId != u8LinkId)
        || (u32Length > MODEM_PLATFORM_RX_MAX_LENGTH))
    {
        return 0U;
    }

    *pu16Length = (uint16_t)u32Length;
    return 1U;
}

/*******************************************************
  * Name    :ModemService_HexToBytes
  * Author  :LoH
  * Function:Convert one fixed-length hexadecimal modem response to bytes.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_HexToBytes(const char *pcHex, uint16_t u16Length, uint8_t *pu8Bytes)
{
    uint16_t u16Index;
    uint8_t u8HighNibble;
    uint8_t u8LowNibble;
    char cHigh;
    char cLow;

    if ((pcHex == 0) || (pu8Bytes == 0) || (pcHex[(uint16_t)(u16Length * 2U)] != '\0'))
    {
        return 0U;
    }

    for (u16Index = 0U; u16Index < u16Length; u16Index++)
    {
        cHigh = pcHex[u16Index * 2U];
        cLow = pcHex[(u16Index * 2U) + 1U];
        if ((cHigh >= '0') && (cHigh <= '9'))
        {
            u8HighNibble = (uint8_t)(cHigh - '0');
        }
        else if ((cHigh >= 'A') && (cHigh <= 'F'))
        {
            u8HighNibble = (uint8_t)(cHigh - 'A' + 10U);
        }
        else if ((cHigh >= 'a') && (cHigh <= 'f'))
        {
            u8HighNibble = (uint8_t)(cHigh - 'a' + 10U);
        }
        else
        {
            return 0U;
        }

        if ((cLow >= '0') && (cLow <= '9'))
        {
            u8LowNibble = (uint8_t)(cLow - '0');
        }
        else if ((cLow >= 'A') && (cLow <= 'F'))
        {
            u8LowNibble = (uint8_t)(cLow - 'A' + 10U);
        }
        else if ((cLow >= 'a') && (cLow <= 'f'))
        {
            u8LowNibble = (uint8_t)(cLow - 'a' + 10U);
        }
        else
        {
            return 0U;
        }

        pu8Bytes[u16Index] = (uint8_t)((u8HighNibble << 4U) | u8LowNibble);
    }
    return 1U;
}

/*******************************************************
  * Name    :ModemService_IsPlatformFrameValid
  * Author  :LoH
  * Function:Verify the candidate AECS type-and-length frame header.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_IsPlatformFrameValid(const uint8_t *pu8Frame, uint16_t u16Length)
{
    uint32_t u32PayloadLength;

    if ((pu8Frame == 0) || (u16Length < 5U))
    {
        return 0U;
    }

    u32PayloadLength = ((uint32_t)pu8Frame[1] << 24U)
                       | ((uint32_t)pu8Frame[2] << 16U)
                       | ((uint32_t)pu8Frame[3] << 8U)
                       | (uint32_t)pu8Frame[4];
    return ((u32PayloadLength + 5U) == u16Length) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemService_ExtractPlatformReplyType
  * Author  :LoH
  * Function:Extract the bounded JSON type value from a verified AECS frame.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemService_ExtractPlatformReplyType(const uint8_t *pu8Frame, uint16_t u16FrameLength,
                                                      char *pcType, uint16_t u16TypeLength)
{
    static const char cTypeKey[] = "\"type\":\"";
    uint16_t u16PayloadLength;
    uint16_t u16Index;
    uint16_t u16TypeIndex;
    uint16_t u16TypeStartIndex;

    if ((pu8Frame == 0) || (pcType == 0) || (u16FrameLength < 5U) || (u16TypeLength < 2U))
    {
        return 0U;
    }

    pcType[0] = '\0';
    u16PayloadLength = (uint16_t)(u16FrameLength - 5U);
    for (u16Index = 0U; (uint16_t)(u16Index + sizeof(cTypeKey) - 1U) <= u16PayloadLength; u16Index++)
    {
        if (memcmp(&pu8Frame[5U + u16Index], cTypeKey, sizeof(cTypeKey) - 1U) == 0)
        {
            u16TypeStartIndex = (uint16_t)(u16Index + sizeof(cTypeKey) - 1U);
            for (u16TypeIndex = 0U; (uint16_t)(u16TypeStartIndex + u16TypeIndex) < u16PayloadLength; u16TypeIndex++)
            {
                if (pu8Frame[5U + u16TypeStartIndex + u16TypeIndex] == '"')
                {
                    if (u16TypeIndex == 0U)
                    {
                        return 0U;
                    }

                    pcType[u16TypeIndex] = '\0';
                    return 1U;
                }

                if ((u16TypeIndex >= (uint16_t)(u16TypeLength - 1U))
                    || (pu8Frame[5U + u16TypeStartIndex + u16TypeIndex] < 0x20U)
                    || (pu8Frame[5U + u16TypeStartIndex + u16TypeIndex] > 0x7EU)
                    || (pu8Frame[5U + u16TypeStartIndex + u16TypeIndex] == '\\'))
                {
                    return 0U;
                }

                pcType[u16TypeIndex] = (char)pu8Frame[5U + u16TypeStartIndex + u16TypeIndex];
            }

            return 0U;
        }
    }

    return 0U;
}

/*******************************************************
  * Name    :ModemService_LogPlatformReplyType
  * Author  :LoH
  * Function:Report the parsed platform reply type without logging the complete payload.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_LogPlatformReplyType(const char *pcType)
{
    char acLog[MODEM_PLATFORM_REPLY_LOG_LENGTH];
    int i32LogLength;

    if (pcType == 0)
    {
        HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
        return;
    }

    i32LogLength = snprintf(acLog, sizeof(acLog), "PLATFORM_RX_TYPE:%s\r\n", pcType);
    if ((i32LogLength > 0) && ((uint32_t)i32LogLength < sizeof(acLog)))
    {
        HMI_LOG_MODEM(acLog);
    }
    else
    {
        HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
    }
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
    s_mSvcRun.u8TcpOpenRetryCount = 0U;
    s_mSvcRun.u32TcpOpenRetryHoldMs = 0U;
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
    s_mSvcRun.u8TcpOpenRetryCount = 0U;
    s_mSvcRun.u32TcpOpenRetryHoldMs = 0U;
    s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mSvcRun.u32HeartbeatHoldMs = 0U;
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
                    HMI_LOG_MODEM("PDP_CLEANUP\r\n");
                    s_mSvcRun.enPdpOpenStep = MODEM_PDP_CLOSE_STEP_QUEUE;
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

        case MODEM_PDP_CLOSE_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_PDP_CLOSE, MODEM_AT_RSP_PDP_CLOSED, MODEM_AT_CFG_PDP_OPEN_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_CLOSE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PDP_CLOSE_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PDP_CLOSE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PDP_CLOSE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("PDP_CLOSED\r\n");
                s_mSvcRun.enPdpOpenStep = MODEM_PDP_OPEN_STEP_QUEUE;
            }
            else
            {
                /* Wait for +NETCLOSE: 0 or a ModemPort timeout. */
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
  * Function:Configure TCP, connect to the platform and apply bounded connection retries.
  * Version :V1.0.0
  * Data    :2026.08.28
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
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_RX_MODE_STEP_QUEUE;
            }
            else
            {
                /* Wait for CIPCFG final OK. */
            }
            break;

        case MODEM_TCP_RX_MODE_STEP_QUEUE:
            if (ModemPort_QueueCommand(MODEM_AT_CMD_TCP_RX_MODE, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_RX_MODE_STEP_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("TCP_RX_MODE_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_TCP_RX_MODE_STEP_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("TCP_RX_MODE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                HMI_LOG_MODEM("TCP_RX_MODE_READY\r\n");
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_QUEUE;
            }
            else
            {
                /* Wait for CIPRXGET manual-mode final OK. */
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
                ModemService_EnterTcpOpenRetry();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsTcpOpened(ModemPort_GetLastMatchedResponse()) != 0U)
                {
                    HMI_LOG_MODEM("TCP_CONNECTED\r\n");
                    ModemService_EnterPlatformRegistering();
                }
                else
                {
                    HMI_LOG_MODEM("TCP_OPEN_RESULT_FAILED\r\n");
                    ModemService_EnterTcpOpenRetry();
                }
            }
            else
            {
                /* Wait for +CIPOPEN: 0,0 or a ModemPort timeout. */
            }
            break;

        case MODEM_TCP_OPEN_RETRY_WAIT:
            if (s_mSvcRun.u32TcpOpenRetryHoldMs < MODEM_TCP_OPEN_RETRY_HOLD_MS)
            {
                s_mSvcRun.u32TcpOpenRetryHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
            }

            if (s_mSvcRun.u32TcpOpenRetryHoldMs >= MODEM_TCP_OPEN_RETRY_HOLD_MS)
            {
                s_mSvcRun.enTcpConnectStep = MODEM_TCP_OPEN_STEP_QUEUE;
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
  * Function:Send the candidate AECS registration frame and verify CIPSEND lengths.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessPlatformRegistering(void)
{
    switch (s_mSvcRun.enPlatformRegisterStep)
    {
        case MODEM_PLATFORM_REGISTER_STEP_PREPARE:
            if (ModemService_PrepareRegisterFrame() != 0U)
            {
                s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_FRAME_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mSvcRun.acRegisterCommand, MODEM_AT_RSP_PROMPT, MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_PROMPT_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_SEND:
            if (ModemPort_SendData(s_mSvcRun.au8RegisterFrame, s_mSvcRun.u16RegisterFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_SEND_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_RESULT_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mSvcRun.u16RegisterFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("PLATFORM_REGISTERED\r\n");
                    s_mSvcSts.enNetworkState = MODEM_NETWORK_ONLINE;
                }
                else
                {
                    HMI_LOG_MODEM("REGISTER_CONFIRM_FAILED\r\n");
                    ModemService_EnterNetworkSleep();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("REGISTER_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPlatformHeartbeat
  * Author  :LoH
  * Function:Send one periodic candidate AECS heartbeat without colliding with receive or eCall work.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessPlatformHeartbeat(void)
{
    if ((s_mSvcRun.enPlatformReceiveStep != MODEM_PLATFORM_RX_STEP_IDLE)
        || (s_mSvcRun.u8EmergencyRequestPending != 0U)
        || (s_mSvcRun.u8NetworkSleepRequested != 0U))
    {
        return;
    }

    switch (s_mSvcRun.enPlatformHeartbeatStep)
    {
        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT:
            if (s_mSvcRun.u32HeartbeatHoldMs < MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mSvcRun.u32HeartbeatHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
            }

            if (s_mSvcRun.u32HeartbeatHoldMs >= MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE;
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE:
            if (ModemService_PrepareHeartbeatFrame() != 0U)
            {
                s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_FRAME_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mSvcRun.acHeartbeatCommand, MODEM_AT_RSP_PROMPT, MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_PROMPT_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_SEND:
            if (ModemPort_SendData(s_mSvcRun.au8HeartbeatFrame, s_mSvcRun.u16HeartbeatFrameLength, MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_SEND_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_RESULT_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mSvcRun.u16HeartbeatFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("HEARTBEAT_SENT\r\n");
                    s_mSvcRun.u32HeartbeatHoldMs = 0U;
                    s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
                }
                else
                {
                    HMI_LOG_MODEM("HEARTBEAT_CONFIRM_FAILED\r\n");
                    ModemService_EnterNetworkSleep();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("HEARTBEAT_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessOnline
  * Author  :LoH
  * Function:Maintain online state and apply the KL15 delayed sleep policy.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessOnline(void)
{
    char acPlatformReplyType[MODEM_PLATFORM_REPLY_TYPE_LENGTH];
    int i32CommandLength;

    switch (s_mSvcRun.enPlatformReceiveStep)
    {
        case MODEM_PLATFORM_RX_STEP_IDLE:
            if (ModemPort_TakeRxDataEvent(&s_mSvcRun.u8PlatformReceiveLinkId) != 0U)
            {
                s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE;
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE:
            i32CommandLength = snprintf(s_mSvcRun.acReceiveLengthCommand, sizeof(s_mSvcRun.acReceiveLengthCommand),
                                        MODEM_AT_CMD_TCP_RX_LENGTH_FORMAT, s_mSvcRun.u8PlatformReceiveLinkId);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mSvcRun.acReceiveLengthCommand))
                && (ModemPort_QueueCommand(s_mSvcRun.acReceiveLengthCommand, MODEM_AT_RSP_TCP_RX_LENGTH, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemService_ParseReceiveLength(ModemPort_GetLastMatchedResponse(), s_mSvcRun.u8PlatformReceiveLinkId,
                                                    &s_mSvcRun.u16PlatformReceiveLength) != 0U)
                {
                    if (s_mSvcRun.u16PlatformReceiveLength != 0U)
                    {
                        s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_QUEUE;
                    }
                    else
                    {
                        HMI_LOG_MODEM("PLATFORM_RX_EMPTY\r\n");
                        s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
                    }
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_LENGTH_INVALID\r\n");
                    ModemService_EnterNetworkSleep();
                }
            }
            else
            {
                /* Wait for +CIPRXGET: 4,<link>,<restLength>. */
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_QUEUE:
            i32CommandLength = snprintf(s_mSvcRun.acReceiveDataCommand, sizeof(s_mSvcRun.acReceiveDataCommand),
                                        MODEM_AT_CMD_TCP_RX_DATA_FORMAT, s_mSvcRun.u8PlatformReceiveLinkId,
                                        s_mSvcRun.u16PlatformReceiveLength);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mSvcRun.acReceiveDataCommand))
                && (ModemPort_QueueCommand(s_mSvcRun.acReceiveDataCommand, MODEM_AT_RSP_OK, MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_QUEUE_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_FAILED\r\n");
                ModemService_EnterNetworkSleep();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if ((ModemService_HexToBytes(ModemPort_GetLastDataLine(), s_mSvcRun.u16PlatformReceiveLength,
                                             s_mSvcRun.au8PlatformReceiveFrame) != 0U)
                    && (ModemService_IsPlatformFrameValid(s_mSvcRun.au8PlatformReceiveFrame,
                                                          s_mSvcRun.u16PlatformReceiveLength) != 0U))
                {
                    s_mSvcRun.u16PlatformReceiveFrameLength = s_mSvcRun.u16PlatformReceiveLength;
                    HMI_LOG_MODEM("PLATFORM_RX_FRAME_READY\r\n");
                    if (ModemService_ExtractPlatformReplyType(s_mSvcRun.au8PlatformReceiveFrame, s_mSvcRun.u16PlatformReceiveFrameLength, acPlatformReplyType, sizeof(acPlatformReplyType)) != 0U)
                    {
                        ModemService_LogPlatformReplyType(acPlatformReplyType);
                    }
                    else
                    {
                        HMI_LOG_MODEM("PLATFORM_RX_TYPE_INVALID\r\n");
                    }
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_FRAME_INVALID\r\n");
                }
                s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
            }
            else
            {
                /* Wait for hex payload and final OK from CIPRXGET mode three. */
            }
            break;

        default:
            HMI_LOG_MODEM("PLATFORM_RX_STEP_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }

    ModemService_ProcessPlatformHeartbeat();

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
    s_mSvcRun.u8TcpOpenRetryCount = 0U;
    s_mSvcRun.u32TcpOpenRetryHoldMs = 0U;
    s_mSvcRun.enPlatformRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mSvcRun.enPlatformReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mSvcRun.enPlatformHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mSvcRun.u32HeartbeatHoldMs = 0U;
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
    if (ModemPort_TakePdnDeactivation() != 0U)
    {
        if (s_mSvcSts.enNetworkState != MODEM_NETWORK_SLEEP)
        {
            HMI_LOG_MODEM("PDN_DEACT_RESTART\r\n");
            ModemService_EnterAtSync();
        }
        else
        {
            /* Do not wake a KL15-controlled sleeping network for a stale URC. */
        }
    }

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
