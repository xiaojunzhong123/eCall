/*******************************************************
  * Name    :Memory.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _MEMORY_H_
#define _MEMORY_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
extern Bool     Memory_CheckIfInvalid(uint8_t *p_buff, uint8_t len);
extern uint16_t Memory_AverageShort(volatile uint16_t *p_buff, uint8_t len);
extern void     Memory_Fill(volatile uint8_t *p_buff, uint8_t data, uint8_t len);
extern void     Memory_FillLong(volatile uint8_t *p_buff, uint8_t data, uint16_t len);
extern void     Memory_FillShort(volatile uint8_t *p_buff, uint8_t data, uint32_t len);
extern Bool     Memory_Compare(volatile uint8_t *p_des, uint8_t *p_src, uint8_t len);
extern void     Memory_Copy(volatile uint8_t *p_des, volatile uint8_t *p_src, uint8_t len);
extern void     Memory_CopyConst(volatile uint8_t *p_des, const uint8_t *p_src, uint8_t len);
extern void     Memory_CopyLong(volatile uint8_t *p_des, volatile uint8_t *p_src, uint32_t len);
extern void     Memory_CopyShort(volatile uint8_t *p_des, volatile uint8_t *p_src, uint16_t len);
#endif

