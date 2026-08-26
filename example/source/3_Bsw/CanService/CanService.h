/*******************************************************************************
 * @file  CanService.h
 * @brief CAN service interface independent of a specific CAN controller.
 ******************************************************************************/
#ifndef __CAN_SERVICE_H__
#define __CAN_SERVICE_H__

#include <stdint.h>

void CanService_Init(void);
void CanService_Process(void);
uint8_t CanService_GetMockEcallTrigger(void);

#endif
