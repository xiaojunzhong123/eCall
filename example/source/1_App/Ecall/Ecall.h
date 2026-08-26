/*******************************************************************************
 * @file  Ecall.h
 * @brief TBOX eCall application state machine interface.
 ******************************************************************************/
#ifndef __ECALL_H__
#define __ECALL_H__

#include <stdint.h>

typedef enum
{
    ECALL_STATE_STANDBY = 0U,
    ECALL_STATE_WAIT_GNSS,
    ECALL_STATE_BUILD_MSD,
    ECALL_STATE_WAIT_CALL_ACTIVE,
    ECALL_STATE_IN_CALL,
    ECALL_STATE_COMPLETE,
    ECALL_STATE_FAILED
} EcallState_t;

typedef enum
{
    ECALL_TRIGGER_MANUAL = 0U,
    ECALL_TRIGGER_AUTO,
    ECALL_TRIGGER_CAN
} EcallTrigger_t;

typedef struct
{
    uint8_t trigger_mask;
    uint8_t gnss_valid;
    int32_t latitude_e7;
    int32_t longitude_e7;
    uint32_t utc_seconds;
} EcallMsd_t;

void Ecall_Init(void);
void Ecall_OnTrigger(EcallTrigger_t trigger);
void Ecall_OnGnssResult(const EcallMsd_t *msd, uint8_t valid);
void Ecall_OnGnssTimeout(void);
void Ecall_OnCallActive(void);
void Ecall_OnCallEnded(void);
void Ecall_OnCallFailed(void);
EcallState_t Ecall_GetState(void);
uint8_t Ecall_IsSessionActive(void);

#endif
