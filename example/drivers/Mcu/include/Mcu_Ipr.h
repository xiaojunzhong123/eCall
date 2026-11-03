/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_Ipr.h
 *
 * Description: Implementation of MCU IP Router layer
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
 *   @file       Mcu_Ipr.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef MCU_IPR_H
#define MCU_IPR_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Mcu.h"

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
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function initializes Mcu module
 * 
 *  @details        Design ID:         Mcu_Ipr_Init_Activity
 *
 *  @param[in]      para_HwIPsConfig_ptr    Pointer to hardware IP configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_Init().
 *
 *  @pre            None
 *
 */
void Mcu_Ipr_Init(
    const Mcu_HwConfigType * para_HwIPsConfig_ptr
);

#if (MCU_ENABLE_INIT_CLOCK == STD_ON)
/**
 *  @brief          This function will initializes clocks clock options.
 * 
 *  @details        Design ID:         Mcu_Ipr_InitClock_Activity
 *
 *  @param[in]      para_ClockConfig_ptr    Pointer to clock configuration structure
 *                                          (member of 'Mcu_ConfigType' struct).
 *
 *  @return         Std_ReturnType    Status of clock initialization
 *  @retval         E_OK              Clock initialization was successful
 *  @retval         E_NOT_OK          Clock initialization failed
 *
 *  @note           This function will be called by Mcu_InitClock().
 *
 *  @pre            None
 *
 */
Std_ReturnType Mcu_Ipr_InitClock(
    const Mcu_ClockConfigSetType * para_ClockConfig_ptr
);
#endif /*(MCU_ENABLE_INIT_CLOCK == STD_ON)*/

#if (MCU_SUPPORT_PLL == STD_ON)
/**
 *  @brief          This function will get the PLL status
 * 
 *  @details        Design ID:         Mcu_Ipr_GetPllStatus_Activity
 *
 *  @return         Mcu_PllStatusType           PLL status information
 *  @retval         MCU_PLL_LOCKED              PLL is locked and stable
 *  @retval         MCU_PLL_UNLOCKED            PLL is unlocked or not stable
 *  @retval         MCU_PLL_STATUS_UNDEFINED    PLL status is unknown
 *
 *  @note           This function will be called by Mcu_GetPllStatus().
 *
 *  @pre            None
 *
 */
Mcu_PllStatusType Mcu_Ipr_GetPllStatus(
    void
);
#endif /*(MCU_SUPPORT_PLL == STD_ON)*/

#if (MCU_SUPPORT_PLL == STD_ON)
/**
 *  @brief          This function will distribute PLL clock
 * 
 *  @details        Design ID:         Mcu_Ipr_DistributePllClock_Activity
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_DistributePllClock().
 *
 *  @pre            None
 *
 */
void Mcu_Ipr_DistributePllClock(
    void
);
#endif /*(MCU_SUPPORT_PLL == STD_ON)*/

/**
 *  @brief          This function will initializes the mode structure of MCU.
 * 
 *  @details        Design ID:         Mcu_Ipr_SetMode_Activity
 *
 *  @param[in]      para_ModeConfig_ptr    Pointer to mode configuration structure.
 *
 *  @return         void
 *
 *  @note           This function initializes the mode.
 *                  Called by Mcu_SetMode().
 *
 *  @pre            None
 *
 */
void Mcu_Ipr_SetMode(
    const Mcu_ModeConfigType * para_ModeConfig_ptr
);

/**
 *  @brief          This function will initializes the WKT Timer.
 * 
 *  @details        Design ID:         Mcu_Ipr_InitWktTimer_Activity
 *
 *  @param[in]      para_WktClockSource_en    WKT Clock Source
 *  @param[in]      para_WktCmpVal_u16        WKT Comparison Value  
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_InitWktTimer().
 *
 *  @pre            None
 *
 */
void Mcu_Ipr_InitWktTimer(
    Mcu_Ipr_WktClockSourceType para_WktClockSource_en,
    uint16_t para_WktCmpVal_u16
);

/**
 *  @brief          This function will initializes a given RAM section
 * 
 *  @details        Design ID:         Mcu_Ipr_InitRamSection_Activity
 *
 *  @param[in]      para_RamConfig_ptr    Pointer to RAM sector configuration structure.
 *
 *  @return         Std_ReturnType    Initialize a given RAM section
 *  @retval         E_OK              Init RAM section successful
 *  @retval         E_NOT_OK          Init RAM section failure
 *
 *  @note           This function will be called by Mcu_InitRamSection().
 *
 *  @pre            None
 *
 */
