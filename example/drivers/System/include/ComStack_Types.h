/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: ComStack_Types.h
 *
 * Description: Implementation of COMSTACK_TYPES High Level layer
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
 *   @file       ComStack_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "ComStack_Cfg.h"

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define COMSTACK_TYPES_AR_RELEASE_MAJOR_VERSION        4
#define COMSTACK_TYPES_AR_RELEASE_MINOR_VERSION        4
#define COMSTACK_TYPES_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Result of a buffer request. 
 */
typedef enum
{
    BUFREQ_OK       = 0U,
    BUFREQ_E_NOT_OK = 1U,
    BUFREQ_E_BUSY   = 2U,
    BUFREQ_E_OVFL   = 3U
} BufReq_ReturnType;

/**
 * @brief This type represents the errors for Pretended Networking
 */
typedef enum
{
  ICOM_SWITCH_E_OK = 0,
  ICOM_SWITCH_E_FAILED = 1
} IcomSwitch_ErrorType;

/**
 * @brief  This type represents the handle of a communication network.
 */
typedef uint8_t        NetworkHandleType;

/**
 * @brief This type represents PDU info type
 */
typedef struct
{
  uint8_t * SduDataPtr;   /*!< Pointer to the buffer of SDU data */
  PduLengthType  SduLength;                                   /*!< Length of the SDU in bytes. */
} PduInfoType;

/**
 * @brief   This type represents the configuration ID of an
 */
typedef uint8_t        IcomConfigIdType;



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

#endif /* COMSTACK_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
