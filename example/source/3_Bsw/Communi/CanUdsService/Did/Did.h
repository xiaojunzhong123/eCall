/*******************************************************
  * Name    :Did.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _DID_H_
#define _DID_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
/*-------------function port--------------------------*/
extern Return_Type Did_0x22ReadDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);
extern Return_Type Did_0x2EWriteDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);
#endif

