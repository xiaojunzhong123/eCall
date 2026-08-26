/*******************************************************
  * Name    :SysManage_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _SYSMANAGE_CFG_H_
#define _SYSMANAGE_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Task.h"
#include "RteSys.h"
#include "Mcu.h"
#include "Gpt.h"
#include "Can.h"
#include "Adc.h"
#include "Spi.h"
#include "radio.h"
#include "sleep_manage.h"
#include "CDD_BTS.h"
#include "Wdg.h"
/*-------------define---------------------------------*/
/*-------------function port--------------------------*/
/*-------------Task port------------------------------*/
#define SysManage_TaskInitSchdule() Task_InitSchdule()
#define SysManage_TaskRunSchdule()  Task_RunSchdule()
/*-------------RteSys port----------------------------*/
#define SysManage_GetCanSleepFlag() RteSys_GetCanSleepFlag()
/*-------------enum and struct------------------------*/
typedef enum
{
    SYSMANAGE_MODE_INIT = 0x00,
    SYSMANAGE_MODE_NORMAL,
    SYSMANAGE_MODE_TWBS,
    SYSMANAGE_MODE_POLLING,
} SysManage_Mode_Type;

typedef enum
{
    SYSMANAGE_MODE_SLEEP = 0x00,
    SYSMANAGE_MODE_SCAN,
    SYSMANAGE_MODE_QUICK,
} SysManage_POLLING_Type;
#endif

