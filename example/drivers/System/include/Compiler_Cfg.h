/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Compiler_Cfg.h
 *
 * Description: Implementation of COMPILER CFG High Level layer
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
#ifndef COMPILER_CFG_H
#define COMPILER_CFG_H

/**
 *   @file       Compiler_Cfg.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define COMPILER_CFG_AR_RELEASE_MAJOR_VERSION        4
#define COMPILER_CFG_AR_RELEASE_MINOR_VERSION        4
#define COMPILER_CFG_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief Define compiler used.
 */
#if defined __ghs__
  #define _GREENHILLS_C_HC32A4XX_
#endif
#if defined __GNUC__
  #define _GCC_C_HC32A4XX_
#endif
#if defined __ICCARM__
  #define _IAR_C_HC32A4XX_
#endif

/**
 * @brief RTE memory and pointer classes.
 */
#define RTE_CODE
/**@}*/

/**
 * @brief ADC memory and pointer classes.
 */
#define ADC_CODE
#define ADC_CALLOUT_CODE
#define ADC_CODE_FAST
#define ADC_CODE_SLOW
#define ADC_CONST
#define ADC_CALIB
#define ADC_CONFIG_DATA
#define ADC_CONST_SAVED_RECOVERY_ZONE8
#define ADC_CONST_SAVED_RECOVERY_ZONE16
#define ADC_CONST_SAVED_RECOVERY_ZONE32
#define ADC_APPL_DATA
#define ADC_APPL_CONST
#define REGSPACE
#define ADC_VAR_NOINIT
#define ADC_VAR_CLEARED
#define ADC_VAR_POWER_ON_CLEARED
#define ADC_VAR_INIT
#define ADC_VAR_POWER_ON_INIT
#define ADC_VAR_FAST_NOINIT
#define ADC_VAR_FAST_CLEARED
#define ADC_VAR_FAST_POWER_ON_CLEARED
#define ADC_VAR_FAST_INIT
#define ADC_VAR_FAST_POWER_ON_INIT
#define ADC_VAR_SLOW_NOINIT
#define ADC_VAR_SLOW_CLEARED
#define ADC_VAR_SLOW_POWER_ON_CLEARED
#define ADC_VAR_SLOW_INIT
#define ADC_VAR_SLOW_POWER_ON_INIT
#define ADC_INTERNAL_VAR_NOINIT
#define ADC_INTERNAL_VAR_CLEARED
#define ADC_INTERNAL_VAR_POWER_ON_CLEARED
#define ADC_INTERNAL_VAR_INIT
#define ADC_INTERNAL_VAR_POWER_ON_INIT
#define ADC_VAR_SAVED_ZONE8
#define ADC_VAR_SAVED_ZONE16
#define ADC_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief CAN memory and pointer classes.
 */
#define CAN_CODE
#define CAN_CALLOUT_CODE
#define CAN_CODE_FAST
#define CAN_CODE_SLOW
#define CAN_CONST
#define CAN_CALIB
#define CAN_CONFIG_DATA
#define CAN_CONST_SAVED_RECOVERY_ZONE8
#define CAN_CONST_SAVED_RECOVERY_ZONE16
#define CAN_CONST_SAVED_RECOVERY_ZONE32
#define CAN_APPL_DATA
#define CAN_APPL_CONST
#define CANSPACE
#define CAN_VAR_NOINIT
#define CAN_VAR_CLEARED
#define CAN_VAR_POWER_ON_CLEARED
#define CAN_VAR_INIT
#define CAN_VAR_POWER_ON_INIT
#define CAN_VAR_FAST_NOINIT
#define CAN_VAR_FAST_CLEARED
#define CAN_VAR_FAST_POWER_ON_CLEARED
#define CAN_VAR_FAST_INIT
#define CAN_VAR_FAST_POWER_ON_INIT
#define CAN_VAR_SLOW_NOINIT
#define CAN_VAR_SLOW_CLEARED
#define CAN_VAR_SLOW_POWER_ON_CLEARED
#define CAN_VAR_SLOW_INIT
#define CAN_VAR_SLOW_POWER_ON_INIT
#define CAN_INTERNAL_VAR_NOINIT
#define CAN_INTERNAL_VAR_CLEARED
#define CAN_INTERNAL_VAR_POWER_ON_CLEARED
#define CAN_INTERNAL_VAR_INIT
#define CAN_INTERNAL_VAR_POWER_ON_INIT
#define CAN_VAR_SAVED_ZONE8
#define CAN_VAR_SAVED_ZONE16
#define CAN_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief CANIF memory and pointer classes.
 */

#define CANIF_CODE
#define CANIF_CALLOUT_CODE
#define CANIF_CODE_FAST
#define CANIF_CODE_SLOW
#define CANIF_CONST
#define CANIF_CALIB
#define CANIF_CONFIG_DATA
#define CANIF_CONST_SAVED_RECOVERY_ZONE8
#define CANIF_CONST_SAVED_RECOVERY_ZONE16
#define CANIF_CONST_SAVED_RECOVERY_ZONE32
#define CANIF_APPL_DATA
#define CANIF_APPL_CONST
#define CANIFSPACE
#define CANIF_VAR_NOINIT
#define CANIF_VAR_CLEARED
#define CANIF_VAR_POWER_ON_CLEARED
#define CANIF_VAR_INIT
#define CANIF_VAR_POWER_ON_INIT
#define CANIF_VAR_FAST_NOINIT
#define CANIF_VAR_FAST_CLEARED
#define CANIF_VAR_FAST_POWER_ON_CLEARED
#define CANIF_VAR_FAST_INIT
#define CANIF_VAR_FAST_POWER_ON_INIT
#define CANIF_VAR_SLOW_NOINIT
#define CANIF_VAR_SLOW_CLEARED
#define CANIF_VAR_SLOW_POWER_ON_CLEARED
#define CANIF_VAR_SLOW_INIT
#define CANIF_VAR_SLOW_POWER_ON_INIT
#define CANIF_INTERNAL_VAR_NOINIT
#define CANIF_INTERNAL_VAR_CLEARED
#define CANIF_INTERNAL_VAR_POWER_ON_CLEARED
#define CANIF_INTERNAL_VAR_INIT
#define CANIF_INTERNAL_VAR_POWER_ON_INIT
#define CANIF_VAR_SAVED_ZONE8
#define CANIF_VAR_SAVED_ZONE16
#define CANIF_VAR_SAVED_ZONE32
/**@}*/


