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
/* Process one AC7600 network workflow step. */
void ModemNetwork_Process(void);
/* Return and clear the TCP-connected event. */
uint8_t ModemNetwork_TakeTcpConnected(void);
/* Return and clear the network-failed event. */
uint8_t ModemNetwork_TakeFailure(void);

#endif
