/*******************************************************
  * Name    :CanService.h
  * Author  :LoH
  * Function:Controller-independent CAN service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __CAN_SERVICE_H__
#define __CAN_SERVICE_H__

#include <stdint.h>

/* Initialize CAN service state. */
void CanService_Init(void);
/* Process CAN service work in CanTask context. */
void CanService_Process(void);
/* Return the P1 mock CAN eCall trigger. */
uint8_t CanService_GetMockEcallTrigger(void);

#endif
