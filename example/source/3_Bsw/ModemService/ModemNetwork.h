/*******************************************************
  * Name    :ModemNetwork.h
  * Author  :LoH
  * Function:Private AC7600 network connection flow interface.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#ifndef __MODEM_NETWORK_H__
#define __MODEM_NETWORK_H__

#include <stdint.h>

/* Initialize the private modem network workflow. */
void ModemNetwork_Init(void);
/* Clear the private modem network workflow. */
void ModemNetwork_Reset(void);
/* Start AT synchronization, PDP activation and TCP connection. */
void ModemNetwork_Start(void);
/* Process one AC7600 network workflow step; recovery is inhibited while voice owns the AT port. */
void ModemNetwork_Process(uint8_t u8RecoveryAllowed);
/* Return and clear the transport-recovery-started event. */
uint8_t ModemNetwork_TakeTransportRecoveryStarted(void);
/* Return and clear the TCP-connected event. */
uint8_t ModemNetwork_TakeTcpConnected(void);
/* Return and clear the network-failed event. */
uint8_t ModemNetwork_TakeFailure(void);

#endif
