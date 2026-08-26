/*******************************************************
  * Name    :PowerService.c
  * Author  :LoH
  * Function:eCall power lock P1 mock implementation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "PowerService.h"

static uint8_t m_u8EcallLock = 0U;

/*******************************************************
  * Name    :PowerService_Init
  * Author  :LoH
  * Function:Reset the eCall power lock.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_Init(void)
{
    m_u8EcallLock = 0U;
}

/*******************************************************
  * Name    :PowerService_SetEcallPowerLock
  * Author  :LoH
  * Function:Set or release the eCall power-down lock.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_SetEcallLock(uint8_t u8Enable)
{
    m_u8EcallLock = (u8Enable != 0U) ? 1U : 0U;
}

/*******************************************************
  * Name    :PowerService_IsEcallPowerLocked
  * Author  :LoH
  * Function:Return current eCall power lock state.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
uint8_t PowerService_IsEcallLocked(void)
{
    return m_u8EcallLock;
}

/*******************************************************
  * Name    :PowerService_Process
  * Author  :LoH
  * Function:Provide reserved periodic power processing entry.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_Process(void)
{
}
