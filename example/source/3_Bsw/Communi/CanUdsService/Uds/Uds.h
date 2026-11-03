/*******************************************************
  * Name    :Uds.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _UDS_H_
#define _UDS_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

/*-------------function port--------------------------*/
extern void                 Uds_Init(void);
extern void                 Uds_Main(void);
extern void                 Uds_TimerCtrl(void);
extern void                 Uds_ResetS3ServerTime(void);
extern Uds_DcmSessMode_Type Uds_GetCurrentSessionMode(void);
extern Uds_DcmSecuLvl_Type  Uds_GetCurrentSecurityLevel(void);
extern void                 Uds_DslSetSessionMode(Uds_DcmSessMode_Type sessMode);
extern void                 Uds_SetCurrentSecurityLevel(Uds_DcmSecuLvl_Type secuLvl);

/* 创建本文件所属的诊断任务；不再声明已删除的以太网任务。 */
extern void UdsTask_Create(void);
#endif