/**
 * @brief DEM memory and pointer classes.
 */

#define DEM_CODE
#define DEM_CALLOUT_CODE
#define DEM_CODE_FAST
#define DEM_CODE_SLOW
#define DEM_CONST
#define DEM_CALIB
#define DEM_CONFIG_DATA
#define DEM_CONST_SAVED_RECOVERY_ZONE8
#define DEM_CONST_SAVED_RECOVERY_ZONE16
#define DEM_CONST_SAVED_RECOVERY_ZONE32
#define DEM_APPL_DATA
#define DEM_APPL_CONST
#define DEMSPACE
#define DEM_VAR_NOINIT
#define DEM_VAR_CLEARED
#define DEM_VAR_POWER_ON_CLEARED
#define DEM_VAR_INIT
#define DEM_VAR_POWER_ON_INIT
#define DEM_VAR_FAST_NOINIT
#define DEM_VAR_FAST_CLEARED
#define DEM_VAR_FAST_POWER_ON_CLEARED
#define DEM_VAR_FAST_INIT
#define DEM_VAR_FAST_POWER_ON_INIT
#define DEM_VAR_SLOW_NOINIT
#define DEM_VAR_SLOW_CLEARED
#define DEM_VAR_SLOW_POWER_ON_CLEARED
#define DEM_VAR_SLOW_INIT
#define DEM_VAR_SLOW_POWER_ON_INIT
#define DEM_INTERNAL_VAR_NOINIT
#define DEM_INTERNAL_VAR_CLEARED
#define DEM_INTERNAL_VAR_POWER_ON_CLEARED
#define DEM_INTERNAL_VAR_INIT
#define DEM_INTERNAL_VAR_POWER_ON_INIT
#define DEM_VAR_SAVED_ZONE8
#define DEM_VAR_SAVED_ZONE16
#define DEM_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief DET memory and pointer classes.
 */

#define DET_CODE
#define DET_CALLOUT_CODE
#define DET_CODE_FAST
#define DET_CODE_SLOW
#define DET_CONST
#define DET_CALIB
#define DET_CONFIG_DATA
#define DET_CONST_SAVED_RECOVERY_ZONE8
#define DET_CONST_SAVED_RECOVERY_ZONE16
#define DET_CONST_SAVED_RECOVERY_ZONE32
#define DET_APPL_DATA
#define DET_APPL_CONST
#define DETSPACE
#define DET_VAR_NOINIT
#define DET_VAR_CLEARED
#define DET_VAR_POWER_ON_CLEARED
#define DET_VAR_INIT
#define DET_VAR_POWER_ON_INIT
#define DET_VAR_FAST_NOINIT
#define DET_VAR_FAST_CLEARED
#define DET_VAR_FAST_POWER_ON_CLEARED
#define DET_VAR_FAST_INIT
#define DET_VAR_FAST_POWER_ON_INIT
#define DET_VAR_SLOW_NOINIT
#define DET_VAR_SLOW_CLEARED
#define DET_VAR_SLOW_POWER_ON_CLEARED
#define DET_VAR_SLOW_INIT
#define DET_VAR_SLOW_POWER_ON_INIT
#define DET_INTERNAL_VAR_NOINIT
#define DET_INTERNAL_VAR_CLEARED
#define DET_INTERNAL_VAR_POWER_ON_CLEARED
#define DET_INTERNAL_VAR_INIT
#define DET_INTERNAL_VAR_POWER_ON_INIT
#define DET_VAR_SAVED_ZONE8
#define DET_VAR_SAVED_ZONE16
#define DET_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief DIO memory and pointer classes.
 */

#define DIO_CODE
#define DIO_CALLOUT_CODE
#define DIO_CODE_FAST
#define DIO_CODE_SLOW
#define DIO_CONST
#define DIO_CALIB
#define DIO_CONFIG_DATA
#define DIO_CONST_SAVED_RECOVERY_ZONE8
#define DIO_CONST_SAVED_RECOVERY_ZONE16
#define DIO_CONST_SAVED_RECOVERY_ZONE32
#define DIO_APPL_DATA
#define DIO_APPL_CONST
#define DIOSPACE
#define DIO_VAR_NOINIT
#define DIO_VAR_CLEARED
#define DIO_VAR_POWER_ON_CLEARED
#define DIO_VAR_INIT
#define DIO_VAR_POWER_ON_INIT
#define DIO_VAR_FAST_NOINIT
#define DIO_VAR_FAST_CLEARED
#define DIO_VAR_FAST_POWER_ON_CLEARED
#define DIO_VAR_FAST_INIT
#define DIO_VAR_FAST_POWER_ON_INIT
#define DIO_VAR_SLOW_NOINIT
#define DIO_VAR_SLOW_CLEARED
#define DIO_VAR_SLOW_POWER_ON_CLEARED
#define DIO_VAR_SLOW_INIT
#define DIO_VAR_SLOW_POWER_ON_INIT
#define DIO_INTERNAL_VAR_NOINIT
#define DIO_INTERNAL_VAR_CLEARED
#define DIO_INTERNAL_VAR_POWER_ON_CLEARED
#define DIO_INTERNAL_VAR_INIT
#define DIO_INTERNAL_VAR_POWER_ON_INIT
#define DIO_VAR_SAVED_ZONE8
#define DIO_VAR_SAVED_ZONE16
#define DIO_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief ETH memory and pointer classes.
 */
