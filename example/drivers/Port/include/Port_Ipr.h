/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Port_Ipr.h
 *
 * Description: Implementation of PORT IP Router layer
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
 *   @file       Port_Ipr.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifndef PORT_IPR_H
#define PORT_IPR_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "Gpio_Port_Ip.h"
#include "Port_VersionInfo.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define Port_Ipr_Init                       Port_Gpio_Init

#if (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)
#define Port_Ipr_SetPinDirection            Port_Gpio_SetPinDirection
#endif /*!< End of (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE) */

#if (STD_ON == PORT_SET_PIN_MODE_API_ENABLE)
#define Port_Ipr_SetPinMode                 Port_Gpio_SetPinMode
#endif /*!< End of (STD_ON == PORT_SET_PIN_MODE_API_ENABLE) */

#define Port_Ipr_RefreshPortDirection       Port_Gpio_RefreshPortDirection

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
#endif /* _PORT_IPR_H_ */
/**
 * @}
 */
/*---------------------- End of File -----------------------------------------*/
