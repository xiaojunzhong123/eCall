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

typedef enum
{
    MODEM_PORT_VOICE_EVENT_NONE = 0U,
    MODEM_PORT_VOICE_EVENT_PROGRESS,
    MODEM_PORT_VOICE_EVENT_CONNECTED,
    MODEM_PORT_VOICE_EVENT_DISCONNECTED,
    MODEM_PORT_VOICE_EVENT_ENDED,
    MODEM_PORT_VOICE_EVENT_FAILED
} ModemPortVoiceEvent_t;

void ModemPort_Init(void);
uint8_t ModemPort_QueueCommand(const char *pcCommand, const char *pcExpected, uint32_t u32TimeoutMs);
/* Send one fixed-length payload after a completed CIPSEND prompt transaction. */
uint8_t ModemPort_SendData(const uint8_t *pu8Data, uint16_t u16Length, const char *pcExpected, uint32_t u32TimeoutMs);
void ModemPort_Process(void);
uint8_t ModemPort_IsIdle(void);
uint8_t ModemPort_HasFailure(void);
ModemPortResult_t ModemPort_GetLastResult(void);
/* Return the response line that completed the latest command. */
const char *ModemPort_GetLastMatchedResponse(void);
/* Return the latest hexadecimal payload line received by a CIPSEND/CIPRXGET transaction. */
const char *ModemPort_GetLastDataLine(void);
/* Return and clear one buffered TCP receive-data notification. */
uint8_t ModemPort_TakeRxDataEvent(uint8_t *pu8LinkId);
/* Return and clear one PDP deactivation notification. */
uint8_t ModemPort_TakePdnDeactivation(void);
/* Return and clear one TCP link-close notification. */
uint8_t ModemPort_TakeTcpClosed(void);
/* Return and clear one AC7600 voice-call result event. */
ModemPortVoiceEvent_t ModemPort_TakeVoiceEvent(void);
/* Clear only the AT transaction failure while preserving asynchronous URC events. */
void ModemPort_ClearTransactionFailure(void);
void ModemPort_ClearFailure(void);

#endif
