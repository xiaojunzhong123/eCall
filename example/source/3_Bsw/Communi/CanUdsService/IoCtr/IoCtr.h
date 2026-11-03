/*******************************************************
  * Name    :IoCtr.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _IOCTR_H_
#define _IOCTR_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
/*-------------function port--------------------------*/
extern void        IoCtr_Init(void);
extern void        IoCtr_TimerCtrl(void);
extern void        IoCtr_ReturnControlToEcu(void);
extern Return_Type IoCtr_0x2FInputOutputControlByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);
#endif

