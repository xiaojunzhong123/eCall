/*******************************************************
  * Name    :CanService.c
  * Author  :LoH
  * Function:CAN service P1 mock implementation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "CanService.h"

/*******************************************************
  * Name    :CanService_Init
  * Author  :LoH
  * Function:Initialize P1 CAN service mock state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void CanService_Init(void)
{
}

/*******************************************************
  * Name    :CanService_Process
  * Author  :LoH
  * Function:Execute one CAN service processing cycle.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void CanService_Process(void)
{
}

/*******************************************************
  * Name    :CanService_GetMockEcallTrigger
  * Author  :LoH
  * Function:Return mock trigger; CANFD parsing is deferred.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
uint8_t CanService_GetMockEcallTrigger(void)
{
    return 0U;
}
