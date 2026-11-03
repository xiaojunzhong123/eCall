/*******************************************************
  * Name    :SecuAccs_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _SECUACCS_CFG_H_
#define _SECUACCS_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Uds_Def.h"
#include "Memory.h"
#include "Uds.h"
//#include "Nvm.h"
/*-------------define---------------------------------*/
#define SECUACCS_CFG_LIST_NUM                     UDS_SECUACCS_LEN
/*-------------function port--------------------------*/
/*-------------Memory---------------------------------*/
#define SecuAccs_Compare(p_des, p_src, len)       Memory_Compare(p_des, p_src, len)
/*-------------Uds port-------------------------------*/
#define SecuAccs_GetCurrentSecurityLevel()        Uds_GetCurrentSecurityLevel()
#define SecuAccs_SetCurrentSecurityLevel(secuLvl) Uds_SetCurrentSecurityLevel(secuLvl)
/*-------------Nvm port-------------------------------*/
#define SecuAccs_NvmRead(addr, p_buff, len)       0//Nvm_Read(addr, p_buff, len)
#define SecuAccs_NvmWrite(addr, p_buff, len)      0//Nvm_Write(addr, p_buff, len)
/*-------------enum and struct------------------------*/
typedef enum
{
    SECUACCS_MSG_REQ_SEED = 0x00,
    SECUACCS_MSG_SEND_KEY = 0x01
} SecuAccs_Msg_Type;

typedef enum
{
    SECUACCS_RET_OK = 0x00,
    SECUACCS_RET_NOT_OK,
    SECUACCS_RET_REQ_UNLOCK,
    SECUACCS_RET_NEW_SEED
} SecuAccs_Return_Type;

typedef enum
{
    SECUACCS_STATE_ALL_SECULVL_LOCK = 0x00,
    SECUACCS_STATE_SEED_SEND_WAIT_KEY,
    SECUACCS_STATE_ONE_SECULVL_UNLOCK,
    SECUACCS_STATE_ONE_UNLOCK_WAIT_KEY
} SecuAccs_State_Type;

typedef void (*SecuAccs_GetSeedKey_Fun)(uint8_t *seed, uint8_t *key);

typedef struct
{
    uint8_t  seed[4];
    uint8_t  key[4];
    Bool     seedActive;
    uint8_t  atmptCnt;
    uint32_t secuDelayTick;
} SecuAccs_Status_Type;

typedef struct
{
    const Uds_DcmSecuLvl_Type     secuAccsLvl;
    const uint8_t                 secuAccsType;
    const uint8_t                 seedSize;
    const uint8_t                 keySize;
    const uint32_t                secuDelayMax;
    const uint8_t                 atmptCntMax;
    const Bool                    staticSeed;
    const SecuAccs_GetSeedKey_Fun getSeedKeyFun;
} SecuAccs_Cfg_Type;

typedef struct
{
    SecuAccs_State_Type      state;
    uint8_t                  preSecAccsType;
    SecuAccs_Msg_Type        msgType;
    SecuAccs_Status_Type    *p_curStatus;
    const SecuAccs_Cfg_Type *p_curCfg;
} SecuAccs_Info_Type;
/*-------------variable statement---------------------*/
extern uint16_t                g_SecuAccsRand;
extern const SecuAccs_Cfg_Type g_SecuAccsCfgList[SECUACCS_CFG_LIST_NUM];
#endif

