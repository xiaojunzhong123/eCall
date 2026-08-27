/*******************************************************
  * Name    :Uart.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _UART_H_
#define _UART_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "MsgIF.h"
/*-------------Function port--------------------------*/
extern void Uart_Init(void);
extern void Uart_DeInit(void);
extern Bool Uart_TxData(uint8_t *p_buff, uint8_t len);
extern Bool Uart_RxData(uint8_t *p_buff, uint8_t len);
extern uint8_t Uart_ReadRxData(uint8_t *pu8Buffer, uint8_t u8Length);
extern uint16_t Uart_GetRxOverflowCount(void);
#endif

