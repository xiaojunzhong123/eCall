/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip.h
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
 *   @file       Power_Ip.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef POWER_IP_H
#define POWER_IP_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_Types.h"

/***********************************************************************************************************************
 *   VERSION CHECK
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
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function initializes the module Reset control
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_HwIPsConfigPtr_ptr    Pointer to hardware IP configuration structure.
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_Ipr_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_Init(
    const Power_Ip_HwConfigType * para_HwIPsConfigPtr_ptr
);

/**
 *  @brief          This function make pointer to the mode structure.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Power_Ip_SetMode_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ModeConfigPtr_ptr        Pointer to mode configuration structure.
 *  @param[in]      para_McuClockConfigptr_ptr    Pointer to clock configuration structure.
 *
 *  @return         void
 *
 *  @note           This function initializes the mode structure by configuring
 *                  the MC_ME module.
 *                  This function will called by Mcu_Ipr_SetMode().
 *
 *  @pre            None
 *
 */
void Power_Ip_SetMode(
    const Power_Ip_HwModeConfigType * para_ModeConfigPtr_ptr,
    const Clock_Ip_ClockConfigSetType * para_McuClockConfigptr_ptr
);

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          This function checks if backup domain power-on occurred
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_CheckBackupDomainPowerOn_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         boolean    TRUE: Backup domain power-on occurred                
 *                             FALSE: Backup domain power-on did not occur      
 *
 *  @note           This function reads VBATPORF flag and clears it if set
 *
 *  @pre            None
 *
 */
boolean Power_Ip_CheckBackupDomainPowerOn(
    void
);
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

/**
 *  @brief          This function initializes WKT Timer
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_WktTimerConfig_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ClkSrc_u16               Clock source for WKT timer
 *  @param[in]      para_CmpVal_u16               Comparison value of the Counter (0-0xFFF)
 * 
 *  @return         void
 *
 *  @note           This function configures the Wake-up Timer (WKT) with the specified
 *                  clock source and comparison value. The comparison value can be a number
 *                  between Min_Data = 0 and Max_Data = 0xFFF.
 *
 *  @pre            None
 *
 */
void Power_Ip_WktTimerConfig(
    uint16_t para_ClkSrc_u16, 
    uint16_t para_CmpVal_u16
);

/**
 *  @brief          This function enables or disables WKT Timer.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_WktTimerEnableConfig_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_WKTTimerEnable_bool               TRUE to enable, FALSE to disable WKT Timer
 * 
 *  @return         void
 *
 *  @note           This function controls the enable/disable state of the Wake-up Timer (WKT).
 *                  When enabled, the timer will count and can wake up the system from low-power modes.
 *
 *  @pre            None
 *
 */
void Power_Ip_WktTimerEnableConfig(
    boolean para_WKTTimerEnable_bool
);

/**
 *  @brief          This function gets WKT Timer count match flag.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_GetWktTimerStatus_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         boolean    TRUE if WKT timer count match occurred, FALSE otherwise
 *
 *  @note           This function reads the WKOVF (Wake-up Timer Overflow) flag to determine
 *                  if the WKT timer has reached its comparison value and generated a match event.
 *
 *  @pre            None
 *
 */
boolean Power_Ip_GetWktTimerStatus(
    void
);

/**
 *  @brief          This function clears WKT Timer count match flag.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ClearWktTimerStatus_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function clears the WKOVF (Wake-up Timer Overflow) flag by writing
 *                  to the appropriate register bit. This should be called after handling
 *                  a WKT timer match event.
 *
 *  @pre            None
 *
 */
void Power_Ip_ClearWktTimerStatus(
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

#endif /* POWER_IP_H */

/*---------------------- End of File ----------------------------------------*/
