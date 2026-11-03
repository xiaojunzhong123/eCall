/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Gpio_Port_Ip.h
 *
 * Description: Implementation of GPIO IP layer
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
 *   @file       Gpio_Port_Ip.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifndef GPIO_PORT_IP_H
#define GPIO_PORT_IP_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Gpio_Port_Ip_Types.h"
#include "Port_Cfg.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

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
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#define PORT_START_SEC_CODE
#include "Port_MemMap.h"
/**
 *  @brief          Initializes the Port Driver module
 *
 *  @details        SRS ID:            None
 *                                     
 *                  Design ID:         Port_Gpio_Init_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Port_Gpio_Init(
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
);

#if (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)
/**
 *  @brief          Set Pin Direction for Port Pin
 *
 *  @details        SRS ID:            None
 *                                     
 *                  Design ID:         Port_Gpio_SetPinDirection_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Pin_u8            Pin index
 *  @param[in]      para_Direction_u8      Direction want to change
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 *
 *  @return         uint8_t
 *  @retval         PORT_GPIO_PARAM_INVALID_DIRECTION   Set Pin Direction Fail cause Expected Direction was wrong
 *  @retval         PORT_GPIO_DIRECTION_UNCHANGEABLE    Set Pin Direction Fail cause Direction Can't be changed
 *  @retval         PORT_GPIO_PROCESS_SUCCESS           Set Pin Direction Success

 * 
 *  @note           None
 *
 *  @pre            None
 *
 */
uint8_t Port_Gpio_SetPinDirection
(
  uint8_t  para_Pin_u8,
  uint8_t  para_Direction_u8,
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
);
#endif /*!< End of (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)*/

#if (STD_ON == PORT_SET_PIN_MODE_API_ENABLE)
/**
 *  @brief          Set Pin Mode for Port Pin.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Port_Gpio_SetPinMode_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_PinId_u8    Pin index
 *  @param[in]      para_Mode_u32    Port pin mode
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 *
 *  @return         uint8_t
 *  @retval         PORT_GPIO_MODE_UNCHANGEABLE       Set Pin Mode Fail cause Mode Can't be changed
 *  @retval         PORT_GPIO_IP_PROCESS_SUCCESS      Set Pin Mode Success
 *
 *  @note           None
 *
 *  @pre            Driver must be initialized before
 *
 */
uint8_t Port_Gpio_SetPinMode
(
  uint8_t  para_Pin_u8,
  uint8_t  para_Mode_u8,
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
);
#endif /*!< End of (STD_ON == PORT_SET_PIN_MODE_API_ENABLE)*/

/**
 *  @brief          Refresh Port Direction.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Port_Gpio_RefreshPortDirection_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 * 
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            Driver must be initialized before
 *
 */
void Port_Gpio_RefreshPortDirection
(
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
);

#define PORT_STOP_SEC_CODE
#include "Port_MemMap.h"

#ifdef __cplusplus
}
#endif
#endif /* _GPIO_PORT_IP_H_ */
/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
