/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Mode_Types.h
 *
 * Description: Implementation of POWER IP layer
 *              Compiler:                IAR GCC
 *              AutoSAR Version:         4.4.0
 *
 *
 * Copyright (c) 2022 - 2026 Xiaohua Semiconductor Co., Ltd.
 * 
 * All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

/**
 *   @file       Power_Ip_Mode_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef POWER_IP_MODE_TYPES_H
#define POWER_IP_MODE_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "StandardTypes.h"
#include "System_Hw_Reg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief Word Clear mask.
 */
#define PWC_WORD_ZERO                   (uint16_t)0x0000U

/**
 * @brief Double word clear mask.
 */
#define PWC_DOUBLE_WORD_ZERO            (uint32_t)0x00000000U

/**
 * @brief Lock/unlock Code for each module
 */
#define PWC_UNLOCK_CODE0                (0xA501U)
#define PWC_UNLOCK_CODE1                (0xA502U)
#define PWC_UNLOCK_CODE3                (0xA508U)

/**
 * @brief PPWC FCG0 Unlock/Lock code
 */
#define PWC_FCG0_REG_UNLOCK_KEY         (0xA5A50001U)
#define PWC_FCG0_REG_LOCK_KEY           (0xA5A50000U)

/**
 * @brief ENABLE/DISABLE wake-up source
 */
#define PWC_HCLK_VALUE                  (g_SystemCoreClock_u32 >> ((CM_CMU->SCFGR & CMU_SCFGR_HCLKS) >> CMU_SCFGR_HCLKS_POS))
#define PWC_PD_WKUP2_POS                (16U)
#define CM4_SCR_SLEEPONEXIT_MASK32      ((uint32_t)0x00000002U)
#define CM4_SCR_SLEEPDEEP_MASK32        ((uint32_t)0x00000004U)

/**
 * @brief  PWC PVD Config.
 */
#define PWC_PVD_ON                      (PWC_PVDCR0_PVD1EN)
#define PWC_PVD_OFF                     (0x00U)

/**
 * @brief  PWC PVD channel.
 */
#define PWC_PVD_CH1                     (0x00U)
#define PWC_PVD_CH2                     (0x01U)

/**
 * @brief  PWC PVD Compare Config.
 */
#define PWC_PVD_CMP_OFF                 (0x00U)
#define PWC_PVD_CMP_ON                  (PWC_PVDCR1_PVD1CMPOE)

/**
 * @brief  PWC PVD Exception Type Select.
 */
#define PWC_PVD_EXP_TYPE_NONE           (0x00U)
#define PWC_PVD_EXP_TYPE_INT            (0x0101U)
#define PWC_PVD_EXP_TYPE_NMI            (0x0001U)
#define PWC_PVD_EXP_TYPE_RST            (PWC_PVDCR1_PVD1IRE | PWC_PVDCR1_PVD1IRS)
#define PWC_PVD_EXP_NMI_POS             (8U)

/**
 * @brief  PWC PVD Detection voltage.
 *
 * @note |     HC32A4A0      ||
 *       |    PVD1 | PVD2    ||
 * LVL0  |    2.0V | 2.1V    ||
 * LVL1  |    2.1V | 2.3V    ||
 * LVL2  |    2.3V | 2.5V    ||
 * LVL3  |    2.5V | 2.6V    ||
 * LVL4  |    2.6V | 2.6V    ||
 * LVL5  |    2.6V | 2.8V    ||
 * LVL6  |    2.8V | 2.9V    ||
 * LVL7  |    2.9V |  ---    ||
 * EXVCC |    ---  | EXVC  C ||
 */
#define PWC_PVD_THRESHOLD_LVL0          (0x00U)
#define PWC_PVD_THRESHOLD_LVL1          (0x01U)
#define PWC_PVD_THRESHOLD_LVL2          (0x02U)
#define PWC_PVD_THRESHOLD_LVL3          (0x03U)
#define PWC_PVD_THRESHOLD_LVL4          (0x04U)
#define PWC_PVD_THRESHOLD_LVL5          (0x05U)
#define PWC_PVD_THRESHOLD_LVL6          (0x06U)
#define PWC_PVD_THRESHOLD_LVL7          (0x07U)
#define PWC_PVD_EXTVCC                  (0x07U)

/**
 * @brief  PVD trigger setting.
 */
#define PWC_PVD_TRIG_FALLING            (0x00U << PWC_PVDICR_PVD1EDGS_POS)
#define PWC_PVD_TRIG_RISING             (0x01U << PWC_PVDICR_PVD1EDGS_POS)
#define PWC_PVD_TRIG_BOTH               (0x02U << PWC_PVDICR_PVD1EDGS_POS)

/**
 * @brief  PVD flag.
 */
#define PWC_PVD1_FLAG_DETECT            (PWC_PVDDSR_PVD1DETFLG)             /*!< VCC across VPVD1   */
#define PWC_PVD2_FLAG_DETECT            (PWC_PVDDSR_PVD2DETFLG)             /*!< VCC across VPVD2   */

