/*******************************************************
  * Name    :ModemService.c
  * Author  :LoH
  * Function:Modem emergency call P1 mock implementation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "ModemService.h"

static ModemCallState_t m_enCallState = MODEM_CALL_IDLE;
static uint32_t m_u32ProcessCount = 0U;
static EcallMsd_t m_stLastMsd;

/*******************************************************
  * Name    :ModemService_Init
  * Author  :LoH
  * Function:Reset modem call state before task scheduling.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void ModemService_Init(void)
{
    m_enCallState = MODEM_CALL_IDLE;
    m_u32ProcessCount = 0U;
}

/*******************************************************
  * Name    :ModemService_RequestEmergencyCall
  * Author  :LoH
  * Function:Latch MSD and start mock dialing sequence.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
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

/*******************************************************
  * Name    :ModemService_Process
  * Author  :LoH
  * Function:Advance mock dialing, active and end states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
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

/*******************************************************
  * Name    :ModemService_GetCallState
  * Author  :LoH
  * Function:Return modem call state for EcallTask notification.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
ModemCallState_t ModemService_GetCallState(void)
{
    return m_enCallState;
}
