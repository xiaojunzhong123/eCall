/*******************************************************************************
 * @file  Hmi.c
 * @brief Sample display implementation with a static log ring buffer.
 ******************************************************************************/
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

static void Hmi_Write(const uint8_t *pu8Text, uint8_t u8Length)
{
    (void)Uart_TxData((uint8_t *)pu8Text, u8Length);
}

void Hmi_Init(void)
{
    m_u8ReadIndex = 0U;
    m_u8WriteIndex = 0U;
    m_u8LogCount = 0U;
    m_u16DropCount = 0U;
}

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

    taskENTER_CRITICAL();
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
    taskEXIT_CRITICAL();
}

void Hmi_Process(void)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Length = 0U;
    uint8_t u8Index;

    taskENTER_CRITICAL();
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
    taskEXIT_CRITICAL();

    if (u8Length != 0U)
    {
        Hmi_Write(au8Text, u8Length);
    }
}

void Hmi_ShowTaskStarted(const char *pcTaskName)
{
    uint8_t au8Text[HMI_LOG_SLOT_LENGTH];
    uint8_t u8Index = 0U;
    const char *pcPrefix = "[TASK] ";
    const char *pcSuffix = " started\r\n";

    if (pcTaskName == 0)
    {
        return;
    }

    while ((*pcPrefix != '\0') && (u8Index < (HMI_LOG_SLOT_LENGTH - 1U)))
    {
        au8Text[u8Index++] = (uint8_t)*pcPrefix++;
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
    Hmi_Log((const char *)au8Text);
}

void Hmi_ShowEcallState(EcallState_t enState)
{
    if (enState == ECALL_STATE_STANDBY)
    {
        Hmi_ShowText("[ECALL] STANDBY\r\n");
    }
    else if (enState == ECALL_STATE_WAIT_GNSS)
    {
        Hmi_ShowText("[ECALL] WAIT_GNSS\r\n");
    }
    else if (enState == ECALL_STATE_BUILD_MSD)
    {
        Hmi_ShowText("[ECALL] BUILD_MSD\r\n");
    }
    else if (enState == ECALL_STATE_WAIT_CALL_ACTIVE)
    {
        Hmi_ShowText("[ECALL] WAIT_CALL_ACTIVE\r\n");
    }
    else if (enState == ECALL_STATE_IN_CALL)
    {
        Hmi_ShowText("[ECALL] IN_CALL\r\n");
    }
    else if (enState == ECALL_STATE_COMPLETE)
    {
        Hmi_ShowText("[ECALL] COMPLETE\r\n");
    }
    else if (enState == ECALL_STATE_FAILED)
    {
        Hmi_ShowText("[ECALL] FAILED\r\n");
    }
    else
    {
        Hmi_ShowText("[ECALL] UNKNOWN\r\n");
    }
}

void Hmi_ShowText(const char *pcText)
{
    Hmi_Log(pcText);
}
