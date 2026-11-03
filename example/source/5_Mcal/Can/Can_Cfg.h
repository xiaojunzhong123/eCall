/*******************************************************
  * Name    :Can_Cfg.h
  * Author  :LoH
  * Function:HC32 CAN2 CAN FD compile-time configuration.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef _CAN_CFG_H_
#define _CAN_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "hc32_ll.h"
/*-------------define---------------------------------*/
#define CAN_CFG_RX_RING_SIZE              8U
#define CAN_CFG_ISR_MAX_DRAIN_FRAMES      16U

#define CAN_CFG_RX_SRS_INFO_ID             0x10CU

/* 40 MHz CAN clock: 20 TQ gives 500 kbit/s with an 80% sample point. */
#define CAN_CFG_NOMINAL_PRESCALER         4U
#define CAN_CFG_NOMINAL_TIME_SEG1         16U
#define CAN_CFG_NOMINAL_TIME_SEG2         4U
#define CAN_CFG_NOMINAL_SJW               2U

/* 40 MHz CAN clock: 20 TQ gives 2 Mbit/s with an 80% sample point. */
#define CAN_CFG_DATA_PRESCALER            1U
#define CAN_CFG_DATA_TIME_SEG1            16U
#define CAN_CFG_DATA_TIME_SEG2            4U
#define CAN_CFG_DATA_SJW                  4U

#endif
