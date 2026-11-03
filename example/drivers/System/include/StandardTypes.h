/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: StandardTypes.h
 *
 * Description: Implementation of STANDARDTYPES High Level layer
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
 *   @file       StandardTypes.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "PlatformTypes.h"
#include "Compiler.h"

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define STD_AR_RELEASE_MAJOR_VERSION      4
#define STD_AR_RELEASE_MINOR_VERSION      4
#define STD_AR_RELEASE_REVISION_VERSION   0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief   Physical state high 5V or 3.3V
 */
#define STD_HIGH    0x01u

/**
 * @brief Physical state low - 0V.
 */
#define STD_LOW     0x00u

/**
 * @brief   ON State
 */
#define STD_ON      0x01u

/**
 * @brief   OFF state
 */
#define STD_OFF     0x00u

/**
 * @brief   Return code for error case
 */
#define E_NOT_OK    0x01u


/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief   E_OK is defined within OSEK so in order to avoid redefinition problem,
 *          it shall be defined as following
 */
#ifndef STATUSTYPE_DEFINED
  #define STATUSTYPE_DEFINED
/**
 * @brief   Return code for successful case
 */
  #define E_OK      0x00u

/**
 * @brief   This type is defined for OSEK compliance
 */
  typedef uint8_t StatusType;
#endif

/**
 * @brief   Standard API return type
 */
typedef uint8_t Std_ReturnType;

/**
 * @brief   Std_VersionInfoType_structure for <Msn>_GetVersionInfo() function
 */
typedef struct
{
  uint16_t  vendorID;               /* vendor ID */
  uint16_t  moduleID;               /* module ID */
  uint8_t   sw_major_version;       /* module software major version */
  uint8_t   sw_minor_version;       /* module software minor version */
  uint8_t   sw_patch_version;       /* module software patch version */
} Std_VersionInfoType;

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

#endif /* STANDARDTYPES_H */

/*---------------------- End of File ----------------------------------------*/
