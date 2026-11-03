/*******************************************************
  * Name    :Nvm.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Nvm_Cfg.h"
#include "Nvm.h"
#include "App_Memory.h"
/*-------------variable statement---------------------*/
uint8_t g_Uds_vin[UDS_VIN_LEN]           = { 0 };
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Nvm_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Nvm_Init(void)
{
}

/*******************************************************
  * Name    :Nvm_Read
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Nvm_Read(uint32_t address, uint8_t *dest, uint16_t length)
{
    if (address == UDS_VIN_ADDR)
    {
        Memory_Copy(dest, g_Uds_vin, UDS_VIN_LEN);
    }
    else
    {
    }
    return TRUE;
}

/*******************************************************
  * Name    :Nvm_Write
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Nvm_Write(uint32_t address, uint8_t *src, uint16_t length)
{
    if (address == UDS_VIN_ADDR)
    {
        Memory_Copy(g_Uds_vin, src, UDS_VIN_LEN);
        EE_Set_Write_Info(Eeprom_vin);
    }
    else
    {
    }
    return TRUE;
}

/*******************************************************
  * Name    :Nvm_EraseSector
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Nvm_EraseSector(uint32_t u32Addr, uint32_t u32Size)
{
    return TRUE;
}
/*******************************************************
  * Name    :Nvm_GetProgramStatus
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Return_Type Nvm_GetProgramStatus(void)
{
    return RET_OK;
}