#define ETH_CODE
#define ETH_CALLOUT_CODE
#define ETH_CODE_FAST
#define ETH_CODE_SLOW
#define ETH_CONST
#define ETH_CALIB
#define ETH_CONFIG_DATA
#define ETH_CONST_SAVED_RECOVERY_ZONE8
#define ETH_CONST_SAVED_RECOVERY_ZONE16
#define ETH_CONST_SAVED_RECOVERY_ZONE32
#define ETH_APPL_DATA
#define ETH_APPL_CONST
#define ETHSPACE
#define ETH_VAR_NOINIT
#define ETH_VAR_CLEARED
#define ETH_VAR_POWER_ON_CLEARED
#define ETH_VAR_INIT
#define ETH_VAR_POWER_ON_INIT
#define ETH_VAR_FAST_NOINIT
#define ETH_VAR_FAST_CLEARED
#define ETH_VAR_FAST_POWER_ON_CLEARED
#define ETH_VAR_FAST_INIT
#define ETH_VAR_FAST_POWER_ON_INIT
#define ETH_VAR_SLOW_NOINIT
#define ETH_VAR_SLOW_CLEARED
#define ETH_VAR_SLOW_POWER_ON_CLEARED
#define ETH_VAR_SLOW_INIT
#define ETH_VAR_SLOW_POWER_ON_INIT
#define ETH_INTERNAL_VAR_NOINIT
#define ETH_INTERNAL_VAR_CLEARED
#define ETH_INTERNAL_VAR_POWER_ON_CLEARED
#define ETH_INTERNAL_VAR_INIT
#define ETH_INTERNAL_VAR_POWER_ON_INIT
#define ETH_VAR_SAVED_ZONE8
#define ETH_VAR_SAVED_ZONE16
#define ETH_VAR_SAVED_ZONE32
/**@}*/


/**
 * @brief FLS memory and pointer classes.
 */

#define FLS_CODE
#define FLS_CALLOUT_CODE
#define FLS_CODE_FAST
#define FLS_CODE_SLOW
#define FLS_CONST
#define FLS_CALIB
#define FLS_CONFIG_DATA
#define FLS_CONST_SAVED_RECOVERY_ZONE8
#define FLS_CONST_SAVED_RECOVERY_ZONE16
#define FLS_CONST_SAVED_RECOVERY_ZONE32
#define FLS_APPL_DATA
#define FLS_APPL_CONST
#define FLSSPACE
#define FLS_VAR_NOINIT
#define FLS_VAR_CLEARED
#define FLS_VAR_POWER_ON_CLEARED
#define FLS_VAR_INIT
#define FLS_VAR_POWER_ON_INIT
#define FLS_VAR_FAST_NOINIT
#define FLS_VAR_FAST_CLEARED
#define FLS_VAR_FAST_POWER_ON_CLEARED
#define FLS_VAR_FAST_INIT
#define FLS_VAR_FAST_POWER_ON_INIT
#define FLS_VAR_SLOW_NOINIT
#define FLS_VAR_SLOW_CLEARED
#define FLS_VAR_SLOW_POWER_ON_CLEARED
#define FLS_VAR_SLOW_INIT
#define FLS_VAR_SLOW_POWER_ON_INIT
#define FLS_INTERNAL_VAR_NOINIT
#define FLS_INTERNAL_VAR_CLEARED
#define FLS_INTERNAL_VAR_POWER_ON_CLEARED
#define FLS_INTERNAL_VAR_INIT
#define FLS_INTERNAL_VAR_POWER_ON_INIT
#define FLS_VAR_SAVED_ZONE8
#define FLS_VAR_SAVED_ZONE16
#define FLS_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief FlexRay memory and pointer classes.
 */

#define FR_CODE
#define FR_CALLOUT_CODE
#define FR_CODE_FAST
#define FR_CODE_SLOW
#define FR_CONST
#define FR_CALIB
#define FR_CONFIG_DATA
#define FR_CONST_SAVED_RECOVERY_ZONE8
#define FR_CONST_SAVED_RECOVERY_ZONE16
#define FR_CONST_SAVED_RECOVERY_ZONE32
#define FR_APPL_DATA
#define FR_APPL_CONST
#define FRSPACE
#define FR_VAR_NOINIT
#define FR_VAR_CLEARED
#define FR_VAR_POWER_ON_CLEARED
#define FR_VAR_INIT
#define FR_VAR_POWER_ON_INIT
#define FR_VAR_FAST_NOINIT
#define FR_VAR_FAST_CLEARED
#define FR_VAR_FAST_POWER_ON_CLEARED
#define FR_VAR_FAST_INIT
#define FR_VAR_FAST_POWER_ON_INIT
#define FR_VAR_SLOW_NOINIT
#define FR_VAR_SLOW_CLEARED
#define FR_VAR_SLOW_POWER_ON_CLEARED
#define FR_VAR_SLOW_INIT
#define FR_VAR_SLOW_POWER_ON_INIT
#define FR_INTERNAL_VAR_NOINIT
#define FR_INTERNAL_VAR_CLEARED
#define FR_INTERNAL_VAR_POWER_ON_CLEARED
#define FR_INTERNAL_VAR_INIT
#define FR_INTERNAL_VAR_POWER_ON_INIT
#define FR_VAR_SAVED_ZONE8
#define FR_VAR_SAVED_ZONE16
#define FR_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief GPT memory and pointer classes.
 */

