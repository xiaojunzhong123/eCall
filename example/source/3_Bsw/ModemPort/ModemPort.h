/*******************************************************
  * Name    :ModemPort.h
  * Author  :LoH
  * Function:Modem UART AT queue port interface.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
#ifndef __MODEM_PORT_H__
#define __MODEM_PORT_H__

#include <stdint.h>

typedef enum
{
    MODEM_PORT_RESULT_IDLE = 0U,
    MODEM_PORT_RESULT_OK,
    MODEM_PORT_RESULT_ERROR,
    MODEM_PORT_RESULT_TIMEOUT
} ModemPortResult_t;

void ModemPort_Init(void);
uint8_t ModemPort_QueueCommand(const char *pcCommand, const char *pcExpected, uint32_t u32TimeoutMs);
void ModemPort_Process(void);
uint8_t ModemPort_IsIdle(void);
uint8_t ModemPort_HasFailure(void);
ModemPortResult_t ModemPort_GetLastResult(void);
void ModemPort_ClearFailure(void);

#endif
