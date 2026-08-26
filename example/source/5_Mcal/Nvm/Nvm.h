/*******************************************************
  * Name    :Nvm.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _NVM_H_
#define _NVM_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
/*-------------variable--------------------------*/
extern uint8_t g_Uds_boot[UDS_BOOT_LEN];
extern uint8_t g_Uds_app[UDS_APP_LEN];
extern uint8_t g_Uds_secuAccs[UDS_SECUACCS_LEN];
extern uint8_t g_Uds_vin[UDS_VIN_LEN];

/*-------------function port--------------------------*/
extern void        Nvm_Init(void);
extern Return_Type Nvm_GetProgramStatus(void);
extern Bool        Nvm_Read(uint32_t address, uint8_t *dest, uint16_t length);
extern Bool        Nvm_Write(uint32_t address, uint8_t *src, uint16_t length);
extern Bool        Nvm_EraseSector(uint32_t u32Addr, uint32_t u32Size);
#endif

