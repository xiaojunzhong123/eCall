/*******************************************************
  * Name    :ModemService.h
  * Author  :LoH
  * Function:Modem emergency call service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __MODEM_SERVICE_H__
#define __MODEM_SERVICE_H__

#include "Ecall.h"

typedef enum
{
    MODEM_CALL_IDLE = 0U,
    MODEM_CALL_PREPARING,
    MODEM_CALL_DIALING,
    MODEM_CALL_ACTIVE,
    MODEM_CALL_ENDED,
    MODEM_CALL_FAILED
} ModemCallState_t;

typedef enum
{
    MODEM_NETWORK_SLEEP = 0U,
    MODEM_NETWORK_AT_SYNC,
    MODEM_NETWORK_PDP_OPENING,
    MODEM_NETWORK_PDP_ACTIVE,
    MODEM_NETWORK_TCP_CONNECTING,
    MODEM_NETWORK_PLATFORM_REGISTERING,
    MODEM_NETWORK_ONLINE,
    MODEM_NETWORK_MSD_UPLOADING
} ModemNetworkState_t;

/* Initialize modem call service state. */
void ModemService_Init(void);
/* Request one emergency call with a frozen MSD. */
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd);
/* Request normal KL15-controlled network online operation. */
void ModemService_RequestNetworkOnline(void);
/* Request normal KL15-controlled network sleep after pending work. */
void ModemService_RequestNetworkSleep(void);
/* Process modem call state in ModemTask context. */
void ModemService_Process(void);
/* Return the current modem call state. */
ModemCallState_t ModemService_GetCallState(void);
/* Return the current modem network state. */
ModemNetworkState_t ModemService_GetNetworkState(void);

#endif