Std_ReturnType Mcu_Ipr_InitRamSection(
    const Mcu_RamSectorConfigType * para_RamConfig_ptr
);

/**
 *  @brief          This function will get RAM state
 * 
 *  @details        Design ID:         Mcu_Ipr_GetRamState_Activity
 *
 *  @return         Mcu_RamStateType    RAM state information
 *  @retval         MCU_RAM_VALID       RAM content is valid
 *  @retval         MCU_RAM_INVALID     RAM content is invalid
 *
 *  @note           This function will be called by Mcu_GetRamState().
 *
 *  @pre            None
 *
 */
Mcu_RamStateType Mcu_Ipr_GetRamState(
    void
);

/**
 *  @brief          This function performs a microcontroller reset.
 * 
 *  @details        Design ID:         Mcu_Ipr_PerformReset_Activity
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_PerformReset()
 *
 *  @pre            None
 *
 */
void Mcu_Ipr_PerformReset(
    void
);

/**
 *  @brief          This function returns the Reset reason that is read from the hardware
 * 
 *  @details        Design ID:         Mcu_Ipr_GetResetReason_Activity
 *
 *  @return         Mcu_ResetType          Reset reason
 *  @retval         MCU_PO_RESET           This is internal reset caused by the power-on reset circuit.
 *  @retval         MCU_PIN_RESET          This is a reset caused by the NRST pin being driven low
 *  @retval         MCU_BO_RESET           This is an internal reset caused by the voltage monitoring circuit 1.
 *  @retval         MCU_PVD1_RESET         This is reset caused by the voltage monitoring circuit.
 *  @retval         MCU_PVD2_RESET         This is reset caused by the voltage monitoring circuit 1.
 *  @retval         MCU_WD_RESET           This is reset caused by the watchdog timer
 *  @retval         MCU_SWD_RESET          This is reset caused by the dedicated watchdog timer
 *  @retval         MCU_PD_RESET           This is reset generated when the power down wake-up event occurs when
 *                                         device is in power-down mode
 *  @retval         MCU_SW_RESET           This is reset generated by writing the SYSRESETREQ bit of ARM register AIRCR.
 *  @retval         MCU_MPUE_RESET         This is reset generated when an MPU error occurs.
 *  @retval         MCU_RAPE_RESET         This is reset generated when an error in the RAM parity check
 *  @retval         MCU_RAEC_RESET         This is reset generated when an error in the RAM ECC check
 *  @retval         MCU_CKFE_RESET         This is reset generated when the built-in FCM module detects that the
 *                                         clock frequency is abnormal, and it's configured as reset clock source.
 *  @retval         MCU_XTALE_RESET        This is reset generated when the external high-speed oscillator fault
 *                                         detection function is enable and configured as reset mode.
 *  @retval         MCU_LKUP_RESET         This is reset generated when Cortex-M4 encounter a serious exception.
 *  @retval         MCU_RESET_UNDEFINED    Unknow.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Mcu_ResetType Mcu_Ipr_GetResetReason(
    void
);

/**
 *  @brief          This function returns the reset reason that is read from the hardware.
 * 
 *  @details        Design ID:         Mcu_Ipr_GetResetRawValue_Activity
 *
 *  @return         Mcu_RawResetType    Raw reset value from hardware register
 *
 *  @note           This function will be called by Mcu_GetResetRawValue().
 *
 *  @pre            None
 *
 */
Mcu_RawResetType Mcu_Ipr_GetResetRawValue(
    void
);

#if (STD_ON == MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API)
/**
 *  @brief          This function performs module software reset.
 *
 *  @details        Design ID:         Mcu_Ipr_PerformSoftwareModuleReset_Activity
 *
 *  @param[in]      SwModuleReset      Software module reset selection
 *
 *  @return         Std_ReturnType    Perform module software reset
 *  @retval         E_OK              Performs module software reset was successful
 *  @retval         E_NOT_OK          Performs module software reset was not successful
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Mcu_Ipr_PerformSoftwareModuleReset(
    Mcu_ModuleType SwModuleReset
);
#endif /*!< (STD_ON == MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API) */

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* MCU_IPR_H */

/*---------------------- End of File ----------------------------------------*/