/**
 * @brief  PVD digital filter ON or OFF.
 */
#define PWC_PVD_FILTER_ON               (0x00U)
#define PWC_PVD_FILTER_OFF              (0x01U)

/**
 * @brief  PVD digital filter sample ability.
 */
#define PWC_PVD_FILTER_LRC_DIV4         (0x00U << PWC_PVDFCR_PVD1NFCKS_POS) /*!< 0.25 LRC cycle */
#define PWC_PVD_FILTER_LRC_DIV2         (0x01U << PWC_PVDFCR_PVD1NFCKS_POS) /*!< 0.5 LRC cycle  */
#define PWC_PVD_FILTER_LRC_DIV1         (0x02U << PWC_PVDFCR_PVD1NFCKS_POS) /*!< 1 LRC cycle    */
#define PWC_PVD_FILTER_LRC_MUL2         (0x03U << PWC_PVDFCR_PVD1NFCKS_POS) /*!< 2 LRC cycles   */

/**
 * @brief  Power down mode wakeup Event status selection.
 */
#define PWC_PD_WKUP_FLAG0_POS           (0U)
#define PWC_PD_WKUP_FLAG1_POS           (8U)
#define PWC_PD_WKUP_FLAG_WKUP0          (PWC_PDWKF0_PTWK0F << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_WKUP1          (PWC_PDWKF0_PTWK1F << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_WKUP2          (PWC_PDWKF0_PTWK2F << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_WKUP3          (PWC_PDWKF0_PTWK3F << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_PVD1           (PWC_PDWKF0_VD1WKF << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_PVD2           (PWC_PDWKF0_VD2WKF << PWC_PD_WKUP_FLAG0_POS)
#define PWC_PD_WKUP_FLAG_RTCPRD         ((uint16_t)PWC_PDWKF1_RTCPRDWKF << PWC_PD_WKUP_FLAG1_POS)
#define PWC_PD_WKUP_FLAG_RTCALM         ((uint16_t)PWC_PDWKF1_RTCALMWKF << PWC_PD_WKUP_FLAG1_POS)
#define PWC_PD_WKUP_FLAG_WKTM           ((uint16_t)PWC_PDWKF1_WKTMWKF << PWC_PD_WKUP_FLAG1_POS)
#define PWC_PD_WKUP_FLAG_ALL            (PWC_PD_WKUP_FLAG_WKUP0  | PWC_PD_WKUP_FLAG_WKUP1  | PWC_PD_WKUP_FLAG_WKUP2 | \
                     PWC_PD_WKUP_FLAG_WKUP3  | PWC_PD_WKUP_FLAG_PVD1   | PWC_PD_WKUP_FLAG_PVD2  | \
                     PWC_PD_WKUP_FLAG_RTCPRD | PWC_PD_WKUP_FLAG_RTCALM | PWC_PD_WKUP_FLAG_WKTM)

/**
 * @brief  Power down mode wakeup trigger edge selection.
 */
#define PWC_PD_WKUP_TRIG_FALLING        (0x00U)
#define PWC_PD_WKUP_TRIG_RISING         (0x01U)
#define PWC_WORKING                     (0x00U)
#define PWC_STOP                        (0x01U)
#define PWC_POWERDOWN                   (0x03U)

/**
 * @brief  Power Monitor.
 */
#define PWC_POWER_MONITOR_DISABLE        (0x00U)
#define PWC_POWER_MONITOR_ENABLE         (0x01U)  
#define PWC_POWER_MONITOR_SRC_INTERNAL   (0x00U)
#define PWC_POWER_MONITOR_SRC_VBAT_DIV2   (0x01U)
#define PWC_VBAT_MONITOR_DISABLE          (0x00U)
#define PWC_VBAT_MONITOR_ENABLE           (0x01U)
#define PWC_VBAT_MONITOR_SRC_1_8_V        (0x00U)
#define PWC_VBAT_MONITOR_SRC_2_1_V        (0x01U)
#define PWC_VBAT_VBATDIV_DISABLE          (0x00U)
#define PWC_VBAT_VBATDIV_ENABLE           (0x01U)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief   This is structure for Turn off unused clock source
 *          in the low-power mode.
 */
typedef struct{
  uint32_t Xtal32Control   : 2;           /*!< XTAL32 oscillator Start/Stop.                       */
  uint32_t HRCControl      : 2;           /*!< HRC Start/Stop.                                     */
  uint32_t MRCControl      : 2;           /*!< MRC Start/Stop.                                     */
  uint32_t XtalControl     : 2;           /*!< XTAL oscillator Start/Stop.                         */
  uint32_t LRCControl      : 2;           /*!< LRC Start/Stop.                                     */
  uint32_t PLLHControl     : 2;           /*!< PLLH Start/Stop.                                    */
  uint32_t PLLAControl     : 2;           /*!< PLLA Start/Stop.                                    */
  uint32_t Reserved         : 18;          /*!< Reserved.                                            */
}Power_Ip_TurnOffClockSourceType;

