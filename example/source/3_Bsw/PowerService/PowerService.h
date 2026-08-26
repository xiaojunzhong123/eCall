/*******************************************************
  * Name    :PowerService.h
  * Author  :LoH
  * Function:eCall power lock service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __POWER_SERVICE_H__
#define __POWER_SERVICE_H__

#include <stdint.h>

/* Initialize power lock state. */
void PowerService_Init(void);
/* Enable or release the eCall power lock. */
void PowerService_SetEcallLock(uint8_t u8Enable);
/* Read the eCall power lock state. */
uint8_t PowerService_IsEcallLocked(void);
/* Process future power service work in PowTask context. */
void PowerService_Process(void);

#endif
