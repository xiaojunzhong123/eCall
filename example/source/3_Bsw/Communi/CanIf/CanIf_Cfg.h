/*******************************************************
  * Name    :CanIf_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANIF_CFG_H_
#define _CANIF_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Can.h"
#include "Memory.h"
#include "Uds_Def.h"
/*-------------define---------------------------------*/
#define CANIF_APP_RX_BUFF_SIZE         5
#define CANIF_APP_TX_BUFF_SIZE         5
#define CANIF_NM_RX_BUFF_SIZE          5
#define CANIF_NM_TX_BUFF_SIZE          5
#define CANIF_UDS_RX_BUFF_SIZE         5
#define CANIF_UDS_TX_BUFF_SIZE         5
/*-------------function port--------------------------*/
/*-------------Memory---------------------------------*/
#define CanIf_Copy(p_buff, p_src, len) Memory_Copy(p_buff, p_src, len)
/*-------------Can------------------------------------*/
#define CanIF_CheckIsInNoAck()         Can_GetNoAckFlag()
#define CanIF_CheckIsInBusOff()        Can_GetBusOffFlag()
/*-------------enum and struct------------------------*/
typedef union
{
    uint8_t data;
    struct
    {
        bits_t nmEnAppRx  : 1;
        bits_t nmEnAppTx  : 1;
        bits_t nmEnUdsRx  : 1;
        bits_t nmEnUdsTx  : 1;
        bits_t udsEnNmRx  : 1;
        bits_t udsEnNmTx  : 1;
        bits_t udsEnAppRx : 1;
        bits_t udsEnAppTx : 1;
    } bits;
} CanIF_CommCtr_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_APP_RX_BUFF_SIZE];
} CanIf_AppRxBuff_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_APP_TX_BUFF_SIZE];
} CanIf_AppTxBuff_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_NM_RX_BUFF_SIZE];
} CanIf_NmRxBuff_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_NM_TX_BUFF_SIZE];
} CanIf_NmTxBuff_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_UDS_RX_BUFF_SIZE];
} CanIf_UdsRxBuff_Type;

typedef struct
{
    uint8_t  readindex;
    uint8_t  writeindex;
    uint8_t  full;
    Can_Type msg[CANIF_UDS_TX_BUFF_SIZE];
} CanIf_UdsTxBuff_Type;
#endif

