/*******************************************************
  * Name    :CanNm_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANNM_CFG_H_
#define _CANNM_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "CanIF.h"
#include "CanTransciever.h"
/*-------------define---------------------------------*/
#define CANNM_BASE_ID                               0x400
#define CANNM_ID                                    0x545
#define CANNM_CYCLE_TIME                            1000
/*-------------function port--------------------------*/
/*-------------CanIF port-----------------------------*/
#define CanNm_SwtichAppUdsCtlMsg(isRxMsg, isEnable) CanIF_SwtichAppUdsCtlByNm(isRxMsg, isEnable)
#define CanNm_WriteTxMsg(frame, id, p_buff, len)    CanIf_WriteNmTxMsg(frame, id, p_buff, len)
#endif

