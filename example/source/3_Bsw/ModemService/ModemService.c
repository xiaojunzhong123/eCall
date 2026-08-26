/*******************************************************************************
 * @file  ModemService.c
 * @brief Modem call service P1 mock implementation.
 ******************************************************************************/
#include "ModemService.h"

static ModemCallState_t m_enCallState = MODEM_CALL_IDLE;
static uint32_t m_u32ProcessCount = 0U;
static EcallMsd_t m_stLastMsd;

void ModemService_Init(void)
{
    m_enCallState = MODEM_CALL_IDLE;
    m_u32ProcessCount = 0U;
}

void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd)
{
    if ((pstMsd != 0) && ((m_enCallState == MODEM_CALL_IDLE) ||
                             (m_enCallState == MODEM_CALL_ENDED) ||
                             (m_enCallState == MODEM_CALL_FAILED)))
    {
        m_stLastMsd = *pstMsd;
        m_u32ProcessCount = 0U;
        m_enCallState = MODEM_CALL_DIALING;
    }
}

void ModemService_Process(void)
{
    if (m_enCallState == MODEM_CALL_DIALING)
    {
        m_u32ProcessCount++;
        if (m_u32ProcessCount >= 3U)
        {
            m_u32ProcessCount = 0U;
            m_enCallState = MODEM_CALL_ACTIVE;
        }
    }
    else if (m_enCallState == MODEM_CALL_ACTIVE)
    {
        m_u32ProcessCount++;
        if (m_u32ProcessCount >= 10U)
        {
            m_enCallState = MODEM_CALL_ENDED;
        }
    }
    else
    {
        /* No processing is required for other mock states. */
    }
}

ModemCallState_t ModemService_GetCallState(void)
{
    return m_enCallState;
}
