/*******************************************************
  * Name    :Did_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Did_Cfg.h"
/*-------------variable statement---------------------*/
static const uint8_t g_DidF888Buff[8]  = "KX:A.0.0";
static const uint8_t g_DidF189Buff[12] = "SWAPP:A.1.0 ";
static const uint8_t g_DidF089Buff[12] = "HWAPP:A.1.0 ";
static const uint8_t g_DidF187Buff[12] = "1100000-DC01";
static const uint8_t g_DidF18ABuff[7]  = "S100010";
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Did_CheckIfService0x22ConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Did_CheckIfService0x22ConditionsNotCorrect(uint16_t did)
{
    (void)did;
    return FALSE;
}

/*******************************************************
  * Name    :Did_CheckIfService0x2EConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Did_CheckIfService0x2EConditionsNotCorrect(uint16_t did)
{
    (void)did;
    return FALSE;
}

/*******************************************************
  * Name    :Did_F186ReadWriteFun
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool Did_F186ReadWriteFun(Bool isRead, uint8_t *p_buff, uint8_t len)
{
    if (isRead == TRUE)
    {
        p_buff[0] = (uint8_t)Did_GetCurrentSessionMode();
        return TRUE;
    }
    return FALSE;
}

/*******************************************************
  * Name    :Did_F185ReadWriteFun
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool Did_F185ReadWriteFun(Bool isRead, uint8_t *p_buff, uint8_t len)
{
    if (isRead == TRUE)
    {
        p_buff[0] = (uint8_t)((Did_GetVoltageValue() >> 8) & 0xFF);
        p_buff[1] = (uint8_t)((Did_GetVoltageValue() >> 0) & 0xFF);
        return TRUE;
    }
    return FALSE;
}

// clang-format off
/*-------------variable statement---------------------*/
const Did_Cfg_Type g_DidCfgList[] =
{
/*01*/{0xF188,  8, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_FLS, DID_ADDR_NOE, g_DidF888Buff, NULL,               },//kaixuan version
/*02*/{0xF189, 12, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_FLS, DID_ADDR_NOE, g_DidF189Buff, NULL,               },//software version
/*03*/{0xF089, 12, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_FLS, DID_ADDR_NOE, g_DidF089Buff, NULL,               },//hardware version
/*04*/{0xF187, 12, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_FLS, DID_ADDR_NOE, g_DidF187Buff, NULL                },//sn
/*05*/{0xF18A,  7, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_FLS, DID_ADDR_NOE, g_DidF18ABuff, NULL                },//supplier code
/*06*/{0xF190, 17, (uint8_t)DID_READ_LV1 | (uint8_t)DID_WRITE_LV1, DID_STORAGE_NVM, UDS_VIN_ADDR, NULL,          NULL                },//vehicle VIN
/*07*/{0xF186,  1, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_RAM, DID_ADDR_NOE, NULL,          Did_F186ReadWriteFun},//current session mode
/*08*/{0xF185,  2, (uint8_t)DID_READ_LV0 | (uint8_t)DID_WRITE_NOE, DID_STORAGE_RAM, DID_ADDR_NOE, NULL,          Did_F185ReadWriteFun},//current voltage value
};

const uint8_t g_DidCfgListNum = ArraySize(g_DidCfgList);

