/*******************************************************
  * Name    :Ecall.c
  * Author  :LoH
  * Function:TBOX eCall application state machine.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "Ecall.h"
#include <stddef.h>
#include "GnssService.h"
#include "Hmi.h"
#include "ModemService.h"
#include "PowerService.h"

static EcallState_t m_enEcallState = ECALL_STATE_STANDBY;
static EcallMsd_t m_stEcallMsd;
static uint8_t m_u8SessionActive = 0U;
static uint8_t m_u8CallRequestSent = 0U;

/*******************************************************
  * Name    :Ecall_ClearMsd
  * Author  :LoH
  * Function:Clear the local MSD cache.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void Ecall_ClearMsd(void)
{
    m_stEcallMsd.trigger_mask = 0U;
    m_stEcallMsd.gnss_valid = 0U;
    m_stEcallMsd.latitude_e7 = 0;
    m_stEcallMsd.longitude_e7 = 0;
    m_stEcallMsd.utc_seconds = 0U;
}

/*******************************************************
  * Name    :Ecall_GetTriggerMask
  * Author  :LoH
  * Function:Convert one trigger source to persistent trigger bit.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static uint8_t Ecall_GetTriggerMask(EcallTrigger_t enTrigger)
{
    if (enTrigger == ECALL_TRIGGER_MANUAL)
    {
        return ECALL_TRIGGER_MASK_MANUAL;
    }
    else if (enTrigger == ECALL_TRIGGER_AUTO)
    {
        return ECALL_TRIGGER_MASK_AUTO;
    }
    else if (enTrigger == ECALL_TRIGGER_CAN)
    {
        return ECALL_TRIGGER_MASK_CAN;
    }
    else
    {
        return 0U;
    }
}

/*******************************************************
  * Name    :Ecall_Init
  * Author  :LoH
  * Function:Initialize all eCall session data to standby.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_Init(void)
{
    m_enEcallState = ECALL_STATE_STANDBY;
    Ecall_ClearMsd();
    m_u8SessionActive = 0U;
    m_u8CallRequestSent = 0U;
    Hmi_SetEcallCancelWindow(0U);
}

/*******************************************************
  * Name    :Ecall_OnTrigger
  * Author  :LoH
  * Function:Process one eCall trigger source.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnTrigger(EcallTrigger_t enTrigger)
{
    Ecall_OnTriggerMask(Ecall_GetTriggerMask(enTrigger));
}

/*******************************************************
  * Name    :Ecall_OnTriggerMask
  * Author  :LoH
  * Function:Start session or merge active trigger sources.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnTriggerMask(uint8_t u8TriggerMask)
{
    uint8_t u8AutomaticTrigger;

    u8TriggerMask &= ECALL_TRIGGER_MASK_ALL;
    if (u8TriggerMask == 0U)
    {
        return;
    }

    u8AutomaticTrigger = u8TriggerMask & (ECALL_TRIGGER_MASK_AUTO | ECALL_TRIGGER_MASK_CAN);

    if (m_u8SessionActive == 0U)
    {
        Ecall_ClearMsd();
        m_stEcallMsd.trigger_mask = u8TriggerMask;
        m_u8SessionActive = 1U;
        m_u8CallRequestSent = 0U;
        PowerService_SetEcallLock(1U);

        if (u8AutomaticTrigger != 0U)
        {
            m_enEcallState = ECALL_STATE_WAIT_GNSS;
        }
        else
        {
            m_enEcallState = ECALL_STATE_MANUAL_CANCEL_WINDOW;
            Hmi_SetEcallCancelWindow(1U);
        }
        Hmi_ShowEcallState(m_enEcallState);
    }
    else
    {
        m_stEcallMsd.trigger_mask |= u8TriggerMask;

        if ((m_enEcallState == ECALL_STATE_MANUAL_CANCEL_WINDOW)
            && (u8AutomaticTrigger != 0U))
        {
            Hmi_SetEcallCancelWindow(0U);
            m_enEcallState = ECALL_STATE_WAIT_GNSS;
            HMI_LOG_ECALL("AUTO_TRIGGER_PREEMPTS_CANCEL\r\n");
            Hmi_ShowEcallState(m_enEcallState);
        }
    }
}

/*******************************************************
  * Name    :Ecall_OnManualCancel
  * Author  :LoH
  * Function:Cancel a manual request during cancel window.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnManualCancel(void)
{
    if (m_enEcallState != ECALL_STATE_MANUAL_CANCEL_WINDOW)
    {
        return;
    }

    Hmi_SetEcallCancelWindow(0U);
    Ecall_ClearMsd();
    PowerService_SetEcallLock(0U);
    m_u8SessionActive = 0U;
    m_u8CallRequestSent = 0U;
    m_enEcallState = ECALL_STATE_STANDBY;
    HMI_LOG_ECALL("MANUAL_CANCELLED\r\n");
    Hmi_ShowEcallState(m_enEcallState);
}

/*******************************************************
  * Name    :Ecall_OnManualCancelTimeout
  * Author  :LoH
  * Function:Continue manual eCall after cancel window timeout.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnManualCancelTimeout(void)
{
    if (m_enEcallState != ECALL_STATE_MANUAL_CANCEL_WINDOW)
    {
        return;
    }

    Hmi_SetEcallCancelWindow(0U);
    m_enEcallState = ECALL_STATE_WAIT_GNSS;
    HMI_LOG_ECALL("MANUAL_CANCEL_WINDOW_TIMEOUT\r\n");
    Hmi_ShowEcallState(m_enEcallState);
}

/*******************************************************
  * Name    :Ecall_OnGnssResult
  * Author  :LoH
  * Function:Freeze GNSS data and request emergency call.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
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
    if (m_u8CallRequestSent == 0U)
    {
        m_u8CallRequestSent = 1U;
        ModemService_RequestEmergencyCall(&m_stEcallMsd);
    }
    m_enEcallState = ECALL_STATE_WAIT_CALL_ACTIVE;
    Hmi_ShowEcallState(m_enEcallState);
}

/*******************************************************
  * Name    :Ecall_OnGnssTimeout
  * Author  :LoH
  * Function:Handle GNSS timeout and continue emergency call.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnGnssTimeout(void)
{
    Ecall_OnGnssResult(NULL, 0U);
}

/*******************************************************
  * Name    :Ecall_OnCallActive
  * Author  :LoH
  * Function:Update state when modem reports active call.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnCallActive(void)
{
    if ((m_u8SessionActive != 0U) && (m_enEcallState == ECALL_STATE_WAIT_CALL_ACTIVE))
    {
        m_enEcallState = ECALL_STATE_IN_CALL;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

/*******************************************************
  * Name    :Ecall_OnCallEnded
  * Author  :LoH
  * Function:Complete active session after call ends.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnCallEnded(void)
{
    if (m_u8SessionActive != 0U)
    {
        Hmi_SetEcallCancelWindow(0U);
        m_enEcallState = ECALL_STATE_COMPLETE;
        Hmi_ShowEcallState(m_enEcallState);
        PowerService_SetEcallLock(0U);
        m_u8SessionActive = 0U;
        m_u8CallRequestSent = 0U;
        m_enEcallState = ECALL_STATE_STANDBY;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

/*******************************************************
  * Name    :Ecall_OnCallFailed
  * Author  :LoH
  * Function:Complete active session after call failure.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void Ecall_OnCallFailed(void)
{
    if (m_u8SessionActive != 0U)
    {
        Hmi_SetEcallCancelWindow(0U);
        m_enEcallState = ECALL_STATE_FAILED;
        Hmi_ShowEcallState(m_enEcallState);
        PowerService_SetEcallLock(0U);
        m_u8SessionActive = 0U;
        m_u8CallRequestSent = 0U;
        m_enEcallState = ECALL_STATE_STANDBY;
        Hmi_ShowEcallState(m_enEcallState);
    }
}

/*******************************************************
  * Name    :Ecall_GetState
  * Author  :LoH
  * Function:Return current eCall session state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
EcallState_t Ecall_GetState(void)
{
    return m_enEcallState;
}

/*******************************************************
  * Name    :Ecall_IsSessionActive
  * Author  :LoH
  * Function:Return whether an eCall session is active.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
uint8_t Ecall_IsSessionActive(void)
{
    return m_u8SessionActive;
}
