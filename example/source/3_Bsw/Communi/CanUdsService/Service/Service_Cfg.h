/*******************************************************
  * Name    :Service_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _SERVICE_CFG_H_
#define _SERVICE_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
//#include "Nvm.h"
#include "../../../../5_Mcal/Mcu/Mcu.h"
#include "Uds.h"
#include "Did.h"
#include "Dtc.h"
#include "CanIf.h"
#include "RteSys.h"
#include "Routine.h"
#include "SecuAccs.h"
#include "IoCtr.h"
/*-------------function port--------------------------*/
/*-------------Nvm port-------------------------------*/
#define Service_NvmRead(addr, p_buff, len)                                                  0//Nvm_Read(addr, p_buff, len)
#define Service_NvmWrite(addr, p_buff, len)                                                 0//Nvm_Write(addr, p_buff, len)
/*-------------Mcu------------------------------------*/
#define Service_SoftReset()                                                                 Mcu_SoftReset()
/*-------------RteSys---------------------------------*/
#define Service_GetSysTimeMs()                                                              RteSys_GetSysTimeMs()
/*-------------CanIf port-----------------------------*/
#define Service_SwtichCtlByUds(ctrlType, comType)                                           CanIF_SwtichCtlByUds(ctrlType, comType)
/*-------------Uds port-------------------------------*/
#define Service_GetCurrentSessionMode()                                                     Uds_GetCurrentSessionMode()
#define Service_ResetS3ServerTime()                                                         Uds_ResetS3ServerTime()
#define Service_DslSetSessionMode(sessMode)                                                 Uds_DslSetSessionMode(sessMode)
/*-------------Did port-------------------------------*/
#define Service_0x22HandleReadDataByIdentifier(opStatus, p_msgContext, p_errCode)           Did_0x22ReadDataByIdentifier(opStatus, p_msgContext, p_errCode)
#define Service_0x2EHandleWriteDataByIdentifier(opStatus, p_msgContext, p_errCode)          Did_0x2EWriteDataByIdentifier(opStatus, p_msgContext, p_errCode)
/*-------------Routine port---------------------------*/
#define Service_0x31HandleRoutineControl(opStatus, p_msgContext, p_errCode)                 Routine_0x31RoutineControl(opStatus, p_msgContext, p_errCode)
/*-------------SecuAccs port--------------------------*/
#define Service_0x27HandleSecurityAccess(opStatus, p_msgContext, p_errCode)                 SecuAccs_0x27SecurityAccess(opStatus, p_msgContext, p_errCode)
/*-------------Dtc port-------------------------------*/
#define Service_0x14HandleClearDiagnosticInformation(opStatus, p_msgContext, p_errCode)     Dtc_0x14ClearDiagnosticInformation(opStatus, p_msgContext, p_errCode)
#define Service_0x19HandleReadDtcInformation(opStatus, p_msgContext, p_errCode)             Dtc_0x19ReadDtcInformation(opStatus, p_msgContext, p_errCode)
#define Service_0x85HandleControlDtcSetting(opStatus, p_msgContext, p_errCode)              Dtc_0x85ControlDtcSetting(opStatus, p_msgContext, p_errCode)
/*-------------IoCtr port-----------------------------*/
#define Service_0x2FHandleInputOutputControlByIdentifier(opStatus, p_msgContext, p_errCode) IoCtr_0x2FInputOutputControlByIdentifier(opStatus, p_msgContext, p_errCode)
#endif

