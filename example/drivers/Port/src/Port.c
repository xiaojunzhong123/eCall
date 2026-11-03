/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Port.c
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
 *   @file       Port.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Port.h"
#include "Port_Ipr.h"
#if (PORT_DETECTION_DEV_ERROR_ENABLE == STD_ON)
/**
 * @brief Include the related header files
 * @details SRS ID: SWS_Port_00146
 */
#include "Det.h"
#endif

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check static code version with generated code version */
#if ((PORT_SW_MAJOR_VERSION != PORT_CFG_SW_MAJOR_VERSION) || \
     (PORT_SW_MINOR_VERSION != PORT_CFG_SW_MINOR_VERSION) || \
     (PORT_SW_PATCH_VERSION != PORT_CFG_SW_PATCH_VERSION) \
    )
    #error "static code and generated code have different software release version"
#endif

#ifndef DISABLE_EXTERNAL_VERSION_CHECK
#if (PORT_DETECTION_DEV_ERROR_ENABLE == STD_ON)
  /* Check AutoSar Version Numbers of Det.h and Port.h */
  #if ((DET_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION) || \
     (DET_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)    \
    )
    #error "Inconsistent Software Versions of Det.h and Port.h"
  #endif
#endif
#endif
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/
#if(STD_ON == PORT_PRECOMPILE_ENABLE)
#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
/**
 * @brief  The port configuration data
 */
extern const Port_ConfigType  Port_Config;
#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Port_MemMap.h"
#endif
/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define PORT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Port_MemMap.h"
/**
 * @brief  The port configuration data for current file
 */
static const Port_ConfigType * s_PortConfig_ptr;
#define PORT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Port_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define PORT_START_SEC_CODE
#include "Port_MemMap.h"

#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
LOCAL_INLINE Std_ReturnType Port_ValidatePtrInit(
  const Port_ConfigType * ConfigPtr
);
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
/**
 *  @brief          The function will validate Input parameter of the initialization pointer
 *
 *  @details        SRS ID:            SWS_Port_00051, ProductionReq_Port_0004, ProductionReq_Port_0005
 *                  Design ID:         SDD_PORT_6
 *
 *  @param[in]      ConfigPtr          Specifies the pointer to the configuration set
 *
 *  @return         Std_ReturnType
 *  @retval         E_OK              The function call is valid
 *  @retval         E_NOT_OK          The function call is invalid
 *
 *  @note           This function will Validate the initialization pointer
 *
 *  @pre            None
 *
 */
LOCAL_INLINE Std_ReturnType Port_ValidatePtrInit(
  const Port_ConfigType * ConfigPtr
)
{
  Std_ReturnType RetVal_dd = (Std_ReturnType)E_OK;

#if(STD_ON == PORT_PRECOMPILE_ENABLE)
  if (NULL_PTR != ConfigPtr)
#else
  if (NULL_PTR == ConfigPtr)
#endif /*!< End of (STD_ON == PORT_PRECOMPILE_ENABLE) */
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_INIT, (uint8_t)PORT_E_INIT_FAILED);
    RetVal_dd = (Std_ReturnType)E_NOT_OK;
  }
  return RetVal_dd;
}
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Initializes the Port Driver module.
 *
 *  @details        Service ID:        0x00
 *                  SRS ID:            SWS_Port_00001, SWS_Port_00004, SWS_Port_00079, SWS_Port_00081, SWS_Port_00005
 *                                     SWS_Port_00207, SWS_Port_00140, SWS_Port_00041, SWS_Port_00078, SWS_Port_00213 
 *                                     SWS_Port_00042, SWS_Port_00214, SWS_Port_00215, SWS_Port_00217, SWS_Port_00043
 *                                     SWS_Port_00071, SWS_Port_00002, SWS_Port_00121, SWS_Port_00055, SWS_Port_00113
 *                                     SWS_Port_00075
 *                  Design ID:         SDD_PORT_1
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
)
{
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
  Std_ReturnType  RetVal_dd = (Std_ReturnType)E_NOT_OK;

  RetVal_dd = Port_ValidatePtrInit(ConfigPtr);
  if ((Std_ReturnType)E_OK == RetVal_dd)
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  {
#if (STD_ON == PORT_PRECOMPILE_ENABLE)
    s_PortConfig_ptr = &Port_Config;
    (void)ConfigPtr;
#else /*!< End of (STD_OFF == PORT_PRECOMPILE_ENABLE) */
    s_PortConfig_ptr = ConfigPtr;
#endif /*!< End of (STD_ON == PORT_PRECOMPILE_ENABLE) */

    Port_Ipr_Init(s_PortConfig_ptr);
  }
}

