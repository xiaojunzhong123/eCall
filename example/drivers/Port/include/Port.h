/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Port.h
 *
 * Description: Implementation of PORT High Level layer
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
 *   @file       Port.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifndef PORT_H
#define PORT_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/**
 * @brief Include the related header files
 * @details SRS ID: SWS_Port_00129, SWS_Port_00208
 */
#include "Std_Types.h"
#include "Port_Cfg.h"
#include "Port_VersionInfo.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define PORT_MODULE_ID                     124

#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
/**
 * @brief API service is call with invalid Port Pin ID
 */
#define PORT_E_PARAM_PIN                                (0x0AU)

/**
 * @brief API service is call with Port Pin which is not configured as changeable
 */
#define PORT_E_DIRECTION_UNCHANGEABLE                   (0x0BU)

/**
 * @brief Wrong parameter is passed to Port_Init
 */
#define PORT_E_INIT_FAILED                              (0x0CU)

/**
 * @brief Invalid mode is passed to Port_SetPinMode
 */
#define PORT_E_PARAM_INVALID_MODE                       (0x0DU)

/**
 * @brief Mode, which is unchangeable, is passed to Port_SetPinMode
 */
#define PORT_E_MODE_UNCHANGEABLE                        (0x0EU)

/**
 * @brief Service is called before module initialization
 */
#define PORT_E_UNINIT                                   (0x0FU)

/**
 * @brief Null Pointer is passed to services
 */
#define PORT_E_PARAM_POINTER                            (0x10U)

/**
 * @brief Invalid direction is passed to Port_SetPinDirection
 */
#define PORT_E_PARAM_INVALID_DIRECTION                  (0x12U)

/**
 * @brief Port_Init service ID
 */
#define PORT_SID_INIT                                      (0U)

/**
 * @brief Port_SetPinDirection service ID
 */
#define PORT_SID_SET_PIN_DIRECTION                         (1U)

/**
 * @brief Port_RefreshPortDirection service ID
 */
#define PORT_SID_REFRESH_PORT_DIRECTION                    (2U)

/**
 * @brief Port_GetVersionInfo service ID
 */
#define PORT_SID_GET_VERSIONINFO                           (3U)

/**
 * @brief Port_SetPinMode service ID
 */
#define PORT_SID_SET_PIN_MODE                              (4U)

#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */

#if(STD_OFF == PORT_PRECOMPILE_ENABLE)
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
PORT_PARAM_CONFIG_EXT
#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
#endif /*!< End of (STD_OFF == PORT_PRECOMPILE_ENABLE) */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Type of possible directions a port pin can have
 *  @details SRS ID:     SWS_Port_00230, SWS_Port_00046, AR_RS_PORT_00004
 */
typedef enum
{
  PORT_PIN_IN = 0,
  PORT_PIN_OUT = 1,
  PORT_PIN_INOUT
} Port_PinDirectionType;

/**
 *  @brief   Type of port pin number
 *  @details SRS ID:     SWS_Port_00229, SWS_Port_00013, SWS_Port_00219, SWS_Port_00013, SWS_Port_00207
 */
typedef uint8_t Port_PinType;

/**
 *  @brief   Type of possible pin mode
 *  @details SRS ID:     SWS_Port_00231, SWS_Port_00124, SWS_Port_00212, SWS_Port_00221, SWS_Port_00082, SWS_Port_00124, SWS_Port_00221
 */
typedef uint8_t Port_PinModeType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define PORT_START_SEC_CODE
#include "Port_MemMap.h"
/**
 *  @brief          Initializes the Port Driver module.
 *
 *  @details        Service ID:        0x00
 *                  SRS ID:            SWS_Port_00001, SWS_Port_00004, SWS_Port_00079, SWS_Port_00081, SWS_Port_00005
 *                                     SWS_Port_00207, SWS_Port_00140, SWS_Port_00041, SWS_Port_00078, SWS_Port_00213 
 *                                     SWS_Port_00042, SWS_Port_00214, SWS_Port_00215, SWS_Port_00217, SWS_Port_00043
 *                                     SWS_Port_00071, SWS_Port_00002, SWS_Port_00121, SWS_Port_00055, SWS_Port_00113
 *                  Design ID:         Port_Init_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      ConfigPtr    Pointer to configuration set.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Port_Init(
  const Port_ConfigType * ConfigPtr
);

#if (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)
/**
 *  @brief          Set the port pin direction
 *
 *  @details        Service ID:        0x01
 *                  SRS ID:            SWS_Port_00077, SWS_Port_00087, SWS_Port_00137
 *                                     SWS_Port_00141, SWS_Port_00063, SWS_Port_00051
 *                                     SWS_Port_00054, SWS_Port_00086, ProductionReq_Port_0005
 *                                     ProductionReq_Port_0006, SWS_Port_00086, SWS_Port_00138
 *                  Design ID:         Port_SetPinDirection_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      Pin          Port Pin ID number
 *  @param[in]      Direction    Port Pin Direction
 *
 *  @return         void
 *
 *  @note           Driver must be initialized before
 *
 *  @pre            None
 *
 */
void Port_SetPinDirection(
  Port_PinType  Pin,
  Port_PinDirectionType  Direction
);
#endif /*!< End of (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE) */

#if (STD_ON == PORT_SET_PIN_MODE_API_ENABLE)
/**
 *  @brief          Set the port pin mode
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            SWS_Port_00051, SWS_Port_00077, SWS_Port_00087
 *                                     SWS_Port_00212, SWS_Port_00145, SWS_Port_00125
 *                                     SWS_Port_00128, SWS_Port_00223, ProductionReq_Port_0005 
 *                                     ProductionReq_Port_0007
 *                  Design ID:         Port_SetPinMode_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      Pin     Port Pin ID number
 *  @param[in]      Mode    New Port Pin mode to be set on port pin
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            Driver must be initialized before
 *
 */
void Port_SetPinMode(
  Port_PinType  Pin,
  Port_PinModeType  Mode
);
#endif /*!< End of (STD_ON == PORT_SET_PIN_MODE_API_ENABLE) */
/**
 *  @brief          Refresh port pin direction.
 *
 *  @details        Service ID:        0x02
 *                  SRS ID:            SWS_Port_00066, SWS_Port_00077, SWS_Port_00087
 *                                     SWS_Port_00142, SWS_Port_00060, SWS_Port_00061
 *                  Design ID:         Port_RefreshPortDirection_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @return         void
 *
 *  @note           Driver must be initialized before
 *
 *  @pre            None
 *
 */
void Port_RefreshPortDirection(
  void
);

#if (STD_ON == PORT_VERSION_INFO_API_ENABLE)
/**
 *  @brief          Returns the version information of this module.
 *
 *  @details        Service ID:        0x03
 *                  SRS ID:            SWS_Port_00077,SWS_Port_00087,SWS_Port_00129
 *                                     SWS_Port_00143,SWS_Port_00146,SWS_Port_00225
 *                  Design ID:         Port_GetVersionInfo_Activity
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      Versioninfo    Pointer to where to store the version information of this module.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Port_GetVersionInfo(
  Std_VersionInfoType * versioninfo
);
#endif  /*!< End of (STD_ON == PORT_VERSION_INFO_API_ENABLE) */

#define PORT_STOP_SEC_CODE
#include "Port_MemMap.h"

#ifdef __cplusplus
}
#endif
#endif  /* _PORT_H_ */
/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
