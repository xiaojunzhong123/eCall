/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_Ipr_Types.h
 *
 * Description: Implementation of Mcu IP Router Layer Driver
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
 *   @file       Mcu_Ipr_Types.h
 *   @version    1.0.3
 *   @brief      Mcu IP Router Layer Driver
 *   @addtogroup mcu_ipr
 *   @{
 */

#ifndef MCU_IPR_TYPES_H
#define MCU_IPR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Reset_Ip_Types.h"
#include "Mcu_Cfg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Represent different peripheral for software module reset.
 */
typedef enum
{
    MCU_PERIPH_ERMU    = 8U,   /*!< Software module reset Ermu */
    MCU_PERIPH_PKE     = 12U,  /*!< Software module reset Pke */
    MCU_PERIPH_KEY = 13U,  /*!< Software module reset KEY */
    MCU_PERIPH_DMA1    = 14U,  /*!< Software module reset Dma1 */
    MCU_PERIPH_DMA2    = 15U,  /*!< Software module reset Dma2 */
    MCU_PERIPH_FCM     = 16U,  /*!< Software module reset Fcm */
    MCU_PERIPH_AOS     = 17U,  /*!< Software module reset Aos */
    MCU_PERIPH_CTC     = 18U,  /*!< Software module reset Ctc */
    MCU_PERIPH_MAU     = 19U,  /*!< Software module reset Mau */
    MCU_PERIPH_SKE     = 20U,  /*!< Software module reset Ske */
    MCU_PERIPH_HASH    = 21U,  /*!< Software module reset Hash */
    MCU_PERIPH_TRNG    = 22U,  /*!< Software module reset Trng */
    MCU_PERIPH_CRC     = 23U,  /*!< Software module reset Crc */
    MCU_PERIPH_DCU1    = 24U,  /*!< Software module reset Dcu1 */
    MCU_PERIPH_DCU2    = 25U,  /*!< Software module reset Dcu2 */
    MCU_PERIPH_DCU3    = 26U,  /*!< Software module reset Dcu3 */
    MCU_PERIPH_DCU4    = 27U,  /*!< Software module reset Dcu4 */
    MCU_PERIPH_DCU5    = 28U,  /*!< Software module reset Dcu5 */
    MCU_PERIPH_DCU6    = 29U,  /*!< Software module reset Dcu6 */
    MCU_PERIPH_DCU7    = 30U,  /*!< Software module reset Dcu7 */
    MCU_PERIPH_DCU8    = 31U,  /*!< Software module reset Dcu8 */
    MCU_PERIPH_CAN1    = 32U,  /*!< Software module reset Can1 */
    MCU_PERIPH_CAN2    = 33U,  /*!< Software module reset Can2 */
    MCU_PERIPH_ETHMAC  = 34U,  /*!< Software module reset Ethmac */
    MCU_PERIPH_QSPI    = 35U,  /*!< Software module reset Qspi */
    MCU_PERIPH_I2C1    = 36U,  /*!< Software module reset I2c1 */
    MCU_PERIPH_I2C2    = 37U,  /*!< Software module reset I2c2 */
    MCU_PERIPH_I2C3    = 38U,  /*!< Software module reset I2c3 */
    MCU_PERIPH_I2C4    = 39U,  /*!< Software module reset I2c4 */
    MCU_PERIPH_I2C5    = 40U,  /*!< Software module reset I2c5 */
    MCU_PERIPH_I2C6    = 41U,  /*!< Software module reset I2c6 */
    MCU_PERIPH_SDIOC1  = 42U,  /*!< Software module reset Sdio1 */
    MCU_PERIPH_SDIOC2  = 43U,  /*!< Software module reset Sdio2 */
    MCU_PERIPH_I2S1    = 44U,  /*!< Software module reset I2s1 */
    MCU_PERIPH_I2S2    = 45U,  /*!< Software module reset I2s2 */
    MCU_PERIPH_I2S3    = 46U,  /*!< Software module reset I2s3 */
    MCU_PERIPH_I2S4    = 47U,  /*!< Software module reset I2s4 */
    MCU_PERIPH_SPI1    = 48U,  /*!< Software module reset Spi1 */
    MCU_PERIPH_SPI2    = 49U,  /*!< Software module reset Spi2 */
    MCU_PERIPH_SPI3    = 50U,  /*!< Software module reset Spi3 */
    MCU_PERIPH_SPI4    = 51U,  /*!< Software module reset Spi4 */
    MCU_PERIPH_SPI5    = 52U,  /*!< Software module reset Spi5 */
    MCU_PERIPH_SPI6    = 53U,  /*!< Software module reset Spi6 */
    MCU_PERIPH_USBFS   = 54U,  /*!< Software module reset Usbfs */
    MCU_PERIPH_USBHS   = 55U,  /*!< Software module reset Usbhs */
    MCU_PERIPH_FMAC1   = 56U,  /*!< Software module reset Fmac1 */
    MCU_PERIPH_FMAC2   = 57U,  /*!< Software module reset Fmac2 */
    MCU_PERIPH_FMAC3   = 58U,  /*!< Software module reset Fmac3 */
    MCU_PERIPH_FMAC4   = 59U,  /*!< Software module reset Fmac4 */
    MCU_PERIPH_MCAN1   = 60U,  /*!< Software module reset Mcan1 */
    MCU_PERIPH_MCAN2   = 61U,  /*!< Software module reset Mcan2 */
    MCU_PERIPH_TMR6    = 64U,  /*!< Software module reset Tmr6 */
    MCU_PERIPH_TMR4_1  = 72U,  /*!< Software module reset Tmr4_1 */
    MCU_PERIPH_TMR4_2  = 73U,  /*!< Software module reset Tmr4_2 */
    MCU_PERIPH_TMR4_3  = 74U,  /*!< Software module reset Tmr4_3 */
    MCU_PERIPH_TMR0_1  = 76U,  /*!< Software module reset Tmr0_1 */
    MCU_PERIPH_TMR0_2  = 77U,  /*!< Software module reset Tmr0_2 */
    MCU_PERIPH_TMR0_3  = 78U,  /*!< Software module reset Tmr0_3 */
    MCU_PERIPH_EMB     = 79U,  /*!< Software module reset Emb */
    MCU_PERIPH_TMR2_1  = 80U,  /*!< Software module reset Tmr2_1 */
    MCU_PERIPH_TMR2_2  = 81U,  /*!< Software module reset Tmr2_2 */
    MCU_PERIPH_TMR2_3  = 82U,  /*!< Software module reset Tmr2_3 */
    MCU_PERIPH_TMR2_4  = 83U,  /*!< Software module reset Tmr2_4 */
    MCU_PERIPH_TMRA_1  = 84U,  /*!< Software module reset TMRA_1 */
    MCU_PERIPH_TMRA_2  = 85U,  /*!< Software module reset TMRA_2 */
    MCU_PERIPH_TMRA_3  = 86U,  /*!< Software module reset TMRA_3 */
    MCU_PERIPH_TMRA_4  = 87U,  /*!< Software module reset TMRA_4 */
    MCU_PERIPH_TMRA_5  = 88U,  /*!< Software module reset TMRA_5 */
    MCU_PERIPH_TMRA_6  = 89U,  /*!< Software module reset TMRA_6 */
    MCU_PERIPH_TMRA_7  = 90U,  /*!< Software module reset TMRA_7 */
    MCU_PERIPH_TMRA_8  = 91U,  /*!< Software module reset TMRA_8 */
    MCU_PERIPH_TMRA_9  = 92U,  /*!< Software module reset TMRA_9 */
    MCU_PERIPH_TMRA_10 = 93U,  /*!< Software module reset TMRA_10 */
    MCU_PERIPH_TMRA_11 = 94U,  /*!< Software module reset TMRA_11 */
    MCU_PERIPH_TMRA_12 = 95U,  /*!< Software module reset TMRA_12 */
    MCU_PERIPH_ADC1    = 96U,   /*!< Software module reset Adc1 */
    MCU_PERIPH_ADC2    = 97U,   /*!< Software module reset Adc2 */
    MCU_PERIPH_ADC3    = 98U,   /*!< Software module reset Adc3 */
    MCU_PERIPH_DAC1    = 100U,  /*!< Software module reset Dac1 */
    MCU_PERIPH_DAC2    = 101U,  /*!< Software module reset Dac2 */
    MCU_PERIPH_CMP12   = 104U,  /*!< Software module reset Cmp12 */
    MCU_PERIPH_CMP34   = 105U,  /*!< Software module reset Cmp34 */
    MCU_PERIPH_TMR0_4  = 106U,  /*!< Software module reset Tmrg4 */
    MCU_PERIPH_TMR0_5  = 107U,  /*!< Software module reset Tmrg5 */
    MCU_PERIPH_OTS     = 108U,  /*!< Software module reset Ots */
    MCU_PERIPH_DVP     = 111U,  /*!< Software module reset Dvp */
    MCU_PERIPH_SMC     = 112U,  /*!< Software module reset Smc */
    MCU_PERIPH_ORC     = 113U,  /*!< Software module reset Orc */
    MCU_PERIPH_NFC     = 114U,  /*!< Software module reset Nfc */
    MCU_PERIPH_USART1  = 116U,  /*!< Software module reset Usart1 */
    MCU_PERIPH_USART2  = 117U,  /*!< Software module reset Usart2 */
    MCU_PERIPH_USART3  = 118U,  /*!< Software module reset Usart3 */
    MCU_PERIPH_USART4  = 119U,  /*!< Software module reset Usart4 */
    MCU_PERIPH_USART5  = 120U,  /*!< Software module reset Usart5 */
    MCU_PERIPH_USART6  = 121U,  /*!< Software module reset Usart6 */
    MCU_PERIPH_USART7  = 122U,  /*!< Software module reset Usart7 */
    MCU_PERIPH_USART8  = 123U,  /*!< Software module reset Usart8 */
    MCU_PERIPH_USART9  = 124U,  /*!< Software module reset Usart9 */
    MCU_PERIPH_USART10 = 125U,  /*!< Software module reset Usart10 */
} Mcu_Ipr_ModuleType;

/**
 *  @brief   Represent reset types.
 */
typedef enum
{
    MCU_PO_RESET            = McuConf_McuResetReasonConf_MCU_PO_RESET,            /*!< Power on reset */
    MCU_PIN_RESET           = McuConf_McuResetReasonConf_MCU_PIN_RESET,           /*!< NRST pin input low level reset */
    MCU_BO_RESET            = McuConf_McuResetReasonConf_MCU_BO_RESET,            /*!< Brown out reset */
    MCU_PVD1_RESET          = McuConf_McuResetReasonConf_MCU_PVD1_RESET,          /*!< PVD1 reset */
    MCU_PVD2_RESET          = McuConf_McuResetReasonConf_MCU_PVD2_RESET,          /*!< PVD2 reset */
    MCU_WD_RESET            = McuConf_McuResetReasonConf_MCU_WD_RESET,            /*!< Watchdog reset */
    MCU_SWD_RESET           = McuConf_McuResetReasonConf_MCU_SWD_RESET,           /*!< Dedicated watchdog reset */
    MCU_PD_RESET            = McuConf_McuResetReasonConf_MCU_PD_RESET,            /*!< Power-down wake up events occurs when devices is in power-down mode */
    MCU_SW_RESET            = McuConf_McuResetReasonConf_MCU_SW_RESET,            /*!< Set the reset register */
    MCU_MPUE_RESET          = McuConf_McuResetReasonConf_MCU_MPUE_RESET,          /*!< Reset generated by MPU access error */
    MCU_RAPE_RESET          = McuConf_McuResetReasonConf_MCU_RAPE_RESET,          /*!< Parity error occurs in the RAM */
    MCU_RAEC_RESET          = McuConf_McuResetReasonConf_MCU_RAEC_RESET,          /*!< ECC error occurs in the RAM */
    MCU_CKFE_RESET          = McuConf_McuResetReasonConf_MCU_CKFE_RESET,          /*!< Clock frequency abnormal detected by the FCM */
    MCU_XTALE_RESET         = McuConf_McuResetReasonConf_MCU_XTALE_RESET,         /*!< External high-speed oscillator oscillation failure is detected */
    MCU_LKUP_RESET          = McuConf_McuResetReasonConf_MCU_LKUP_RESET,          /*!< When the cortex M4 encounters a serious exception */
    MCU_FLEC_RESET          = McuConf_McuResetReasonConf_MCU_FLEC_RESET,          /*!< ECC error occurs in the Flash */
    MCU_ERMU_RESET          = McuConf_McuResetReasonConf_MCU_ERMU_RESET,          /*!< ERMU reset */
    MCU_MULTIPLE_RESET      = McuConf_McuResetReasonConf_MCU_MULTIPLE_RESET,      /*!< More than 1 reset events are logged sans "Power on event" */
    MCU_RESET_UNDEFINED     = McuConf_McuResetReasonConf_MCU_RESET_UNDEFINED      /*!< Reset is undefined */
} Mcu_Ipr_ResetType;

/**
 * @brief Represent WKT Clock Source
 */
typedef enum
{
    MCU_WKT_CLK_SRC_64HZ            = 0x00U,      /*!< 64Hz Clock   */
    MCU_WKT_CLK_SRC_XTAL32          = 0x01U,      /*!< XTAL32 Clock */
    MCU_WKT_CLK_SRC_RTCLRC          = 0x02U,      /*!< RTCLRC Clock */
} Mcu_Ipr_WktClockSourceType;


/***********************************************************************************************************************
 *  CONSTANTS
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

#endif /*!< MCU_IPR_TYPES_H */