#define GPT_CODE
#define GPT_CALLOUT_CODE
#define GPT_CODE_FAST
#define GPT_CODE_SLOW
#define GPT_CONST
#define GPT_CALIB
#define GPT_CONFIG_DATA
#define GPT_CONST_SAVED_RECOVERY_ZONE8
#define GPT_CONST_SAVED_RECOVERY_ZONE16
#define GPT_CONST_SAVED_RECOVERY_ZONE32
#define GPT_APPL_DATA
#define GPT_APPL_CONST
#define GPTSPACE
#define GPT_VAR_NOINIT
#define GPT_VAR_CLEARED
#define GPT_VAR_POWER_ON_CLEARED
#define GPT_VAR_INIT
#define GPT_VAR_POWER_ON_INIT
#define GPT_VAR_FAST_NOINIT
#define GPT_VAR_FAST_CLEARED
#define GPT_VAR_FAST_POWER_ON_CLEARED
#define GPT_VAR_FAST_INIT
#define GPT_VAR_FAST_POWER_ON_INIT
#define GPT_VAR_SLOW_NOINIT
#define GPT_VAR_SLOW_CLEARED
#define GPT_VAR_SLOW_POWER_ON_CLEARED
#define GPT_VAR_SLOW_INIT
#define GPT_VAR_SLOW_POWER_ON_INIT
#define GPT_INTERNAL_VAR_NOINIT
#define GPT_INTERNAL_VAR_CLEARED
#define GPT_INTERNAL_VAR_POWER_ON_CLEARED
#define GPT_INTERNAL_VAR_INIT
#define GPT_INTERNAL_VAR_POWER_ON_INIT
#define GPT_VAR_SAVED_ZONE8
#define GPT_VAR_SAVED_ZONE16
#define GPT_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief ICU memory and pointer classes.
 */

#define ICU_CODE
#define ICU_CALLOUT_CODE
#define ICU_CODE_FAST
#define ICU_CODE_SLOW
#define ICU_CONST
#define ICU_CALIB
#define ICU_CONFIG_DATA
#define ICU_CONST_SAVED_RECOVERY_ZONE8
#define ICU_CONST_SAVED_RECOVERY_ZONE16
#define ICU_CONST_SAVED_RECOVERY_ZONE32
#define ICU_APPL_DATA
#define ICU_APPL_CONST
#define ICUSPACE
#define ICU_VAR_NOINIT
#define ICU_VAR_CLEARED
#define ICU_VAR_POWER_ON_CLEARED
#define ICU_VAR_INIT
#define ICU_VAR_POWER_ON_INIT
#define ICU_VAR_FAST_NOINIT
#define ICU_VAR_FAST_CLEARED
#define ICU_VAR_FAST_POWER_ON_CLEARED
#define ICU_VAR_FAST_INIT
#define ICU_VAR_FAST_POWER_ON_INIT
#define ICU_VAR_SLOW_NOINIT
#define ICU_VAR_SLOW_CLEARED
#define ICU_VAR_SLOW_POWER_ON_CLEARED
#define ICU_VAR_SLOW_INIT
#define ICU_VAR_SLOW_POWER_ON_INIT
#define ICU_INTERNAL_VAR_NOINIT
#define ICU_INTERNAL_VAR_CLEARED
#define ICU_INTERNAL_VAR_POWER_ON_CLEARED
#define ICU_INTERNAL_VAR_INIT
#define ICU_INTERNAL_VAR_POWER_ON_INIT
#define ICU_VAR_SAVED_ZONE8
#define ICU_VAR_SAVED_ZONE16
#define ICU_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief I2C memory and pointer classes.
 */

#define I2C_CODE
#define I2C_CALLOUT_CODE
#define I2C_CODE_FAST
#define I2C_CODE_SLOW
#define I2C_CONST
#define I2C_CALIB
#define I2C_CONFIG_DATA
#define I2C_CONST_SAVED_RECOVERY_ZONE8
#define I2C_CONST_SAVED_RECOVERY_ZONE16
#define I2C_CONST_SAVED_RECOVERY_ZONE32
#define I2C_APPL_DATA
#define I2C_APPL_CONST
#define I2CSPACE
#define I2C_VAR_NOINIT
#define I2C_VAR_CLEARED
#define I2C_VAR_POWER_ON_CLEARED
#define I2C_VAR_INIT
#define I2C_VAR_POWER_ON_INIT
#define I2C_VAR_FAST_NOINIT
#define I2C_VAR_FAST_CLEARED
#define I2C_VAR_FAST_POWER_ON_CLEARED
#define I2C_VAR_FAST_INIT
#define I2C_VAR_FAST_POWER_ON_INIT
#define I2C_VAR_SLOW_NOINIT
#define I2C_VAR_SLOW_CLEARED
#define I2C_VAR_SLOW_POWER_ON_CLEARED
#define I2C_VAR_SLOW_INIT
#define I2C_VAR_SLOW_POWER_ON_INIT
#define I2C_INTERNAL_VAR_NOINIT
#define I2C_INTERNAL_VAR_CLEARED
#define I2C_INTERNAL_VAR_POWER_ON_CLEARED
#define I2C_INTERNAL_VAR_INIT
#define I2C_INTERNAL_VAR_POWER_ON_INIT
#define I2C_VAR_SAVED_ZONE8
#define I2C_VAR_SAVED_ZONE16
#define I2C_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief LIN memory and pointer classes.
 */

