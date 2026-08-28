/*******************************************************
  * Name    :ModemPort.c
  * Author  :LoH
  * Function:Modem UART AT queue port implementation.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include "Hmi.h"
#include "ModemPort.h"
#include "Uart.h"

#define MODEM_PORT_QUEUE_LENGTH     8U
#define MODEM_PORT_LINE_LENGTH      1536U
#define MODEM_PORT_LOG_LENGTH       64U
#define MODEM_PORT_PROMPT_TOKEN      ">"

typedef struct
{
    const char *pcCommand;
    const char *pcExpected;
    uint32_t u32TimeoutMs;
} ModemPortCommand_t;

typedef struct
{
    ModemPortCommand_t astCommandQueue[MODEM_PORT_QUEUE_LENGTH];
    ModemPortCommand_t stActiveCommand;
    uint8_t au8Line[MODEM_PORT_LINE_LENGTH];
    uint8_t u8ReadIndex;
    uint8_t u8WriteIndex;
    uint8_t u8Count;
    uint8_t u8Active;
    uint16_t u16LineLength;
    uint32_t u32StartMs;
} ModemPortRuntime_t;

typedef struct
{
    ModemPortResult_t enLastResult;
    uint8_t u8HasFailure;
    uint8_t u8RxDataPending;
    uint8_t u8RxDataLinkId;
    uint8_t u8PdnDeactivated;
    char acLastMatchedResponse[MODEM_PORT_LINE_LENGTH];
    char acLastDataLine[MODEM_PORT_LINE_LENGTH];
} ModemPortStatus_t;

static ModemPortRuntime_t m_stModemPortRuntime;
static ModemPortStatus_t m_stModemPortStatus;

/*******************************************************
  * Name    :ModemPort_CopyMatchedResponse
  * Author  :LoH
  * Function:Store one matched modem response line for service-level parsing.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemPort_CopyMatchedResponse(const char *pcLine)
{
    uint16_t u16Index = 0U;

    if (pcLine == 0)
    {
        m_stModemPortStatus.acLastMatchedResponse[0] = '\0';
        return;
    }

    while ((pcLine[u16Index] != '\0') && (u16Index < (MODEM_PORT_LINE_LENGTH - 1U)))
    {
        m_stModemPortStatus.acLastMatchedResponse[u16Index] = pcLine[u16Index];
        u16Index++;
    }
    m_stModemPortStatus.acLastMatchedResponse[u16Index] = '\0';
}

/*******************************************************
  * Name    :ModemPort_IsHexDataLine
  * Author  :LoH
  * Function:Return whether one response line contains an even count of hex characters.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPort_IsHexDataLine(const char *pcLine)
{
    uint16_t u16Index = 0U;

    if (pcLine == 0)
    {
        return 0U;
    }

    while (pcLine[u16Index] != '\0')
    {
        if (!(((pcLine[u16Index] >= '0') && (pcLine[u16Index] <= '9'))
              || ((pcLine[u16Index] >= 'A') && (pcLine[u16Index] <= 'F'))
              || ((pcLine[u16Index] >= 'a') && (pcLine[u16Index] <= 'f'))))
        {
            return 0U;
        }
        u16Index++;
    }

    return ((u16Index != 0U) && ((u16Index % 2U) == 0U)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPort_CopyDataLine
  * Author  :LoH
  * Function:Store one hexadecimal TCP payload line for service-level decoding.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemPort_CopyDataLine(const char *pcLine)
{
    uint16_t u16Index = 0U;

    while ((pcLine[u16Index] != '\0') && (u16Index < (MODEM_PORT_LINE_LENGTH - 1U)))
    {
        m_stModemPortStatus.acLastDataLine[u16Index] = pcLine[u16Index];
        u16Index++;
    }
    m_stModemPortStatus.acLastDataLine[u16Index] = '\0';
}

/*******************************************************
  * Name    :ModemPort_LogResponseLine
  * Author  :LoH
  * Function:Submit one complete modem response line as one timestamped HMI record.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemPort_LogResponseLine(const char *pcLine)
{
    uint8_t au8Log[MODEM_PORT_LOG_LENGTH];
    uint8_t u8Index = 0U;
    const char *pcPrefix = "  [MODEM RX] ";

    while ((*pcPrefix != '\0') && (u8Index < (MODEM_PORT_LOG_LENGTH - 3U)))
    {
        au8Log[u8Index++] = (uint8_t)*pcPrefix++;
    }
    while ((*pcLine != '\0') && (u8Index < (MODEM_PORT_LOG_LENGTH - 3U)))
    {
        au8Log[u8Index++] = (uint8_t)*pcLine++;
    }
    au8Log[u8Index++] = '\r';
    au8Log[u8Index++] = '\n';
    au8Log[u8Index] = '\0';
    Hmi_Log((const char *)au8Log);
}

/*******************************************************
  * Name    :ModemPort_Contains
  * Author  :LoH
  * Function:Check whether a response line contains a token.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static uint8_t ModemPort_Contains(const char *pcText, const char *pcToken)
{
    uint16_t u16TextIndex;
    uint16_t u16TokenIndex;

    if ((pcText == 0) || (pcToken == 0))
    {
        return 0U;
    }
    u16TextIndex = 0U;
    while (pcText[u16TextIndex] != '\0')
    {
        u16TokenIndex = 0U;
        while ((pcToken[u16TokenIndex] != '\0')
               && (pcText[u16TextIndex + u16TokenIndex] != '\0')
               && (pcText[u16TextIndex + u16TokenIndex] == pcToken[u16TokenIndex]))
        {
            u16TokenIndex++;
        }
        if (pcToken[u16TokenIndex] == '\0')
        {
            return 1U;
        }
        u16TextIndex++;
    }
    return 0U;
}

/*******************************************************
  * Name    :ModemPort_FinishActiveCommand
  * Author  :LoH
  * Function:Finish the active AT command with one result.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemPort_FinishActiveCommand(ModemPortResult_t enResult)
{
    m_stModemPortStatus.enLastResult = enResult;
    m_stModemPortRuntime.u8Active = 0U;
    if (enResult != MODEM_PORT_RESULT_OK)
    {
        m_stModemPortStatus.u8HasFailure = 1U;
    }
}

/*******************************************************
  * Name    :ModemPort_StartNextCommand
  * Author  :LoH
  * Function:Start the next queued AT command in task context.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemPort_StartNextCommand(void)
{
    uint8_t u8Length = 0U;

    if ((m_stModemPortRuntime.u8Active != 0U)
        || (m_stModemPortRuntime.u8Count == 0U)
        || (m_stModemPortStatus.u8HasFailure != 0U))
    {
        return;
    }
    m_stModemPortRuntime.stActiveCommand =
        m_stModemPortRuntime.astCommandQueue[m_stModemPortRuntime.u8ReadIndex];
    m_stModemPortRuntime.u8ReadIndex++;
    if (m_stModemPortRuntime.u8ReadIndex >= MODEM_PORT_QUEUE_LENGTH)
    {
        m_stModemPortRuntime.u8ReadIndex = 0U;
    }
    m_stModemPortRuntime.u8Count--;
    while ((m_stModemPortRuntime.stActiveCommand.pcCommand[u8Length] != '\0')
           && (u8Length < 255U))
    {
        u8Length++;
    }
    if ((u8Length == 0U)
        || (m_stModemPortRuntime.stActiveCommand.pcCommand[u8Length] != '\0')
        || (Uart_TxData((uint8_t *)m_stModemPortRuntime.stActiveCommand.pcCommand,
                        u8Length) == FALSE))
    {
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_ERROR);
        return;
    }
    Hmi_LogWithPrefix("  [MODEM TX] ", m_stModemPortRuntime.stActiveCommand.pcCommand);
    m_stModemPortRuntime.u32StartMs = osKernelSysTick();
    m_stModemPortRuntime.u8Active = 1U;
}

/*******************************************************
  * Name    :ModemPort_HandleResponseLine
  * Author  :LoH
  * Function:Log and classify one complete modem response line.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
static void ModemPort_HandleResponseLine(const char *pcLine)
{
    unsigned int u32LinkId;

    ModemPort_LogResponseLine(pcLine);

    if ((ModemPort_Contains(pcLine, "+CGEV:") != 0U)
        && (ModemPort_Contains(pcLine, "PDN DEACT") != 0U))
    {
        m_stModemPortStatus.u8PdnDeactivated = 1U;
        if (m_stModemPortRuntime.u8Active != 0U)
        {
            ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_ERROR);
        }
        return;
    }

    if ((sscanf(pcLine, "+CIPRXGET: 1,%u", &u32LinkId) == 1)
        && (u32LinkId <= 255U))
    {
        m_stModemPortStatus.u8RxDataPending = 1U;
        m_stModemPortStatus.u8RxDataLinkId = (uint8_t)u32LinkId;
    }

    if ((m_stModemPortRuntime.u8Active != 0U) && (ModemPort_IsHexDataLine(pcLine) != 0U))
    {
        ModemPort_CopyDataLine(pcLine);
    }

    if (m_stModemPortRuntime.u8Active == 0U)
    {
        return;
    }
    if (ModemPort_Contains(pcLine, m_stModemPortRuntime.stActiveCommand.pcExpected) != 0U)
    {
        ModemPort_CopyMatchedResponse(pcLine);
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_OK);
    }
    else if ((ModemPort_Contains(pcLine, "ERROR") != 0U)
             || (ModemPort_Contains(pcLine, "BUSY") != 0U)
             || (ModemPort_Contains(pcLine, "NO CARRIER") != 0U))
    {
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_ERROR);
    }
}

/*******************************************************
  * Name    :ModemPort_IsPromptExpected
  * Author  :LoH
  * Function:Return whether the active AT transaction waits for a CIPSEND prompt.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static uint8_t ModemPort_IsPromptExpected(void)
{
    return ((m_stModemPortRuntime.u8Active != 0U)
            && (m_stModemPortRuntime.stActiveCommand.pcExpected != 0)
            && (strcmp(m_stModemPortRuntime.stActiveCommand.pcExpected, MODEM_PORT_PROMPT_TOKEN) == 0)) ? 1U : 0U;
}

/*******************************************************
  * Name    :ModemPort_Init
  * Author  :LoH
  * Function:Reset the AT queue, parser and transaction data.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemPort_Init(void)
{
    (void)memset(&m_stModemPortRuntime, 0, sizeof(m_stModemPortRuntime));
    (void)memset(&m_stModemPortStatus, 0, sizeof(m_stModemPortStatus));
    m_stModemPortStatus.enLastResult = MODEM_PORT_RESULT_IDLE;
}

/*******************************************************
  * Name    :ModemPort_QueueCommand
  * Author  :LoH
  * Function:Queue one AT command and its expected response token.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
uint8_t ModemPort_QueueCommand(const char *pcCommand, const char *pcExpected, uint32_t u32TimeoutMs)
{
    if ((pcCommand == 0)
        || (pcExpected == 0)
        || (u32TimeoutMs == 0U)
        || (m_stModemPortRuntime.u8Count >= MODEM_PORT_QUEUE_LENGTH)
        || (m_stModemPortStatus.u8HasFailure != 0U))
    {
        return 0U;
    }
    m_stModemPortRuntime.astCommandQueue[m_stModemPortRuntime.u8WriteIndex].pcCommand = pcCommand;
    m_stModemPortRuntime.astCommandQueue[m_stModemPortRuntime.u8WriteIndex].pcExpected = pcExpected;
    m_stModemPortRuntime.astCommandQueue[m_stModemPortRuntime.u8WriteIndex].u32TimeoutMs = u32TimeoutMs;
    m_stModemPortRuntime.u8WriteIndex++;
    if (m_stModemPortRuntime.u8WriteIndex >= MODEM_PORT_QUEUE_LENGTH)
    {
        m_stModemPortRuntime.u8WriteIndex = 0U;
    }
    m_stModemPortRuntime.u8Count++;
    return 1U;
}

/*******************************************************
  * Name    :ModemPort_SendData
  * Author  :LoH
  * Function:Send one fixed-length CIPSEND payload and wait for its result URC.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemPort_SendData(const uint8_t *pu8Data, uint16_t u16Length, const char *pcExpected, uint32_t u32TimeoutMs)
{
    uint16_t u16Offset = 0U;
    uint8_t u8ChunkLength;

    if ((pu8Data == 0)
        || (u16Length == 0U)
        || (pcExpected == 0)
        || (u32TimeoutMs == 0U)
        || (ModemPort_IsIdle() == 0U)
        || (m_stModemPortStatus.u8HasFailure != 0U))
    {
        return 0U;
    }

    while (u16Offset < u16Length)
    {
        u8ChunkLength = ((u16Length - u16Offset) > 255U) ? 255U : (uint8_t)(u16Length - u16Offset);
        if (Uart_TxData((uint8_t *)&pu8Data[u16Offset], u8ChunkLength) == FALSE)
        {
            ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_ERROR);
            return 0U;
        }
        u16Offset += u8ChunkLength;
    }

    m_stModemPortRuntime.stActiveCommand.pcCommand = 0;
    m_stModemPortRuntime.stActiveCommand.pcExpected = pcExpected;
    m_stModemPortRuntime.stActiveCommand.u32TimeoutMs = u32TimeoutMs;
    m_stModemPortRuntime.u32StartMs = osKernelSysTick();
    m_stModemPortRuntime.u8Active = 1U;
    HMI_LOG_MODEM("TX_DATA\r\n");
    return 1U;
}

/*******************************************************
  * Name    :ModemPort_Process
  * Author  :LoH
  * Function:Run queued AT commands, text/hex parser and timeout supervision.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemPort_Process(void)
{
    uint8_t u8Data;

    if (m_stModemPortRuntime.u8Active == 0U)
    {
        ModemPort_StartNextCommand();
    }
    while (Uart_ReadRxData(&u8Data, 1U) != 0U)
    {
        if ((u8Data == '>') && (ModemPort_IsPromptExpected() != 0U))
        {
            ModemPort_HandleResponseLine(MODEM_PORT_PROMPT_TOKEN);
        }
        else if (u8Data == '\r')
        {
            continue;
        }
        if (u8Data == '\n')
        {
            if (m_stModemPortRuntime.u16LineLength != 0U)
            {
                m_stModemPortRuntime.au8Line[m_stModemPortRuntime.u16LineLength] = '\0';
                ModemPort_HandleResponseLine((const char *)m_stModemPortRuntime.au8Line);
                m_stModemPortRuntime.u16LineLength = 0U;
            }
        }
        else if (m_stModemPortRuntime.u16LineLength < (MODEM_PORT_LINE_LENGTH - 1U))
        {
            m_stModemPortRuntime.au8Line[m_stModemPortRuntime.u16LineLength] = u8Data;
            m_stModemPortRuntime.u16LineLength++;
        }
        else
        {
            m_stModemPortRuntime.u16LineLength = 0U;
            m_stModemPortStatus.u8HasFailure = 1U;
            m_stModemPortStatus.enLastResult = MODEM_PORT_RESULT_ERROR;
            HMI_LOG_MODEM("RX_LINE_OVERFLOW\r\n");
        }
    }
    if ((m_stModemPortRuntime.u8Active != 0U)
        && ((osKernelSysTick() - m_stModemPortRuntime.u32StartMs)
            >= m_stModemPortRuntime.stActiveCommand.u32TimeoutMs))
    {
        HMI_LOG_MODEM("AT_TIMEOUT\r\n");
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_TIMEOUT);
    }
}

uint8_t ModemPort_IsIdle(void)
{
    return ((m_stModemPortRuntime.u8Active == 0U)
            && (m_stModemPortRuntime.u8Count == 0U)) ? 1U : 0U;
}

uint8_t ModemPort_HasFailure(void)
{
    return m_stModemPortStatus.u8HasFailure;
}

ModemPortResult_t ModemPort_GetLastResult(void)
{
    return m_stModemPortStatus.enLastResult;
}

/*******************************************************
  * Name    :ModemPort_GetLastMatchedResponse
  * Author  :LoH
  * Function:Return the response line that completed the latest command.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
const char *ModemPort_GetLastMatchedResponse(void)
{
    return m_stModemPortStatus.acLastMatchedResponse;
}

/*******************************************************
  * Name    :ModemPort_GetLastDataLine
  * Author  :LoH
  * Function:Return the latest hexadecimal payload line received from AC7600.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
const char *ModemPort_GetLastDataLine(void)
{
    return m_stModemPortStatus.acLastDataLine;
}

/*******************************************************
  * Name    :ModemPort_TakeRxDataEvent
  * Author  :LoH
  * Function:Return and clear one buffered TCP receive-data notification.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemPort_TakeRxDataEvent(uint8_t *pu8LinkId)
{
    if ((pu8LinkId == 0) || (m_stModemPortStatus.u8RxDataPending == 0U))
    {
        return 0U;
    }

    *pu8LinkId = m_stModemPortStatus.u8RxDataLinkId;
    m_stModemPortStatus.u8RxDataPending = 0U;
    return 1U;
}

/*******************************************************
  * Name    :ModemPort_TakePdnDeactivation
  * Author  :LoH
  * Function:Return and clear one AC7600 PDP deactivation notification.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
uint8_t ModemPort_TakePdnDeactivation(void)
{
    if (m_stModemPortStatus.u8PdnDeactivated == 0U)
    {
        return 0U;
    }

    m_stModemPortStatus.u8PdnDeactivated = 0U;
    return 1U;
}

void ModemPort_ClearFailure(void)
{
    if (m_stModemPortRuntime.u8Active == 0U)
    {
        m_stModemPortRuntime.u8ReadIndex = 0U;
        m_stModemPortRuntime.u8WriteIndex = 0U;
        m_stModemPortRuntime.u8Count = 0U;
        m_stModemPortStatus.u8HasFailure = 0U;
        m_stModemPortStatus.u8RxDataPending = 0U;
        m_stModemPortStatus.u8PdnDeactivated = 0U;
        m_stModemPortStatus.enLastResult = MODEM_PORT_RESULT_IDLE;
        m_stModemPortStatus.acLastMatchedResponse[0] = '\0';
        m_stModemPortStatus.acLastDataLine[0] = '\0';
    }
}
