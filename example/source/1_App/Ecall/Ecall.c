/*******************************************************************************
 * @file  Ecall.c
 * @brief TBOX eCall application state machine.
 ******************************************************************************/
#include "Ecall.h"
#include <stddef.h>
#include "GnssService.h"
#include "Hmi.h"
#include "ModemService.h"
#include "PowerService.h"

static EcallState_t m_enEcallState = ECALL_STATE_STANDBY;
static EcallMsd_t m_stEcallMsd;
static uint8_t m_u8SessionActive = 0U;

static void Ecall_AddTrigger(EcallTrigger_t enTrigger)
{
    if (enTrigger == ECALL_TRIGGER_MANUAL)
    {
        m_stEcallMsd.trigger_mask |= 0x01U;
    }
    else if (enTrigger == ECALL_TRIGGER_AUTO)
    {
        m_stEcallMsd.trigger_mask |= 0x02U;
    }
    else if (enTrigger == ECALL_TRIGGER_CAN)
    {
        m_stEcallMsd.trigger_mask |= 0x04U;
    }
    else
    {
        /* Invalid trigger is ignored. */
    }
}

void Ecall_Init(void)
{
    m_enEcallState = ECALL_STATE_STANDBY;
    m_stEcallMsd.trigger_mask = 0U;
    m_stEcallMsd.gnss_valid = 0U;
    m_stEcallMsd.latitude_e7 = 0;
    m_stEcallMsd.longitude_e7 = 0;
    m_stEcallMsd.utc_seconds = 0U;
    m_u8SessionActive = 0U;
}

void Ecall_OnTrigger(EcallTrigger_t enTrigger)
{
    if (m_u8SessionActive == 0U)
    {
        m_stEcallMsd.trigger_mask = 0U;
        m_stEcallMsd.gnss_valid = 0U;
        m_stEcallMsd.latitude_e7 = 0;
        m_stEcallMsd.longitude_e7 = 0;
        m_stEcallMsd.utc_seconds = 0U;
        m_u8SessionActive = 1U;
        PowerService_SetEcallLock(1U);
        m_enEcallState = ECALL_STATE_WAIT_GNSS;
        Hmi_ShowEcallState(m_enEcallState);
    }

    Ecall_AddTrigger(enTrigger);
}

void Ecall_OnGnssResult(const EcallMsd_t *pstMsd, uint8_t u8Valid)
{
    if ((m_u8SessionActive == 0U) || (m_enEcallState != ECALL_STATE_WAIT_GNSS))
    {
        return;
    }

    if (pstMsd != NULL)
    {
        m_stEcallMsd.latitude_e7 = pstMsd->latitude_e7;
        m_stEcallMsd.longitude_e7 = pstMsd->longitude_e7;
        m_stEcallMsd.utc_seconds = pstMsd->utc_seconds;
    }
    m_stEcallMsd.gnss_valid = u8Valid;

    m_enEcallState = ECALL_STATE_BUILD_MSD;
    Hmi_ShowEcallState(m_enEcallState);

    /* P1 only freezes the MSD structure; standard encoding is deferred. */
    ModemService_RequestEmergencyCall(&m_stEcallMsd);
    m_enEcallState = ECALL_STATE_WAIT_CALL_ACTIVE;
    Hmi_ShowEcallState(m_enEcallState);
}

void Ecall_OnGnssTimeout(void)
{
    Ecall_OnGnssResult(NULL, 0U);
}

void Ecall_OnCallActive(void)
{
    if ((m_u8SessionActive != 0U) && (m_enEcallState == ECALL_STATE_WAIT_CALL_ACTIVE))
    {
        m_enEcallState = ECALL_STATE_IN_CALL;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

void Ecall_OnCallEnded(void)
{
    if (m_u8SessionActive != 0U)
    {
        m_enEcallState = ECALL_STATE_COMPLETE;
        Hmi_ShowEcallState(m_enEcallState);
        PowerService_SetEcallLock(0U);
        m_u8SessionActive = 0U;
        m_enEcallState = ECALL_STATE_STANDBY;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

void Ecall_OnCallFailed(void)
{
    if (m_u8SessionActive != 0U)
    {
        m_enEcallState = ECALL_STATE_FAILED;
        Hmi_ShowEcallState(m_enEcallState);
        PowerService_SetEcallLock(0U);
        m_u8SessionActive = 0U;
        m_enEcallState = ECALL_STATE_STANDBY;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

EcallState_t Ecall_GetState(void)
{
    return m_enEcallState;
}

uint8_t Ecall_IsSessionActive(void)
{
    return m_u8SessionActive;
}
