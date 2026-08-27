/*******************************************************
  * Name    :ModemService.c
  * Author  :LoH
  * Function:Modem emergency call P1 mock implementation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "ModemService.h"
#include "Hmi.h"
#include "ModemAt_Cfg.h"
#include "ModemPort.h"

#define MODEM_NETWORK_PROCESS_PERIOD_MS     100U
#define MODEM_NETWORK_SLEEP_HOLD_MS         300000UL

static ModemCallState_t m_enCallState = MODEM_CALL_IDLE;
static ModemNetworkState_t m_enNetworkState = MODEM_NETWORK_SLEEP;
static uint16_t m_u16NetworkProcessCount = 0U;
static uint16_t m_u16CallProcessCount = 0U;
static uint8_t m_u8EmergencyRequestPending = 0U;
static uint8_t m_u8NetworkSleepRequested = 0U;
static uint8_t m_u8NetworkProbeRequested = 0U;
static uint32_t m_u32NetworkSleepHoldMs = 0U;
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
    m_enNetworkState = MODEM_NETWORK_SLEEP;
    m_u16NetworkProcessCount = 0U;
    m_u16CallProcessCount = 0U;
    m_u8EmergencyRequestPending = 0U;
    m_u8NetworkSleepRequested = 0U;
    m_u8NetworkProbeRequested = 0U;
    m_u32NetworkSleepHoldMs = 0U;
    ModemPort_Init();
}

/*******************************************************
  * Name    :ModemService_RequestEmergencyCall
  * Author  :LoH
  * Function:Latch MSD and request mock emergency network service.
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
        m_u16CallProcessCount = 0U;
        m_u8EmergencyRequestPending = 1U;
        m_u8NetworkSleepRequested = 0U;
        m_enCallState = MODEM_CALL_PREPARING;

        if (m_enNetworkState == MODEM_NETWORK_ONLINE)
        {
            m_enNetworkState = MODEM_NETWORK_MSD_UPLOADING;
            m_u16NetworkProcessCount = 0U;
        }
        else if (m_enNetworkState == MODEM_NETWORK_SLEEP)
        {
            m_enNetworkState = MODEM_NETWORK_AT_SYNC;
            m_u16NetworkProcessCount = 0U;
        }
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkOnline
  * Author  :LoH
  * Function:Request normal KL15-controlled network online state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void ModemService_RequestNetworkOnline(void)
{
    m_u8NetworkSleepRequested = 0U;
    m_u32NetworkSleepHoldMs = 0U;

    if (m_enNetworkState == MODEM_NETWORK_SLEEP)
    {
        m_enNetworkState = MODEM_NETWORK_AT_SYNC;
        m_u16NetworkProcessCount = 0U;
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkSleep
  * Author  :LoH
  * Function:Request normal network sleep after pending emergency work.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void ModemService_RequestNetworkSleep(void)
{
    if (m_u8NetworkSleepRequested == 0U)
    {
        m_u8NetworkSleepRequested = 1U;
        m_u32NetworkSleepHoldMs = 0U;
        HMI_LOG_MODEM("KL15_OFF_HOLD\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_Process
  * Author  :LoH
  * Function:Advance mock ML307A network, upload and call states.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void ModemService_Process(void)
{
    if (m_enNetworkState == MODEM_NETWORK_AT_SYNC)
    {
        if (m_u8NetworkProbeRequested == 0U)
        {
            ModemPort_ClearFailure();
            if ((ModemPort_QueueCommand(MODEM_AT_CMD_BASIC, MODEM_AT_RSP_OK,
                                        MODEM_AT_CFG_SYNC_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIM_STATUS, MODEM_AT_RSP_OK,
                                            MODEM_AT_CFG_SIM_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_SIGNAL_QUALITY, MODEM_AT_RSP_OK,
                                            MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U)
                && (ModemPort_QueueCommand(MODEM_AT_CMD_CIRCUIT_REGISTRATION, MODEM_AT_RSP_OK,
                                            MODEM_AT_CFG_NETWORK_TIMEOUT_MS) != 0U))
            {
                m_u8NetworkProbeRequested = 1U;
            }
            else
            {
                HMI_LOG_MODEM("AT_QUEUE_FAILED\r\n");
                m_enNetworkState = MODEM_NETWORK_SLEEP;
                m_enCallState = MODEM_CALL_FAILED;
            }
        }
        else if (ModemPort_HasFailure() != 0U)
        {
            HMI_LOG_MODEM("NETWORK_PROBE_FAILED\r\n");
            m_u8NetworkProbeRequested = 0U;
            m_enNetworkState = MODEM_NETWORK_SLEEP;
            if (m_u8EmergencyRequestPending != 0U)
            {
                m_enCallState = MODEM_CALL_FAILED;
            }
        }
        else if (ModemPort_IsIdle() != 0U)
        {
            m_u8NetworkProbeRequested = 0U;
            m_u16NetworkProcessCount = 0U;
            m_enNetworkState = MODEM_NETWORK_TCP_CONNECTING;
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_SIM_CHECK)
    {
        m_u16NetworkProcessCount++;
        if (m_u16NetworkProcessCount >= 1U)
        {
            m_u16NetworkProcessCount = 0U;
            m_enNetworkState = MODEM_NETWORK_REGISTERING;
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_REGISTERING)
    {
        m_u16NetworkProcessCount++;
        if (m_u16NetworkProcessCount >= 2U)
        {
            m_u16NetworkProcessCount = 0U;
            m_enNetworkState = MODEM_NETWORK_TCP_CONNECTING;
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_TCP_CONNECTING)
    {
        m_u16NetworkProcessCount++;
        if (m_u16NetworkProcessCount >= 2U)
        {
            m_u16NetworkProcessCount = 0U;
            m_enNetworkState = MODEM_NETWORK_PLATFORM_REGISTERING;
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_PLATFORM_REGISTERING)
    {
        m_u16NetworkProcessCount++;
        if (m_u16NetworkProcessCount >= 1U)
        {
            m_u16NetworkProcessCount = 0U;
            m_enNetworkState = MODEM_NETWORK_ONLINE;
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_ONLINE)
    {
        if (m_u8EmergencyRequestPending != 0U)
        {
            m_enNetworkState = MODEM_NETWORK_MSD_UPLOADING;
            m_u16NetworkProcessCount = 0U;
        }
        else if ((m_u8NetworkSleepRequested != 0U)
                 && ((m_enCallState == MODEM_CALL_IDLE)
                     || (m_enCallState == MODEM_CALL_ENDED)
                     || (m_enCallState == MODEM_CALL_FAILED)))
        {
            if (m_u32NetworkSleepHoldMs < MODEM_NETWORK_SLEEP_HOLD_MS)
            {
                m_u32NetworkSleepHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
            }

            if (m_u32NetworkSleepHoldMs >= MODEM_NETWORK_SLEEP_HOLD_MS)
            {
                HMI_LOG_MODEM("KL15_OFF_SLEEP\r\n");
                m_enNetworkState = MODEM_NETWORK_SLEEP;
                m_u16NetworkProcessCount = 0U;
                m_u32NetworkSleepHoldMs = 0U;
            }
        }
        else
        {
            /* Future ML307A heartbeat is driven by the ModemTask timer. */
        }
    }
    else if (m_enNetworkState == MODEM_NETWORK_MSD_UPLOADING)
    {
        m_u16NetworkProcessCount++;
        if (m_u16NetworkProcessCount >= 1U)
        {
            m_u16NetworkProcessCount = 0U;
            m_u8EmergencyRequestPending = 0U;
            m_enCallState = MODEM_CALL_DIALING;
            m_enNetworkState = MODEM_NETWORK_ONLINE;
        }
    }
    else
    {
        /* MODEM_NETWORK_SLEEP waits for KL15 or eCall requests. */
    }

    if (m_enCallState == MODEM_CALL_DIALING)
    {
        m_u16CallProcessCount++;
        if (m_u16CallProcessCount >= 3U)
        {
            m_u16CallProcessCount = 0U;
            m_enCallState = MODEM_CALL_ACTIVE;
        }
    }
    else if (m_enCallState == MODEM_CALL_ACTIVE)
    {
        m_u16CallProcessCount++;
        if (m_u16CallProcessCount >= 10U)
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

/*******************************************************
  * Name    :ModemService_GetNetworkState
  * Author  :LoH
  * Function:Return current modem network state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
ModemNetworkState_t ModemService_GetNetworkState(void)
{
    return m_enNetworkState;
}
