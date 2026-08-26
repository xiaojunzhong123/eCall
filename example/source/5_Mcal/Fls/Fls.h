/*******************************************************
  * Name    :Fls.h
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _FLS_H_
#define _FLS_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------Variable declarations------------------------*/

/*-------------functFLSn port--------------------------*/
extern Bool        Fls_Init(void);
extern Bool        Fls_DeInit(void);
extern Return_Type Fls_GetProgramStatus(void);
extern Return_Type Fls_Erase(uint32_t addr, uint32_t len);
extern Bool        Fls_Read(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len);
extern Bool        Fls_Program(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len);
extern Bool        Fls_DriverRamCode(uint32_t addr, uint8_t *p_buff, uint16_t len);
#endif

