/*******************************************************
  * Name    :ModemService.c
  * Author  :LoH
  * Function:Modem emergency call service implementation.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#include "ModemService.h"
#include <string.h>
#include "Hmi.h"
#include "ModemCall.h"
#include "ModemNetwork.h"
#include "ModemPlatform.h"
#include "ModemPort.h"

#define MODEM_NETWORK_PROCESS_PERIOD_MS     100U
#define MODEM_NETWORK_SLEEP_HOLD_MS         300000UL

typedef struct
{
    uint8_t u8EmergencyRequestPending;
    uint8_t u8NetworkSleepRequested;
    uint32_t u32NetworkSleepHoldMs;
} ModemServiceRuntime_t;

typedef struct
{
    ModemCallState_t enCallState;
    ModemServiceState_t enServiceState;
} ModemServiceStatus_t;

static ModemServiceRuntime_t s_mSvcRun;
static ModemServiceStatus_t s_mSvcSts;

/*******************************************************
  * Name    :ModemService_EnterNetworkSleep
  * Author  :LoH
  * Function:Stop network and platform work and report a pending eCall failure.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_EnterNetworkSleep(void)
{
    s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
    ModemNetwork_Reset();
    ModemPlatform_Reset();

    if (s_mSvcRun.u8EmergencyRequestPending != 0U)
    {
        s_mSvcSts.enCallState = MODEM_CALL_FAILED;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessNetworkStartup
  * Author  :LoH
  * Function:Drive the private network module until TCP is ready or fails.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessNetworkStartup(void)
{
    ModemNetwork_Process();

    if (ModemNetwork_TakeFailure() != 0U)
    {
        ModemService_EnterNetworkSleep();
    }
    else if (ModemNetwork_TakeTcpConnected() != 0U)
    {
        ModemPlatform_Start();
        s_mSvcSts.enServiceState = MODEM_SERVICE_PLATFORM_ACTIVE;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessPlatform
  * Author  :LoH
  * Function:Drive the private platform workflow and handle its failure event.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessPlatform(void)
{
    ModemPlatformEvent_t enEvent;
    uint8_t u8HeartbeatEnabled;

    u8HeartbeatEnabled = (s_mSvcRun.u8NetworkSleepRequested == 0U) ? 1U : 0U;
    ModemPlatform_Process(u8HeartbeatEnabled);
    enEvent = ModemPlatform_TakeEvent();

    switch (enEvent)
    {
        case MODEM_PLATFORM_EVENT_NONE:
        case MODEM_PLATFORM_EVENT_REGISTERED:
        case MODEM_PLATFORM_EVENT_MSD_STARTED:
        case MODEM_PLATFORM_EVENT_MSD_UPLOADED:
            break;

        case MODEM_PLATFORM_EVENT_FAILED:
        default:
            ModemService_EnterNetworkSleep();
            break;
    }
}

/*******************************************************
  * Name    :ModemService_ProcessOnlineSleep
  * Author  :LoH
  * Function:Apply the KL15 delayed sleep policy after platform online work.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
static void ModemService_ProcessOnlineSleep(void)
{
    if ((s_mSvcRun.u8NetworkSleepRequested != 0U)
        && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        if (s_mSvcRun.u32NetworkSleepHoldMs < MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            s_mSvcRun.u32NetworkSleepHoldMs += MODEM_NETWORK_PROCESS_PERIOD_MS;
        }

        if (s_mSvcRun.u32NetworkSleepHoldMs >= MODEM_NETWORK_SLEEP_HOLD_MS)
        {
            HMI_LOG_MODEM("KL15_OFF_SLEEP\r\n");
            ModemService_EnterNetworkSleep();
            s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        }
    }
}

/*******************************************************
  * Name    :ModemService_Init
  * Author  :LoH
  * Function:Reset modem network, platform and call states before task scheduling.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemService_Init(void)
{
    (void)memset(&s_mSvcRun, 0, sizeof(s_mSvcRun));
    (void)memset(&s_mSvcSts, 0, sizeof(s_mSvcSts));
    s_mSvcSts.enCallState = MODEM_CALL_IDLE;
    s_mSvcSts.enServiceState = MODEM_SERVICE_SLEEP;
    ModemPort_Init();
    ModemNetwork_Init();
    ModemPlatform_Init();
    ModemCall_Init();
}

/*******************************************************
  * Name    :ModemService_RequestEmergencyCall
  * Author  :LoH
  * Function:Latch MSD and request emergency network service.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd)
{
    if ((pstMsd != 0)
        && ((s_mSvcSts.enCallState == MODEM_CALL_IDLE)
            || (s_mSvcSts.enCallState == MODEM_CALL_ENDED)
            || (s_mSvcSts.enCallState == MODEM_CALL_FAILED)))
    {
        ModemCall_Reset();
        ModemPlatform_RequestMsdUpload(pstMsd);
        s_mSvcRun.u8EmergencyRequestPending = 1U;
        s_mSvcRun.u8NetworkSleepRequested = 0U;
        if (s_mSvcSts.enServiceState == MODEM_SERVICE_SLEEP)
        {
            ModemNetwork_Start();
            s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
        }
        else
        {
            /* Existing network or platform work continues before MSD upload. */
        }
        s_mSvcSts.enCallState = MODEM_CALL_PREPARING;
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkOnline
  * Author  :LoH
  * Function:Request normal KL15-controlled network online state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkOnline(void)
{
    s_mSvcRun.u8NetworkSleepRequested = 0U;
    s_mSvcRun.u32NetworkSleepHoldMs = 0U;

    if (s_mSvcSts.enServiceState == MODEM_SERVICE_SLEEP)
    {
        ModemNetwork_Start();
        s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
    }
}

