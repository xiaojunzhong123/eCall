/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Types.h
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
 *   @file       Reset_Ip_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef RESET_IP_TYPES_H
#define RESET_IP_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "System_Hw_Reg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define CM4_AIRCR_BASEADDR              ((uint32_t)0xE000ED0CU)
#define CM4_AIRCR_SYSRESETREQ_MASK      (0x4U)
#define CM4_AIRCR_PRIGROUP_MASK         (0x700U)
#define CM4_AIRCR_VECTKEY_MASK          (0xFFFF0000U)
#define CM4_AIRCR_VECTKEY_SHIFT         (16U)
#define CM4_AIRCR_VECTKEY(x)            (((uint32_t)(((uint32_t)(x))<<CM4_AIRCR_VECTKEY_SHIFT))&CM4_AIRCR_VECTKEY_MASK)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Represent IP Reset operation return codes.
 */
typedef enum
{
    RESET_IP_STATUS_SUCCESS = 0U, /*!< Generic operation success */
    RESET_IP_STATUS_ERROR   = 1U, /*!< Generic operation failure */
    RESET_IP_STATUS_TIMEOUT = 2U  /*!< Generic operation timeout */
} Reset_Ip_StatusType;
 
 /**
 * @brief System type Control.
 */
typedef struct {
  volatile uint32_t AIRCR;                                        /*!< Application Interrupt and Reset Control Register , offset: 0xC.          */
  volatile uint32_t SCR;                                          /*!< System Control Register, offset: 0x0.                                    */
} Reset_Ip_CM4ResetType;

/**
 * @brief  Mcu Reset define reset type.
 */
typedef enum 
{
  RESET_IP_PO_RESET           = 0U,                                    /*!< Power on reset.                                                        */
  RESET_IP_PIN_RESET          = 1U,                                    /*!< NRST pin input low level.                                              */
  RESET_IP_BO_RESET           = 2U,                                    /*!< VCC voltage drop below VBOR voltage.                                   */
  RESET_IP_PVD1_RESET         = 3U,                                    /*!< VCC voltage drop below PVD1 voltage.                                   */
  RESET_IP_PVD2_RESET         = 4U,                                    /*!< VCC voltage drop below PVD2 voltage.                                   */
  RESET_IP_WD_RESET           = 5U,                                    /*!< Refresh error or overflow error occurs in watchdog.                    */
  RESET_IP_SWD_RESET          = 6U,                                    /*!< Refresh error or overflow error occurred in the dedicated watchdog.    */
  RESET_IP_PD_RESET           = 7U,                                    /*!< Power-down wake up events occurs when devices is in power-down mode.   */
  RESET_IP_SW_RESET           = 8U,                                    /*!< Set the reset register.                                                */
  RESET_IP_MPUE_RESET         = 9U,                                    /*!< Reset generate by MPU access error.                                    */
  RESET_IP_RAPE_RESET         = 10U,                                   /*!< Parity error occurs in the RAM.                                        */
  RESET_IP_RAEC_RESET         = 11U,                                   /*!< ECC error occurs in the RAM.                                           */
  RESET_IP_CKFE_RESET         = 12U,                                   /*!< Clock frequency abnormal detected by the FCM.                          */
  RESET_IP_XTALE_RESET        = 13U,                                   /*!< External high-speed oscillator oscillation failure is detected.        */
  RESET_IP_LKUP_RESET         = 14U,                                   /*!< When the cortex M4 encounters a serious exception.                     */
  RESET_IP_FLEC_RESET         = 15U,                                   /*!< ECC error occurs in the Flash.                                         */
  RESET_IP_ERMU_RESET         = 16U,                                   /*!< ERMU reset.                                                            */
  RESET_IP_RESET_MULTIPLE     = 32U,                                   /*!< Multiple reset reason.                                                 */
  RESET_IP_RESET_UNDEFINED    = 33U                                    /*!< Reset reason is undefine.                                              */
} Reset_Ip_ResetReasonType;

/**
 *  @brief   Represent different software module source.
 *  @details SRS ID: RESET_IP_000_005
 */