#define LIN_CODE
#define LIN_CALLOUT_CODE
#define LIN_CODE_FAST
#define LIN_CODE_SLOW
#define LIN_CONST
#define LIN_CALIB
#define LIN_CONFIG_DATA
#define LIN_CONST_SAVED_RECOVERY_ZONE8
#define LIN_CONST_SAVED_RECOVERY_ZONE16
#define LIN_CONST_SAVED_RECOVERY_ZONE32
#define LIN_APPL_DATA
#define LIN_APPL_CONST
#define LINSPACE
#define LIN_VAR_NOINIT
#define LIN_VAR_CLEARED
#define LIN_VAR_POWER_ON_CLEARED
#define LIN_VAR_INIT
#define LIN_VAR_POWER_ON_INIT
#define LIN_VAR_FAST_NOINIT
#define LIN_VAR_FAST_CLEARED
#define LIN_VAR_FAST_POWER_ON_CLEARED
#define LIN_VAR_FAST_INIT
#define LIN_VAR_FAST_POWER_ON_INIT
#define LIN_VAR_SLOW_NOINIT
#define LIN_VAR_SLOW_CLEARED
#define LIN_VAR_SLOW_POWER_ON_CLEARED
#define LIN_VAR_SLOW_INIT
#define LIN_VAR_SLOW_POWER_ON_INIT
#define LIN_INTERNAL_VAR_NOINIT
#define LIN_INTERNAL_VAR_CLEARED
#define LIN_INTERNAL_VAR_POWER_ON_CLEARED
#define LIN_INTERNAL_VAR_INIT
#define LIN_INTERNAL_VAR_POWER_ON_INIT
#define LIN_VAR_SAVED_ZONE8
#define LIN_VAR_SAVED_ZONE16
#define LIN_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief MCU memory and pointer classes.
 */

#define MCU_CODE
#define MCU_CALLOUT_CODE
#define MCU_CODE_FAST
#define MCU_CODE_SLOW
#define MCU_CONST
#define MCU_CALIB
#define MCU_CONFIG_DATA
#define MCU_CONST_SAVED_RECOVERY_ZONE8
#define MCU_CONST_SAVED_RECOVERY_ZONE16
#define MCU_CONST_SAVED_RECOVERY_ZONE32
#define MCU_APPL_DATA
#define MCU_APPL_CONST
#define MCUSPACE
#define MCU_VAR_NOINIT
#define MCU_VAR_CLEARED
#define MCU_VAR_POWER_ON_CLEARED
#define MCU_VAR_INIT
#define MCU_VAR_POWER_ON_INIT
#define MCU_VAR_FAST_NOINIT
#define MCU_VAR_FAST_CLEARED
#define MCU_VAR_FAST_POWER_ON_CLEARED
#define MCU_VAR_FAST_INIT
#define MCU_VAR_FAST_POWER_ON_INIT
#define MCU_VAR_SLOW_NOINIT
#define MCU_VAR_SLOW_CLEARED
#define MCU_VAR_SLOW_POWER_ON_CLEARED
#define MCU_VAR_SLOW_INIT
#define MCU_VAR_SLOW_POWER_ON_INIT
#define MCU_INTERNAL_VAR_NOINIT
#define MCU_INTERNAL_VAR_CLEARED
#define MCU_INTERNAL_VAR_POWER_ON_CLEARED
#define MCU_INTERNAL_VAR_INIT
#define MCU_INTERNAL_VAR_POWER_ON_INIT
#define MCU_VAR_SAVED_ZONE8
#define MCU_VAR_SAVED_ZONE16
#define MCU_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief OCU memory and pointer classes.
 */

#define OCU_CODE
#define OCU_CALLOUT_CODE
#define OCU_CODE_FAST
#define OCU_CODE_SLOW
#define OCU_CONST
#define OCU_CALIB
#define OCU_CONFIG_DATA
#define OCU_CONST_SAVED_RECOVERY_ZONE8
#define OCU_CONST_SAVED_RECOVERY_ZONE16
#define OCU_CONST_SAVED_RECOVERY_ZONE32
#define OCU_APPL_DATA
#define OCU_APPL_CONST
#define OCUSPACE
#define OCU_VAR_NOINIT
#define OCU_VAR_CLEARED
#define OCU_VAR_POWER_ON_CLEARED
#define OCU_VAR_INIT
#define OCU_VAR_POWER_ON_INIT
#define OCU_VAR_FAST_NOINIT
#define OCU_VAR_FAST_CLEARED
#define OCU_VAR_FAST_POWER_ON_CLEARED
#define OCU_VAR_FAST_INIT
#define OCU_VAR_FAST_POWER_ON_INIT
#define OCU_VAR_SLOW_NOINIT
#define OCU_VAR_SLOW_CLEARED
#define OCU_VAR_SLOW_POWER_ON_CLEARED
#define OCU_VAR_SLOW_INIT
#define OCU_VAR_SLOW_POWER_ON_INIT
#define OCU_INTERNAL_VAR_NOINIT
#define OCU_INTERNAL_VAR_CLEARED
#define OCU_INTERNAL_VAR_POWER_ON_CLEARED
#define OCU_INTERNAL_VAR_INIT
#define OCU_INTERNAL_VAR_POWER_ON_INIT
#define OCU_VAR_SAVED_ZONE8
#define OCU_VAR_SAVED_ZONE16
#define OCU_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief PORT memory and pointer classes.
 */

