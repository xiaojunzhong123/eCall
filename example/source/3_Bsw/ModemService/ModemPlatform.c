/*******************************************************
  * Name    :ModemPlatform.c
  * Author  :LoH
  * Function:Candidate AECS registration, receive, heartbeat and MSD workflow.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#include "ModemPlatform.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_PLATFORM_PROCESS_PERIOD_MS        100U
#define MODEM_PLATFORM_JSON_LENGTH               64U
#define MODEM_PLATFORM_FRAME_LENGTH              (MODEM_PLATFORM_JSON_LENGTH + 5U)
#define MODEM_PLATFORM_COMMAND_LENGTH            32U
#define MODEM_PLATFORM_RX_MAX_LENGTH             750U
#define MODEM_PLATFORM_REPLY_TYPE_LENGTH          32U
#define MODEM_PLATFORM_REPLY_LOG_LENGTH           56U
#define MODEM_PLATFORM_MSD_PAYLOAD_LENGTH        111U
#define MODEM_PLATFORM_MSD_FRAME_LENGTH          (MODEM_PLATFORM_MSD_PAYLOAD_LENGTH + 5U)

typedef enum
{
    MODEM_PLATFORM_PHASE_IDLE = 0U,
    MODEM_PLATFORM_PHASE_REGISTERING,
    MODEM_PLATFORM_PHASE_ONLINE,
    MODEM_PLATFORM_PHASE_MSD_UPLOADING
} ModemPlatformPhase_t;

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

typedef enum
{
    MODEM_PLATFORM_MSD_STEP_PREPARE = 0U,
    MODEM_PLATFORM_MSD_STEP_QUEUE,
    MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT,
    MODEM_PLATFORM_MSD_STEP_SEND,
    MODEM_PLATFORM_MSD_STEP_WAIT_RESULT
} ModemPlatformMsdStep_t;

typedef struct
{
    ModemPlatformRegisterStep_t enRegisterStep;
    ModemPlatformReceiveStep_t enReceiveStep;
    ModemPlatformHeartbeatStep_t enHeartbeatStep;
    ModemPlatformMsdStep_t enMsdStep;
    uint8_t u8MsdUploadRequested;
    uint8_t u8MsdAckReceived;
    uint32_t u32HeartbeatHoldMs;
    char acRegisterCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acHeartbeatCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acMsdCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveLengthCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    char acReceiveDataCommand[MODEM_PLATFORM_COMMAND_LENGTH];
    uint8_t au8RegisterFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8HeartbeatFrame[MODEM_PLATFORM_FRAME_LENGTH];
    uint8_t au8MsdFrame[MODEM_PLATFORM_MSD_FRAME_LENGTH];
    uint8_t au8ReceiveFrame[MODEM_PLATFORM_RX_MAX_LENGTH];
    uint16_t u16RegisterFrameLength;
    uint16_t u16HeartbeatFrameLength;
    uint16_t u16MsdFrameLength;
    uint16_t u16ReceiveLength;
    uint16_t u16ReceiveFrameLength;
    uint8_t u8ReceiveLinkId;
    EcallMsd_t stLastMsd;
} ModemPlatformRuntime_t;

typedef struct
{
    ModemPlatformPhase_t enPhase;
    ModemPlatformEvent_t enEvent;
    uint8_t u8MsdUploadComplete;
} ModemPlatformStatus_t;

static ModemPlatformRuntime_t s_mPlatRun;
static ModemPlatformStatus_t s_mPlatSts;

/*******************************************************
  * Name    :ModemPlatform_EnterFailure
  * Author  :LoH
  * Function:Stop platform work with one consumable failure event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_EnterFailure(void)
{
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_IDLE;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_FAILED;
}

/*******************************************************
  * Name    :ModemPlatform_IsDataSendComplete
  * Author  :LoH
  * Function:Return whether CIPSEND confirms every requested platform byte.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_IsDataSendComplete(const char *pcResponse, uint16_t u16ExpectedLength)
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
  * Name    :ModemPlatform_PrepareRegisterFrame
  * Author  :LoH
  * Function:Build the candidate AECS registration frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_PrepareRegisterFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mPlatRun.au8RegisterFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                             MODEM_PLATFORM_REGISTER_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mPlatRun.au8RegisterFrame[0] = MODEM_PLATFORM_MSG_REGISTER;
    s_mPlatRun.au8RegisterFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mPlatRun.au8RegisterFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mPlatRun.au8RegisterFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mPlatRun.au8RegisterFrame[4] = (uint8_t)u32JsonLength;
    s_mPlatRun.u16RegisterFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mPlatRun.acRegisterCommand, sizeof(s_mPlatRun.acRegisterCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID,
                                s_mPlatRun.u16RegisterFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acRegisterCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_PrepareHeartbeatFrame
  * Author  :LoH
  * Function:Build the candidate AECS heartbeat frame and CIPSEND command.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_PrepareHeartbeatFrame(void)
{
    int i32JsonLength;
    int i32CommandLength;
    uint32_t u32JsonLength;

    i32JsonLength = snprintf((char *)&s_mPlatRun.au8HeartbeatFrame[5], MODEM_PLATFORM_JSON_LENGTH,
                             MODEM_PLATFORM_HEARTBEAT_JSON_FORMAT, MODEM_PLATFORM_TEST_DEVICE_ID);
    if ((i32JsonLength <= 0) || ((uint32_t)i32JsonLength >= MODEM_PLATFORM_JSON_LENGTH))
    {
        return 0U;
    }

    u32JsonLength = (uint32_t)i32JsonLength;
    s_mPlatRun.au8HeartbeatFrame[0] = MODEM_PLATFORM_MSG_HEARTBEAT;
    s_mPlatRun.au8HeartbeatFrame[1] = (uint8_t)(u32JsonLength >> 24U);
    s_mPlatRun.au8HeartbeatFrame[2] = (uint8_t)(u32JsonLength >> 16U);
    s_mPlatRun.au8HeartbeatFrame[3] = (uint8_t)(u32JsonLength >> 8U);
    s_mPlatRun.au8HeartbeatFrame[4] = (uint8_t)u32JsonLength;
    s_mPlatRun.u16HeartbeatFrameLength = (uint16_t)(u32JsonLength + 5U);

    i32CommandLength = snprintf(s_mPlatRun.acHeartbeatCommand, sizeof(s_mPlatRun.acHeartbeatCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID,
                                s_mPlatRun.u16HeartbeatFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acHeartbeatCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_PackU32Be
  * Author  :LoH
  * Function:Store one unsigned 32-bit value in big-endian byte order.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_PackU32Be(uint8_t *pu8Buffer, uint32_t u32Value)
{
    pu8Buffer[0] = (uint8_t)(u32Value >> 24U);
    pu8Buffer[1] = (uint8_t)(u32Value >> 16U);
    pu8Buffer[2] = (uint8_t)(u32Value >> 8U);
    pu8Buffer[3] = (uint8_t)u32Value;
}

/*******************************************************
  * Name    :ModemPlatform_PrepareMsdFrame
  * Author  :LoH
  * Function:Build the reference-compatible candidate 111-byte MSD platform frame.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_PrepareMsdFrame(void)
{
    int i32CommandLength;
    uint8_t *pu8Payload;

    (void)memset(s_mPlatRun.au8MsdFrame, 0, sizeof(s_mPlatRun.au8MsdFrame));
    s_mPlatRun.au8MsdFrame[0] = MODEM_PLATFORM_MSG_MSD;
    ModemPlatform_PackU32Be(&s_mPlatRun.au8MsdFrame[1], MODEM_PLATFORM_MSD_PAYLOAD_LENGTH);
    pu8Payload = &s_mPlatRun.au8MsdFrame[5];
    pu8Payload[0] = MODEM_PLATFORM_MSD_VERSION;
    pu8Payload[1] = MODEM_PLATFORM_MSD_IDENTIFIER;
    pu8Payload[2] = s_mPlatRun.stLastMsd.trigger_mask;
    pu8Payload[3] = MODEM_PLATFORM_MSD_TEST_CALL;
    pu8Payload[4] = MODEM_PLATFORM_MSD_VEHICLE_TYPE;
    (void)memcpy(&pu8Payload[5], MODEM_PLATFORM_TEST_VIN, MODEM_PLATFORM_MSD_VIN_LENGTH);
    pu8Payload[22] = MODEM_PLATFORM_MSD_POWER_TYPE;
    ModemPlatform_PackU32Be(&pu8Payload[23], s_mPlatRun.stLastMsd.utc_seconds);
    ModemPlatform_PackU32Be(&pu8Payload[27], (uint32_t)s_mPlatRun.stLastMsd.longitude_e7);
    ModemPlatform_PackU32Be(&pu8Payload[31], (uint32_t)s_mPlatRun.stLastMsd.latitude_e7);
    s_mPlatRun.u16MsdFrameLength = MODEM_PLATFORM_MSD_FRAME_LENGTH;

    i32CommandLength = snprintf(s_mPlatRun.acMsdCommand, sizeof(s_mPlatRun.acMsdCommand),
                                MODEM_AT_CMD_TCP_SEND_FORMAT, MODEM_PLATFORM_LINK_ID,
                                s_mPlatRun.u16MsdFrameLength);
    return ((i32CommandLength > 0)
            && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acMsdCommand))) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_ParseReceiveLength
  * Author  :LoH
  * Function:Read the buffered-data length returned by CIPRXGET mode four.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_ParseReceiveLength(const char *pcResponse, uint8_t u8LinkId, uint16_t *pu16Length)
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
  * Name    :ModemPlatform_HexToBytes
  * Author  :LoH
  * Function:Convert one fixed-length hexadecimal modem response to bytes.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_HexToBytes(const char *pcHex, uint16_t u16Length, uint8_t *pu8Bytes)
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
  * Name    :ModemPlatform_IsFrameValid
  * Author  :LoH
  * Function:Verify the candidate AECS type-and-length frame header.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_IsFrameValid(const uint8_t *pu8Frame, uint16_t u16Length)
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
  * Name    :ModemPlatform_ExtractReplyType
  * Author  :LoH
  * Function:Extract the bounded JSON type value from a verified AECS frame.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPlatform_ExtractReplyType(const uint8_t *pu8Frame, uint16_t u16FrameLength,
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
  * Name    :ModemPlatform_LogReplyType
  * Author  :LoH
  * Function:Report the parsed platform reply type without logging the complete payload.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_LogReplyType(const char *pcType)
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
  * Name    :ModemPlatform_ProcessRegistering
  * Author  :LoH
  * Function:Send the candidate AECS registration frame and verify CIPSEND lengths.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_ProcessRegistering(void)
{
    switch (s_mPlatRun.enRegisterStep)
    {
        case MODEM_PLATFORM_REGISTER_STEP_PREPARE:
            if (ModemPlatform_PrepareRegisterFrame() != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acRegisterCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_SEND:
            if (ModemPort_SendData(s_mPlatRun.au8RegisterFrame, s_mPlatRun.u16RegisterFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("REGISTER_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_REGISTER_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("REGISTER_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u16RegisterFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("PLATFORM_REGISTERED\r\n");
                    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_ONLINE;
                    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_REGISTERED;
                }
                else
                {
                    HMI_LOG_MODEM("REGISTER_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("REGISTER_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessHeartbeat
  * Author  :LoH
  * Function:Send periodic heartbeat without colliding with receive, MSD or sleep work.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_ProcessHeartbeat(uint8_t u8HeartbeatEnabled)
{
    if ((s_mPlatRun.enReceiveStep != MODEM_PLATFORM_RX_STEP_IDLE)
        || (s_mPlatRun.u8MsdUploadRequested != 0U)
        || (u8HeartbeatEnabled == 0U))
    {
        return;
    }

    switch (s_mPlatRun.enHeartbeatStep)
    {
        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT:
            if (s_mPlatRun.u32HeartbeatHoldMs < MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mPlatRun.u32HeartbeatHoldMs += MODEM_PLATFORM_PROCESS_PERIOD_MS;
            }

            if (s_mPlatRun.u32HeartbeatHoldMs >= MODEM_PLATFORM_HEARTBEAT_PERIOD_MS)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE;
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_PREPARE:
            if (ModemPlatform_PrepareHeartbeatFrame() != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acHeartbeatCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_SEND:
            if (ModemPort_SendData(s_mPlatRun.au8HeartbeatFrame, s_mPlatRun.u16HeartbeatFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("HEARTBEAT_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_HEARTBEAT_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("HEARTBEAT_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u16HeartbeatFrameLength) != 0U)
                {
                    HMI_LOG_MODEM("HEARTBEAT_SENT\r\n");
                    s_mPlatRun.u32HeartbeatHoldMs = 0U;
                    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
                }
                else
                {
                    HMI_LOG_MODEM("HEARTBEAT_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("HEARTBEAT_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessOnline
  * Author  :LoH
  * Function:Retrieve buffered platform frames and schedule heartbeat or MSD work.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_ProcessOnline(uint8_t u8HeartbeatEnabled)
{
    char acPlatformReplyType[MODEM_PLATFORM_REPLY_TYPE_LENGTH];
    int i32CommandLength;

    ModemPlatform_ProcessHeartbeat(u8HeartbeatEnabled);

    if (s_mPlatSts.enEvent == MODEM_PLATFORM_EVENT_FAILED)
    {
        return;
    }

    switch (s_mPlatRun.enReceiveStep)
    {
        case MODEM_PLATFORM_RX_STEP_IDLE:
            if ((s_mPlatRun.enHeartbeatStep == MODEM_PLATFORM_HEARTBEAT_STEP_WAIT)
                && (ModemPort_TakeRxDataEvent(&s_mPlatRun.u8ReceiveLinkId) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE;
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_QUEUE:
            i32CommandLength = snprintf(s_mPlatRun.acReceiveLengthCommand,
                                        sizeof(s_mPlatRun.acReceiveLengthCommand),
                                        MODEM_AT_CMD_TCP_RX_LENGTH_FORMAT, s_mPlatRun.u8ReceiveLinkId);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acReceiveLengthCommand))
                && (ModemPort_QueueCommand(s_mPlatRun.acReceiveLengthCommand, MODEM_AT_RSP_TCP_RX_LENGTH,
                                           MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_LENGTH_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_LENGTH_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_LENGTH_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_ParseReceiveLength(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u8ReceiveLinkId,
                                                     &s_mPlatRun.u16ReceiveLength) != 0U)
                {
                    if (s_mPlatRun.u16ReceiveLength != 0U)
                    {
                        s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_QUEUE;
                    }
                    else
                    {
                        HMI_LOG_MODEM("PLATFORM_RX_EMPTY\r\n");
                        s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
                    }
                }
                else
                {
                    HMI_LOG_MODEM("PLATFORM_RX_LENGTH_INVALID\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPRXGET: 4,<link>,<restLength>. */
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_QUEUE:
            i32CommandLength = snprintf(s_mPlatRun.acReceiveDataCommand,
                                        sizeof(s_mPlatRun.acReceiveDataCommand),
                                        MODEM_AT_CMD_TCP_RX_DATA_FORMAT, s_mPlatRun.u8ReceiveLinkId,
                                        s_mPlatRun.u16ReceiveLength);
            if ((i32CommandLength > 0)
                && ((uint32_t)i32CommandLength < sizeof(s_mPlatRun.acReceiveDataCommand))
                && (ModemPort_QueueCommand(s_mPlatRun.acReceiveDataCommand, MODEM_AT_RSP_OK,
                                           MODEM_AT_CFG_TCP_CFG_TIMEOUT_MS) != 0U))
            {
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_DATA_WAIT;
            }
            else
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_RX_STEP_DATA_WAIT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("PLATFORM_RX_DATA_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if ((ModemPlatform_HexToBytes(ModemPort_GetLastDataLine(), s_mPlatRun.u16ReceiveLength,
                                              s_mPlatRun.au8ReceiveFrame) != 0U)
                    && (ModemPlatform_IsFrameValid(s_mPlatRun.au8ReceiveFrame,
                                                   s_mPlatRun.u16ReceiveLength) != 0U))
                {
                    s_mPlatRun.u16ReceiveFrameLength = s_mPlatRun.u16ReceiveLength;
                    HMI_LOG_MODEM("PLATFORM_RX_FRAME_READY\r\n");
                    if (ModemPlatform_ExtractReplyType(s_mPlatRun.au8ReceiveFrame,
                                                       s_mPlatRun.u16ReceiveFrameLength,
                                                       acPlatformReplyType,
                                                       sizeof(acPlatformReplyType)) != 0U)
                    {
                        ModemPlatform_LogReplyType(acPlatformReplyType);
                        if ((s_mPlatSts.u8MsdUploadComplete != 0U)
                            && (s_mPlatRun.u8MsdAckReceived == 0U)
                            && (strcmp(acPlatformReplyType, "msd_ack") == 0))
                        {
                            s_mPlatRun.u8MsdAckReceived = 1U;
                            s_mPlatRun.u8MsdUploadRequested = 0U;
                            s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_MSD_UPLOADED;
                            HMI_LOG_MODEM("MSD_ACK_CALL_START\r\n");
                        }
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
                s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
            }
            else
            {
                /* Wait for hex payload and final OK from CIPRXGET mode three. */
            }
            break;

        default:
            HMI_LOG_MODEM("PLATFORM_RX_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }

    if (s_mPlatSts.enEvent == MODEM_PLATFORM_EVENT_FAILED)
    {
        return;
    }

    if ((s_mPlatRun.u8MsdUploadRequested != 0U)
        && (s_mPlatSts.u8MsdUploadComplete == 0U)
        && (s_mPlatRun.enReceiveStep == MODEM_PLATFORM_RX_STEP_IDLE)
        && (ModemPort_IsIdle() != 0U))
    {
        s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_MSD_UPLOADING;
        s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_MSD_STARTED;
    }
}

/*******************************************************
  * Name    :ModemPlatform_ProcessMsdUploading
  * Author  :LoH
  * Function:Upload one reference-compatible MSD frame and verify CIPSEND lengths.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPlatform_ProcessMsdUploading(void)
{
    switch (s_mPlatRun.enMsdStep)
    {
        case MODEM_PLATFORM_MSD_STEP_PREPARE:
            if (ModemPlatform_PrepareMsdFrame() != 0U)
            {
                HMI_LOG_MODEM("MSD_UPLOAD_START\r\n");
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_QUEUE;
            }
            else
            {
                HMI_LOG_MODEM("MSD_FRAME_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_QUEUE:
            ModemPort_ClearFailure();
            if (ModemPort_QueueCommand(s_mPlatRun.acMsdCommand, MODEM_AT_RSP_PROMPT,
                                       MODEM_AT_CFG_SEND_PROMPT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT;
            }
            else
            {
                HMI_LOG_MODEM("MSD_QUEUE_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_WAIT_PROMPT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("MSD_PROMPT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_SEND;
            }
            else
            {
                /* Wait for the CIPSEND prompt. */
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_SEND:
            if (ModemPort_SendData(s_mPlatRun.au8MsdFrame, s_mPlatRun.u16MsdFrameLength,
                                   MODEM_AT_RSP_SEND, MODEM_AT_CFG_SEND_RESULT_TIMEOUT_MS) != 0U)
            {
                s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_WAIT_RESULT;
            }
            else
            {
                HMI_LOG_MODEM("MSD_SEND_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            break;

        case MODEM_PLATFORM_MSD_STEP_WAIT_RESULT:
            if (ModemPort_HasFailure() != 0U)
            {
                HMI_LOG_MODEM("MSD_RESULT_FAILED\r\n");
                ModemPlatform_EnterFailure();
            }
            else if (ModemPort_IsIdle() != 0U)
            {
                if (ModemPlatform_IsDataSendComplete(ModemPort_GetLastMatchedResponse(), s_mPlatRun.u16MsdFrameLength) != 0U)
                {
                    s_mPlatSts.u8MsdUploadComplete = 1U;
                    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_ONLINE;
                    HMI_LOG_MODEM("MSD_UPLOADED_WAIT_ACK\r\n");
                }
                else
                {
                    HMI_LOG_MODEM("MSD_CONFIRM_FAILED\r\n");
                    ModemPlatform_EnterFailure();
                }
            }
            else
            {
                /* Wait for +CIPSEND: <link>,<req>,<cnf>. */
            }
            break;

        default:
            HMI_LOG_MODEM("MSD_STEP_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_Init
  * Author  :LoH
  * Function:Initialize private platform runtime and status data.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Init(void)
{
    (void)memset(&s_mPlatRun, 0, sizeof(s_mPlatRun));
    (void)memset(&s_mPlatSts, 0, sizeof(s_mPlatSts));
    s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_IDLE;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
}

/*******************************************************
  * Name    :ModemPlatform_Reset
  * Author  :LoH
  * Function:Stop active platform work while preserving a pending emergency MSD.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Reset(void)
{
    EcallMsd_t stPendingMsd = s_mPlatRun.stLastMsd;
    uint8_t u8MsdUploadRequested = s_mPlatRun.u8MsdUploadRequested;

    ModemPlatform_Init();
    if (u8MsdUploadRequested != 0U)
    {
        s_mPlatRun.stLastMsd = stPendingMsd;
        s_mPlatRun.u8MsdUploadRequested = 1U;
    }
}

/*******************************************************
  * Name    :ModemPlatform_Start
  * Author  :LoH
  * Function:Start platform registration while preserving a pending MSD request.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Start(void)
{
    s_mPlatRun.enRegisterStep = MODEM_PLATFORM_REGISTER_STEP_PREPARE;
    s_mPlatRun.enReceiveStep = MODEM_PLATFORM_RX_STEP_IDLE;
    s_mPlatRun.enHeartbeatStep = MODEM_PLATFORM_HEARTBEAT_STEP_WAIT;
    s_mPlatRun.u32HeartbeatHoldMs = 0U;
    s_mPlatSts.enPhase = MODEM_PLATFORM_PHASE_REGISTERING;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
}

/*******************************************************
  * Name    :ModemPlatform_RequestMsdUpload
  * Author  :LoH
  * Function:Latch one frozen MSD for upload after registration or active receive work.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_RequestMsdUpload(const EcallMsd_t *pstMsd)
{
    if (pstMsd != 0)
    {
        s_mPlatRun.stLastMsd = *pstMsd;
        s_mPlatRun.u8MsdUploadRequested = 1U;
        s_mPlatRun.u8MsdAckReceived = 0U;
        s_mPlatRun.enMsdStep = MODEM_PLATFORM_MSD_STEP_PREPARE;
        s_mPlatSts.u8MsdUploadComplete = 0U;
    }
}

/*******************************************************
  * Name    :ModemPlatform_Process
  * Author  :LoH
  * Function:Dispatch one private platform phase.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemPlatform_Process(uint8_t u8HeartbeatEnabled)
{
    switch (s_mPlatSts.enPhase)
    {
        case MODEM_PLATFORM_PHASE_IDLE:
            break;

        case MODEM_PLATFORM_PHASE_REGISTERING:
            ModemPlatform_ProcessRegistering();
            break;

        case MODEM_PLATFORM_PHASE_ONLINE:
            ModemPlatform_ProcessOnline(u8HeartbeatEnabled);
            break;

        case MODEM_PLATFORM_PHASE_MSD_UPLOADING:
            ModemPlatform_ProcessMsdUploading();
            break;

        default:
            HMI_LOG_MODEM("PLATFORM_PHASE_UNKNOWN\r\n");
            ModemPlatform_EnterFailure();
            break;
    }
}

/*******************************************************
  * Name    :ModemPlatform_IsOnline
  * Author  :LoH
  * Function:Return whether the private platform workflow is stably online.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemPlatform_IsOnline(void)
{
    return (s_mPlatSts.enPhase == MODEM_PLATFORM_PHASE_ONLINE) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPlatform_TakeEvent
  * Author  :LoH
  * Function:Return and clear one platform result event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
ModemPlatformEvent_t ModemPlatform_TakeEvent(void)
{
    ModemPlatformEvent_t enEvent = s_mPlatSts.enEvent;
    s_mPlatSts.enEvent = MODEM_PLATFORM_EVENT_NONE;
    return enEvent;
}