/*******************************************************
  * Name    :ModemService_RequestNetworkSleep
  * Author  :LoH
  * Function:Request normal network sleep after pending emergency work.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
void ModemService_RequestNetworkSleep(void)
{
    if (s_mSvcRun.u8NetworkSleepRequested == 0U)
    {
        s_mSvcRun.u8NetworkSleepRequested = 1U;
        s_mSvcRun.u32NetworkSleepHoldMs = 0U;
        HMI_LOG_MODEM("KL15_OFF_HOLD\r\n");
    }
}

/*******************************************************
  * Name    :ModemService_Process
  * Author  :LoH
  * Function:Dispatch network, platform and independent call workflows.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
void ModemService_Process(void)
{
    uint8_t u8PlatformWasOnline;

    if (ModemPort_TakePdnDeactivation() != 0U)
    {
        if (s_mSvcSts.enServiceState != MODEM_SERVICE_SLEEP)
        {
            HMI_LOG_MODEM("PDN_DEACT_RESTART\r\n");
            ModemNetwork_Start();
            s_mSvcSts.enServiceState = MODEM_SERVICE_NETWORK_STARTUP;
        }
        else
        {
            /* Do not wake a KL15-controlled sleeping network for a stale URC. */
        }
    }

    switch (s_mSvcSts.enServiceState)
    {
        case MODEM_SERVICE_SLEEP:
            /* Network start is requested by KL15 or eCall through ModemNetwork_Start(). */
            break;

        case MODEM_SERVICE_NETWORK_STARTUP:
            ModemService_ProcessNetworkStartup();
            break;

        case MODEM_SERVICE_PLATFORM_ACTIVE:
            u8PlatformWasOnline = ModemPlatform_IsOnline();
            ModemService_ProcessPlatform();
            if ((u8PlatformWasOnline != 0U)
                && (s_mSvcSts.enServiceState == MODEM_SERVICE_PLATFORM_ACTIVE)
                && (ModemPlatform_IsOnline() != 0U))
            {
                ModemService_ProcessOnlineSleep();
            }
            break;

        default:
            HMI_LOG_MODEM("NETWORK_STATE_UNKNOWN\r\n");
            ModemService_EnterNetworkSleep();
            break;
    }

    ModemCall_Process(&s_mSvcSts.enCallState);
}

/*******************************************************
  * Name    :ModemService_GetCallState
  * Author  :LoH
  * Function:Return modem call state for EcallTask notification.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemCallState_t ModemService_GetCallState(void)
{
    return s_mSvcSts.enCallState;
}

/*******************************************************
  * Name    :ModemService_GetState
  * Author  :LoH
  * Function:Return current modem service lifecycle state.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
ModemServiceState_t ModemService_GetState(void)
{
    return s_mSvcSts.enServiceState;
}
