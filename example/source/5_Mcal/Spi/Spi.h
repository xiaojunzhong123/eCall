/*******************************************************
  * Name    :Spi.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _SPI_H_
#define _SPI_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------Function port--------------------------*/
extern void Spi_Init(void);
extern void Low_Spi_DeInit(void);
extern void Spi3_send(uint16_t *sendbuff, uint16_t *recvbuff, uint16_t len);
extern void Spi4_send(uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len);
#endif