typedef enum
{
  RESET_IP_SW_MODULE_RESET_ERMU    = 8U,   /*!< Software module reset Ermu */
  RESET_IP_SW_MODULE_RESET_PKE     = 12U,  /*!< Software module reset Pke */
  RESET_IP_SW_MODULE_RESET_KEY = 13U,  /*!< Software module reset KEY */
  RESET_IP_SW_MODULE_RESET_DMA1    = 14U,  /*!< Software module reset Dma1 */
  RESET_IP_SW_MODULE_RESET_DMA2    = 15U,  /*!< Software module reset Dma2 */
  RESET_IP_SW_MODULE_RESET_FCM     = 16U,  /*!< Software module reset Fcm */
  RESET_IP_SW_MODULE_RESET_AOS     = 17U,  /*!< Software module reset Aos */
  RESET_IP_SW_MODULE_RESET_CTC     = 18U,  /*!< Software module reset Ctc */
  RESET_IP_SW_MODULE_RESET_MAU     = 19U,  /*!< Software module reset Mau */
  RESET_IP_SW_MODULE_RESET_SKE     = 20U,  /*!< Software module reset Ske */
  RESET_IP_SW_MODULE_RESET_HASH    = 21U,  /*!< Software module reset Hash */
  RESET_IP_SW_MODULE_RESET_TRNG    = 22U,  /*!< Software module reset Trng */
  RESET_IP_SW_MODULE_RESET_CRC     = 23U,  /*!< Software module reset Crc */
  RESET_IP_SW_MODULE_RESET_DCU1    = 24U,  /*!< Software module reset Dcu1 */
  RESET_IP_SW_MODULE_RESET_DCU2    = 25U,  /*!< Software module reset Dcu2 */
  RESET_IP_SW_MODULE_RESET_DCU3    = 26U,  /*!< Software module reset Dcu3 */
  RESET_IP_SW_MODULE_RESET_DCU4    = 27U,  /*!< Software module reset Dcu4 */
  RESET_IP_SW_MODULE_RESET_DCU5    = 28U,  /*!< Software module reset Dcu5 */
  RESET_IP_SW_MODULE_RESET_DCU6    = 29U,  /*!< Software module reset Dcu6 */
  RESET_IP_SW_MODULE_RESET_DCU7    = 30U,  /*!< Software module reset Dcu7 */
  RESET_IP_SW_MODULE_RESET_DCU8    = 31U,  /*!< Software module reset Dcu8 */
  RESET_IP_SW_MODULE_RESET_CAN1    = 32U,  /*!< Software module reset Can1 */
  RESET_IP_SW_MODULE_RESET_CAN2    = 33U,  /*!< Software module reset Can2 */
  RESET_IP_SW_MODULE_RESET_ETHMAC  = 34U,  /*!< Software module reset Ethmac */
  RESET_IP_SW_MODULE_RESET_QSPI    = 35U,  /*!< Software module reset Qspi */
  RESET_IP_SW_MODULE_RESET_I2C1    = 36U,  /*!< Software module reset I2c1 */
  RESET_IP_SW_MODULE_RESET_I2C2    = 37U,  /*!< Software module reset I2c2 */
  RESET_IP_SW_MODULE_RESET_I2C3    = 38U,  /*!< Software module reset I2c3 */
  RESET_IP_SW_MODULE_RESET_I2C4    = 39U,  /*!< Software module reset I2c4 */
  RESET_IP_SW_MODULE_RESET_I2C5    = 40U,  /*!< Software module reset I2c5 */
  RESET_IP_SW_MODULE_RESET_I2C6    = 41U,  /*!< Software module reset I2c6 */
  RESET_IP_SW_MODULE_RESET_SDIOC1  = 42U,  /*!< Software module reset Sdio1 */
  RESET_IP_SW_MODULE_RESET_SDIOC2  = 43U,  /*!< Software module reset Sdio2 */
  RESET_IP_SW_MODULE_RESET_I2S1    = 44U,  /*!< Software module reset I2s1 */
  RESET_IP_SW_MODULE_RESET_I2S2    = 45U,  /*!< Software module reset I2s2 */
  RESET_IP_SW_MODULE_RESET_I2S3    = 46U,  /*!< Software module reset I2s3 */
  RESET_IP_SW_MODULE_RESET_I2S4    = 47U,  /*!< Software module reset I2s4 */
  RESET_IP_SW_MODULE_RESET_SPI1    = 48U,  /*!< Software module reset Spi1 */
  RESET_IP_SW_MODULE_RESET_SPI2    = 49U,  /*!< Software module reset Spi2 */
  RESET_IP_SW_MODULE_RESET_SPI3    = 50U,  /*!< Software module reset Spi3 */
  RESET_IP_SW_MODULE_RESET_SPI4    = 51U,  /*!< Software module reset Spi4 */
  RESET_IP_SW_MODULE_RESET_SPI5    = 52U,  /*!< Software module reset Spi5 */
  RESET_IP_SW_MODULE_RESET_SPI6    = 53U,  /*!< Software module reset Spi6 */
  RESET_IP_SW_MODULE_RESET_USBFS   = 54U,  /*!< Software module reset Usbfs */
  RESET_IP_SW_MODULE_RESET_USBHS   = 55U,  /*!< Software module reset Usbhs */
  RESET_IP_SW_MODULE_RESET_FMAC1   = 56U,  /*!< Software module reset Fmac1 */
  RESET_IP_SW_MODULE_RESET_FMAC2   = 57U,  /*!< Software module reset Fmac2 */
  RESET_IP_SW_MODULE_RESET_FMAC3   = 58U,  /*!< Software module reset Fmac3 */
  RESET_IP_SW_MODULE_RESET_FMAC4   = 59U,  /*!< Software module reset Fmac4 */
  RESET_IP_SW_MODULE_RESET_MCAN1   = 60U,  /*!< Software module reset Mcan1 */
  RESET_IP_SW_MODULE_RESET_MCAN2   = 61U,  /*!< Software module reset Mcan2 */
  RESET_IP_SW_MODULE_RESET_TMR6    = 64U,  /*!< Software module reset Tmr6 */
  RESET_IP_SW_MODULE_RESET_TMR4_1  = 72U,  /*!< Software module reset Tmr4_1 */
  RESET_IP_SW_MODULE_RESET_TMR4_2  = 73U,  /*!< Software module reset Tmr4_2 */
  RESET_IP_SW_MODULE_RESET_TMR4_3  = 74U,  /*!< Software module reset Tmr4_3 */
  RESET_IP_SW_MODULE_RESET_TMR0_1  = 76U,  /*!< Software module reset Tmr0_1 */
  RESET_IP_SW_MODULE_RESET_TMR0_2  = 77U,  /*!< Software module reset Tmr0_2 */
  RESET_IP_SW_MODULE_RESET_TMR0_3  = 78U,  /*!< Software module reset Tmr0_3 */
  RESET_IP_SW_MODULE_RESET_EMB     = 79U,  /*!< Software module reset Emb */
  RESET_IP_SW_MODULE_RESET_TMR2_1  = 80U,  /*!< Software module reset Tmr2_1 */
  RESET_IP_SW_MODULE_RESET_TMR2_2  = 81U,  /*!< Software module reset Tmr2_2 */
  RESET_IP_SW_MODULE_RESET_TMR2_3  = 82U,  /*!< Software module reset Tmr2_3 */
  RESET_IP_SW_MODULE_RESET_TMR2_4  = 83U,  /*!< Software module reset Tmr2_4 */
  RESET_IP_SW_MODULE_RESET_TMRA_1  = 84U,  /*!< Software module reset TMRA_1 */
  RESET_IP_SW_MODULE_RESET_TMRA_2  = 85U,  /*!< Software module reset TMRA_2 */
  RESET_IP_SW_MODULE_RESET_TMRA_3  = 86U,  /*!< Software module reset TMRA_3 */
  RESET_IP_SW_MODULE_RESET_TMRA_4  = 87U,  /*!< Software module reset TMRA_4 */
  RESET_IP_SW_MODULE_RESET_TMRA_5  = 88U,  /*!< Software module reset TMRA_5 */
  RESET_IP_SW_MODULE_RESET_TMRA_6  = 89U,  /*!< Software module reset TMRA_6 */
  RESET_IP_SW_MODULE_RESET_TMRA_7  = 90U,  /*!< Software module reset TMRA_7 */
  RESET_IP_SW_MODULE_RESET_TMRA_8  = 91U,  /*!< Software module reset TMRA_8 */
  RESET_IP_SW_MODULE_RESET_TMRA_9  = 92U,  /*!< Software module reset TMRA_9 */
  RESET_IP_SW_MODULE_RESET_TMRA_10 = 93U,  /*!< Software module reset TMRA_10 */
  RESET_IP_SW_MODULE_RESET_TMRA_11 = 94U,  /*!< Software module reset TMRA_11 */
  RESET_IP_SW_MODULE_RESET_TMRA_12 = 95U,  /*!< Software module reset TMRA_12 */
  RESET_IP_SW_MODULE_RESET_ADC1    = 96U,   /*!< Software module reset Adc1 */
  RESET_IP_SW_MODULE_RESET_ADC2    = 97U,   /*!< Software module reset Adc2 */
  RESET_IP_SW_MODULE_RESET_ADC3    = 98U,   /*!< Software module reset Adc3 */
  RESET_IP_SW_MODULE_RESET_DAC1    = 100U,  /*!< Software module reset Dac1 */
  RESET_IP_SW_MODULE_RESET_DAC2    = 101U,  /*!< Software module reset Dac2 */
  RESET_IP_SW_MODULE_RESET_CMP12   = 104U,  /*!< Software module reset Cmp12 */
  RESET_IP_SW_MODULE_RESET_CMP34   = 105U,  /*!< Software module reset Cmp34 */
  RESET_IP_SW_MODULE_RESET_TMR0_4  = 106U,  /*!< Software module reset Tmrg4 */
  RESET_IP_SW_MODULE_RESET_TMR0_5  = 107U,  /*!< Software module reset Tmrg5 */
  RESET_IP_SW_MODULE_RESET_OTS     = 108U,  /*!< Software module reset Ots */
  RESET_IP_SW_MODULE_RESET_DVP     = 111U,  /*!< Software module reset Dvp */
  RESET_IP_SW_MODULE_RESET_SMC     = 112U,  /*!< Software module reset Smc */
  RESET_IP_SW_MODULE_RESET_ORC     = 113U,  /*!< Software module reset Orc */
  RESET_IP_SW_MODULE_RESET_NFC     = 114U,  /*!< Software module reset Nfc */
  RESET_IP_SW_MODULE_RESET_USART1  = 116U,  /*!< Software module reset Usart1 */
  RESET_IP_SW_MODULE_RESET_USART2  = 117U,  /*!< Software module reset Usart2 */
  RESET_IP_SW_MODULE_RESET_USART3  = 118U,  /*!< Software module reset Usart3 */
  RESET_IP_SW_MODULE_RESET_USART4  = 119U,  /*!< Software module reset Usart4 */
  RESET_IP_SW_MODULE_RESET_USART5  = 120U,  /*!< Software module reset Usart5 */
  RESET_IP_SW_MODULE_RESET_USART6  = 121U,  /*!< Software module reset Usart6 */
  RESET_IP_SW_MODULE_RESET_USART7  = 122U,  /*!< Software module reset Usart7 */
  RESET_IP_SW_MODULE_RESET_USART8  = 123U,  /*!< Software module reset Usart8 */
  RESET_IP_SW_MODULE_RESET_USART9  = 124U,  /*!< Software module reset Usart9 */
  RESET_IP_SW_MODULE_RESET_USART10 = 125U,  /*!< Software module reset Usart10 */
} Reset_Ip_SoftwareModuleResetSourceControlType;

/**
 *  @brief   Contain software module reset configuration.
 *  @details SRS ID: RESET_IP_000_006
 */
typedef struct
{
  uint32_t SwModuleReset0_u32; /*!< Software module reset 0 */
  uint32_t SwModuleReset1_u32; /*!< Software module reset 1 */
  uint32_t SwModuleReset2_u32; /*!< Software module reset 2 */
  uint32_t SwModuleReset3_u32; /*!< Software module reset 3 */
} Reset_Ip_SoftwareModuleResetConfigType;


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

#endif /* RESET_IP_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
