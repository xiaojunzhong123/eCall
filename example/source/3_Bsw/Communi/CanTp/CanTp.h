/*******************************************************
  * Name    :CanTp.h
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#ifndef _CANTP_H_
#define _CANTP_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

/*-------------function statement---------------------*/
extern void CanTp_Main(void);
extern void CanTp_TimerCtrl(void);
extern Bool CanTp_UdsTxMsg(Frame_Type frame, uint32_t id, uint8_t *p_buff, uint16_t len);
extern Bool CanTp_UdsRxMsg(Frame_Type *p_frame, uint32_t *p_id, uint8_t *p_buff, uint16_t *p_len);
extern void CanTpTask_Create(void);
#endif

