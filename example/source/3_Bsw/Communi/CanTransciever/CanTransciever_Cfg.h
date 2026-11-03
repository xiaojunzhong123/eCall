/*******************************************************
  * Name    :CanTransciever_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANTRANSCIEVER_CFG_H_
#define _CANTRANSCIEVER_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Rte.h"
#include "Can.h"
#include "CanNm.h"
#include "CanIf.h"
#include "Uds_Def.h"
#include "CanMatrix.h"
#include "CanNm_Cfg.h"
/*-------------define---------------------------------*/
#define CANTRANSCIEVER_BUSOFF_DTC_CNT                             3
#define CANTRANSCIEVER_MAX_START_APP_TIME                         100
#define CANTRANSCIEVER_MAX_TX_MSG_CNT                             1
#define CANTRANSCIEVER_START_UDS_TIME                             1700
#define CANTRANSCIEVER_NOACK_TIME                                 150
#define CANTRANSCIEVER_PAUSE_TIME                                 150
#define CANTRANSCIEVER_BUSOFF_FAST_CNT                            10
#define CANTRANSCIEVER_BUSOFF_FAST_TIME                           50
#define CANTRANSCIEVER_BUSOFF_SLOW_TIME                           1000
/*-------------Tle8457 port---------------------------*/
#define CanTransciever_SbcInit()                                  //
#define CanTransciever_SbcDeInit()                                //
/*-------------Rte port-------------------------------*/
#define CanTransciever_SetDtcCurErrFlag(id, flag)                 Rte_SetDtcCurErrFlag(id, flag)
/*-------------Can port-------------------------------*/
#define CanTransciever_McalInit()                                 Can_Init()
#define CanTransciever_McalDeInit()                               Net_Can_DeInit()
#define CanTransciever_HasEmptyMail()                             Can_HasEmptyMail()
#define CanTransciever_ClearMail()                                Can_ClearMail()
#define CanTransciever_ClearNoAckFlag()                           Can_ClearNoAckFlag()
#define CanTransciever_GetNoAckFlag()                             Can_GetNoAckFlag()
#define CanTransciever_ClearBusOffFlag()                          Can_ClearBusOffFlag()
#define CanTransciever_GetBusOffFlag()                            Can_GetBusOffFlag()
#define CanTransciever_ClearBusOffCnt()                           Can_ClearBusOffCnt()
#define CanTransciever_GetBusOffCnt()                             Can_GetBusOffCnt()
#define CanTransciever_CheckIfTxOK()                              Can_CheckIfTxOK()
#define CanTransciever_TxMsg(frame, id, p_buff, len)              Can_TxMsg(frame, id, p_buff, len)
/*-------------CanNm port-----------------------------*/
#define CanTransciever_SetTxMsgOKFlag()                           CanNm_SetTxMsgOKFlag()
#define CanTransciever_CheckIfAllowTxMsg()                        CanNM_CheckIfAllowTxMsg()
#define CanTransciever_StartTxNmMsg()                             CanNm_StartTxMsg()
#define CanTransciever_StopTxNmMsg()                              CanNm_StopTxMsg()
/*-------------CanMatrix port-------------------------*/
#define CanTransciever_MatrixTxMsg()                              CanMatrix_TxMsgMain()
#define CanTransciever_CheckIfRxMsg(id)                           CanMatrix_CheckIfRxMsg(id)
/*-------------CanIf port-----------------------------*/
#define CanTransciever_ClearAllTxBuff()                           CanIf_ClearAllRxAndTxBuff()
#define CanTransciever_EnableTxAppUdsMsg()                        CanIF_SwtichAppUdsCtlByNm(FALSE, TRUE)
#define CanTransciever_DisableTxAppUdsMsg()                       CanIF_SwtichAppUdsCtlByNm(FALSE, FALSE)
#define CanTransciever_WriteAppRxMsg(frame, id, p_buff, len)      CanIf_WriteAppRxMsg(frame, id, p_buff, len)
#define CanTransciever_WriteNmRxMsg(frame, id, p_buff, len)       CanIf_WriteNmRxMsg(frame, id, p_buff, len)
#define CanTransciever_WriteUdsRxMsg(frame, id, p_buff, len)      CanIf_WriteUdsRxMsg(frame, id, p_buff, len)
#define CanTransciever_ReadAppTxMsg(p_frame, p_id, p_buff, p_len) CanIf_ReadAppTxMsg(p_frame, p_id, p_buff, p_len)
#define CanTransciever_ReadNmTxMsg(p_frame, p_id, p_buff, p_len)  CanIf_ReadNmTxMsg(p_frame, p_id, p_buff, p_len)
#define CanTransciever_ReadUdsTxMsg(p_frame, p_id, p_buff, p_len) CanIf_ReadUdsTxMsg(p_frame, p_id, p_buff, p_len)
/*-------------enum and struct------------------------*/
typedef enum
{
    CANTRANSCIEVER_NOACK_SEND     = 0x00,
    CANTRANSCIEVER_NOACK_WAIT     = 0x01,
    CANTRANSCIEVER_NOACK_TIME_OUT = 0x02
} CanTransciever_NoAck_TYPE;

typedef enum
{
    CANTRANSCIEVER_BUSOFF_CHECK    = 0x00,
    CANTRANSCIEVER_BUSOFF_QUICK    = 0x01,
    CANTRANSCIEVER_BUSOFF_SLOW     = 0x02,
    CANTRANSCIEVER_BUSOFF_RECOVERY = 0x03
} CanTransciever_BusOff_TYPE;

typedef struct
{
    uint16_t startApp;
    uint16_t startUds;
    uint16_t noAck;
    uint16_t busOff;
} CanTransciever_TimeTick_Type;
#endif

