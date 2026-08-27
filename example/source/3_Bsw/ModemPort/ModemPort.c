/*******************************************************
  * Name    :ModemPort.c
  * Author  :LoH
  * Function:Modem UART AT queue port implementation.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#include "cmsis_os.h"
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

static ModemPortCommand_t m_astCommandQueue[MODEM_PORT_QUEUE_LENGTH];
static ModemPortCommand_t m_stActiveCommand;
static uint8_t m_au8Line[MODEM_PORT_LINE_LENGTH];
static uint8_t m_u8QueueReadIndex = 0U;
static uint8_t m_u8QueueWriteIndex = 0U;
static uint8_t m_u8QueueCount = 0U;
static uint8_t m_u8LineLength = 0U;
static uint8_t m_u8CommandActive = 0U;
static uint8_t m_u8Failure = 0U;
static uint32_t m_u32CommandStartMs = 0U;
static ModemPortResult_t m_enLastResult = MODEM_PORT_RESULT_IDLE;

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
    m_enLastResult = enResult;
    m_u8CommandActive = 0U;
    if (enResult != MODEM_PORT_RESULT_OK)
    {
        m_u8Failure = 1U;
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

    if ((m_u8CommandActive != 0U) || (m_u8QueueCount == 0U) || (m_u8Failure != 0U))
    {
        return;
    }
    m_stActiveCommand = m_astCommandQueue[m_u8QueueReadIndex];
    m_u8QueueReadIndex++;
    if (m_u8QueueReadIndex >= MODEM_PORT_QUEUE_LENGTH)
    {
        m_u8QueueReadIndex = 0U;
    }
    m_u8QueueCount--;
    while ((m_stActiveCommand.pcCommand[u8Length] != '\0') && (u8Length < 255U))
    {
        u8Length++;
    }
    if ((u8Length == 0U) || (m_stActiveCommand.pcCommand[u8Length] != '\0')
        || (Uart_TxData((uint8_t *)m_stActiveCommand.pcCommand, u8Length) == FALSE))
    {
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_ERROR);
        return;
    }
    Hmi_LogWithPrefix("[MODEM TX] ", m_stActiveCommand.pcCommand);
    m_u32CommandStartMs = osKernelSysTick();
    m_u8CommandActive = 1U;
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

    if (m_u8CommandActive == 0U)
    {
        return;
    }
    if (ModemPort_Contains(pcLine, m_stActiveCommand.pcExpected) != 0U)
    {
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
    m_u8QueueReadIndex = 0U;
    m_u8QueueWriteIndex = 0U;
    m_u8QueueCount = 0U;
    m_u8LineLength = 0U;
    m_u8CommandActive = 0U;
    m_u8Failure = 0U;
    m_enLastResult = MODEM_PORT_RESULT_IDLE;
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
    if ((pcCommand == 0) || (pcExpected == 0) || (u32TimeoutMs == 0U)
        || (m_u8QueueCount >= MODEM_PORT_QUEUE_LENGTH) || (m_u8Failure != 0U))
    {
        return 0U;
    }
    m_astCommandQueue[m_u8QueueWriteIndex].pcCommand = pcCommand;
    m_astCommandQueue[m_u8QueueWriteIndex].pcExpected = pcExpected;
    m_astCommandQueue[m_u8QueueWriteIndex].u32TimeoutMs = u32TimeoutMs;
    m_u8QueueWriteIndex++;
    if (m_u8QueueWriteIndex >= MODEM_PORT_QUEUE_LENGTH)
    {
        m_u8QueueWriteIndex = 0U;
    }
    m_u8QueueCount++;
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

    if (m_u8CommandActive == 0U)
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
            if (m_u8LineLength != 0U)
            {
                m_au8Line[m_u8LineLength] = '\0';
                ModemPort_HandleResponseLine((const char *)m_au8Line);
                m_u8LineLength = 0U;
            }
        }
        else if (m_u8LineLength < (MODEM_PORT_LINE_LENGTH - 1U))
        {
            m_au8Line[m_u8LineLength] = u8Data;
            m_u8LineLength++;
        }
        else
        {
            m_u8LineLength = 0U;
            m_u8Failure = 1U;
            m_enLastResult = MODEM_PORT_RESULT_ERROR;
            HMI_LOG_MODEM("RX_LINE_OVERFLOW\r\n");
        }
    }
    if ((m_u8CommandActive != 0U)
        && ((osKernelSysTick() - m_u32CommandStartMs) >= m_stActiveCommand.u32TimeoutMs))
    {
        HMI_LOG_MODEM("AT_TIMEOUT\r\n");
        ModemPort_FinishActiveCommand(MODEM_PORT_RESULT_TIMEOUT);
    }
}

uint8_t ModemPort_IsIdle(void)
{
    return ((m_u8CommandActive == 0U) && (m_u8QueueCount == 0U)) ? 1U : 0U;
}

uint8_t ModemPort_HasFailure(void)
{
    return m_u8Failure;
}

ModemPortResult_t ModemPort_GetLastResult(void)
{
    return m_enLastResult;
}

void ModemPort_ClearFailure(void)
{
    if (m_u8CommandActive == 0U)
    {
        m_u8QueueReadIndex = 0U;
        m_u8QueueWriteIndex = 0U;
        m_u8QueueCount = 0U;
        m_u8Failure = 0U;
        m_enLastResult = MODEM_PORT_RESULT_IDLE;
    }
}