#if (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)
/**
 *  @brief          Set the port pin direction
 *
 *  @details        Service ID:        0x01
 *                  SRS ID:            SWS_Port_00077, SWS_Port_00087, SWS_Port_00137
 *                                     SWS_Port_00141, SWS_Port_00063, SWS_Port_00051
 *                                     SWS_Port_00054, SWS_Port_00086, ProductionReq_Port_0005
 *                                     ProductionReq_Port_0006, SWS_Port_00086, SWS_Port_00138
 *                  Design ID:         SDD_PORT_2
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
)
{
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
  uint8_t  ErrCode_u8;

  if (NULL_PTR == s_PortConfig_ptr)
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_DIRECTION, (uint8_t)PORT_E_UNINIT);
  }
  else if ((PORT_NUM_PINS < Pin) || (0U == Pin))
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_DIRECTION, (uint8_t)PORT_E_PARAM_PIN);
  }
  else
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  {
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
    ErrCode_u8 = Port_Ipr_SetPinDirection(Pin, (uint8_t)Direction, s_PortConfig_ptr);
#else
    (void)Port_Ipr_SetPinDirection(Pin, (uint8_t)Direction, s_PortConfig_ptr);
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */

#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
    if (0U != ErrCode_u8)
    {
      (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_DIRECTION, (uint8_t)ErrCode_u8);
    }
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  }
}
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
 *                  Design ID:         SDD_PORT_5
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
)
{
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
  uint8_t  ErrCode_u8;

  if (NULL_PTR == s_PortConfig_ptr)
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_MODE, (uint8_t)PORT_E_UNINIT);
  }
  else if ((PORT_NUM_PINS < Pin) || (0U == Pin))
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_MODE, (uint8_t)PORT_E_PARAM_PIN);
  }
  else
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  {
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
    ErrCode_u8 = (uint8_t)Port_Ipr_SetPinMode(Pin, Mode, s_PortConfig_ptr);
#else
    (void)Port_Ipr_SetPinMode(Pin, Mode, s_PortConfig_ptr);
#endif

#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
    if (0U != ErrCode_u8)
    {
      (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_SET_PIN_MODE, (uint8_t)ErrCode_u8);
    }
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  }
}
#endif /*!< End of (STD_ON == PORT_SET_PIN_MODE_API_ENABLE) */

/**
 *  @brief          Refresh port pin direction.
 *
 *  @details        Service ID:        0x02
 *                  SRS ID:            SWS_Port_00066, SWS_Port_00077, SWS_Port_00087
 *                                     SWS_Port_00142, SWS_Port_00060, SWS_Port_00061
 *                  Design ID:         SDD_PORT_3
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
)
{
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
  if (NULL_PTR == s_PortConfig_ptr)
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_REFRESH_PORT_DIRECTION, (uint8_t)PORT_E_UNINIT);
  }
  else
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  {
    Port_Ipr_RefreshPortDirection(s_PortConfig_ptr);
  }
}

#if  (STD_ON == PORT_VERSION_INFO_API_ENABLE)
/**
 *  @brief          Returns the version information of this module.
 *
 *  @details        Service ID:        0x03
 *                  SRS ID:            SWS_Port_00077,SWS_Port_00087,SWS_Port_00129
 *                                     SWS_Port_00143,SWS_Port_00146,SWS_Port_00225
 *                  Design ID:         SDD_PORT_4
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
)
{
#if (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE)
  if (NULL_PTR == versioninfo)
  {
    (void)Det_ReportError((uint16_t)PORT_MODULE_ID, 0x0U, (uint8_t)PORT_SID_GET_VERSIONINFO, (uint8_t)PORT_E_PARAM_POINTER);
  }
  else
#endif /*!< End of (STD_ON == PORT_DETECTION_DEV_ERROR_ENABLE) */
  {
    (versioninfo)->vendorID         = (uint16_t)PORT_VENDOR_ID;
    (versioninfo)->moduleID         = (uint16_t)PORT_MODULE_ID;
    (versioninfo)->sw_major_version = (uint8_t)PORT_SW_MAJOR_VERSION;
    (versioninfo)->sw_minor_version = (uint8_t)PORT_SW_MINOR_VERSION;
    (versioninfo)->sw_patch_version = (uint8_t)PORT_SW_PATCH_VERSION;
  }
}
#endif /*!< End of (STD_ON == PORT_VERSION_INFO_API_ENABLE) */

#define PORT_STOP_SEC_CODE
#include "Port_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
