/*******************************************************
  * Name    :Dtc_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Dtc_Cfg.h"
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Dtc_CheckIfService0x14ConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Dtc_CheckIfService0x14ConditionsNotCorrect(uint32_t groupId)
{
    (void)groupId;
    return FALSE;
}

/*******************************************************
  * Name    :Dtc_CheckIfService0x19ConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Dtc_CheckIfService0x19ConditionsNotCorrect(void)
{
    return FALSE;
}

/*******************************************************
  * Name    :Dtc_CheckIfService0x85ConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Dtc_CheckIfService0x85ConditionsNotCorrect(void)
{
    return FALSE;
}

// clang-format off
/*-------------variable statement---------------------*/
const Dtc_Cfg_Type g_DtcCfgList[DTC_CFG_LIST_NUM] =
{
/* 1*/{0xD10017, DTC_ID_HIGH_VOLTAGE, UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND, 5},
/* 2*/{0xD10116, DTC_ID_LOW_VOLTAGE,  UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND, 5},
/* 3*/{0xC00188, DTC_ID_BUS_OFF,      UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND, 5},
};

