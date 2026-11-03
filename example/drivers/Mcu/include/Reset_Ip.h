/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Reset_Ip.h
 *
 * Description: Implementation of RESET IP layer
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
 *   @file       Reset_Ip.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */
#ifndef RESET_IP_RESET_H
#define RESET_IP_RESET_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Reset_Ip_Types.h"
#include "Reset_Ip_Cfg.h"

/***********************************************************************************************************************
 *  VERSION CHECKS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define PWC_UNLOCK_CODE1                (0xA502U)
#define PWC_WRITE_ENABLE                (0xA500U)
/**
 * @brief  Inline assembler support for DSB instruction.
 */
#define ASM_DSB() ASM_KEYWORD("dsb")

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
 *  @brief          This function enable Cortex-M4 lockup reset.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function initializes the Reset IP module by enabling
 *                  lockup reset and configuring RMU registers.
 *
 *  @pre            None
 *
 */
void Reset_Ip_Init(
    void
);

/**
 *  @brief          This function reads the reset type from the hardware.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_GetResetReason_Activity
 *                  Sync/Async:        Synchronous
 *  @return         Reset_Ip_ResetReasonType    Reset reason from hardware   
 *  @retval         RESET_IP_PO_RESET           VCC power up
 *  @retval         RESET_IP_PIN_RESET          NRST pin input low level
 *  @retval         RESET_IP_BO_RESET           VCC voltage drop below VBOR voltage
 *  @retval         RESET_IP_PVD1_RESET         VCC voltage drop below PVD1 voltage
 *  @retval         RESET_IP_PVD2_RESET         VCC voltage drop below PVD2 voltage
 *  @retval         RESET_IP_WD_RESET           Refresh error or overflow error occurs in watchdog
 *  @retval         RESET_IP_SWD_RESET          Refresh error or overflow error occurred in the dedicated watchdog
 *  @retval         RESET_IP_PD_RESET           Power-down wake up events occurs when devices is in power-down mode
 *  @retval         RESET_IP_SW_RESET           Set the reset register
 *  @retval         RESET_IP_MPUE_RESET         Reset generated by MPU access error
 *  @retval         RESET_IP_RAPE_RESET         Parity error occurs in the RAM
 *  @retval         RESET_IP_RAEC_RESET         ECC error occurs in the RAM
 *  @retval         RESET_IP_CKFE_RESET         Clock frequency abnormal detected by the FCM
 *  @retval         RESET_IP_XTALE_RESET        External high-speed oscillator oscillation failure is detected
 *  @retval         RESET_IP_LKUP_RESET         When the cortex M4 encounters a serious exception
 *  @retval         RESET_IP_FLEC_RESET         ECC error occurs in the Flash
 *  @retval         RESET_IP_ERMU_RESET         ERMU reset
 *  @retval         RESET_IP_RESET_MULTIPLE     Multiple reset reason
 *  @retval         RESET_IP_RESET_UNDEFINED    Unknown
 *
 *  @note           This function will be called by Mcu_Ipr_GetResetReason().
 *
 *  @pre            None
 *
 */
Reset_Ip_ResetReasonType Reset_Ip_GetResetReason(
    void
);

/**
 *  @brief          This function reads the reset type from the hardware register.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_GetResetRawValue_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         uint32_t    Raw reset value from hardware register containing reset flags
 *
 *  @note           This function will be called by Power_Ip_GetResetRawValue().
 *
 *  @pre            None
 *
 */
uint32_t Reset_Ip_GetResetRawValue(
    void
);

/**
 *  @brief          Clear reset reason.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_ClearResetReason_Activity
 *                  Sync/Async:        Synchronous
 *  @return         Std_ReturnType    Status of reset reason clearing operation
 * 
 *  @retval         E_OK              Reset reason cleared successfully
 *  @retval         E_NOT_OK          Reset reason clearing failed or timeout occurred
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Reset_Ip_ClearResetReason(
    void
);

/**
 *  @brief          This function will reset microcontroller using hardware reset feature.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_PerformReset_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function performs a system reset by setting the SYSRESETREQ bit
 *                  in the AIRCR register. It ensures all memory accesses are completed
 *                  before triggering the reset. This function will be called by Mcu_PerformReset().
 *
 *  @pre            None
 *
 */
void Reset_Ip_PerformReset(
    void
);

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          Set software module reset.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_SetSoftwareModuleReset_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_SwModuleReset_ptr    Point to Software Module Reset configuration
 *
 *  @return         Std_ReturnType    Status of software module reset operation
 *  @retval         E_OK              Software module reset was successful
 *  @retval         E_NOT_OK          Software module reset was NOT successful or timeout occurred
 *
 *  @note           None
 *                  
 *
 *  @pre            None
 *
 */
Std_ReturnType Reset_Ip_SetSoftwareModuleReset(
    const Reset_Ip_SoftwareModuleResetConfigType * para_SwModuleReset_ptr
);
#endif  /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* POWER_IP_RESET_H */

/*---------------------- End of File ----------------------------------------*/
