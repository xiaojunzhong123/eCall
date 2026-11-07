/*******************************************************
  * Name    :Did.h
  * Author  :xiaojunzhong
  * Function:BSW 诊断 DID 接口及上电车辆配置装载入口。
  * Version :V1.1.0
  * Data    :2026.11.04
*******************************************************/
#ifndef _DID_H_
#define _DID_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
/*-------------function port--------------------------*/
/* 上电且 RTE 初始化后调用一次；同步装载 VIN、车型及能源，各组失败时保持无效。 */
extern void Did_Init(void);
extern Return_Type Did_0x22ReadDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);
extern Return_Type Did_0x2EWriteDataByIdentifier(Uds_DcmOpStatus_Type opStatus, Uds_DcmMsgContext_Type *p_msgContext, Uds_DcmNegRespCode_Type *p_errCode);
#endif

