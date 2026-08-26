/*******************************************************
  * Name    :Hmi.c
  * Author  :LoH
  * Function:TBOX HMI log implementation with static ring buffer.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "FreeRTOS.h"
#include "Hmi.h"
#include "Uart.h"

#define HMI_LOG_SLOT_COUNT             8U
#define HMI_LOG_SLOT_LENGTH            64U

typedef struct
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Length;
} HmiLogSlot_t;

static HmiLogSlot_t m_astLogSlot[HMI_LOG_SLOT_COUNT];
static uint8_t m_u8ReadIndex = 0U;
static uint8_t m_u8WriteIndex = 0U;
static uint8_t m_u8LogCount = 0U;
static uint16_t m_u16DropCount = 0U;

/*******************************************************
  * Name    :Hmi_Write
  * Author  :LoH
  * Function:Send one complete log record through UART.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void Hmi_Write(const uint8_t *pu8Text, uint8_t u8Length)
{
    (void)Uart_TxData((uint8_t *)pu8Text, u8Length);
}

/*******************************************************
  * Name    :Hmi_Init
  * Author  :LoH
  * Function:Reset HMI log ring buffer runtime data.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Init(void)
{
    m_u8ReadIndex = 0U;
    m_u8WriteIndex = 0U;
    m_u8LogCount = 0U;
    m_u16DropCount = 0U;
}

/*******************************************************
  * Name    :Hmi_Log
  * Author  :LoH
  * Function:Copy one log record into static ring buffer.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Log(const char *pcText)
{
    uint8_t u8Length = 0U;
    uint8_t u8Index;

    if (pcText == 0)
    {
        return;
    }

    while ((pcText[u8Length] != '\0') && (u8Length < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        u8Length++;
    }

    portENTER_CRITICAL();
    if (m_u8LogCount >= HMI_LOG_SLOT_COUNT)
    {
        m_u16DropCount++;
    }
    else
    {
        u8Index = 0U;
        while (u8Index < u8Length)
        {
            m_astLogSlot[m_u8WriteIndex].au8Text[u8Index] = (uint8_t)pcText[u8Index];
            u8Index++;
        }
        m_astLogSlot[m_u8WriteIndex].au8Text[u8Length] = '\0';
        m_astLogSlot[m_u8WriteIndex].u8Length = u8Length;
        m_u8WriteIndex++;
        if (m_u8WriteIndex >= HMI_LOG_SLOT_COUNT)
        {
            m_u8WriteIndex = 0U;
        }
        m_u8LogCount++;
    }
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :Hmi_LogWithPrefix
  * Author  :LoH
  * Function:Merge module prefix and text into one record.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_LogWithPrefix(const char *pcPrefix, const char *pcText)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Index = 0U;

    if ((pcPrefix == 0) || (pcText == 0))
    {
        return;
    }

    while ((*pcPrefix != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcPrefix++;
    }
    while ((*pcText != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcText++;
    }
    au8Text[u8Index] = '\0';
    Hmi_Log((const char *)au8Text);
}

/*******************************************************
  * Name    :Hmi_Process
  * Author  :LoH
  * Function:Transmit one log record in AppTask context.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_Process(void)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Length = 0U;
    uint8_t u8Index;

    portENTER_CRITICAL();
    if (m_u8LogCount != 0U)
    {
        u8Length = m_astLogSlot[m_u8ReadIndex].u8Length;
        u8Index = 0U;
        while (u8Index < u8Length)
        {
            au8Text[u8Index] = m_astLogSlot[m_u8ReadIndex].au8Text[u8Index];
            u8Index++;
        }
        m_u8ReadIndex++;
        if (m_u8ReadIndex >= HMI_LOG_SLOT_COUNT)
        {
            m_u8ReadIndex = 0U;
        }
        m_u8LogCount--;
    }
    portEXIT_CRITICAL();

    if (u8Length != 0U)
    {
        Hmi_Write(au8Text, u8Length);
    }
}

/*******************************************************
  * Name    :Hmi_ShowTaskStarted
  * Author  :LoH
  * Function:Build and submit task-started status message.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_ShowTaskStarted(const char *pcTaskName)
{
#if (HMI_LOG_TASK_ENABLE != 0U)
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Index = 0U;
    const char *pcSuffix = " started\r\n";

    if (pcTaskName == 0)
    {
        return;
    }

    while ((*pcTaskName != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcTaskName++;
    }
    while ((*pcSuffix != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcSuffix++;
    }
    au8Text[u8Index] = '\0';
    HMI_LOG_TASK((const char *)au8Text);
#else
    (void)pcTaskName;
#endif
}

/*******************************************************
  * Name    :Hmi_ShowEcallState
  * Author  :LoH
  * Function:Convert eCall state to a module log message.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_ShowEcallState(EcallState_t enState)
{
#if (HMI_LOG_ECALL_ENABLE != 0U)
    if (enState == ECALL_STATE_STANDBY)
    {
        HMI_LOG_ECALL("STANDBY\r\n");
    }
    else if (enState == ECALL_STATE_WAIT_GNSS)
    {
        HMI_LOG_ECALL("WAIT_GNSS\r\n");
    }
    else if (enState == ECALL_STATE_BUILD_MSD)
    {
        HMI_LOG_ECALL("BUILD_MSD\r\n");
    }
    else if (enState == ECALL_STATE_WAIT_CALL_ACTIVE)
    {
        HMI_LOG_ECALL("WAIT_CALL_ACTIVE\r\n");
    }
    else if (enState == ECALL_STATE_IN_CALL)
    {
        HMI_LOG_ECALL("IN_CALL\r\n");
    }
    else if (enState == ECALL_STATE_COMPLETE)
    {
        HMI_LOG_ECALL("COMPLETE\r\n");
    }
    else if (enState == ECALL_STATE_FAILED)
    {
        HMI_LOG_ECALL("FAILED\r\n");
    }
    else
    {
        HMI_LOG_ECALL("UNKNOWN\r\n");
    }
#else
    (void)enState;
#endif
}

/*******************************************************
  * Name    :Hmi_ShowText
  * Author  :LoH
  * Function:Submit plain HMI log message.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Hmi_ShowText(const char *pcText)
{
    Hmi_Log(pcText);
}
