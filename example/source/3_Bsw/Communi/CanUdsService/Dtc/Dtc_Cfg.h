/*******************************************************
  * Name    :Dtc_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _DTC_CFG_H_
#define _DTC_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
#include "Uds.h"
#include "Rte.h"
#include "RteSys.h"
#include "Memory.h"
//#include "Nvm.h"
/*-------------define---------------------------------*/
#define DTC_ACTIVE_TIME                 1500
#define DTC_RETRY_MAX_CNT               3
#define DTC_ALLOW_MAX_AGING_CNT         40
#define DTC_CFG_LIST_NUM                DTC_ID_MAX_NUM
#define DTC_TEST_FAILED                 0x01
#define DTC_CONFIRMED                   0x08
#define DTC_SUPPORT_MASK                0x09
/*-------------function port--------------------------*/
/*-------------Rte port-------------------------------*/
#define Dtc_GetDtcCurErrFlag(id)        Rte_GetDtcCurErrFlag(id)
/*-------------RteSys port----------------------------*/
#define Dtc_GetSysTimeMs()              RteSys_GetSysTimeMs()
#define Dtc_GetIgnStatusFlag()          RteSys_GetIgnStatusFlag()
#define Dtc_SetDtcDeactiveFlag(flag)    RteSys_SetDtcDeactiveFlag(flag)
/*-------------Uds port-------------------------------*/
#define Dtc_GetCurrentSessionMode()     Uds_GetCurrentSessionMode()
/*-------------Memory---------------------------------*/
#define Dtc_Copy(p_buff, p_src, len)    Memory_CopyConst(p_buff, p_src, len)
#define Dtc_Fill(p_buff, data, len)     Memory_Fill(p_buff, data, len)
/*-------------Nvm port-------------------------------*/
#define Dtc_NvmRead(addr, p_buff, len)  0//Nvm_Read(addr, p_buff, len)
#define Dtc_NvmWrite(addr, p_buff, len) 0//Nvm_Write(addr, p_buff, len)
#define Dtc_NvmGetProgramStatus()       0//Nvm_GetProgramStatus()
/*-------------enum and struct------------------------*/
typedef enum
{
    DTC_FORMAT_ISO_15031 = 0x00,
    DTC_FORMAT_ISO_14229 = 0x01
} Dtc_Format_Type;

typedef enum
{
    DTC_MODE_INIT = 0x00,
    DTC_MODE_WAIT,
    DTC_MODE_READ,
    DTC_MODE_NORMAL,
    DTC_MODE_CLOSED
} Dtc_Mode_Type;

typedef struct
{
    uint8_t status;
    uint8_t ignStatus;
    uint8_t voltage;
    Bool    storage;
} Dtc_Data_Type;

typedef struct
{
    uint8_t occurrence;
    uint8_t aging;
} Dtc_Count_Type;

typedef struct
{
    bits_t  dtcStatus : 5;
    bits_t  ignStatus : 5;
    uint8_t voltage;
    uint8_t occurrence;
    uint8_t aging;
} Dtc_Eeprom_Type;

typedef struct
{
    const uint32_t     groupId;
    const Rte_Dtc_Type id;
    const uint8_t      supSessMode;
    const uint8_t      agingThres;
} Dtc_Cfg_Type;
/*-------------function statement---------------------*/
extern Bool Dtc_CheckIfService0x14ConditionsNotCorrect(uint32_t groupId);
extern Bool Dtc_CheckIfService0x19ConditionsNotCorrect(void);
extern Bool Dtc_CheckIfService0x85ConditionsNotCorrect(void);
/*-------------variable statement---------------------*/
extern const Dtc_Cfg_Type g_DtcCfgList[DTC_CFG_LIST_NUM];
#endif

