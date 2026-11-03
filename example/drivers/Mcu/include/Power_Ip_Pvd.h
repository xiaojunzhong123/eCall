/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Pvd.h
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
 *   @file       Power_Ip_Pvd.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */
#ifndef POWER_IP_PVD_H
#define POWER_IP_PVD_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_Types.h"

/***********************************************************************************************************************
 *  VERSION CHECKS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

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
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function configures the Power Management Controller for PVD1
 *                  The operating voltages are monitored by a set of on-chip supervisory circuits
 *                  to ensure that this device works within the correct voltage range.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD1Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PVDConfig_ptr    Pointer to PVD1 configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures PVD1 (Programmable Voltage Detector 1) including
 *                  filter settings, threshold voltage, enable/disable state, compare output,
 *                  trigger edge, and exception type (IRQ/NMI). This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD1Init(
    const Power_Ip_PWC_PVD1ConfigType * para_PVDConfig_ptr
);

/**
 *  @brief          This function configures the Power Management Controller for PVD2
 *                  The operating voltages are monitored by a set of on-chip supervisory circuits
 *                  to ensure that this device works within the correct voltage range.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD2Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PVDConfig_ptr    Pointer to PVD2 configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures PVD2 (Programmable Voltage Detector 2) including
 *                  filter settings, threshold voltage, enable/disable state, compare output,
 *                  trigger edge, and exception type (IRQ/NMI). This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD2Init(
    const Power_Ip_PWC_PVD2ConfigType * para_PVDConfig_ptr
);

/**
 *  @brief          This function configures the Power Monitor.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PowerMonitorInit_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerMonitorConfig_ptr    Pointer to Power Monitor configuration structure
 *
 *  @return         void
 *
 *  @note           This function configures VBAT monitoring and power monitoring features
 *                  including source selection (1.8V/2.1V for VBAT, VBAT/2 or internal for power monitor)
 *                  and enable/disable states. This function will be called by Power_Ip_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_PowerMonitorInit(
    const Power_Ip_PowerMonitorConfigType * para_PowerMonitorConfig_ptr
);


/**
 *  @brief          This function handles the voltage error detection.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_PVD_VoltageError_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function is called when PVD1 or PVD2 detects a voltage error.
 *                  It clears the corresponding detection flags and optionally triggers
 *                  a user notification if POWER_IP_PWC_PVD_NOTIFICATION is defined.
 *
 *  @pre            None
 *
 */
void Power_Ip_PVD_VoltageError(
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

#endif /* POWER_IP_PVD_H */

/*---------------------- End of File ----------------------------------------*/