#define PORT_CODE
#define PORT_CALLOUT_CODE
#define PORT_CODE_FAST
#define PORT_CODE_SLOW
#define PORT_CONST
#define PORT_CALIB
#define PORT_CONFIG_DATA
#define PORT_CONST_SAVED_RECOVERY_ZONE8
#define PORT_CONST_SAVED_RECOVERY_ZONE16
#define PORT_CONST_SAVED_RECOVERY_ZONE32
#define PORT_APPL_DATA
#define PORT_APPL_CONST
#define PORTSPACE
#define PORT_VAR_NOINIT
#define PORT_VAR_CLEARED
#define PORT_VAR_POWER_ON_CLEARED
#define PORT_VAR_INIT
#define PORT_VAR_POWER_ON_INIT
#define PORT_VAR_FAST_NOINIT
#define PORT_VAR_FAST_CLEARED
#define PORT_VAR_FAST_POWER_ON_CLEARED
#define PORT_VAR_FAST_INIT
#define PORT_VAR_FAST_POWER_ON_INIT
#define PORT_VAR_SLOW_NOINIT
#define PORT_VAR_SLOW_CLEARED
#define PORT_VAR_SLOW_POWER_ON_CLEARED
#define PORT_VAR_SLOW_INIT
#define PORT_VAR_SLOW_POWER_ON_INIT
#define PORT_INTERNAL_VAR_NOINIT
#define PORT_INTERNAL_VAR_CLEARED
#define PORT_INTERNAL_VAR_POWER_ON_CLEARED
#define PORT_INTERNAL_VAR_INIT
#define PORT_INTERNAL_VAR_POWER_ON_INIT
#define PORT_VAR_SAVED_ZONE8
#define PORT_VAR_SAVED_ZONE16
#define PORT_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief PWM memory and pointer classes.
 */

#define PWM_CODE
#define PWM_CALLOUT_CODE
#define PWM_CODE_FAST
#define PWM_CODE_SLOW
#define PWM_CONST
#define PWM_CALIB
#define PWM_CONFIG_DATA
#define PWM_CONST_SAVED_RECOVERY_ZONE8
#define PWM_CONST_SAVED_RECOVERY_ZONE16
#define PWM_CONST_SAVED_RECOVERY_ZONE32
#define PWM_APPL_DATA
#define PWM_APPL_CONST
#define PWMSPACE
#define PWM_VAR_NOINIT
#define PWM_VAR_CLEARED
#define PWM_VAR_POWER_ON_CLEARED
#define PWM_VAR_INIT
#define PWM_VAR_POWER_ON_INIT
#define PWM_VAR_FAST_NOINIT
#define PWM_VAR_FAST_CLEARED
#define PWM_VAR_FAST_POWER_ON_CLEARED
#define PWM_VAR_FAST_INIT
#define PWM_VAR_FAST_POWER_ON_INIT
#define PWM_VAR_SLOW_NOINIT
#define PWM_VAR_SLOW_CLEARED
#define PWM_VAR_SLOW_POWER_ON_CLEARED
#define PWM_VAR_SLOW_INIT
#define PWM_VAR_SLOW_POWER_ON_INIT
#define PWM_INTERNAL_VAR_NOINIT
#define PWM_INTERNAL_VAR_CLEARED
#define PWM_INTERNAL_VAR_POWER_ON_CLEARED
#define PWM_INTERNAL_VAR_INIT
#define PWM_INTERNAL_VAR_POWER_ON_INIT
#define PWM_VAR_SAVED_ZONE8
#define PWM_VAR_SAVED_ZONE16
#define PWM_VAR_SAVED_ZONE32
/**@}*/


/**
 * @brief RamTST memory and pointer classes.
 */

#define RAMTST_CODE
#define RAMTST_CALLOUT_CODE
#define RAMTST_CODE_FAST
#define RAMTST_CODE_SLOW
#define RAMTST_CONST
#define RAMTST_CALIB
#define RAMTST_CONFIG_DATA
#define RAMTST_CONST_SAVED_RECOVERY_ZONE8
#define RAMTST_CONST_SAVED_RECOVERY_ZONE16
#define RAMTST_CONST_SAVED_RECOVERY_ZONE32
#define RAMTST_APPL_DATA
#define RAMTST_APPL_CONST
#define RAMTSTSPACE
#define RAMTST_VAR_NOINIT
#define RAMTST_VAR_CLEARED
#define RAMTST_VAR_POWER_ON_CLEARED
#define RAMTST_VAR_INIT
#define RAMTST_VAR_POWER_ON_INIT
#define RAMTST_VAR_FAST_NOINIT
#define RAMTST_VAR_FAST_CLEARED
#define RAMTST_VAR_FAST_POWER_ON_CLEARED
#define RAMTST_VAR_FAST_INIT
#define RAMTST_VAR_FAST_POWER_ON_INIT
#define RAMTST_VAR_SLOW_NOINIT
#define RAMTST_VAR_SLOW_CLEARED
#define RAMTST_VAR_SLOW_POWER_ON_CLEARED
#define RAMTST_VAR_SLOW_INIT
#define RAMTST_VAR_SLOW_POWER_ON_INIT
#define RAMTST_INTERNAL_VAR_NOINIT
#define RAMTST_INTERNAL_VAR_CLEARED
#define RAMTST_INTERNAL_VAR_POWER_ON_CLEARED
#define RAMTST_INTERNAL_VAR_INIT
#define RAMTST_INTERNAL_VAR_POWER_ON_INIT
#define RAMTST_VAR_SAVED_ZONE8
#define RAMTST_VAR_SAVED_ZONE16
#define RAMTST_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief SchM memory and pointer classes.
 */

