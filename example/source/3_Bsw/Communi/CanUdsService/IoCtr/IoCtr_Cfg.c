/*******************************************************
  * Name    :IoCtr_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "IoCtr_Cfg.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :IoCtr_CheckIfService0x2FConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool IoCtr_CheckIfService0x2FConditionsNotCorrect(uint16_t did)
{
    (void)did;
    return FALSE;
}

/*******************************************************
  * Name    :IoCtr_0101ControlFun
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Return_Type IoCtr_0101ControlFun(Uds_DcmOpStatus_Type opStatus, IoCtr_MsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode)
{
    (void)opStatus;
    (void)p_msgContext;
    (void)p_errCode;
    switch (p_msgContext->ctrParm)
    {
        case IOCTR_RETURN_CONTROL_TO_ECU:
            break;
        case IOCTR_RESET_TO_DEFAULT:
            break;
        case IOCTR_FREEZE_CURRENT_STATE:
            break;
        case IOCTR_SHORT_TERM_ADJUST:
            break;
        default:
            break;
    }
    return RET_OK;
}

// clang-format off
/*-------------variable statement---------------------*/
const IoCtr_Cfg_Type g_IoCtrCfgList[IOCTR_CFG_LIST_NUM] = 
{
/* 1*/{0x0101, 2, 0, UDS_SESSION_EXTEND, UDS_DCM_SEC_LEV_1, FALSE, FALSE, TRUE, 0, IoCtr_0101ControlFun}, //---
};

