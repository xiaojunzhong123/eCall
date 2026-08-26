/*******************************************************
  * Name    :RteSys.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _RTESYS_H_
#define _RTESYS_H_
/*-------------include file---------------------------*/
#include "Types.h"
typedef enum
{
    RTE_FUNIND_LearnRkeReq = 0x00,
    RTE_FUNIND_RkeCMD,
    RTE_FUNIND_RkeNumber,
    RTE_FUNIND_CANWakeup,
    RTE_FUNIND_LtchOverheatProtd,
    RTE_FUNIND_SearchCarSig,
    RTE_FUNIND_IGNoffAllowSwWork,
    RTE_FUNIND_RfIntFlag,
    RTE_FUNIND_RkeLearnSuccess,
    RTE_FUNIND_Quit_KeyLearn,
    RTE_FUNIND_SysVoltMode,
    RTE_FUNIND_CanVoltMode,
    RTE_FUNIND_LockActiveReson,
    RTE_FUNIND_AntifTriggerFlag,
    RTE_FUNIND_BefoeSleepDoorState,
    RTE_FUNIND_NetSleepRequest,
    RTE_FUNIND_NetIsActive,
    RTE_FUNIND_IntoTestMode,
    RTE_FUNIND_RkeLongPress,
    RTE_FUNIND_RfSleepmMode,
    RTE_Fun_ID_MAX_NUM
} Rte_Fun_Type;
/*-------------function port--------------------------*/

#endif