#define SCHM_CODE
#define SCHM_CALLOUT_CODE
#define SCHM_CODE_FAST
#define SCHM_CODE_SLOW
#define SCHM_CONST
#define SCHM_CALIB
#define SCHM_CONFIG_DATA
#define SCHM_CONST_SAVED_RECOVERY_ZONE8
#define SCHM_CONST_SAVED_RECOVERY_ZONE16
#define SCHM_CONST_SAVED_RECOVERY_ZONE32
#define SCHM_APPL_DATA
#define SCHM_APPL_CONST
#define SCHMSPACE
#define SCHM_VAR_NOINIT
#define SCHM_VAR_CLEARED
#define SCHM_VAR_POWER_ON_CLEARED
#define SCHM_VAR_INIT
#define SCHM_VAR_POWER_ON_INIT
#define SCHM_VAR_FAST_NOINIT
#define SCHM_VAR_FAST_CLEARED
#define SCHM_VAR_FAST_POWER_ON_CLEARED
#define SCHM_VAR_FAST_INIT
#define SCHM_VAR_FAST_POWER_ON_INIT
#define SCHM_VAR_SLOW_NOINIT
#define SCHM_VAR_SLOW_CLEARED
#define SCHM_VAR_SLOW_POWER_ON_CLEARED
#define SCHM_VAR_SLOW_INIT
#define SCHM_VAR_SLOW_POWER_ON_INIT
#define SCHM_INTERNAL_VAR_NOINIT
#define SCHM_INTERNAL_VAR_CLEARED
#define SCHM_INTERNAL_VAR_POWER_ON_CLEARED
#define SCHM_INTERNAL_VAR_INIT
#define SCHM_INTERNAL_VAR_POWER_ON_INIT
#define SCHM_VAR_SAVED_ZONE8
#define SCHM_VAR_SAVED_ZONE16
#define SCHM_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief SPI memory and pointer classes.
 */

#define SPI_CODE
#define SPI_CALLOUT_CODE
#define SPI_CODE_FAST
#define SPI_CODE_SLOW
#define SPI_CONST
#define SPI_CALIB
#define SPI_CONFIG_DATA
#define SPI_CONST_SAVED_RECOVERY_ZONE8
#define SPI_CONST_SAVED_RECOVERY_ZONE16
#define SPI_CONST_SAVED_RECOVERY_ZONE32
#define SPI_APPL_DATA
#define SPI_APPL_CONST
#define SPISPACE
#define SPI_VAR_NOINIT
#define SPI_VAR_CLEARED
#define SPI_VAR_POWER_ON_CLEARED
#define SPI_VAR_INIT
#define SPI_VAR_POWER_ON_INIT
#define SPI_VAR_FAST_NOINIT
#define SPI_VAR_FAST_CLEARED
#define SPI_VAR_FAST_POWER_ON_CLEARED
#define SPI_VAR_FAST_INIT
#define SPI_VAR_FAST_POWER_ON_INIT
#define SPI_VAR_SLOW_NOINIT
#define SPI_VAR_SLOW_CLEARED
#define SPI_VAR_SLOW_POWER_ON_CLEARED
#define SPI_VAR_SLOW_INIT
#define SPI_VAR_SLOW_POWER_ON_INIT
#define SPI_INTERNAL_VAR_NOINIT
#define SPI_INTERNAL_VAR_CLEARED
#define SPI_INTERNAL_VAR_POWER_ON_CLEARED
#define SPI_INTERNAL_VAR_INIT
#define SPI_INTERNAL_VAR_POWER_ON_INIT
#define SPI_VAR_SAVED_ZONE8
#define SPI_VAR_SAVED_ZONE16
#define SPI_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief WDG memory and pointer classes.
 */

#define WDG_CODE
#define WDG_CALLOUT_CODE
#define WDG_CODE_FAST
#define WDG_CODE_SLOW
#define WDG_CONST
#define WDG_CALIB
#define WDG_CONFIG_DATA
#define WDG_CONST_SAVED_RECOVERY_ZONE8
#define WDG_CONST_SAVED_RECOVERY_ZONE16
#define WDG_CONST_SAVED_RECOVERY_ZONE32
#define WDG_APPL_DATA
#define WDG_APPL_CONST
#define WDGSPACE
#define WDG_VAR_NOINIT
#define WDG_VAR_CLEARED
#define WDG_VAR_POWER_ON_CLEARED
#define WDG_VAR_INIT
#define WDG_VAR_POWER_ON_INIT
#define WDG_VAR_FAST_NOINIT
#define WDG_VAR_FAST_CLEARED
#define WDG_VAR_FAST_POWER_ON_CLEARED
#define WDG_VAR_FAST_INIT
#define WDG_VAR_FAST_POWER_ON_INIT
#define WDG_VAR_SLOW_NOINIT
#define WDG_VAR_SLOW_CLEARED
#define WDG_VAR_SLOW_POWER_ON_CLEARED
#define WDG_VAR_SLOW_INIT
#define WDG_VAR_SLOW_POWER_ON_INIT
#define WDG_INTERNAL_VAR_NOINIT
#define WDG_INTERNAL_VAR_CLEARED
#define WDG_INTERNAL_VAR_POWER_ON_CLEARED
#define WDG_INTERNAL_VAR_INIT
#define WDG_INTERNAL_VAR_POWER_ON_INIT
#define WDG_VAR_SAVED_ZONE8
#define WDG_VAR_SAVED_ZONE16
#define WDG_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief WDGIF memory and pointer classes.
 */

