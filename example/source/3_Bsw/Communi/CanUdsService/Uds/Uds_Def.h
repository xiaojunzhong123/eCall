/*******************************************************
  * Name    :Uds_Def.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _UDS_DEF_H_
#define _UDS_DEF_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------define---------------------------------*/
#define UDS_FUNC_ADDR_ID            0x7DF
#define UDS_PHYS_ADDR_ID            0x791
#define UDS_RESP_ADDR_ID            0x799
#define UDS_RX_BUFF_NB              514
#define UDS_TX_BUFF_NB              100
#define UDS_MAX_NUMBER_OF_BLOCK_LEN UDS_RX_BUFF_NB
#define UDS_FLS_BLOCK_SIZE          (UDS_MAX_NUMBER_OF_BLOCK_LEN - 2)
#define UDS_CRC_TYPE                CRC_16
#define UDS_APP_START_ADDR          0x00009000
#define UDS_APP_MAX_END_ADDR        0x000FFFFF
#define UDS_APP_CODE_MARK           "KX2024Bcm008_App"
#define UDS_APP_CODE_MARK_LEN       16
#define UDS_APP_CONST_ADDR          0x00009410
#define UDS_APP_CONST_VALUE         UDS_APP_CODE_MARK
#define UDS_APP_CONST_LEN           UDS_APP_CODE_MARK_LEN
#define UDS_BOOT_VALID_ADDR         0x14000000
#define UDS_BOOT_VALID_VALUE        "boot"
#define UDS_BOOT_CLEAR_VALID_VALUE  "none"
#define UDS_BOOT_LEN                9
#define UDS_APP_INVALID_ADDR        0x14000010
#define UDS_APP_INVALID_VALUE       "inap"
#define UDS_APP_CLEAR_INVALID_VALUE "none"
#define UDS_APP_LEN                 4
#define UDS_SECUACCS_ADDR           0x14000020
#define UDS_SECUACCS_LEN            1
#define UDS_VIN_ADDR                0x14000030
#define UDS_VIN_LEN                 17
#define UDS_DTC_ADDR                0x14000060
#define UDS_DTC_LEN                 16
/*-------------enum and struct------------------------*/
typedef enum
{
    UDS_SESSION_DEFAULT = 0x02,
    UDS_SESSION_PROGRAM = 0x04,
    UDS_SESSION_EXTEND  = 0x08
} Uds_Session_Type;

typedef enum
{
    UDS_CTR_ENABLE_RX_AND_TX         = 0x00,
    UDS_CTR_ENABLE_RX_AND_DISABLE_TX = 0x01,
    UDS_CTR_DISABLE_RX_AND_ENABLE_TX = 0x02,
    UDS_CTR_DISABLE_RX_AND_TX        = 0x03
} Uds_CtrlType_Type;

typedef enum
{
    UDS_COM_NULL_MSG              = 0x00,
    UDS_COM_NORMAL_MSG            = 0x01,
    UDS_COM_NM_MSG                = 0x02,
    UDS_COM_NORMAL_MSG_AND_NM_MSG = 0x03
} Uds_ComType_Type;

typedef enum
{
    UDS_MODULE_NONE = 0x00,
    UDS_MODULE_DRIVER,
    UDS_MODULE_APP,
    UDS_MODULE_CAL
} Uds_Module_Type;

typedef enum
{
    UDS_DSL_PHY_ADDR = 0x01,
    UDS_DSL_FUN_ADDR
} Uds_DslAddr_Type;

typedef enum
{
    UDS_DCM_DEFAULT_SESSION        = 0x01,
    UDS_DCM_PROGRAMMING_SESSION    = 0x02,
    UDS_DCM_EXTENDED_DIAGN_SESSION = 0x03
} Uds_DcmSessMode_Type;

typedef enum
{
    UDS_DCM_SEC_LEV_NONE = 0x00,
    UDS_DCM_SEC_LEV_1,
    UDS_DCM_SEC_LEV_2
} Uds_DcmSecuLvl_Type;

typedef enum
{
    UDS_DCM_INITIAL = 0x00,
    UDS_DCM_PENDING,
    UDS_DCM_CANCEL,
    UDS_DCM_FORCE_RCRRP_OK,
    UDS_DCM_POS_RESP_SENT,
    UDS_DCM_POS_RESP_FAILED,
    UDS_DCM_NEG_RESP_SENT,
    UDS_DCM_NEG_RESP_FAILED
} Uds_DcmOpStatus_Type;

