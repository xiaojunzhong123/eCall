/*******************************************************
  * Name    :IoCtr_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _IOCTR_CFG_H_
#define _IOCTR_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
#include "Uds.h"
#include "Memory.h"
/*-------------define---------------------------------*/
#define IOCTR_CFG_LIST_NUM              6
/*-------------function port--------------------------*/
/*-------------Uds port-------------------------------*/
#define IoCtr_GetCurrentSessionMode()   Uds_GetCurrentSessionMode()
#define IoCtr_GetCurrentSecurityLevel() Uds_GetCurrentSecurityLevel()
/*-------------Memory---------------------------------*/
#define IoCtr_Fill(p_buff, data, len)   Memory_Fill(p_buff, data, len)
/*-------------enum and struct------------------------*/
typedef enum
{
    IOCTR_RETURN_CONTROL_TO_ECU = 0x00,
    IOCTR_RESET_TO_DEFAULT      = 0x01,
    IOCTR_FREEZE_CURRENT_STATE  = 0x02,
    IOCTR_SHORT_TERM_ADJUST     = 0x03
} IoCtr_ParmState_Type;

typedef struct
{
    uint8_t             *p_rxData;
    uint8_t             *p_txData;
    uint16_t             rxLen;
    uint16_t             txLen;
    IoCtr_ParmState_Type ctrParm;
} IoCtr_MsgContext_Type;

typedef Return_Type (*IoCtr_Hindle_Fun)(Uds_DcmOpStatus_Type opStatus, IoCtr_MsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);

typedef struct
{
    const uint16_t            did;
    const uint8_t             stateLen;
    const uint8_t             maskLen;
    const uint8_t             supSessMode;
    const Uds_DcmSecuLvl_Type secuAccsLvl;
    const Bool                supResetToDefault;
    const Bool                supFreezeCurrentState;
    const Bool                supShortTermAdjust;
    const uint16_t            ctrTime;
    const IoCtr_Hindle_Fun    dataHandleFun;
} IoCtr_Cfg_Type;
/*-------------function statement---------------------*/
extern Bool IoCtr_CheckIfService0x2FConditionsNotCorrect(uint16_t did);
/*-------------variable statement---------------------*/
extern const IoCtr_Cfg_Type g_IoCtrCfgList[IOCTR_CFG_LIST_NUM];
#endif