#define WDGIF_CODE
#define WDGIF_CALLOUT_CODE
#define WDGIF_CODE_FAST
#define WDGIF_CODE_SLOW
#define WDGIF_CONST
#define WDGIF_CALIB
#define WDGIF_CONFIG_DATA
#define WDGIF_CONST_SAVED_RECOVERY_ZONE8
#define WDGIF_CONST_SAVED_RECOVERY_ZONE16
#define WDGIF_CONST_SAVED_RECOVERY_ZONE32
#define WDGIF_APPL_DATA
#define WDGIF_APPL_CONST
#define WDGIFSPACE
#define WDGIF_VAR_NOINIT
#define WDGIF_VAR_CLEARED
#define WDGIF_VAR_POWER_ON_CLEARED
#define WDGIF_VAR_INIT
#define WDGIF_VAR_POWER_ON_INIT
#define WDGIF_VAR_FAST_NOINIT
#define WDGIF_VAR_FAST_CLEARED
#define WDGIF_VAR_FAST_POWER_ON_CLEARED
#define WDGIF_VAR_FAST_INIT
#define WDGIF_VAR_FAST_POWER_ON_INIT
#define WDGIF_VAR_SLOW_NOINIT
#define WDGIF_VAR_SLOW_CLEARED
#define WDGIF_VAR_SLOW_POWER_ON_CLEARED
#define WDGIF_VAR_SLOW_INIT
#define WDGIF_VAR_SLOW_POWER_ON_INIT
#define WDGIF_INTERNAL_VAR_NOINIT
#define WDGIF_INTERNAL_VAR_CLEARED
#define WDGIF_INTERNAL_VAR_POWER_ON_CLEARED
#define WDGIF_INTERNAL_VAR_INIT
#define WDGIF_INTERNAL_VAR_POWER_ON_INIT
#define WDGIF_VAR_SAVED_ZONE8
#define WDGIF_VAR_SAVED_ZONE16
#define WDGIF_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief UART memory and pointer classes.
 */

#define UART_CODE
#define UART_CALLOUT_CODE
#define UART_CODE_FAST
#define UART_CODE_SLOW
#define UART_CONST
#define UART_CALIB
#define UART_CONFIG_DATA
#define UART_CONST_SAVED_RECOVERY_ZONE8
#define UART_CONST_SAVED_RECOVERY_ZONE16
#define UART_CONST_SAVED_RECOVERY_ZONE32
#define UART_APPL_DATA
#define UART_APPL_CONST
#define UARTSPACE
#define UART_VAR_NOINIT
#define UART_VAR_CLEARED
#define UART_VAR_POWER_ON_CLEARED
#define UART_VAR_INIT
#define UART_VAR_POWER_ON_INIT
#define UART_VAR_FAST_NOINIT
#define UART_VAR_FAST_CLEARED
#define UART_VAR_FAST_POWER_ON_CLEARED
#define UART_VAR_FAST_INIT
#define UART_VAR_FAST_POWER_ON_INIT
#define UART_VAR_SLOW_NOINIT
#define UART_VAR_SLOW_CLEARED
#define UART_VAR_SLOW_POWER_ON_CLEARED
#define UART_VAR_SLOW_INIT
#define UART_VAR_SLOW_POWER_ON_INIT
#define UART_INTERNAL_VAR_NOINIT
#define UART_INTERNAL_VAR_CLEARED
#define UART_INTERNAL_VAR_POWER_ON_CLEARED
#define UART_INTERNAL_VAR_INIT
#define UART_INTERNAL_VAR_POWER_ON_INIT
#define UART_VAR_SAVED_ZONE8
#define UART_VAR_SAVED_ZONE16
#define UART_VAR_SAVED_ZONE32
/**@}*/

/**
 * @brief SYSTEM memory and pointer classes.
 */

#define SYSTEM_CODE
#define SYSTEM_CALLOUT_CODE
#define SYSTEM_CODE_FAST
#define SYSTEM_CODE_SLOW
#define SYSTEM_CONST
#define SYSTEM_CALIB
#define SYSTEM_CONFIG_DATA
#define SYSTEM_CONST_SAVED_RECOVERY_ZONE8
#define SYSTEM_CONST_SAVED_RECOVERY_ZONE16
#define SYSTEM_CONST_SAVED_RECOVERY_ZONE32
#define SYSTEM_APPL_DATA
#define SYSTEM_APPL_CONST
#define SYSTEMSPACE
#define SYSTEM_VAR_NOINIT
#define SYSTEM_VAR_CLEARED
#define SYSTEM_VAR_POWER_ON_CLEARED
#define SYSTEM_VAR_INIT
#define SYSTEM_VAR_POWER_ON_INIT
#define SYSTEM_VAR_FAST_NOINIT
#define SYSTEM_VAR_FAST_CLEARED
#define SYSTEM_VAR_FAST_POWER_ON_CLEARED
#define SYSTEM_VAR_FAST_INIT
#define SYSTEM_VAR_FAST_POWER_ON_INIT
#define SYSTEM_VAR_SLOW_NOINIT
#define SYSTEM_VAR_SLOW_CLEARED
#define SYSTEM_VAR_SLOW_POWER_ON_CLEARED
#define SYSTEM_VAR_SLOW_INIT
#define SYSTEM_VAR_SLOW_POWER_ON_INIT
#define SYSTEM_INTERNAL_VAR_NOINIT
#define SYSTEM_INTERNAL_VAR_CLEARED
#define SYSTEM_INTERNAL_VAR_POWER_ON_CLEARED
#define SYSTEM_INTERNAL_VAR_INIT
#define SYSTEM_INTERNAL_VAR_POWER_ON_INIT
#define SYSTEM_VAR_SAVED_ZONE8
#define SYSTEM_VAR_SAVED_ZONE16
#define SYSTEM_VAR_SAVED_ZONE32
/**@}*/
/**
 * @brief Define for ComStack Data.
 */
#define AUTOSAR_COMSTACKDATA

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

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

#endif /* COMPILER_CFG_H */

/*---------------------- End of File ----------------------------------------*/
