/*******************************************************
  * Name    :Can.h
  * Author  :LoH
  * Function:HC32 CAN2 CAN FD driver interface.
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#ifndef _CAN_H_
#define _CAN_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------define---------------------------------*/
#define CAN_RX_DATA_MAX_LENGTH 64U

typedef struct
{
    uint32_t u32Id;
    uint8_t  u8Length;
    uint8_t  u8Ide;
    uint8_t  u8Fdf;
    uint8_t  u8Brs;
    uint8_t  au8Data[CAN_RX_DATA_MAX_LENGTH];
} CanRxFrame_t;

/* Initialize CAN2 and its PHY for 500 kbit/s nominal and 2 Mbit/s data rate. */
void Can_Init(void);
/* Deinitialize CAN2 and its PHY for normal-network or low-power shutdown. */
void Net_Can_DeInit(void);
void LowPower_Can_DeInit(void);
/* Copy one received frame from the ISR-owned fixed ring buffer. */
uint8_t Can_TakeRxFrame(CanRxFrame_t *pstFrame);
/* Send one standard-ID classic CAN or CAN FD+BRS frame. */
Bool Can_TxMsg(Frame_Type enFrame, uint32_t u32Id, const uint8_t *pu8Data, uint8_t u8Length);
/* Read stable CAN error and traffic counters from task context. */
void Can_ClearNoAckFlag(void);
Bool Can_GetNoAckFlag(void);

uint8_t Can_GetBusOffFlag(void);
uint32_t Can_GetBusOffCount(void);
uint32_t Can_GetRxOverflowCount(void);
uint32_t Can_GetTxErrorCount(void);
void Can_ClearBusOffFlag(void);
void Can_ClearBusOffCnt(void);
/* 旧通信框架兼容接口：返回现有累计计数，不清零。 */
uint16_t Can_GetBusOffCnt(void);
/* 返回现有 STB 发送完成标志，TRUE 为已完成，不清除标志。 */
Bool Can_CheckIfTxOK(void);
Bool Can_HasEmptyMail(void);
void Can_ClearMail(void);
#endif
