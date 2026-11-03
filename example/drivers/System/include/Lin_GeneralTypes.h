/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Lin_GeneralTypes.h
 *
 * Description: Implementation of LIN GENERALTYPES High Level layer
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
 *   @file       Lin_GeneralTypes.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 Common
 *   @addtogroup system_driver
 *   @{
 */

#ifndef LIN_GENERAL_TYPES_H
#define LIN_GENERAL_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "PlatformTypes.h"

/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define LIN_GENERALTYPES_AR_RELEASE_MAJOR_VERSION        4
#define LIN_GENERALTYPES_AR_RELEASE_MINOR_VERSION        4
#define LIN_GENERALTYPES_AR_RELEASE_REVISION_VERSION     0
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
 * @brief Checksum models (Enhanced checksum model or Classic checksum model for the LIN Frame
 */
typedef enum
{
  LIN_ENHANCED_CS,
  LIN_CLASSIC_CS
} Lin_FrameCsModelType;

/**
 * @brief Frame response types.
 */
typedef enum
{
  LIN_FRAMERESPONSE_TX = 0,  /* master response.*/
  LIN_FRAMERESPONSE_RX,      /* slave response */
  LIN_FRAMERESPONSE_IGNORE   /* response from slave to slave */
                             /* in this case, master shall not receive the response.*/
} Lin_FrameResponseType;

/**
 * @brief  Status of LIN Frame and Channel returned from Lin_GetStatus() API
 */
typedef enum
 {
  LIN_NOT_OK = 0,       /* Error occurred.*/
  LIN_TX_OK,            /* Successful transmission */
  LIN_TX_BUSY,          /* Transmission is in-progress */
  LIN_TX_HEADER_ERROR,  /* Error occurred in header */
  LIN_TX_ERROR,         /* Error occurred in transmission */
  LIN_RX_OK,            /* Successful reception */
  LIN_RX_BUSY,          /* Reception is in-progress*/
  LIN_RX_ERROR,         /* Error occurred in reception */
  LIN_RX_NO_RESPONSE,   /* No response */
  LIN_OPERATIONAL,      /* Normal operation; ready for next transaction.*/
  LIN_CH_SLEEP          /* In sleep state */
} Lin_StatusType;


/**
 * @brief LIN Slave error type that are detected during header reception
 *        or response transmission or response reception
 */

typedef enum
{
  LIN_ERR_HEADER = 0,   /* Error in header */
  LIN_ERR_RESP_STOPBIT, /* Error in response */
  LIN_ERR_RESP_CHKSUM,  /* Checksum error */
  LIN_ERR_RESP_DATABIT, /* Error of transmitted data bit of response */
  LIN_ERR_NO_RESP,      /* No response */
  LIN_ERR_INC_RESP      /* Incomplete response */
} Lin_SlaveErrorType;

/***********************************************************************************************************************
 *  STRUCTURES AND OTHER TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief  Data length of a LIN Frame - the number of SDU data bytes to copy
 */
typedef uint8_t Lin_FrameDlType;

/**
 * @brief          The LIN identifier (0..0x3F) with parity bits.
 */
typedef uint8_t Lin_FramePidType;

/**
 * @brief          Lin Pdu Type: PID, checksum model, response type, data length and SDU
 */
typedef struct
{
  Lin_FramePidType      Pid;     /* LIN frame identifier */
  Lin_FrameCsModelType  Cs;      /* Checksum model */
  Lin_FrameResponseType Drc;     /* Response type */
  Lin_FrameDlType       Dl;      /* Data length */
  uint8_t*                SduPtr;  /* Sdu Pointer */
} Lin_PduType;

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

#endif /* COMSTACK_CFG_H */

/*---------------------- End of File ----------------------------------------*/