/**
 * @brief   Structure configure for stop clock source.
 */
typedef struct{
  uint32_t DCU8      : 1;                 /*!< DCU8 function clock control,   0-Enable, 1-Disable. Bit 31  */
  uint32_t DCU7      : 1;                 /*!< DCU7 function clock control,   0-Enable, 1-Disable. Bit 30  */
  uint32_t DCU6      : 1;                 /*!< DCU6 function clock control,   0-Enable, 1-Disable. Bit 29  */
  uint32_t DCU5      : 1;                 /*!< DCU5 function clock control,   0-Enable, 1-Disable. Bit 28  */
  uint32_t DCU4      : 1;                 /*!< DCU4 function clock control,   0-Enable, 1-Disable. Bit 27  */
  uint32_t DCU3      : 1;                 /*!< DCU3 function clock control,   0-Enable, 1-Disable. Bit 26  */
  uint32_t DCU2      : 1;                 /*!< DCU2 function clock control,   0-Enable, 1-Disable. Bit 25  */
  uint32_t DCU1      : 1;                 /*!< DCU1 function clock control,   0-Enable, 1-Disable. Bit 24  */
  uint32_t CRC       : 1;                 /*!< CRC function clock control,    0-Enable, 1-Disable. Bit 23  */
  uint32_t TRNG      : 1;                 /*!< TRNG function clock control,   0-Enable, 1-Disable. Bit 22  */
  uint32_t HASH      : 1;                 /*!< HASH function clock control,   0-Enable, 1-Disable. Bit 21  */
  uint32_t SKE_AES   : 1;                 /*!< SKE/AES function clock control,    0-Enable, 1-Disable. Bit 20  */
  uint32_t MAU       : 1;                 /*!< MAU function clock control,    0-Enable, 1-Disable. Bit 19  */
  uint32_t CTC       : 1;                 /*!< CTC function clock control,    0-Enable, 1-Disable. Bit 18  */
  uint32_t AOS       : 1;                 /*!< AOS function clock control,    0-Enable, 1-Disable. Bit 17  */
  uint32_t FCM       : 1;                 /*!< FCM function clock control,    0-Enable, 1-Disable. Bit 16  */
  uint32_t DMA2      : 1;                 /*!< DMA2 function clock control,   0-Enable, 1-Disable. Bit 15  */
  uint32_t DMA1      : 1;                 /*!< DMA1 function clock control,   0-Enable, 1-Disable. Bit 14  */
  uint32_t KEY   : 1;                 /*!< KEY function clock control 0-Enable, 1-Disable. Bit 13  */
  uint32_t PKE       : 1;                 /*!< PKE function clock control,    0-Enable, 1-Disable. Bit 12  */
  uint32_t Reserved1 : 1;                 /*!< Reserved bits [11]                                          */
  uint32_t SRAMB     : 1;                 /*!< SRAMB function clock control,  0-Enable, 1-Disable. Bit 10  */
  uint32_t Reserved2 : 1;                 /*!< Reserved bits [9]                                           */
  uint32_t ERMU      : 1;                 /*!< ERMU function clock control,   0-Enable, 1-Disable. Bit 8   */
  uint32_t SRAM4     : 1;                 /*!< SRAM4 function clock control,  0-Enable, 1-Disable. Bit 7   */
  uint32_t SRAM3     : 1;                 /*!< SRAM3 function clock control,  0-Enable, 1-Disable. Bit 6   */
  uint32_t SRAM2     : 1;                 /*!< SRAM2 function clock control,  0-Enable, 1-Disable. Bit 5   */
  uint32_t SRAM1     : 1;                 /*!< SRAM1 function clock control,  0-Enable, 1-Disable. Bit 4   */
  uint32_t Reserved3 : 3;                 /*!< Reserved bits [3:1]                                         */
  uint32_t SRAMH     : 1;                 /*!< SRAMH function clock control,  0-Enable, 1-Disable. Bit 0   */
}Power_Ip_FCG0Type;

/**
 * @brief   Structure configure for stop clock source.
 */
