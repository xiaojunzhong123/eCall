/*******************************************************
  * Name    :Pwm.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _PWM_H_
#define _PWM_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
extern void Pwm_Init(void);
extern void Pwm_DeInit(void);
extern void Pwm_UpdataCompValue(uint8_t PwmPercent);
#endif

