/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Osr_Types.h
 *
 * Description: Implementation of OSR TYPES High Level layer
 *              Compiler:                IAR GCC
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
 *   @file       Osr_Types.h
 *   @version    1.0.3
 *   @brief      Osr_Types Header File
 *   @addtogroup system_driver
 *   @{
 */

#ifndef OSR_TYPES_H
#define OSR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief  OSR 32bit mask
 *
 */
#define OSR_32BIT_MASK            (0xFFFFU)

/**
 * @brief  OSR 64bit mask
 *
 */
#define OSR_64BIT_MASK            (0xFFFFFFFFU)

/**
 * @brief  OSR get counter api id
 *
 */
#define OSR_USVAL                 (1000000U)

/**
 * @brief  OSR initial status api id
 *
 */
#define OSR_INIT_ID               ((uint8_t)0x09U)

/**
 * @brief  OSR get counter api id
 *
 */
#define OSR_GETCOUNTER_ID         ((uint8_t)0x0aU)

/**
 * @brief  OSR get elapsed api id
 *
 */
#define OSR_GETELAPSED_ID         ((uint8_t)0x0bU)

/**
 * @brief  OSR us to ticks api id
 *
 */
#define OSR_US2TICKS_ID           ((uint8_t)0x0cU)

/**
 * @brief  OSR uninit error
 *
 */
#define OSR_E_UNINIT               ((uint8_t)0x05U)

/**
 * @brief  OSR init failed error
 *
 */
#define OSR_E_INIT_FAILED          ((uint8_t)0x06U)

/**
 * @brief  OSR invalid core ID error
 *
 */
#define OSR_E_INV_CORE_IDX         ((uint8_t)0x07U)

/**
 * @brief  OSR invalid api error
 *
 */
#define OSR_E_INV_API              ((uint8_t)0x08U)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Osr Counter enum type
 *  @details SRS ID:     AR_RS_System_00032
 *           Design ID:  
 */
typedef enum
{
    OSR_COUNTER_DUMMY     = 0U,  /* define dummy counter */
    OSR_COUNTER_AUTOSAROS = 1U,  /* define autosar os counter */
    OSR_COUNTER_SYSTICK   = 2U,  /* define systick counter */
} Osr_CounterType;

/**
 *  @brief   Osr Type structure
 *  @details SRS ID:     AR_RS_System_00033
 *           Design ID:  
 */
typedef struct
{
    uint8_t CounterIdx_u8;                   /*!< The CounterIdx. */
    uint32_t ClockFrequency_u32;             /*!< The system clock frequency. */
    uint32_t SystickReloadValue_u32;         /*!< The systick reload value. */
    uint8_t SystickClkSrc_u8;                /*!< The systick clock source. */
} Osr_ConfigType;

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

#endif /*!< OSR_TYPES_H */
