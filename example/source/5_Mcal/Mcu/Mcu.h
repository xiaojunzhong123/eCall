/*******************************************************
  * Name    :Mcu.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _MCU_H_
#define _MCU_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
extern void    Mcu_Init(void);
extern void    Mcu_SoftReset(void);
extern void    Mcu_EnableIrq(void);
extern void    Mcu_DisableIrq(void);
extern void    Mcu_JumpToApplication(void);
extern int32_t IAP_JumpToApp(uint32_t u32Addr);
extern int32_t STOP_IsReady(void);
extern void    Mcu_PerSleep(void);
extern uint8_t GetIntoSleepMode(void);
#endif

