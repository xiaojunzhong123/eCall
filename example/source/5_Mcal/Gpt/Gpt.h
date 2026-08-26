/*******************************************************
  * Name    :Gpt.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _GPT_H_
#define _GPT_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------Function port--------------------------*/
extern void Gpt_Init(void);
extern void Gpt_DeInit(void);
extern void Gpt_DelayMs(uint32_t delay);
extern void Gpt_DelayUs(uint32_t us);
extern void Gpt_WakeUpTimerInit(void);
extern void Gpt_WakeUpTimerCycle(uint16_t time);
extern void Gpt_WakeUpTimerDisable(void);
#endif

