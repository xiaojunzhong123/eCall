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

typedef enum
{
    RTE_SWITCH_RELEASED = 0U,
    RTE_SWITCH_PRESSED
} RteSwitchState_t;

typedef enum
{
    RTE_KL15_OFF = 0U,
    RTE_KL15_ON
} RteKl15State_t;

typedef enum
{
    RTESYS_U8_SOS_SWITCH_STATE = 0U,
    RTESYS_U8_KL15_STATE,
    RTESYS_U8_TYPE_MAX
} RteSys_U8_Type;
/*-------------function port--------------------------*/
extern void    RteSys_SetU8Sig(RteSys_U8_Type id, uint8_t sig);
extern uint8_t RteSys_GetU8Sig(RteSys_U8_Type id);

#endif

