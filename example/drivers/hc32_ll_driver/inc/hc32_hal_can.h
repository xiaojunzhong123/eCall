/**
 *******************************************************************************
 * @file  hc32_hal_can.h
 * @brief This file contains all the functions prototypes of the CAN driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2025-08-05       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef __HC32_HAL_CAN_H__
#define __HC32_HAL_CAN_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_def.h"

#include "hc32a4xx.h"
#include "hc32a4xx_conf.h"
/**
 * @addtogroup HAL_Driver
 * @{
 */

/**
 * @addtogroup HAL_CAN
 * @{
 */
#if ((defined LL_CAN_ENABLE) && (LL_CAN_ENABLE == DDL_ON)) || ((defined LL_MCAN_ENABLE) && (LL_MCAN_ENABLE == DDL_ON))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup HAL_CAN_Global_Types HAL CAN Global Types
 * @{
 */

/**
 * @brief HAL CAN bit time configuration structure
 */
/* Classical CAN */
/**
@verbatim
 * |-------------------------|--------------------------------|-------------------------------|
 * |                         |             CM_CANx            |            CM_MCANx           |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalPrescaler     |             1 ~ 256            |            1 ~ 512            |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalTimeSeg1      |             2 ~ 65             |            3 ~ 257            |
 * |                         |    u32NominalTimeSeg1 >=       |                               |
 * |                         |    (u32NominalTimeSeg2 + 2)    |                               |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalTimeSeg2      |             1 ~ 8              |            2 ~ 128            |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalSyncJumpWidth |             1 ~ 8              |            1 ~ 128            |
 * |-------------------------|--------------------------------|-------------------------------|
@endverbatim
   NOTE:
   1. SyncJumpWidth <= TimeSeg2.
*/
/* CAN-FD */
/**
@verbatim
 * |-------------------------|--------------------------------|-------------------------------|
 * |                         |             CM_CANx            |            CM_MCANx           |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalPrescaler     |             1 ~ 256            |            1 ~ 512            |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalTimeSeg1      |             2 ~ 65             |            3 ~ 257            |
 * |                         |    u32NominalTimeSeg1 >=       |                               |
 * |                         |    (u32NominalTimeSeg2 + 2)    |                               |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalTimeSeg2      |             1 ~ 32             |            2 ~ 128            |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32NominalSyncJumpWidth |             1 ~ 16             |            1 ~ 128            |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32DataPrescaler        |             1 ~ 256            |            1 ~ 32             |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32DataTimeSeg1         |             2 ~ 17             |            2 ~ 33             |
 * |                         |    u32DataTimeSeg1 >=          |                               |
 * |                         |    (u32DataTimeSeg2 + 2)       |                               |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32DataTimeSeg2         |             1 ~ 8              |            2 ~ 16             |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32DataSyncJumpWidth    |             1 ~ 8              |            1 ~ 16             |
 * |-------------------------|--------------------------------|-------------------------------|
 * | u32SspOffset            |  equal to u32DataTimeSeg1 is recommended                       |
 * |-------------------------|--------------------------------|-------------------------------|
@endverbatim
   NOTE:
   1. SyncJumpWidth <= TimeSeg2 for both nominal and data bit time.
   2. When CAN-FD used, it is recommended to limit the value of parameter u32DataPrescaler between 1 and 2.
*/
typedef struct {
    /* Nominal bit time configuration, used for classic CAN frame or arbitration phase of CAN-FD frame */
    uint32_t u32NominalPrescaler;           /*!< Specifies the nominal bit rate prescaler. */
    uint32_t u32NominalTimeSeg1;            /*!< Specifies the nominal time segment before sample point(the sum of Sync_Seg, Prop_Seg and Phase_Seg1). */
    uint32_t u32NominalTimeSeg2;            /*!< Specifies the nominal time segment after sample point(Phase_Seg2). */
    uint32_t u32NominalSyncJumpWidth;       /*!< Specifies the nominal (re)synchronization jump width(SJW). */
    /* FD bit time configuration, used for data phase of CAN-FD frame */
    uint32_t u32DataPrescaler;              /*!< Specifies the data bit rate prescaler. */
    uint32_t u32DataTimeSeg1;               /*!< Specifies the data time segment before sample point(the sum of Sync_Seg, Prop_Seg and Phase_Seg1). */
    uint32_t u32DataTimeSeg2;               /*!< Specifies the data time segment after sample point(Phase_Seg2). */
    uint32_t u32DataSyncJumpWidth;          /*!< Specifies the data (re)synchronization jump width(SJW). */
    /* FD TDC(Transmitter Delay Compensation) configuration */
    en_functional_state_t enTdcState;       /*!< Enable or disable TDC(Transmitter Delay Compensation). */
    uint32_t u32SspOffset;                  /*!< Specifies the transmitter delay compensation SSP offset(TQ). */
} stc_hal_can_bit_time_config_t;

/**
 * @brief HAL CAN filter structure definition
 */
typedef struct {
    uint8_t u8FilterType;                   /*!< Specifies the filter type.
                                                 This parameter can be a value of @ref HAL_CAN_Filter_Type
                                                 - For CM_CANx: can only be HAL_CAN_FILTER_TYPE_MASK
                                                 - For CM_MCANx: can be any value
                                            */
    uint32_t u32FilterId1;                  /*!< Specifies the filter ID1.
                                                 - For CM_CANx: it is the ID code.
                                                 - For CM_MCANx: depends on parameter 'u8FilterType'
                                            */
    uint32_t u32FilterId2;                  /*!< Specifies the filter ID2.
                                                 - For CM_CANx: it is the ID mask.
                                                 - For CM_MCANx: depends on parameter 'u8FilterType'
                                            */
} stc_hal_can_filter_t;

/**
 * @brief HAL CAN filter configuration structure
 */
typedef struct {
    stc_hal_can_filter_t *pstcStdFilterList;    /*!< Pointer to the standard ID filter elements list. */
    stc_hal_can_filter_t *pstcExtFilterList;    /*!< Pointer to the extended ID filter elements list. */
    uint8_t u8StdFilterNum;                     /*!< Number of the standard ID filter elements which to be configured.
                                                     This parameter must meet the following conditions:
                                                     - For CM_CANx: 1 <= u8StdFilterNum + u8ExtFilterNum <= 16
                                                     - For CM_MCANx: 1 <= u8StdFilterNum <= 40, if use classical CAN
                                                     - For CM_MCANx: 1 <= u8StdFilterNum <= 26, if use CAN-FD
                                                */
    uint8_t u8ExtFilterNum;                     /*!< Number of the extended ID filter elements which to be configured.
                                                     This parameter must meet the following conditions:
                                                     - For CM_CANx: 1 <= u8StdFilterNum + u8ExtFilterNum <= 16
                                                     - For CM_MCANx: 1 <= u8ExtFilterNum <= 40, if use classical CAN
                                                     - For CM_MCANx: 1 <= u8ExtFilterNum <= 26, if use CAN-FD
                                                */
} stc_hal_can_filter_config_t;

/**
 * @brief HAL CAN IRQ configuration structure
 */
/**
@verbatim
 * |------------|-------------------------------|
 * |            |             IRQn              |
 * |------------|-------------------------------|
 * |  HAL_CAN1  |   INT000_IRQn ~ INT031_IRQn   |
 * |  HAL_CAN2  |   INT092_IRQn ~ INT097_IRQn   |
 * |------------|-------------------------------|
 * |  HAL_CAN3  |   INT000_IRQn ~ INT031_IRQn   |
 * |  HAL_CAN4  |   INT122_IRQn ~ INT127_IRQn   |
 * |------------|-------------------------------|
@endverbatim
*/
typedef struct {
    uint32_t u32IntSel;                             /*!< Select the interrupt.
                                                         This parameter can be any combination of @ref HAL_CAN_Int_Type */
    IRQn_Type enIRQn;                               /*!< Specifies IRQ number
                                                         This parameter can be one of the value of @ref IRQn_Type as showing in the comments above */
    uint8_t u8IrqPriority;                          /*!< Specifies interrupt priority, can be a value of @ref INTC_Priority_Sel */

    func_ptr_t pfnRxCallback;                       /*!< Pointer to the receive callback function.
                                                         This function is called when a new message is received. */
    func_ptr_t pfnRxFullCallback;                   /*!< Pointer to the receive full callback function.
                                                         This function is called when the receive FIFO is full. */
    func_ptr_t pfnTxCallback;                       /*!< Pointer to the transmit complete callback function.
                                                         This function is called when a message has been transmitted. */
    func_ptr_t pfnErrorCallback;                    /*!< Pointer to the error callback function.
                                                         This function is called when an error occurs. */
    func_ptr_t pfnBusOffCallback;                   /*!< Pointer to the bus off callback function.
                                                         This function is called when the CAN controller enters bus off state. */
} stc_hal_can_irq_config_t;

/**
 * @brief HAL CAN initialization structure
 */
typedef struct {
    uint8_t u8Mode;                                 /*!< Specifies the CAN work mode.
                                                         This parameter can be a value of @ref HAL_CAN_Work_Mode */
    uint8_t u8Clock;                                /*!< Specifies the CAN communication clock which is used to generate bit rate.
                                                         This parameter can be a value of @ref HAL_CAN_Com_Clk */
    uint8_t u8FrameForamt;                          /*!< Specifies the CAN frame format.
                                                         This parameter can be a value of @ref HAL_CAN_Frame_Format */
    uint8_t u8RxOvfMode;                            /*!< Specifies the Rx FIFO overflow mode.
                                                         This parameter can be a value of @ref HAL_CAN_Rx_Ovf_Mode */
    uint8_t u8TxFifoMode;                           /*!< Specifies the Tx FIFO mode.
                                                         This parameter can be a value of @ref HAL_CAN_Tx_Fifo_Mode */
    en_functional_state_t enSingleShotTxState;      /*!< Enable or disable single shot transmission. Automatic retransmission by default. */

    stc_hal_can_bit_time_config_t stcBitTimeCfg;    /*!< Bit time configuration structure. */

    stc_hal_can_filter_config_t stcFilterCfg;       /*!< Acceptance filter configuration structure. */

    stc_hal_can_irq_config_t stcIrqCfg;             /*!< IRQ configuration structure. */
} stc_hal_can_init_t;

/**
 * @brief HAL CAN error information structure.
 */
typedef struct {
    uint8_t u8ErrorType;                            /*!< CAN error type.
                                                         For CM_CANx, this parameter indicates the error type of classic CAN and CAN-FD frames.
                                                         For CM_MCANx, this parameter indicates the error type of classic CAN frame and CAN-FD frame without BRS.
                                                         This parameter can be a value of @ref HAL_CAN_Err_Type */
    uint8_t u8DataErrorType;                        /*!< CAN-FD frame data phase error type.
                                                         This parameter is valid only when the frame is CAN-FD with BRS set.
                                                         This parameter can be a value of @ref HAL_CAN_Data_Err_Type */
    uint8_t u8RxErrorCount;                         /*!< Receive error count. */
    uint8_t u8TxErrorCount;                         /*!< Transmit error count. */
} stc_hal_can_error_info_t;

/**
 * @brief HAL CAN frame structure.
 */
typedef struct {
    uint32_t u32ID;                                 /*!< 11 bits standard ID or 29 bits extended ID, depending on IDE. */
    union {
        uint32_t u32Ctrl;
        struct {
            uint32_t DLC: 4;                        /*!< Data length code. Length of the data segment of data frame.
                                                         It should be zero while the frame is remote frame.
                                                         This parameter can be a value of @ref HAL_CAN_Data_Length_Code */
            uint32_t BRS: 1;                        /*!< Bit rate switch. Only valid for CAN-FD frame.
                                                         - 0: Transmit without bit rate switch
                                                         - 1: Transmit with bit rate switch */
            uint32_t FDF: 1;                        /*!< CAN-FD frame.
                                                         - 0: Classic CAN frame
                                                         - 1: CAN-FD frame */
            uint32_t RTR: 1;                        /*!< Remote transmission request bit. Only valid for classical CAN frame.
                                                         It is used to distinguish between data frames and remote frames.
                                                         - 0: Data frame
                                                         - 1: Remote frame */
            uint32_t IDE: 1;                        /*!< Identifier extension bit.
                                                         It is used to distinguish between standard format and extended format.
                                                         - 0: Standard format
                                                         - 1: Extended format */
            uint32_t RSVD: 24;                      /*!< Reserved bits. */
        };
    };
    uint8_t au8Data[64U];                           /*!< Data payload. */
    uint8_t u8DataSize;                             /*!< Output parameter, indicates the data size of received frame. */
} stc_hal_can_frame_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CAN_Global_Macros CAN Global Macros
 * @{
 */

/**
 * @defgroup HAL_CAN_Module HAL CAN Module
 * @{
 */
#define HAL_CAN1                        (0U)                /*!< Corresponds to CM_CAN1 */
#define HAL_CAN2                        (1U)                /*!< Corresponds to CM_CAN2 */
#define HAL_CAN3                        (2U)                /*!< Corresponds to CM_MCAN1 */
#define HAL_CAN4                        (3U)                /*!< Corresponds to CM_MCAN2 */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Work_Mode HAL CAN Work Mode
 * @{
 */
#define HAL_CAN_MD_NORMAL               (0U)                /*!< Normal mode */
#define HAL_CAN_MD_BUS_MON              (1U)                /*!< Bus monitoring mode */
#define HAL_CAN_MD_INTERN_LOOPBACK      (2U)                /*!< Internal loopback mode */
#define HAL_CAN_MD_EXTERN_LOOPBACK      (3U)                /*!< External loopback mode */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Com_Clk HAL CAN Communication Clock
 * @{
 */
#define HAL_CAN_CLK_SYSCLK_DIV2             (0x01U)
#define HAL_CAN_CLK_SYSCLK_DIV3             (0x02U)
#define HAL_CAN_CLK_SYSCLK_DIV4             (0x03U)
#define HAL_CAN_CLK_SYSCLK_DIV5             (0x04U)
#define HAL_CAN_CLK_SYSCLK_DIV6             (0x05U)
#define HAL_CAN_CLK_SYSCLK_DIV7             (0x06U)
#define HAL_CAN_CLK_SYSCLK_DIV8             (0x07U)
#define HAL_CAN_CLK_PLLQ                    (0x08U)
#define HAL_CAN_CLK_PLLR                    (0x09U)
#define HAL_CAN_CLK_PLLXP                   (0x0AU)
#define HAL_CAN_CLK_PLLXQ                   (0x0BU)
#define HAL_CAN_CLK_PLLXR                   (0x0CU)
#define HAL_CAN_CLK_XTAL                    (0x0DU)
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Frame_Format HAL CAN Frame Format
 * @{
 */
#define HAL_CAN_FRAME_CLASSIC               (0x0U)          /*!< Classic CAN mode */
#define HAL_CAN_FRAME_ISO_FD_NO_BRS         (0x1U)          /*!< ISO CAN-FD mode without bit rate switching */
#define HAL_CAN_FRAME_ISO_FD_BRS            (0x2U)          /*!< ISO CAN-FD mode with bit rate switching */
#define HAL_CAN_FRAME_NON_ISO_FD_NO_BRS     (0x3U)          /*!< Non-ISO CAN-FD mode without bit rate switching */
#define HAL_CAN_FRAME_NON_ISO_FD_BRS        (0x4U)          /*!< Non-ISO CAN-FD mode with bit rate switching */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_ID_Type HAL CAN ID Type
 * @{
 */
#define HAL_CAN_STD_ID                  (0U)                /*!< 11-bit Standard ID */
#define HAL_CAN_EXT_ID                  (1U)                /*!< 29-bit Extended ID */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_ID_Mask HAL CAN ID Mask
 * @{
 */
#define HAL_CAN_STD_ID_MASK             (0x7FFUL)           /*!< Standard ID mask */
#define HAL_CAN_EXT_ID_MASK             (0x1FFFFFFFUL)      /*!< Extended ID mask */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Filter_Type HAL CAN Filter Type
 * @{
 */
#define HAL_CAN_FILTER_TYPE_RANGE       (0x0U)              /*!< Range filter from filter ID1 to filter ID2 */
#define HAL_CAN_FILTER_TYPE_DUAL        (0x1U)              /*!< Dual ID filter for filter ID1 or filter ID2 */
#define HAL_CAN_FILTER_TYPE_MASK        (0x2U)              /*!< Classic filter: filter ID1 = filter, filter ID2 = mask
                                                                 A zero bit at the filter mask will mask out the corresponding bit position of the configured ID filter,
                                                                 e.g. the value of the received Message ID at that bit position is not relevant for acceptance filtering.
                                                                 Only those bits of the received Message ID where the corresponding mask bits are one are relevant
                                                                 for acceptance filtering.
                                                                 Use standard ID filter as example: ID1 = 0x123, ID2 = 0x7F0, the ID between 0x120 and 0x12F can be accepted. */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Data_Length_Code HAL CAN Data Length Code
 * @{
 */
#define HAL_CAN_DLC0                    (0x0U)              /*!< Size of data field is 0 bytes. */
#define HAL_CAN_DLC1                    (0x1U)              /*!< Size of data field is 1 bytes. */
#define HAL_CAN_DLC2                    (0x2U)              /*!< Size of data field is 2 bytes. */
#define HAL_CAN_DLC3                    (0x3U)              /*!< Size of data field is 3 bytes. */
#define HAL_CAN_DLC4                    (0x4U)              /*!< Size of data field is 4 bytes. */
#define HAL_CAN_DLC5                    (0x5U)              /*!< Size of data field is 5 bytes. */
#define HAL_CAN_DLC6                    (0x6U)              /*!< Size of data field is 6 bytes. */
#define HAL_CAN_DLC7                    (0x7U)              /*!< Size of data field is 7 bytes. */
#define HAL_CAN_DLC8                    (0x8U)              /*!< Size of data field is 8 bytes. */
#define HAL_CAN_DLC12                   (0x9U)              /*!< Size of data field is 12 bytes. */
#define HAL_CAN_DLC16                   (0xAU)              /*!< Size of data field is 16 bytes. */
#define HAL_CAN_DLC20                   (0xBU)              /*!< Size of data field is 20 bytes. */
#define HAL_CAN_DLC24                   (0xCU)              /*!< Size of data field is 24 bytes. */
#define HAL_CAN_DLC32                   (0xDU)              /*!< Size of data field is 32 bytes. */
#define HAL_CAN_DLC48                   (0xEU)              /*!< Size of data field is 48 bytes. */
#define HAL_CAN_DLC64                   (0xFU)              /*!< Size of data field is 64 bytes. */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Rx_Ovf_Mode HAL CAN Rx Overflow Mode
 * @{
 */
#define HAL_CAN_RX_OVF_BLOCKING         (0x0U)              /*!< Rx overflow blocking mode. New received message will be discarded. */
#define HAL_CAN_RX_OVF_OVERWRITE        (0x1U)              /*!< Rx overflow overwrite mode. New received message will overwrite the oldest received message. */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Tx_Fifo_Mode HAL CAN Tx FIFO Mode
 * @{
 */
#define HAL_CAN_TX_FIFO_MD_FIFO         (0x0U)              /*!< The Message first in first be transmitted. */
#define HAL_CAN_TX_FIFO_MD_PRIO         (0x1U)              /*!< The message with the lowest ID(highest priority) first be transmitted. */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Int_Type HAL CAN Interrupt Type
 * @{
 */
#define HAL_CAN_INT_RX                  (0x1U)              /*!< Received a new frame interrupt. */
#define HAL_CAN_INT_RX_FULL             (0x2U)              /*!< Rx FIFO full interrupt. */
#define HAL_CAN_INT_TX                  (0x4U)              /*!< Frame transmit complete interrupt. */
#define HAL_CAN_INT_ERR                 (0x8U)              /*!< Bus error interrupt. */
#define HAL_CAN_INT_BUS_OFF             (0x10U)             /*!< Bus-off interrupt. */
#define HAL_CAN_INT_ALL                 (0x1FU)             /*!< All interrupts. */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Err_Type HAL CAN Error Type
 * @{
 */
#define HAL_CAN_ERR_NONE                (0x0U)              /*!< No error. */
#define HAL_CAN_ERR_BIT                 (0x1U)              /*!< Bit error. */
#define HAL_CAN_ERR_FORM                (0x2U)              /*!< Form error. */
#define HAL_CAN_ERR_STUFF               (0x3U)              /*!< Stuff error. */
#define HAL_CAN_ERR_ACK                 (0x4U)              /*!< ACK error. */
#define HAL_CAN_ERR_CRC                 (0x5U)              /*!< CRC error. */
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup HAL_CAN_Global_Functions
 * @{
 */
int32_t HAL_CAN_Init(uint8_t u8CanModule, const stc_hal_can_init_t *pstcHalCanInit);
int32_t HAL_CAN_StructInit(stc_hal_can_init_t *pstcHalCanInit);
int32_t HAL_CAN_DeInit(uint8_t u8CanModule);
int32_t HAL_CAN_Start(uint8_t u8CanModule);
int32_t HAL_CAN_Stop(uint8_t u8CanModule);
int32_t HAL_CAN_TxFrame(uint8_t u8CanModule, const stc_hal_can_frame_t *pstcTxFrame);
int32_t HAL_CAN_GetRxFrame(uint8_t u8CanModule, stc_hal_can_frame_t *pstcRxFrame);
int32_t HAL_CAN_GetErrorInfo(uint8_t u8CanModule, stc_hal_can_error_info_t *pu8ErrInfo);
/**
 * @}
 */

#endif /* LL_CAN_ENABLE || LL_MCAN_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_HAL_CAN_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