typedef struct{
  uint32_t  Reserved : 2;                  /*!< Reserved bits [31:30]                                       */
  uint32_t  MCAN2   : 1;                  /*!< MCAN2 function clock control,  0-Enable, 1-Disable. Bit 29  */
  uint32_t  MCAN1   : 1;                  /*!< MCAN1 function clock control,  0-Enable, 1-Disable. Bit 28  */
  uint32_t  FMAC4   : 1;                  /*!< FMAC4 function clock control,  0-Enable, 1-Disable. Bit 27  */
  uint32_t  FMAC3   : 1;                  /*!< FMAC3 function clock control,  0-Enable, 1-Disable. Bit 26  */
  uint32_t  FMAC2   : 1;                  /*!< FMAC2 function clock control,  0-Enable, 1-Disable. Bit 25  */
  uint32_t  FMAC1   : 1;                  /*!< FMAC1 function clock control,  0-Enable, 1-Disable. Bit 24  */
  uint32_t  USBHS   : 1;                  /*!< USBHS function clock control,  0-Enable, 1-Disable. Bit 23  */
  uint32_t  USBFS   : 1;                  /*!< USBFS function clock control,  0-Enable, 1-Disable. Bit 22  */
  uint32_t  SPI6    : 1;                  /*!< SPI6 function clock control,   0-Enable, 1-Disable. Bit 21  */
  uint32_t  SPI5    : 1;                  /*!< SPI5 function clock control,   0-Enable, 1-Disable. Bit 20  */
  uint32_t  SPI4    : 1;                  /*!< SPI4 function clock control,   0-Enable, 1-Disable. Bit 19  */
  uint32_t  SPI3    : 1;                  /*!< SPI3 function clock control,   0-Enable, 1-Disable. Bit 18  */
  uint32_t  SPI2    : 1;                  /*!< SPI2 function clock control,   0-Enable, 1-Disable. Bit 17  */
  uint32_t  SPI1    : 1;                  /*!< SPI1 function clock control,   0-Enable, 1-Disable. Bit 16  */
  uint32_t  I2S4    : 1;                  /*!< I2S4 function clock control,   0-Enable, 1-Disable. Bit 15  */
  uint32_t  I2S3    : 1;                  /*!< I2S3 function clock control,   0-Enable, 1-Disable. Bit 14  */
  uint32_t  I2S2    : 1;                  /*!< I2S2 function clock control,   0-Enable, 1-Disable. Bit 13  */
  uint32_t  I2S1    : 1;                  /*!< I2S1 function clock control,   0-Enable, 1-Disable. Bit 12  */
  uint32_t  SDIOC2  : 1;                  /*!< SDIOC2 function clock control, 0-Enable, 1-Disable. Bit 11  */
  uint32_t  SDIOC1  : 1;                  /*!< SDIOC1 function clock control, 0-Enable, 1-Disable. Bit 10  */
  uint32_t  I2C6    : 1;                  /*!< I2C6 function clock control,   0-Enable, 1-Disable. Bit 9   */
  uint32_t  I2C5    : 1;                  /*!< I2C5 function clock control,   0-Enable, 1-Disable. Bit 8   */
  uint32_t  I2C4    : 1;                  /*!< I2C4 function clock control,   0-Enable, 1-Disable. Bit 7   */
  uint32_t  I2C3    : 1;                  /*!< I2C3 function clock control,   0-Enable, 1-Disable. Bit 6   */
  uint32_t  I2C2    : 1;                  /*!< I2C2 function clock control,   0-Enable, 1-Disable. Bit 5   */
  uint32_t  I2C1    : 1;                  /*!< I2C1 function clock control,   0-Enable, 1-Disable. */
  uint32_t  QSPI    : 1;                  /*!< QSPI function clock control,   0-Enable, 1-Disable. */
  uint32_t  ETHMAC  : 1;                  /*!< ETHMAC function clock control, 0-Enable, 1-Disable. */
  uint32_t  CAN2    : 1;                  /*!< CAN2 function clock control,   0-Enable, 1-Disable. */
  uint32_t  CAN1    : 1;                  /*!< CAN1 function clock control,   0-Enable, 1-Disable. */
}Power_Ip_FCG1Type;

/**
 * @brief   Structure configure for stop clock source.
 */
typedef struct{
  uint32_t  TMRA_12 : 1;                 /*!< TMRA_12 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMRA_11 : 1;                 /*!< TMRA_11 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMRA_10 : 1;                 /*!< TMRA_10 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMRA_9  : 1;                 /*!< TMRA_9 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_8  : 1;                 /*!< TMRA_8 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_7  : 1;                 /*!< TMRA_7 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_6  : 1;                 /*!< TMRA_6 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_5  : 1;                 /*!< TMRA_5 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_4  : 1;                 /*!< TMRA_4 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_3  : 1;                 /*!< TMRA_3 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_2  : 1;                 /*!< TMRA_2 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMRA_1  : 1;                 /*!< TMRA_1 function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMR2_4 : 1;                 /*!< TMR2_4 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR2_3 : 1;                 /*!< TMR2_3 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR2_2 : 1;                 /*!< TMR2_2 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR2_1 : 1;                 /*!< TMR2_1 function clock control, 0-Enable, 1-Disable. */
  uint32_t  EMB    : 1;                 /*!< EMB function clock control,    0-Enable, 1-Disable. */
  uint32_t  TMR0_3 : 1;                 /*!< TMR0_3 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR0_2 : 1;                 /*!< TMR0_2 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR0_1 : 1;                 /*!< TMR0_1 function clock control, 0-Enable, 1-Disable. */
  uint32_t  HRPWM  : 1;                 /*!< HRPWM function clock control,  0-Enable, 1-Disable. */
  uint32_t  TMR4_3 : 1;                 /*!< TMR4_3 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR4_2 : 1;                 /*!< TMR4_2 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR4_1 : 1;                 /*!< TMR4_1 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_8 : 1;                 /*!< TMR6_8 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_7 : 1;                 /*!< TMR6_7 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_6 : 1;                 /*!< TMR6_6 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_5 : 1;                 /*!< TMR6_5 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_4 : 1;                 /*!< TMR6_4 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_3 : 1;                 /*!< TMR6_3 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_2 : 1;                 /*!< TMR6_2 function clock control, 0-Enable, 1-Disable. */
  uint32_t  TMR6_1 : 1;                 /*!< TMR6_1 function clock control, 0-Enable, 1-Disable. */
}Power_Ip_FCG2Type;

