/*******************************************************
  * Name    :Routine_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Routine_Cfg.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Routine_CheckIfService0x31ConditionsNotCorrect
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Routine_CheckIfService0x31ConditionsNotCorrect(uint16_t id)
{
    (void)id;
    return FALSE;
}

/*******************************************************
  * Name    :Routine_0203ControlFun
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Return_Type Routine_0203ControlFun(Uds_DcmOpStatus_Type opStatus, Routine_MsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode)
{
    switch (p_msgContext->ctrType)
    {
        case ROUTINE_CTR_START:
            break;
        case ROUTINE_CTR_STOP:
            break;
        case ROUTINE_CTR_REQ_RESULT:
            break;
        default:
            break;
    }
    return RET_OK;
}

/*******************************************************
  * Name    :Routine_F1F1ControlFun
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Return_Type Routine_F1F1ControlFun(Uds_DcmOpStatus_Type opStatus, Routine_MsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode)
{
    (void)opStatus;
    (void)p_msgContext;
    (void)p_errCode;
    switch (p_msgContext->ctrType)
    {
        case ROUTINE_CTR_START:
            break;
        case ROUTINE_CTR_STOP:
            break;
        case ROUTINE_CTR_REQ_RESULT:
            break;
        default:
            break;
    }
    return RET_OK;
}

// clang-format off
/*-------------variable statement---------------------*/
const Routine_Cfg_Type g_RoutineCfgList[ROUTINE_CFG_LIST_NUM] =
{
/* 1*/{0x0203, 0, UDS_SESSION_EXTEND, UDS_DCM_SEC_LEV_1, FALSE, FALSE, ROUTINE_EXE_DIRECT, Routine_0203ControlFun},//check programming precondition
/* 2*/{0xF1F1, 2, UDS_SESSION_EXTEND, UDS_DCM_SEC_LEV_1, FALSE, FALSE, ROUTINE_EXE_DIRECT, Routine_F1F1ControlFun},//factory Reset
};