typedef enum
{
    UDS_DCM_SID_0X10 = 0x10,   //Diagnostic Session Control
    UDS_DCM_SID_0X11 = 0x11,   //ECU Reset
    UDS_DCM_SID_0X14 = 0x14,   //Clear Diagnostic Information
    UDS_DCM_SID_0X19 = 0x19,   //Read DTC Information
    UDS_DCM_SID_0X22 = 0x22,   //Read Data By Identifier
    UDS_DCM_SID_0X23 = 0x23,   //Read Memory By Address
    UDS_DCM_SID_0x24 = 0x24,   //Read Scaling Data By Identifier
    UDS_DCM_SID_0X27 = 0x27,   //Security Access
    UDS_DCM_SID_0X28 = 0x28,   //Communication Control
    UDS_DCM_SID_0X2A = 0x2A,   //Read Data By Periodic Identifier
    UDS_DCM_SID_0X2C = 0x2C,   //Dynamically Define Data Identifier
    UDS_DCM_SID_0X2E = 0x2E,   //Write Data By Identifier
    UDS_DCM_SID_0X2F = 0x2F,   //Input Output Control By Identifier
    UDS_DCM_SID_0X31 = 0x31,   //Routine Control
    UDS_DCM_SID_0X34 = 0x34,   //Request Download
    UDS_DCM_SID_0X35 = 0x35,   //Request Upload
    UDS_DCM_SID_0X36 = 0x36,   //Transfer Data
    UDS_DCM_SID_0X37 = 0x37,   //Request Transfer Exit
    UDS_DCM_SID_0X38 = 0x38,   //Request File Transfer
    UDS_DCM_SID_0X3D = 0x3D,   //Write Memory By Address
    UDS_DCM_SID_0X3E = 0x3E,   //Tester present
    UDS_DCM_SID_0X83 = 0x83,   //Access Timing Parameter
    UDS_DCM_SID_0X84 = 0x84,   //Secured Data Transmission
    UDS_DCM_SID_0X85 = 0x85,   //Control DTC Setting
    UDS_DCM_SID_0X86 = 0x86,   //Response On Event
    UDS_DCM_SID_0X87 = 0x87    //Link Control
} Uds_DcmSid_Type;

typedef enum
{
    UDS_DCM_E_POS_RESP    = 0x00,   //positive response
    UDS_DCM_E_NRC_GR      = 0x10,   //general rejection
    UDS_DCM_E_NRC_SNS     = 0x11,   //service not support
    UDS_DCM_E_NRC_SFNS    = 0x12,   //subfunction not support
    UDS_DCM_E_NRC_IMLOIF  = 0x13,   //incorrect message length or invalid format
    UDS_DCM_E_NRC_RTL     = 0x14,   //response too long
    UDS_DCM_E_NRC_BRR     = 0x21,   //busy repeat request
    UDS_DCM_E_NRC_CNC     = 0x22,   //conditions not correct
    UDS_DCM_E_NRC_RSE     = 0x24,   //request sequence error
    UDS_DCM_E_NRC_ROOR    = 0x31,   //request out of range
    UDS_DCM_E_NRC_SAD     = 0x33,   //security access denied
    UDS_DCM_E_NRC_IK      = 0x35,   //invalid key
    UDS_DCM_E_NRC_ENOA    = 0x36,   //exceed number of attempts
    UDS_DCM_E_NRC_RTDNE   = 0x37,   //required time delay not expired
    UDS_DCM_E_NRC_UDNA    = 0x70,   //upload download not accepted
    UDS_DCM_E_NRC_TDS     = 0x71,   //transfer data suspended
    UDS_DCM_E_NRC_GPF     = 0x72,   //general progamming failure
    UDS_DCM_E_NRC_WBSC    = 0x73,   //wrong block sequece counter
    UDS_DCM_E_NRC_RCRRP   = 0x78,   //request correctly received response pending
    UDS_DCM_E_NRC_SFNSIAS = 0x7E,   //subfunction is not supported in the current session mode
    UDS_DCM_E_NRC_SNSIAS  = 0x7F,   //service is not supported in the current session mode
    UDS_DCM_E_NRC_VTH     = 0x92,   //voltage too high
    UDS_DCM_E_NRC_VTL     = 0x93    //voltage too low
} Uds_DcmNegRespCode_Type;

typedef struct
{
    Frame_Type       frame;
    uint8_t          sid;
    uint8_t          subFun;
    Bool             supPosResp;
    Uds_DslAddr_Type reqType;
} Uds_DcmProgCon_Type;

typedef void (*Uds_DspStopProcess_Fun)(Uds_DcmOpStatus_Type opStatus);

typedef struct
{
    uint8_t                *p_rxData;
    uint8_t                *p_txData;
    uint16_t                rxLen;
    uint16_t                txLen;
    Bool                    supPosResp;
    uint8_t                 subFun;
    Frame_Type              frame;
    Uds_DslAddr_Type        reqType;
    Uds_DcmNegRespCode_Type finalRespNrc;
    Uds_DspStopProcess_Fun  finalRespToNotify;
} Uds_DcmMsgContext_Type;

typedef struct
{
    const uint16_t funcAddrId;
    const uint16_t physAddrId;
    const uint16_t diagRespId;
    const uint16_t s3ServerMax;
    const uint16_t p2ServerMax;
    const uint16_t p2StartServerMax;
    const uint16_t diagActiveDelay;
} Uds_GenCfg_Type;
/*-------------variable statement---------------------*/
extern const Uds_GenCfg_Type g_UdsGenCfg;
#endif

