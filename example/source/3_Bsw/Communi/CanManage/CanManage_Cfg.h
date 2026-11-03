/*******************************************************
  * Name    :CanManage_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANMANAGE_CFG_H_
#define _CANMANAGE_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "CanNm.h"
#include "RteSys.h"
//#include "BatteryCtr.h"
#include "CanTransciever.h"

/*-------------define---------------------------------*/
#define CANMANAGE_ACTIVE_TIME             25
/*-------------function port--------------------------*/
/*-------------BatteryCtr port------------------------*/
#define CanManage_CheckVoltageIfNotOK()   FALSE //BatteryCtr_IsInLowHighStatus()
/*-------------CanTransciever port--------------------*/
#define CanManage_TranscieverTimerCtrl()  CanTransciever_TimerCtrl()
#define CanManage_TranscieverInit()       CanTransciever_Init()
#define CanManage_TranscieverDeInit()     CanTransciever_DeInit()
#define CanManage_TranscieverWakeUp()     CanTransciever_WakeUp()
#define CanManage_TranscieverSleep()      CanTransciever_Sleep()
#define CanManage_TranscieverNormalDeal() CanTransciever_NormalDeal()
/*-------------CanNm port-----------------------------*/
#define CanManage_NmTimerCtrl()           CanNm_TimerCtrl()
#define CanManage_NmMain()                CanNm_Main()
#define CanManage_NmStart()               CanNm_Start()
#define CanManage_NmStop()                CanNm_Stop()
#define CanManage_WakeUp()                CanNm_WakeUp()
#define CanManage_NmSleep()               CanNm_Sleep()
#define CanManage_NmCheckIfSleep()        CanNm_CheckIfSleep()
#define CanManage_NmSetSleeped()          CanNm_SetSleeped()
/*-------------RteSys port----------------------------*/
#define CanManage_GetSysTimeMs()          RteSys_GetSysTimeMs()
#define CanManage_CheckLocalIfSleep()     RteSys_GetLocalSleepFlag()
#define CanManage_SetCanSleepFlag(flag)   RteSys_SetCanSleepFlag(flag)
/*-------------enum and struct------------------------*/
typedef enum
{
    CANMANAGE_MODE_INIT = 0x00,
    CANMANAGE_MODE_WAIT,
    CANMANAGE_MODE_NORMAL,
    CANMANAGE_MODE_TWBS,
    CANMANAGE_MODE_SLEEP
} CanManage_Mode_Type;
#endif

