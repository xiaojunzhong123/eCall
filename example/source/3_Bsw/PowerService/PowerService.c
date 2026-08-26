/*******************************************************************************
 * @file  PowerService.c
 * @brief Power lock service P1 mock implementation.
 ******************************************************************************/
#include "PowerService.h"

static uint8_t m_u8EcallLock = 0U;

void PowerService_Init(void)
{
    m_u8EcallLock = 0U;
}

void PowerService_SetEcallLock(uint8_t u8Enable)
{
    m_u8EcallLock = (u8Enable != 0U) ? 1U : 0U;
}

uint8_t PowerService_IsEcallLocked(void)
{
    return m_u8EcallLock;
}

void PowerService_Process(void)
{
}
