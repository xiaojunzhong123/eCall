/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: PlatformTypes.h
 *
 * Description: Implementation of PLATFORMTYPES High Level layer
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
 *   @file       PlatformTypes.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifndef PLATFORMTYPES_H
#define PLATFORMTYPES_H

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
#define PLATFORMTYPES_AR_RELEASE_MAJOR_VERSION        4
#define PLATFORMTYPES_AR_RELEASE_MINOR_VERSION        4
#define PLATFORMTYPES_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

#ifndef TRUE
  /**
   * @brief True value
   */
  #define TRUE 1
#endif
#ifndef FALSE
  /**
   * @brief False value
   */
  #define FALSE 0
#endif


/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief   The standard AUTOSAR type boolean
 */
typedef unsigned char boolean;

/**
 * @brief   Unsigned 8 bit integer (0x00..0xFF)
 */
typedef unsigned char uint8_t;

/**
 * @brief   Unsigned 16 bit integer (0x0000..0xFFFF)
 */
typedef unsigned short uint16_t;

/**
 * @brief   Unsigned 32 bit integer (0x00000000..0xFFFFFFFF)
 */
typedef unsigned long uint32_t;

/**
 * @brief   Unsigned 64 bit integer (0x0000000000000000..0xFFFFFFFFFFFFFFFF)
 */
typedef unsigned long long uint64_t;

/**
 * @brief   Signed 8 bit integer (-128 ..+127) (0x80..0x7F)
 */
typedef signed char sint8;

/**
 * @brief   Signed 16 bit integer (-32768 ..+32767) (0x8000..0x7FFF)
 */
typedef signed short sint16;

/**
 * @brief   Signed 32 bit integer (-2147483648.. +2147483647) (0x80000000..0x7FFFFFFF)
 */
typedef signed long sint32;

/**
 * @brief   Signed 64 bit integer (-9223372036854775808..9223372036854775807) (0x8000000000000000..0x7FFFFFFFFFFFFFFF )
 */
typedef signed long long sint64;

/**
 * @brief   Unsigned integer at least 8 bit long (0x00..0xFF)
 */
typedef unsigned long uint8_least;

/**
 * @brief   Unsigned integer at least 16 bit long (0x0000..0xFFFF)
 */
typedef unsigned long uint16_least;

/**
 * @brief   Unsigned integer at least 32 bit long (0 ..+4294967295)
 */
typedef unsigned long uint32_least;

/**
 * @brief   Signed integer at least 8 bit long (-128 ..+127)
 */
typedef signed long sint8_least;

/**
 * @brief   Signed integer at least 16 bit long (-32768 ..+32767)
 */
typedef signed long sint16_least;

/**
 * @brief   Signed integer at least 32 bit long (-2147483648.. +2147483647)
 */
typedef signed long sint32_least;

/**
 * @brief   32bit long floating point
 */
typedef float float32;

/**
 * @brief   64bit long floating point
 */
typedef double float64;


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

#endif /* PLATFORMTYPES_H */

/*---------------------- End of File ----------------------------------------*/
