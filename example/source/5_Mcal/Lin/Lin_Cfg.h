/*******************************************************
  * Name    :Lin_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _LIN_CFG_H_
#define _LIN_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------define---------------------------------*/
extern uint8_t LIN_CalculateChecksum(uint8_t u8PID, const uint8_t au8Data[], uint8_t u8Len);
extern uint8_t LIN_CalculatePIDParity(uint8_t u8ID);
#endif

