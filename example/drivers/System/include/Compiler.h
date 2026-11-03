/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Compiler.h
 *
 * Description: Implementation of COMPILER High Level layer
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
#ifndef COMPILER_H
#define COMPILER_H

/**
 *   @file       Compiler.h
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
#include "Compiler_Cfg.h"

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define COMPILER_AR_RELEASE_MAJOR_VERSION        4
#define COMPILER_AR_RELEASE_MINOR_VERSION        4
#define COMPILER_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/**
 * @brief The memory class AUTOMATIC is used for local variables
 */
#define AUTOMATIC


/**
 * @briefdefines a null pointer.
 *
 */
#define NULL_PTR ((void *)0)

/**
 * @brief  define inline macro.
 */
#define INLINE              inline

/**
 * @brief  define local inline macro
 */
#define LOCAL_INLINE            static inline

/**
 * @brief  define interrupt function macro
 */
#define INTERRUPT_FUNC


/**
 * @brief defines a function with a specified return type and memory class.
 */
#define FUNC(rettype, memclass) rettype

/**
 * @brief defines a pointer to a variable of the specified data type and memory class.
 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/**
 * @brief defines a pointer to a variable of the specified data type and
 * memory class, pointer to a constant variable.
 */
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/**
 * @brief defines a pointer to a variable of the specified data type and
 * memory class, pointer is a constant pointer.
 */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/**
 * @brief  defines a pointer to a variable of the specified data type and
 * memory class, pointer is a constant pointer.
 */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/**
 * @brief defines a pointer to a function with the specified return
 * type and parameter type.
 */
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/**
 * @brief  defines a constant.
 */
#define CONST(consttype, memclass) const consttype

/**
 * @brief defines a variable with the specified data type and memory class.
 */
#define VAR(vartype, memclass) vartype


/**
 * @brief defines a pointer to a function with a constant parameter
 * and a specified return type, pointer to a mutable variable.
 */

#define CONSTP2FUNC(rettype, ptrclass, fctname) rettype (* const fctname)

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

#endif /* COMPILER_H */

/*---------------------- End of File ----------------------------------------*/