/**
 * @brief  Structure configure for stop clock source.
 */
typedef struct{
  uint32_t Reserved1 : 2;                  /*!< Reserved.                                              */
  uint32_t USART10  : 1;                  /*!< USART10 function clock control, 0-Enable, 1-Disable.  */
  uint32_t USART9   : 1;                  /*!< USART9 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART8   : 1;                  /*!< USART8 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART7   : 1;                  /*!< USART7 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART6   : 1;                  /*!< USART6 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART5   : 1;                  /*!< USART5 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART4   : 1;                  /*!< USART4 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART3   : 1;                  /*!< USART3 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART2   : 1;                  /*!< USART2 function clock control,  0-Enable, 1-Disable.  */
  uint32_t USART1   : 1;                  /*!< USART1 function clock control,  0-Enable, 1-Disable.  */
  uint32_t Reserved2 : 1;                  /*!< Reserved.                                              */
  uint32_t NFC      : 1;                  /*!< NFC function clock control,      0-Enable, 1-Disable. */
  uint32_t DMC      : 1;                  /*!< DMC function clock control,      0-Enable, 1-Disable. */
  uint32_t SMC      : 1;                  /*!< SMC function clock control,      0-Enable, 1-Disable. */
  uint32_t DVP      : 1;                  /*!< DVP function clock control,      0-Enable, 1-Disable. */
  uint32_t Reserved3 : 2;                  /*!< Reserved.                                              */
  uint32_t OTS      : 1;                  /*!< OTS function clock control,      0-Enable, 1-Disable. */
  uint32_t TMR0_5   : 1;                  /*!< TMRG5 function clock control,    0-Enable, 1-Disable. */
  uint32_t TMR0_4   : 1;                  /*!< TMRG4 function clock control,    0-Enable, 1-Disable. */
  uint32_t CMP34    : 1;                  /*!< CMP34 function clock control,    0-Enable, 1-Disable. */
  uint32_t CMP12    : 1;                  /*!< CMP12 function clock control,    0-Enable, 1-Disable. */
  uint32_t Reserved4 : 2;                  /*!< Reserved.                                              */
  uint32_t DAC2     : 1;                  /*!< DAC2 function clock control,     0-Enable, 1-Disable. */
  uint32_t DAC1     : 1;                  /*!< DAC1 function clock control,     0-Enable, 1-Disable. */
  uint32_t CMBIAS   : 1;                  /*!< CMBIAS function clock control,   0-Enable, 1-Disable. */
  uint32_t ADC3     : 1;                  /*!< ADC3 function clock control,     0-Enable, 1-Disable. */
  uint32_t ADC2     : 1;                  /*!< ADC2 function clock control,     0-Enable, 1-Disable. */
  uint32_t ADC1     : 1;                  /*!< ADC1 function clock control,     0-Enable, 1-Disable. */
}Power_Ip_FCG3Type;

/**
 * @brief  Structure for turn off unused RAM.
 */
