/*******************************************************
  * Name    :CanTp_Cfg.h
  * Author  :WangHu
  * Function:***
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#ifndef _CANTP_CFG_H
#define _CANTP_CFG_H
/*-------------include file---------------------------*/
#include "Types.h"
#include "CanIF.h"
#include "Memory.h"
#include "Uds_Def.h"
/*-------------define---------------------------------*/
#define CANTP_FUNC_ADDR_ID                            UDS_FUNC_ADDR_ID
#define CANTP_PHYS_ADDR_ID                            UDS_PHYS_ADDR_ID
#define CANTP_RESP_ADDR_ID                            UDS_RESP_ADDR_ID
#define CANTP_RX_BUFF_NB                              UDS_RX_BUFF_NB
#define CANTP_TX_BUFF_NB                              UDS_TX_BUFF_NB
#define CANTP_BYTE_FILL_VALUE                         0x00
#define CANTP_N_BS_TIME                               150
#define CANTP_N_CR_TIME                               150
#define CANTP_N_S_TMIN_TIME                           20
#define CANTP_BS_MAX                                  8
/*-------------function port--------------------------*/
/*-------------Memory---------------------------------*/
#define CanTp_Copy(p_buff, p_src, len)                Memory_CopyShort(p_buff, p_src, len)
#define CanTp_Fill(p_buff, data, len)                 Memory_FillLong(p_buff, data, len)
/*-------------CanIF----------------------------------*/
#define CanTp_ReadRxMsg(p_frame, p_id, p_buff, p_len) CanIf_ReadUdsRxMsg(p_frame, p_id, p_buff, p_len)
#define CanTp_WriteTxMsg(frame, id, p_buff, len)      CanIf_WriteUdsTxMsg(frame, id, p_buff, len)
/*-------------enum and struct------------------------*/
typedef enum
{
    CANTP_FS_CTS  = 0x00,
    CANTP_FS_WAIT = 0x01,
    CANTP_FS_OVF  = 0x02,
    CANTP_FS_NULL = 0x03
} CanTp_Fs_Type;

typedef enum
{
    CANTP_STATE_INIT   = 0x00,
    CANTP_STATE_NORMAL = 0x01,
    CANTP_STATE_OFF    = 0x02
} CanTp_State_Type;

typedef enum
{
    CANTP_RX_STATE_IDLE = 0x00,
    CANTP_RX_STATE_SF   = 0x01,
    CANTP_RX_STATE_FF   = 0x02,
    CANTP_RX_STATE_CF   = 0x03,
    CANTP_RX_STATE_WAIT = 0x04,
    CANTP_RX_STATE_OK   = 0x05
} CanTp_RxState_Type;

typedef enum
{
    CANTP_TX_STATE_IDLE  = 0x00,
    CANTP_TX_STATE_SF    = 0x01,
    CANTP_TX_STATE_FF    = 0x02,
    CANTP_TX_STATE_CF    = 0x03,
    CANTP_TX_STATE_WAIT  = 0x04,
    CANTP_TX_STATE_START = 0x05
} CanTp_TxState_Type;

typedef enum
{
    CANTP_RX_SF_NPCI   = 0x00,
    CANTP_RX_FF_NPCI   = 0x01,
    CANTP_RX_CF_NPCI   = 0x02,
    CANTP_RX_FC_NPCI   = 0x03,
    CANTP_RX_NULL_NPCI = 0x04
} CanTp_RxType_Type;

typedef enum
{
    CANTP_TX_SF_NPCI   = 0x00,
    CANTP_TX_FF_NPCI   = 0x01,
    CANTP_TX_CF_NPCI   = 0x02,
    CANTP_TX_FC_NPCI   = 0x03,
    CANTP_TX_NULL_NPCI = 0x04
} CanTp_TxType_Type;

typedef enum
{
    CANTP_N_IDLE       = 0x00,
    CANTP_N_OK         = 0x01,
    CANTP_N_TIMEOUT_BS = 0x02,
    CANTP_N_TIMEOUT_CR = 0x03,
    CANTP_N_WRONG_SN   = 0x04,
    CANTP_N_INVALID_FS = 0x05,
    CANTP_N_UNEXP_PDU  = 0x06,
    CANTP_N_BUFF_OVFLW = 0x07,
    CANTP_N_ERROR      = 0x08
} CanTp_Result_Type;

typedef union
{
    uint8_t data;
    struct
    {
        bits_t N_Bs    : 1;
        bits_t N_Cr    : 1;
        bits_t S_Tmin  : 1;
        bits_t resverd : 5;
    } bits;
} CanTp_TimeCtr_Type;

typedef struct
{
    uint16_t N_Bs;
    uint16_t N_Cr;
    uint16_t S_Tmin;
} CanTp_TimeTick_Type;

typedef struct
{
    CanTp_Fs_Type FS;
    uint8_t       SN;
    uint8_t       BS;
    uint8_t       BSmax;
    uint8_t       STmin;
    uint32_t      point;
} CanTp_MsgCtr_Type;

typedef struct
{
    Frame_Type frame;
    uint32_t   id;
    uint32_t   len;
    uint8_t    buff[CANTP_RX_BUFF_NB];
} CanTp_RxData_Type;

typedef struct
{
    Frame_Type frame;
    uint32_t   id;
    uint32_t   len;
    uint8_t    buff[CANTP_TX_BUFF_NB];
} CanTp_TxData_Type;
#endif

