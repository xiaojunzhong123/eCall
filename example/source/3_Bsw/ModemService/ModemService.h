/*******************************************************************************
 * @file  ModemService.h
 * @brief Modem call service interface.
 ******************************************************************************/
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

void ModemService_Init(void);
void ModemService_RequestEmergencyCall(const EcallMsd_t *pstMsd);
void ModemService_Process(void);
ModemCallState_t ModemService_GetCallState(void);

#endif