typedef struct {
    uint32_t RAMPDC10    : 1;    /*!< 0x1FFF8000~0x1FFFFFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC9     : 1;    /*!< 0x1FFF0000~0x1FFF7FFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC8     : 1;    /*!< 0x1FFE8000~0x1FFEFFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC7     : 1;    /*!< 0x1FFE0000~0x1FFE7FFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC6     : 1;    /*!< 0x20058000~0x2005FFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC5     : 1;    /*!< 0x20050000~0x20057FFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC4     : 1;    /*!< 0x20040000~0x2004FFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC3     : 1;    /*!< 0x20030000~0x2003FFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC2     : 1;    /*!< 0x20020000~0x2002FFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC1     : 1;    /*!< 0x20010000~0x2001FFFF 0-Power on, 1-Power down. */
    uint32_t RAMPDC0     : 1;    /*!< 0x20000000~0x2000FFFF 0-Power on, 1-Power down. */
    uint32_t PRAMPDC11   : 1;    /*!< RAM for PKE           0-Power on, 1-Power down. */
    uint32_t PRAMPDC10   : 1;    /*!< RAM for MCAN1/MCAN2   0-Power on, 1-Power down. */
    uint32_t PRAMPDC9    : 1;    /*!< RAM for NFC           0-Power on, 1-Power down. */
    uint32_t PRAMPDC8    : 1;    /*!< RAM for SDIO2         0-Power on, 1-Power down. */
    uint32_t PRAMPDC7    : 1;    /*!< RAM for SDIO1         0-Power on, 1-Power down. */
    uint32_t PRAMPDC6    : 1;    /*!< RAM for ETHMACRX      0-Power on, 1-Power down. */
    uint32_t PRAMPDC5    : 1;    /*!< RAM for ETHMACTX      0-Power on, 1-Power down. */
    uint32_t PRAMPDC4    : 1;    /*!< RAM for USBHS         0-Power on, 1-Power down. */
    uint32_t PRAMPDC3    : 1;    /*!< RAM for USBFS         0-Power on, 1-Power down. */
    uint32_t PRAMPDC2    : 1;    /*!< RAM for cache         0-Power on, 1-Power down. */
    uint32_t PRAMPDC1    : 1;    /*!< RAM for CAN2          0-Power on, 1-Power down. */
    uint32_t PRAMPDC0    : 1;    /*!< RAM for CAN1          0-Power on, 1-Power down. */
    uint32_t SRAMB       : 1;    /*!< RAM for SRAMB         0-Power on, 1-Power down. */
    uint32_t Reserved     : 8;    /*!< Reserved.                                        */
} Power_Ip_TurnoffRamType;

/**
 * @brief Structure for PVD1.
 */
typedef struct {
  uint32_t PWC_PVDState_u32;              /*!< PVD state.                                      */
  uint32_t PWC_PVDCompareOutputState_u32; /*!< PVD compare output function setting.            */
  uint32_t PWC_PVDExceptionType_u32;      /*!< PVD interrupt or reset selection.               */
  uint32_t PWC_PVDFilter_u32;             /*!< PVD digital filter function setting.            */
  uint32_t PWC_PVDFilterClock_u32;        /*!< PVD digital filter clock setting.               */
  uint32_t PWC_PVDThresholdVoltage_u32;   /*!< PVD detect voltage setting.                     */
  uint32_t PWC_PVDTriggerEdge_u32;        /*!< PVD trigger setting.                            */
} Power_Ip_PWC_PVD1ConfigType;

/**
 * @brief Structure for PVD2.
 */
typedef struct {
  uint32_t PWC_PVDState_u32;              /*!< PVD state.                                      */
  uint32_t PWC_PVDCompareOutputState_u32; /*!< PVD compare output function setting.            */
  uint32_t PWC_PVDExceptionType_u32;      /*!< PVD interrupt or reset selection.               */
  uint32_t PWC_PVDFilter_u32;             /*!< PVD digital filter function setting.            */
  uint32_t PWC_PVDFilterClock_u32;        /*!< PVD digital filter clock setting.               */
  uint32_t PWC_PVDThresholdVoltage_u32;   /*!< PVD detect voltage setting.                     */
  uint32_t PWC_PVDTriggerEdge_u32;        /*!< PVD trigger setting.                            */
} Power_Ip_PWC_PVD2ConfigType;

/**
 * @brief Structure for Power Monitor.
 */
typedef struct {
  uint32_t PWC_PowerMonitorCmd_u32;
  uint32_t PWC_PowerMonitorSrc_u32;
  uint32_t PWC_VbatMonitorCmd_u32;
  uint32_t PWC_VbatMonitorSrc_u32;
} Power_Ip_PowerMonitorConfigType;

/**
 * @brief Struct storage value of ADC and DAC register using in Stop mode , to make backup.
 */
typedef struct {
  uint32_t  DAC1_DAE  : 1 ;               /*!< channel all enable - DAC 1.                     */
  uint32_t  DAC2_DAE  : 1 ;               /*!< channel all enable - DAC 2.                     */
  uint32_t  ADC1_STRT : 1 ;               /*!< Start AD conversion - ADC 1.                    */
  uint32_t  ADC2_STRT : 1 ;               /*!< Start AD conversion - ADC 2.                    */
  uint32_t  ADC3_STRT : 1 ;               /*!< Start AD conversion - ADC 3.                    */
  uint32_t  FCG3_ADC1 : 1 ;               /*!< ADC1 function Clock control.                    */
  uint32_t  FCG3_ADC2 : 1 ;               /*!< ADC2 function Clock control.                    */
  uint32_t  FCG3_ADC3 : 1 ;               /*!< ADC3 function Clock control.                    */
  uint32_t  Reserved  : 24;               /*!< Reserved.                                        */
} Power_Ip_ADCDACBackuptype;

