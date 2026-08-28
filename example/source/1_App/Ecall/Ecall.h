/*******************************************************
  * Name    :Ecall.h
  * Author  :LoH
  * Function:TBOX eCall application state machine interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __ECALL_H__
#define __ECALL_H__

#include <stdint.h>

typedef enum
{
    ECALL_STATE_STANDBY = 0U,
    ECALL_STATE_MANUAL_CANCEL_WINDOW,
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

#define ECALL_TRIGGER_MASK_MANUAL     0x01U
#define ECALL_TRIGGER_MASK_AUTO       0x02U
#define ECALL_TRIGGER_MASK_CAN        0x04U
#define ECALL_TRIGGER_MASK_ALL        (ECALL_TRIGGER_MASK_MANUAL | \
                                       ECALL_TRIGGER_MASK_AUTO | \
                                       ECALL_TRIGGER_MASK_CAN)

#define ECALL_MANUAL_CANCEL_WINDOW_MS 10000U
#define ECALL_GNSS_HISTORY_COUNT       9U

typedef struct
{
    uint8_t trigger_mask;
    uint8_t gnss_valid;
    int32_t latitude_e7;
    int32_t longitude_e7;
    uint32_t utc_seconds;
    int32_t history_latitude_e7[ECALL_GNSS_HISTORY_COUNT];
    int32_t history_longitude_e7[ECALL_GNSS_HISTORY_COUNT];
} EcallMsd_t;

/* Initialize eCall session state. */
void Ecall_Init(void);
/* Submit one trigger source to the eCall state machine. */
void Ecall_OnTrigger(EcallTrigger_t trigger);
/* Submit merged trigger source bits to the eCall state machine. */
void Ecall_OnTriggerMask(uint8_t u8TriggerMask);
/* Cancel the active manual eCall request before dialing. */
void Ecall_OnManualCancel(void);
/* Start the formal eCall sequence after the cancel window expires. */
void Ecall_OnManualCancelTimeout(void);
/* Update the eCall snapshot with the GNSS result. */
void Ecall_OnGnssResult(const EcallMsd_t *msd, uint8_t valid);
/* Continue eCall when GNSS acquisition times out. */
void Ecall_OnGnssTimeout(void);
/* Report modem call state transitions. */
void Ecall_OnCallActive(void);
void Ecall_OnCallEnded(void);
void Ecall_OnCallFailed(void);
/* Read current eCall state and session activity. */
EcallState_t Ecall_GetState(void);
uint8_t Ecall_IsSessionActive(void);

#endif
