/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Common.h
 *
 * Description: Implementation of COMMON High Level layer
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
 *   @file       Common.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "Compiler.h"
/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define COMMON_AR_RELEASE_MAJOR_VERSION        4
#define COMMON_AR_RELEASE_MINOR_VERSION        4
#define COMMON_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/**
 * @brief  pointer to pointer to variable.
 */
#define P2P2VAR(ptrtype, memclass, ptrclass) ptrtype **

/**
 * @brief pointer to pointer to constant.
 */
#define P2P2CONST(ptrtype, memclass, ptrclass) const ptrtype **

#if defined (_GCC_C_HC32A4XX_)
/**
 * @brief asm keyword.
 */
#define ASM_KEYWORD  __asm__
#elif defined (_IAR_C_HC32A4XX_)
/**
 * @brief asm keyword.
 */
#define ASM_KEYWORD  __asm
#endif

/**
 * @brief add alignment constraint for variable
 */
#define ALIGNED_DATA(v, size)  __attribute__((aligned(size))) v;

#define EXIT_INTERRUPT() ASM_KEYWORD(" DSB")

/**
 * @brief  define force inline macro
 */
#define FORCE_INLINE __attribute__((always_inline))

/**
 * @brief Add section macro to be used.
 */
#define SYS_SECTION(x)          __attribute__((section(#x)))

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief DEM error management
 */
typedef struct
{
  uint32_t state;   /*!< enabling/disabling the DEM error: Active=STD_ON/ Inactive=STD_OFF */
  uint32_t id ;     /*!< ID of DEM error (0 if STD_OFF)*/
}Mcal_DemErrorType;


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

#endif /* COMMON_H */

/*---------------------- End of File ----------------------------------------*/
