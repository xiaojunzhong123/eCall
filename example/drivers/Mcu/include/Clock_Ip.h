/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip.h
 *
 * Description: Implementation of CLOCK IP layer
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
 *   @file       Clock_Ip.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef CLOCK_IP_H
#define CLOCK_IP_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Clock_Ip_Types.h"
#include "Clock_Ip_Cfg.h"
#include "Power_Ip.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief  Lock/unlock Code for each module.
 */
#define RMU_WRITE_ENABLE                (0xA500U)
#define RMU_UNLOCK_CODE0                (0xA501U)
#define RMU_UNLOCK_CODE1                (0xA502U)
#define RMU_UNLOCK_CODE2                (0xA508U)

/**
 * @brief  PWC FCG0 Unlock/Lock code.
 */
#define RMU_FCG0_REG_UNLOCK_KEY         (0xA5A50001U)
#define RMU_FCG0_REG_LOCK_KEY           (0xA5A50000U)

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
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function will create delay period
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_Delay_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_Delay_u32     Time for delay
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Clock_Ip_Delay(
    const uint32_t  para_Delay_u32
);

/**
 *  @brief          Enable or disable the FCGx peripheral clock
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_FcgPeriphClock_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_FuncClockConfig_ptr    The config for peripheral
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Clock_Ip_FcgPeriphClock(
    const Clock_Ip_FuncClockControlType * para_FuncClockConfig_ptr
);

/**
 *  @brief          This function will set frequency divisor of system clock
 *                  and update system clock
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_SetSysClockDiv_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ClockSource_u32    Choose clock source
 *  @param[in]      para_ClockDiv_u32       Choose the divisor
 *
 *  @return         void
 *
 *  @note           This function wil be called by Clock_Ip_ConfigPeriphClock,
 *
 *  @pre            None
 *
 */
void Clock_Ip_SetSysClockDiv(
    const uint32_t  para_ClockSource_u32,
    const uint32_t  para_ClockDiv_u32
);

/**
 *  @brief          Prepare before enter init clock (Enable MRC clock and switch system clock to MRC)
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_PreInitClock_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Clock_Ip_PreInitClock(
    void
);

/**
 *  @brief          This function clear the CMU interrupt flag from CMU module.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_XTAL_STOP_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           Called by ISR when a user notification for CMU XTAL events is configured
 *
 *  @pre            None
 *
 */
void Clock_Ip_XTAL_STOP(
    void
);

/**
 *  @brief          This function clear the CMU interrupt flag from CMU module.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_FCM_INT_OVF_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           Called by ISR when a user notification for CMU FCM events is configured
 *
 *  @pre            None
 *
 */
void Clock_Ip_FCM_INT_OVF(
    void
);

/**
 *  @brief          This function clear the CMU interrupt flag from CMU module.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_FCM_INT_END_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           Called by ISR when a user notification for CMU FCM events is configured
 *
 *  @pre            None
 *
 */
void Clock_Ip_FCM_INT_END(
    void
);

/**
 *  @brief          This function clear the CMU interrupt flag from CMU module.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_FCM_INT_ERR_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           Called by ISR when a user notification for CMU FCM events is configured
 *
 *  @pre            None
 *
 */
void Clock_Ip_FCM_INT_ERR(
    void
);

/**
 *  @brief          This function initializes PLL clock and other MCU specific clock options
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_InitClock_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to clock configuration structure (member of 'Mcu_ConfigType' struct)
 *
 *  @return         Std_ReturnType    Provide the status of initialization clock
 *  @retval         E_OK              Initialization clock was successful
 *  @retval         E_NOT_OK          Initialization clock was NOT successful
 *
 *  @note           This function will be called by Mcu_Ipr_InitClock().
 *
 *  @pre            None
 *
 */
Std_ReturnType Clock_Ip_InitClock(
    const Clock_Ip_ClockHwConfigType * para_ClockConfig_ptr
);

/**
 *  @brief          This function provides the lock status of the PLL
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_GetPllStatus_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         Clock_Ip_PllStatusType      Provide the lock status of PLL
 *  @retval         MCU_PLL_STATUS_UNDEFINED    PLL Status is unknown
 *  @retval         MCU_PLL_LOCKED              PLL is locked
 *  @retval         MCU_PLL_UNLOCKED            PLL is unlocked
 *
 *  @note           This function will be called by Mcu_Ipr_GetPllStatus().
 *
 *  @pre            None
 *
 */
Clock_Ip_PllStatusType Clock_Ip_GetPllStatus(
    void
);

/**
 *  @brief          This function configures and activates the PLL clock
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_DistributePll_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This API is available if only McuNoPll parameter is configured as false
 *                  This function will be called by Mcu_Ipr_DistributePllClock()
 *
 *  @pre            None
 *
 */
void Clock_Ip_DistributePll(
    void
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* CLOCK_IP_H */

/*---------------------- End of File ----------------------------------------*/