/**
 * @brief System clock set after wake-up from stop-mode.
 */
typedef enum {
  PWC_STOP_CLK_KEEP = 0,                /*!< After the stop_mode wakes up, the clock and division maintain the settings before entering.    */
  PWC_STOP_CLK_MRC  = 1                 /*!< After the stop_mode wakes up, the system clock switch to MRC, SCKCFGR register are initialize. */
} Power_Ip_ClkAfterStopWkuType;

/**
 * @brief Stop-mode drive capacity.
 */
typedef enum {
  PWC_STOP_DRV_LOW  = 0,                /*!< Enter stop mode from Low speed.                 */
  PWC_STOP_DRV_HIGH = 1                 /*!< Enter stop mode from high speed.                */
} Power_Ip_StopDrvType;

/**
 * @brief  Control for EXMC  bus address and signals Control.
 */
typedef enum {
  PWC_STOP_EXBUS_HIZ  = 0,              /*!< High impedance.                                  */
  PWC_STOP_EXBUS_HOLD = 1               /*!< maintain the state before PWD or Stop mode.      */
} Power_Ip_ExBusHoldType;

/**
 * @brief Waiting stable after wake-up from stop mode.
 */
typedef enum {
  PWC_STOP_FLASH_WAIT_ON  = 0,          /*!< After wake up from stop mode wait for FLASH to stabilize.     */
  PWC_STOP_FLASH_WAIT_OFF = 1           /*!< After wake up from stop mode, don't wait for Flash stabilize. */
} Power_Ip_FlashWaitType;

/**
 * @brief    Config Struct WKU event edge select.
 */
typedef struct {
  uint32_t   Reserved                : 2;  /*!< Reserved.                                        */
  uint32_t   PWC_PD_WKUP_TRIG_PVD1  : 1;  /*!< VD2 edge select.                                */
  uint32_t   PWC_PD_WKUP_TRIG_PVD2  : 1;  /*!< Vd1 edge select.                                */
  uint32_t   PWC_PD_WKUP_TRIG_WKUP0 : 1;  /*!< PTWK0 edge select.                              */
  uint32_t   PWC_PD_WKUP_TRIG_WKUP1 : 1;  /*!< PTWK1 edge select.                              */
  uint32_t   PWC_PD_WKUP_TRIG_WKUP2 : 1;  /*!< PTWK2 edge select.                              */
  uint32_t   PWC_PD_WKUP_TRIG_WKUP3 : 1;  /*!< PTWK3 edge select.                              */
  uint32_t   Reserved24             : 24; /*!< Reserved.                                        */
} Power_Ip_PWDWKUEdgeSelType;

/**
 * @brief  Define Wake up source for Stop-Mode.
 */
typedef struct {
  uint32_t EIRQ0     : 1;                 /*!< EIRQ 0  stop-mode wake-up enable.               */
  uint32_t EIRQ1     : 1;                 /*!< EIRQ 1  stop-mode wake-up enable.               */
  uint32_t EIRQ2     : 1;                 /*!< EIRQ 2  stop-mode wake-up enable.               */
  uint32_t EIRQ3     : 1;                 /*!< EIRQ 3  stop-mode wake-up enable.               */
  uint32_t EIRQ4     : 1;                 /*!< EIRQ 4  stop-mode wake-up enable.               */
  uint32_t EIRQ5     : 1;                 /*!< EIRQ 5  stop-mode wake-up enable.               */
  uint32_t EIRQ6     : 1;                 /*!< EIRQ 6  stop-mode wake-up enable.               */
  uint32_t EIRQ7     : 1;                 /*!< EIRQ 7  stop-mode wake-up enable.               */
  uint32_t EIRQ8     : 1;                 /*!< EIRQ 8  stop-mode wake-up enable.               */
  uint32_t EIRQ9     : 1;                 /*!< EIRQ 9  stop-mode wake-up enable.               */
  uint32_t EIRQ10    : 1;                 /*!< EIRQ 10 stop-mode wake-up enable.               */
  uint32_t EIRQ11    : 1;                 /*!< EIRQ 11 stop-mode wake-up enable.               */
  uint32_t EIRQ12    : 1;                 /*!< EIRQ 12 stop-mode wake-up enable.               */
  uint32_t EIRQ13    : 1;                 /*!< EIRQ 13 stop-mode wake-up enable.               */
  uint32_t EIRQ14    : 1;                 /*!< EIRQ 14 stop-mode wake-up enable.               */
  uint32_t EIRQ15    : 1;                 /*!< EIRQ 15 stop-mode wake-up enable.               */
  uint32_t SWDT      : 1;                 /*!< SWDT_REFUDF stop-mode wake-up enable.           */
  uint32_t PVD1      : 1;                 /*!< PVD1 stop-mode wake-up enable.                  */
  uint32_t PVD2      : 1;                 /*!< PVD2 stop-mode wake-up enable.                  */
  uint32_t CMP       : 1;                 /*!< CMP stop-mode wake-up enable.                   */
  uint32_t WKTM      : 1;                 /*!< WKTM_PRD stop-mode wake-up enable.              */
  uint32_t RTCALM    : 1;                 /*!< RTC_ALM stop-mode wake-up enable.               */
  uint32_t RTCPRD    : 1;                 /*!< RTC_PRD stop-mode wake-up enable.               */
  uint32_t TMR0CMP   : 1;                 /*!< TMR01_GCMA stop-mode wake-up enable.            */
  uint32_t TMR2GCM   : 1;                 /*!< TMR02_GCMA stop-mode wake-up enable.            */
  uint32_t TMR2OVF   : 1;                 /*!< TMR21_GCMA stop-mode wake-up enable.            */
  uint32_t USART1RX  : 1;                 /*!< TMR21_GCMA stop-mode wake-up enable.            */
  uint32_t USBHS     : 1;                 /*!< USBHS_WKUP stop-mode wake-up enable.            */
  uint32_t USBFS     : 1;                 /*!< USBFS_WKUP stop-mode wake-up enable.            */
  uint32_t ETHPMT    : 1;                 /*!< ETH_WKUP stop-mode wake-up enable.              */
  uint32_t Reserved  : 2;                 /*!< Reserved.                                        */
} Power_Ip_StopModeWKUSrcType;

