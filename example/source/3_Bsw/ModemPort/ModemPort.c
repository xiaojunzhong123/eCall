/*******************************************************
  * Name    :ModemPort.c
  * Author  :LoH
  * Function:Modem UART AT queue port implementation.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#include "cmsis_os.h"
#include <string.h>
#include "Hmi.h"
#include "ModemPort.h"
#include "Uart.h"

#define MODEM_PORT_QUEUE_LENGTH     8U
#define MODEM_PORT_LINE_LENGTH      128U

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
    uint8_t u8LineLength;
    uint32_t u32StartMs;
} ModemPortRuntime_t;

typedef struct
{
    ModemPortResult_t enLastResult;
    uint8_t u8HasFailure;
    char acLastMatchedResponse[MODEM_PORT_LINE_LENGTH];
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
    uint8_t u8Index = 0U;

    if (pcLine == 0)
    {
        m_stModemPortStatus.acLastMatchedResponse[0] = '\0';
        return;
    }

    while ((pcLine[u8Index] != '\0') && (u8Index < (MODEM_PORT_LINE_LENGTH - 1U)))
    {
        m_stModemPortStatus.acLastMatchedResponse[u8Index] = pcLine[u8Index];
        u8Index++;
    }
    m_stModemPortStatus.acLastMatchedResponse[u8Index] = '\0';
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
    uint8_t u8TextIndex;
    uint8_t u8TokenIndex;

    if ((pcText == 0) || (pcToken == 0))
    {
        return 0U;
    }
    u8TextIndex = 0U;
    while (pcText[u8TextIndex] != '\0')
    {
        u8TokenIndex = 0U;
        while ((pcToken[u8TokenIndex] != '\0')
               && (pcText[u8TextIndex + u8TokenIndex] == pcToken[u8TokenIndex]))
        {
            u8TokenIndex++;
        }
        if (pcToken[u8TokenIndex] == '\0')
        {
            return 1U;
        }
        u8TextIndex++;
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
    Hmi_LogWithPrefix("[MODEM TX] ", m_stModemPortRuntime.stActiveCommand.pcCommand);
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
    Hmi_LogWithPrefix("[MODEM RX] ", pcLine);
    Hmi_Log("\r\n");

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
  * Name    :ModemPort_Process
  * Author  :LoH
  * Function:Run queued AT commands, parser and timeout supervision.
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
        if (u8Data == '\r')
        {
            continue;
        }
        if (u8Data == '\n')
        {
            if (m_stModemPortRuntime.u8LineLength != 0U)
            {
                m_stModemPortRuntime.au8Line[m_stModemPortRuntime.u8LineLength] = '\0';
                ModemPort_HandleResponseLine((const char *)m_stModemPortRuntime.au8Line);
                m_stModemPortRuntime.u8LineLength = 0U;
            }
        }
        else if (m_stModemPortRuntime.u8LineLength < (MODEM_PORT_LINE_LENGTH - 1U))
        {
            m_stModemPortRuntime.au8Line[m_stModemPortRuntime.u8LineLength] = u8Data;
            m_stModemPortRuntime.u8LineLength++;
        }
        else
        {
            m_stModemPortRuntime.u8LineLength = 0U;
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

void ModemPort_ClearFailure(void)
{
    if (m_stModemPortRuntime.u8Active == 0U)
    {
        m_stModemPortRuntime.u8ReadIndex = 0U;
        m_stModemPortRuntime.u8WriteIndex = 0U;
        m_stModemPortRuntime.u8Count = 0U;
        m_stModemPortStatus.u8HasFailure = 0U;
        m_stModemPortStatus.enLastResult = MODEM_PORT_RESULT_IDLE;
        m_stModemPortStatus.acLastMatchedResponse[0] = '\0';
    }
}
