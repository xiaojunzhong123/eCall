/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Mode.h
 *
 * Description: Implementation of POWER IP layer
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
 *   @file       Power_Ip_Mode.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef POWER_IP_MODE_H
#define POWER_IP_MODE_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_Types.h"
#include "Power_Ip_Cfg.h"

/***********************************************************************************************************************
 *  VERSION CHECKS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief  Return for not ready.
 */
#define NOT_READY                     (0x01)
/**
 * @brief  Return for ready.
 */
#define READY                         (0x00)

/**
 * @brief  RAMOPM value when the chip works on high-speed run mode.
 */
#define POWER_RAMOPM_HIGH_SPEED       (0x8043U)

/**
 * @brief  RAMOPM value when the chip works on ultra-low-speed run mode.
 */
#define POWER_RAMOPM_ULTRALOWSPEED    (0x9062U)

/**
 * @brief  Time_out value to switch mode.
 */
#define POWER_IP_SWITCH_TIMEOUT       (30U)

/**
 * @brief  Time_out value verify register value when switch High<>Low speed mode.
 */
#define POWER_IP_SWITCH_TIMEOUT2      (0x1000U)

/**
 * @brief  The flag for time out.
 */
#define POWER_IP_TIMEOUT              (0x00U)

/**
 * @brief  Inline assembler support for WFI instruction.
 */
#define ASM_WFI() ASM_KEYWORD("wfi" ::: "memory")


/***********************************************************************************************************************
 *  EXTERN
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
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function configure high speed mode for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModeHighSpeed_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *
 *  @pre            None
 *
 */
void Power_Ip_ModeHighSpeed(
    void
);

/**
 *  @brief          This function configure ultra low speed mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModeUltraLowSpeed_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *
 *  @pre            None
 *
 */
void Power_Ip_ModeUltraLowSpeed(
    void
);

/**
 *  @brief          This function configure sleep mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModeSleep_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *                  Any interrupt or reset can wake up the chip from sleep mode.
 *                  when waking up by interrupt, the chip enter interrupt  handling.
 *                  subroutine, the chip enter reset state when wake up by reset.
 *
 *  @pre            None
 *
 */
void Power_Ip_ModeSleep(
    void
);

/**
 *  @brief          This function configure stop mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModeStop_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerStopModeCfgPtr    Struct configure for Stop mode
 *
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *                  The stop mode can be exited by reset or interrupt.
 *                  The reset can be used to exit stop mode include:
 *                  + pin reset
 *                  + Power on reset
 *                  + Brown out reset (BOR).
 *                  + Programmable voltage detection 1
 *
 *  @pre            None
 *
 */
void Power_Ip_ModeStop(
    const Power_Ip_StopModeCfgType * para_PowerStopModeCfg_ptr
);

/**
 *  @brief          This function configure power down mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModePwd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerDownModeCfgPtr    Struct configure for power-down mode.
 *
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *
 *  @pre            None
 *
 */
void Power_Ip_ModePwd(
    const Power_Ip_PowerDownModeCfgType * para_PowerDownModeCfg_ptr
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* POWER_IP_MODE_H */

/*---------------------- End of File ----------------------------------------*/
