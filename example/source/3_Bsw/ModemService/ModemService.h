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
    MODEM_CALL_DIALING,
    MODEM_CALL_ACTIVE,
    MODEM_CALL_ENDED,
    MODEM_CALL_FAILED
} ModemCallState_t;

/* Initialize modem call service state. */
void ModemService_Init(void);
/* Request one emergency call with a frozen MSD. */
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd);
/* Process modem call state in ModemTask context. */
void ModemService_Process(void);
/* Return the current modem call state. */
ModemCallState_t ModemService_GetCallState(void);

#endif
