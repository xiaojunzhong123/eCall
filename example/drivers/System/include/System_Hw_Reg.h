/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: System_Hw_Reg.h
 *
 * Description: Implementation of SYSTEM HW REG High Level layer
 *              Compiler:                IAR GCC
 *
 *
 * (c) Copyright 2023 - 2026 XIAOHUA SEMICONDUCTOR CO., LTD. All rights reserved.
 *
 * This software is licensed by XHSC PLATFORM SOFTWARE LICENSE AGREEMENT (the "License"). By downloading, installing, or
 * using the Software, you indicate that you accept the terms of the License, and you acknowledge that you have the
 * authority, for yourself or on behalf of your company, to bind your company to these terms. If you do not agree to all
 * of the terms of this License, you should not download or install the Software.
 *
 **********************************************************************************************************************/

/**
 *   @file       System_Hw_Reg.h
 *   @version    1.0.3
 *   @brief      System_Hw_Reg Header File
 *   @addtogroup reg_hw
 *   @{
 */

#ifndef SYSTEM_HW_REG_H
#define SYSTEM_HW_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#if !defined(HC32A4A0PITI) && !defined(HC32A4A0SITI) && !defined(HC32A4A8PITI) && !defined(HC32A4A8PITJ) && !defined(HC32A4A8RITI) && !defined(HC32A4A8RITJ) && !defined(HC32A4A8SITI) && !defined(HC32A4A8SITJ)
#include "System_Hw_Reg_Cfg.h"
#endif
#if defined(HC32A4A0PITI) || defined(HC32A4A0SITI)
#include "Mcal_hc32a4a0.h"
#endif
#if defined(HC32A4A8PITI) || defined(HC32A4A8PITJ) || defined(HC32A4A8RITI) || defined(HC32A4A8RITJ) || defined(HC32A4A8SITI) || defined(HC32A4A8SITJ)
#include "Mcal_hc32a4a8.h"
#endif

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

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

#endif /*!< SYSTEM_HW_REG_H */