/**
 * @brief  Power-down-mode Select Power_IP_PowerDownSelTypes.
 */
typedef enum {
  PWC_PD_MD1 = 0x00U,                   /*!< Power down mode 1.                              */
  PWC_PD_MD2 = 0x01U,                   /*!< Power down mode 2.                              */
  PWC_PD_MD3 = 0x02U,                   /*!< Power down mode 3.                              */
  PWC_PD_MD4 = 0x03U                    /*!< Power down mode 4.                              */
} Power_Ip_PowerDownSelTypes;

/**
 * @brief State IO config in Power-down mode Power_IP_PowerDownIOSelType.
 */
typedef enum {
  PWC_PD_IO_KEEP1 = 0x00U,              /*!< IO holds state.                                 */
  PWC_PD_IO_KEEP2 = PWC_PWRC0_IORTN_0,  /*!< IO remains in the state.                        */
  PWC_PD_IO_HIZ   = PWC_PWRC0_IORTN_1   /*!< The IO is high impedance.                       */
} Power_Ip_PowerDownIOSelType;

/**
 * @brief  Speed config in Power-down mode Pwd_Ip_VCAPSelType Wakeup.
 */
typedef enum {
  PWC_PD_VCAP_0P1UF   = 0x00U,          /*!< VCAP1/VCAP2 = 0.1uF x2 or 0.22uF x1.            */
  PWC_PD_VCAP_0P047UF = 0x01U           /*!< VCAP1/VCAP2 = 0.047uF x2 or 0.1uF x1.           */
} Power_Ip_VCAPSelType;

/**
 * @brief  Source Power-down and  Wake-up.
 */
typedef struct{
  uint32_t PWC_PD_WKUP_WKUP13 : 1;        /*!< WKUP13 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP12 : 1;        /*!< WKUP12 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP11 : 1;        /*!< WKUP11 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP10 : 1;        /*!< WKUP10 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP03 : 1;        /*!< WKUP03 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP02 : 1;        /*!< WKUP02 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP01 : 1;        /*!< WKUP01 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP00 : 1;        /*!< WKUP00 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP33 : 1;        /*!< WKUP33 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP32 : 1;        /*!< WKUP32 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP31 : 1;        /*!< WKUP31 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP30 : 1;        /*!< WKUP30 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP23 : 1;        /*!< WKUP23 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP22 : 1;        /*!< WKUP22 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP21 : 1;        /*!< WKUP21 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKUP20 : 1;        /*!< WKUP20 wake-up event enable.                    */
  uint32_t PWC_PD_WKUP_WKTM   : 1;        /*!< WKTM  wake-up event enable.                     */
  uint32_t Reserved1           : 1;        /*!< Reserved.                                        */
  uint32_t PWC_PD_WKUP_RTCALM : 1;        /*!< RTCALM  wake-up event enable.                   */
  uint32_t PWC_PD_WKUP_RTCPRD : 1;        /*!< RTCPRD wake-up event enable.                    */
  uint32_t Reserved2           : 1;        /*!< Reserved.                                        */
  uint32_t Reserved3           : 1;        /*!< Reserved.                                        */
  uint32_t PWC_PD_WKUP_PVD2   : 1;        /*!< PVD2 wake-up event enable.                      */
  uint32_t PWC_PD_WKUP_PVD1   : 1;        /*!< PVD1 wake-up event enable.                      */
  uint32_t Reserved            : 8;        /*!< Reserved.                                        */
} Power_Ip_WKUPWDEnableType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* POWER_IP_MODE_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
