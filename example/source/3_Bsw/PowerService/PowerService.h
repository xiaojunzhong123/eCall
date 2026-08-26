/*******************************************************************************
 * @file  PowerService.h
 * @brief Power lock service interface.
 ******************************************************************************/
#ifndef __POWER_SERVICE_H__
#define __POWER_SERVICE_H__

#include <stdint.h>

void PowerService_Init(void);
void PowerService_SetEcallLock(uint8_t u8Enable);
uint8_t PowerService_IsEcallLocked(void);
void PowerService_Process(void);

#endif
