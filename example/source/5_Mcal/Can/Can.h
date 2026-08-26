/*******************************************************
  * Name    :Can.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CAN_H_
#define _CAN_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
extern void     Can_Init(void);
extern void     Net_Can_DeInit(void);
extern void     LowPower_Can_DeInit(void);
extern void     Can_ClearMail(void);
extern Bool     Can_CheckIfTxOK(void);
extern Bool     Can_HasEmptyMail(void);
extern Bool     Can_GetNoAckFlag(void);
extern Bool     Can_GetBusOffFlag(void);
extern void     Can_ClearNoAckFlag(void);
extern void     Can_ClearBusOffFlag(void);
extern void     Can_ClearBusOffCnt(void);
extern uint16_t Can_GetBusOffCnt(void);
extern Bool     Can_TxMsg(Frame_Type frame, uint32_t id, uint8_t *p_buff, uint8_t len);
#endif

