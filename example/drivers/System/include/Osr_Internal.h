/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Osr_Internal.h
 *
 * Description: Implementation of OSR INTERNAL High Level layer
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
 *   @file       Osr_Internal.h
 *   @version    1.0.3
 *   @brief      Osr_Internal Header File
 *   @addtogroup system_driver
 *   @{
 */

#ifndef OSR_INTERNAL_H
#define OSR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Osr_Cfg.h"
#if defined(AUTOSAR_OS_USED)
#include "Os.h"
#endif /*!< defined(AUTOSAR_OS_USED) */
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/*
 * Osr_GetCoreID
 */
#ifdef AUTOSAR_OS_USED
#define Osr_GetCoreID() GetCoreID()
#else /* AUTOSAR_OS_USED */
/* Baremetal - FreeRTOS */
#define Osr_GetCoreID() (0U)
/**
 * @brief Compiler abstraction for creating an interrupt handler if no OS is present.
 */
#define ISR(IsrName) void IsrName(void)
#endif

/*
 * Osr_SuspendAllInterrupts
 * Osr_ResumeAllInterrupts
 */
#if !defined(AUTOSAR_OS_USED)
/* Baremetal - FreeRTOS */
/**
 * @brief ContinueAllInterrupts Arm Cortex-M7 instruction.
 */
#define ResumeAllInterrupts()  ASM_KEYWORD(" cpsie i")
/**
 * @brief PauseAllInterrupts Arm Cortex-M7 instruction.
 */
#define SuspendAllInterrupts() ASM_KEYWORD(" cpsid i")
#else
/* #include "Os.h" */
#endif /*!<AUTOSAR_OS_USED*/

/* Osr_SuspendAllInterrupts_define */
#define Osr_SuspendAllInterrupts() SuspendAllInterrupts()
/* Osr_ResumeAllInterrupts_define */
#define Osr_ResumeAllInterrupts() ResumeAllInterrupts()

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

#endif /*!< OSR_INTERNAL_H */
