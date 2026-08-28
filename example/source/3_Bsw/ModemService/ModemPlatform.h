/*******************************************************
  * Name    :ModemPlatform.h
  * Author  :LoH
  * Function:Private candidate AECS platform workflow interface.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#ifndef __MODEM_PLATFORM_H__
#define __MODEM_PLATFORM_H__

#include <stdint.h>
#include "Ecall.h"

typedef enum
{
    MODEM_PLATFORM_EVENT_NONE = 0U,
    MODEM_PLATFORM_EVENT_REGISTERED,
    MODEM_PLATFORM_EVENT_MSD_STARTED,
    MODEM_PLATFORM_EVENT_MSD_UPLOADED,
    MODEM_PLATFORM_EVENT_FAILED
} ModemPlatformEvent_t;

/* Initialize the private platform workflow. */
void ModemPlatform_Init(void);
/* Stop active platform work while preserving a pending emergency MSD request. */
void ModemPlatform_Reset(void);
/* Start platform registration after TCP connection. */
void ModemPlatform_Start(void);
/* Latch one MSD for upload after the platform becomes available. */
void ModemPlatform_RequestMsdUpload(const EcallMsd_t *pstMsd);
/* Process one platform workflow step and optionally advance heartbeat timing. */
void ModemPlatform_Process(uint8_t u8HeartbeatEnabled);
/* Return whether the private platform workflow is stably online. */
uint8_t ModemPlatform_IsOnline(void);
/* Return and clear one platform workflow result event. */
ModemPlatformEvent_t ModemPlatform_TakeEvent(void);

#endif
