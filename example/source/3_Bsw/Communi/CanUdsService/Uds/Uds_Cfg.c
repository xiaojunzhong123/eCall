/*******************************************************
  * Name    :Uds_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Uds_Cfg.h"
// clang-format off
/*-------------variable statement---------------------*/
const Uds_GenCfg_Type g_UdsGenCfg =
{
	UDS_FUNC_ADDR_ID,
    UDS_PHYS_ADDR_ID,
    UDS_RESP_ADDR_ID,
    5000,
    50,
    5000,
    200,
};

const Uds_DcmSubServ_Type g_UdsService0x10CfgList[] =
{
    {
        0x01,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x02,
        UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x03,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x11CfgList[] =
{
    {
        0x01,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x03,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x19CfgList[] =
{
    {
        0x01,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x02,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x04,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x06,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x0A,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x27CfgList[] =
{
    {
        0x01,
        UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x02,
        UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x28CfgList[] =
{
    {
        0x00,
        UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x03,
        UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x3ECfgList[] =
{
    {
        0x00,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmSubServ_Type g_UdsService0x85CfgList[] =
{
    {
        0x01,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    },

    {
        0x02,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT
    }
};

const Uds_DcmServCfg_Type g_UdsServiceCfgList[] =
{
    {
        UDS_DCM_SID_0X10,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        TRUE,
        ArraySize(g_UdsService0x10CfgList),
        g_UdsService0x10CfgList,
        Service_0x10DiagnosticSessionControl
    },

    {
        UDS_DCM_SID_0X11,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        TRUE,
        ArraySize(g_UdsService0x11CfgList),
        g_UdsService0x11CfgList,
        Service_0x11EcuReset
    },

    {
        UDS_DCM_SID_0X14,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x14ClearDiagnosticInformation
    },

    {
        UDS_DCM_SID_0X19,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        ArraySize(g_UdsService0x19CfgList),
        g_UdsService0x19CfgList,
        Service_0x19ReadDtcInformation
    },

    {
        UDS_DCM_SID_0X22,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x22ReadDataByIdentifier
    },

    {
        UDS_DCM_SID_0X23,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_1,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x23ReadMemoryByAddress
    },

    {
        UDS_DCM_SID_0X27,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        ArraySize(g_UdsService0x27CfgList),
        g_UdsService0x27CfgList,
        Service_0x27SecurityAccess
    },

    {
        UDS_DCM_SID_0X28,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        TRUE,
        ArraySize(g_UdsService0x28CfgList),
        g_UdsService0x28CfgList,
        Service_0x28CommunicationControl
    },

    {
        UDS_DCM_SID_0X2E,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x2EWriteDataByIdentifier
    },

    {
        UDS_DCM_SID_0X2F,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR,
        UDS_DCM_SEC_LEV_1,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x2FInputOutputControlByIdentifier
    },

    {
        UDS_DCM_SID_0X31,
        UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR,
        UDS_DCM_SEC_LEV_1,
        UDS_MODE_RULE_DEFAULT,
        FALSE,
        0,
        NULL,
        Service_0x31RoutineControl
    },

    {
        UDS_DCM_SID_0X3E,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        TRUE,
        ArraySize(g_UdsService0x3ECfgList),
        g_UdsService0x3ECfgList,
        Service_0x3ETesterPresent
    },

    {
        UDS_DCM_SID_0X85,
        UDS_SESSION_DEFAULT | UDS_SESSION_EXTEND,
        UDS_SUPPORT_PHY_ADDR | UDS_SUPPORT_FUN_ADDR,
        UDS_DCM_SEC_LEV_NONE,
        UDS_MODE_RULE_DEFAULT,
        TRUE,
        ArraySize(g_UdsService0x85CfgList),
        g_UdsService0x85CfgList,
        Service_0x85ControlDtcSetting
    }
};
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Uds_DcmModeRuleDefault
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool Uds_DcmModeRuleDefault(Uds_DcmNegRespCode_Type* p_errCode)
{
    (void)p_errCode;
    return TRUE;
}
/*-------------variable statement---------------------*/
const Uds_DcmModeRuleCheck_Fun g_UdsModeRuleFunList[] =
{
    Uds_DcmModeRuleDefault
};

const uint8_t g_UdsServiceCfgListNum = ArraySize(g_UdsServiceCfgList);
const uint8_t g_UdsModeRuleFunListNum = ArraySize(g_UdsModeRuleFunList);

