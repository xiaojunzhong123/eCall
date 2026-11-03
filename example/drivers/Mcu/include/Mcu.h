/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu.h
 *
 * Description: Implementation of MCU High Level layer
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
 *   @file       Mcu.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef MCU_H
#define MCU_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Mcu_Cfg.h"
#include "Clock_Ip_Types.h"
#include "Power_Ip_Types.h"
#include "Ram_Ip_Types.h"
#include "Reset_Ip_Types.h"
#include "Mcu_Ipr_Types.h"
#include "Power_Ip_Irq.h"
#include "Clock_Ip_Irq.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define MCU_VENDOR_ID                     168
#define MCU_MODULE_ID                     101
#define MCU_INSTANCE_ID                   0
#define MCU_SW_MAJOR_VERSION              1
#define MCU_SW_MINOR_VERSION              0
#define MCU_SW_PATCH_VERSION              3
#define MCU_AR_RELEASE_MAJOR_VERSION      4
#define MCU_AR_RELEASE_MINOR_VERSION      4
#define MCU_AR_RELEASE_REVISION_VERSION   0

/**
 *  @brief   Mcu_Init API function represents API service ID.
 */
#define MCU_SID_INIT                    ((uint8_t)0x00U)

/**
 *  @brief    Mcu_InitRamSection API function represents API service ID
 *           Parameters used  to raise an error/exception.
 */
#define MCU_SID_INIT_RAMSECTION         ((uint8_t)0x01U)

/**
 *  @brief   Mcu_InitClock API function represents API service ID.
 */
#define MCU_SID_INIT_CLOCK              ((uint8_t)0x02U)

/**
 *  @brief   Mcu_DistributePllClock API function represents API service ID.
 */
#define MCU_SID_DISTRIBUTE_PLLCLOCK     ((uint8_t)0x03U)

/**
 *  @brief   Mcu_GetPllStatus API function represents API service ID.
 */
#define MCU_SID_GET_PLLSTATUS           ((uint8_t)0x04U)

/**
 *  @brief   Mcu_GetResetReason API function represents API service ID.
 */
#define MCU_SID_GET_RESET_REASON        ((uint8_t)0x05U)

/**
 *  @brief   Mcu_GetResetRawValue API function represents API service ID.
 */
#define MCU_SID_GET_RESET_RAWVALUE      ((uint8_t)0x06U)

/**
 *  @brief   Mcu_PerformReset API function represents API service ID.
 */
#define MCU_SID_PERFORM_RESET           ((uint8_t)0x07U)

/**
 *  @brief   Mcu_SetMode API function represents API service ID.
 */
#define MCU_SID_SET_MODE                ((uint8_t)0x08U)

/**
 *  @brief   Mcu_GetVersionInfo API function represents API service ID.
 */
#define MCU_SID_GET_VERSIONINFO         ((uint8_t)0x09U)

/**
 *  @brief   Mcu_GetRamState API function represents API service ID.
 */
#define MCU_SID_GET_RAMSTATE            ((uint8_t)0x0AU)

/**
 *  @brief   Mcu_PerformSoftwareModuleReset API function represents API service ID.
 */
#define MCU_SID_PERFORM_SOFTWARE_MODULE_RESET ((uint8_t)0x0BU)

/**
 *  @brief   Mcu_InitWktTimer API function represents API service ID.
 */         
#define MCU_SID_INIT_WKT_TIMER          ((uint8_t)0x0CU)

/**
 *  @brief   Invalid range for input parameter to input to API.
 */
#define MCU_E_PARAM_CONFIG            ((uint8_t)0x0AU)

/**
 *  @brief   DET Code for invalid Clock Setting.
 */
#define MCU_E_PARAM_CLOCK             ((uint8_t)0x0BU)

/**
 *  @brief   DET Code for invalid Operation Mode.
 */
#define MCU_E_PARAM_MODE              ((uint8_t)0x0CU)

/**
 *  @brief   DET Code for invalid RAM Section handle.
 */
#define MCU_E_PARAM_RAMSECTION        ((uint8_t)0x0DU)

/**
 *  @brief   DET Code to report that PLL Clock is not locked.
 */
#define MCU_E_PLL_NOT_LOCKED          ((uint8_t)0x0EU)

/**
 *  @brief   DET code to report uninitialized state.
 */
#define MCU_E_UNINIT                  ((uint8_t)0x0FU)

/**
 *  @brief   DET code to report NULL_POINTER error.
 */
#define MCU_E_PARAM_POINTER           ((uint8_t)0x10U)

/**
 *  @brief   DET Code to report NULL pointer passed to Mcu_Init API.
 */
#define MCU_E_INIT_FAILED             ((uint8_t)0x11U)

/**
 *  @brief   Application specific value returned by Mcu_GetResetRawValue() if invoked before Mcu_Init().
 */
#define MCU_INVALID_RAW_RESET_VALUE   ((uint32_t)0xFFFFFFFFU)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   Mcu_Ipr_SetMode() API receive parameter is Mcu_ModeConfigType This configuration is transmitted.
 */
typedef Power_Ip_HwModeConfigType             Mcu_ModeConfigType;

/**
 *  @brief   Mcu clock type's Mcu_ClockConfigType Struct configures for.
 */
typedef Clock_Ip_ClockHwConfigType            Mcu_ClockConfigType;

/**
 *  @brief    Mcu clock type's Mcu_FuncClockControlType Struct configures.
 */
typedef Clock_Ip_FuncClockControlType       Mcu_FuncClockControlType;

/**
 *  @brief   Mcu clock type's  Mcu_ClockType Struct.
 *  @details SRS ID:     SWS_Mcu_00232,SWS_Mcu_00233,SWS_Mcu_00251
 */
typedef Clock_Ip_ClockType                  Mcu_ClockType;

/**
 *  @brief   Mcu clock set type's Mcu_ClockConfigSetType Struct configures.
 */
typedef Clock_Ip_ClockConfigSetType         Mcu_ClockConfigSetType;

/**
 *  @brief    ram sector typedef struct configure.
 */
typedef Ram_Ip_HwRamConfigType                Mcu_RamSectorConfigType;

/**
 *  @brief    Mcu mode typedef configure.
 *  @details SRS ID:     SWS_Mcu_00237,SWS_Mcu_00238,SWS_Mcu_00254
 */
typedef Power_Ip_ModeType                   Mcu_ModeType;

/**
 *  @brief   This is PVD content in configuration structure.
 */
typedef Power_Ip_HwConfigType            Mcu_HwConfigType;

/**
 *  @brief   This is configuration structure for Pll status.
 *  @details SRS ID:     SWS_Mcu_00230,SWS_Mcu_00231,SWS_Mcu_00250
 */
typedef Clock_Ip_PllStatus                  Mcu_PllStatusType;

/**
 *  @brief   This is Ram state content in configuration structure.
 *  @details SRS ID:     SWS_Mcu_00256
 */
typedef Ram_Ip_StateType                    Mcu_RamStateType;

/**
 *  @brief   Mcu RawReset Reset register is 32bit register.
 *  @details SRS ID:     SWS_Mcu_00235,SWS_Mcu_00236,SWS_Mcu_00253
 */
typedef uint32_t                              Mcu_RawResetType;

/**
 *  @brief   The Mcu_RamSectionType specifies the identification (ID) for a RAM section, configured via the configuration structure.
 *          The type shall be uint8_t, uint16_t or uint32_t, based on best performance.
 *  @details SRS ID:     SWS_Mcu_00239,SWS_Mcu_00240,SWS_Mcu_00255
 */
typedef uint32_t                              Mcu_RamSectionType;

/**
 *  @brief   Define different peripheral for software module reset.
 */
typedef Mcu_Ipr_ModuleType                  Mcu_ModuleType;

/**
 *  @brief   This is the type of the WKT Clock Source enumerator.
 */
typedef Mcu_Ipr_WktClockSourceType          Mcu_WktClockSourceType;

/**
 *  @brief   This is the type of the reset enumerator containing the subset of reset types. It is not
 *           required that all reset types are supported by hardware.
 *  @details SRS ID:     SWS_Mcu_00134,SWS_Mcu_00234,SWS_Mcu_00252
 */
typedef Mcu_Ipr_ResetType Mcu_ResetType;

/**
 *  @brief  Mcu Config Configuration type structure
 *  @details SRS ID:     SWS_Mcu_00030,SWS_Mcu_00031,SWS_Mcu_00035
 *                       SWS_Mcu_00054,SWS_Mcu_00131,SWS_Mcu_00249
 */
typedef struct {
#if (MCU_CLOCK_FAIL_PB_CONFIGURATION == STD_ON)
  boolean McuClockSrcFailureNotification_bool;                               /*!< Switch to configure Clock source Failure notification. */
#endif /* (MCU_CLOCK_FAIL_PB_CONFIGURATION == STD_ON) */
#if (MCU_ENABLE_INIT_CLOCK == STD_ON)
  Mcu_ClockType McuNumberOfClockConfig_u8;                                   /*!< Total number of MCU clock configurations. */
  const Mcu_ClockConfigSetType (*McuClockConfigSet_ptr)[MCU_NUMBER_OF_CLKCONFIGS]; /*!< This is a pointer to array of "Mcu_ClockConfigType" structures */
#endif /* (MCU_ENABLE_INIT_CLOCK == STD_ON) */
  Mcu_RamSectionType McuNumberOfRamConfigs_u32;                              /*!< Total number of RAM sections. */
#ifndef MCU_MAX_NORAMCONFIGS
  const Mcu_RamSectorConfigType (*McuRamSectorConfigList_ptr)[MCU_NUMBER_OF_RAMCONFIGS]; /*!< RAM data configuration. */
#endif /* (MCU_MAX_NORAMCONFIGS == STD_ON) */
  Mcu_ModeType McuNumberOfMcuModes_u8;                                             /*!< Number of configured MCU Modes. */
  const Mcu_ModeConfigType (*McuModeConfig_Ptr)[MCU_NUMBER_OF_MODECONFIGS];              /*!< Power Modes data configuration. */
  const Mcu_HwConfigType *Mcu_HwIPsConfig_Ptr;                                  /*!< IPs data generic configuration. */
} Mcu_ConfigType;

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"
#if (MCU_PRECOMPILE_ENABLE == STD_OFF)
/**
 *  @brief  Export Post-Build configurations.
 */
MCU_CONFIG_DECLARE
#endif /* (MCU_PRECOMPILE_ENABLE == STD_OFF) */
#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANTS
 **********************************************************************************************************************/
#if (MCU_PRECOMPILE_ENABLE == STD_ON)
#define MCU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"
extern const Mcu_ConfigType Mcu_PreCompileConfig;
#define MCU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Mcu_MemMap.h"
#endif /* #if (MCU_PRECOMPILE_ENABLE == STD_ON) */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This service initializes the MCU driver.
 *
 *  @details        Service ID:        0x00
 *                  SRS ID:            SWS_Mcu_00011,SWS_Mcu_00116,SWS_Mcu_00026,SWS_Mcu_00125
 *                                     SWS_Mcu_00132,SWS_Mcu_00133,SWS_Mcu_00135,SWS_Mcu_00136
 *                                     SWS_Mcu_00137,SWS_Mcu_00138,SWS_Mcu_00139,SWS_Mcu_00145
 *                                     SWS_Mcu_00148,SWS_Mcu_00226,SWS_Mcu_00153,SWS_Mcu_00154
 *                                     SWS_Mcu_00155,SWS_Mcu_00248,SWS_Mcu_00210,SWS_Mcu_00163
 *                                     SWS_Mcu_00126,SWS_Mcu_00208,SWS_Mcu_00247,SWS_Mcu_00153
 *                  Design ID:         SDD_MCU_91
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      ConfigPtr    Pointer to MCU driver configuration set
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 */
void Mcu_Init(
    const Mcu_ConfigType * ConfigPtr
);

/**
 *  @brief          This service initializes the RAM section wise.
 *
 *  @details        Service ID:        0x01
 *                  SRS ID:            SWS_Mcu_00011,SWS_Mcu_00136,SWS_Mcu_00154,SWS_Mcu_00163
 *                                     SWS_Mcu_00017,SWS_Mcu_00021,SWS_Mcu_00226,SWS_Mcu_00152
 *                  Design ID:         SDD_MCU_92
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      RamSection    Selects RAM memory section provided in configuration set
 *
 *  @return         Std_ReturnType    Provides the return state
 *
 *  @note           None
 *
 *  @pre            None
 */
Std_ReturnType Mcu_InitRamSection(
    Mcu_RamSectionType  RamSection
);

#if (MCU_ENABLE_INIT_CLOCK == STD_ON)
/**
 *  @brief          This service initializes the Pll and other MCU specific clock options.
 *
 *  @details        Service ID:        0x02
 *                  SRS ID:            SWS_Mcu_00137,SWS_Mcu_00138,SWS_Mcu_00139,SWS_Mcu_00163,SWS_Mcu_00017
 *                                     SWS_Mcu_00053,SWS_Mcu_00257,SWS_Mcu_00051,SWS_Mcu_00152,SWS_Mcu_00155
 *                                     SWS_Mcu_00210,SWS_Mcu_00012,SWS_Mcu_00166,SWS_Mcu_00019,SWS_Mcu_00020
 *                                     SWS_Mcu_00258,SWS_Mcu_00226,SWS_Mcu_00244,SWS_Mcu_00245
 *                  Design ID:         SDD_MCU_93
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      ClockSetting    Clock setting
 *
 *  @return         Std_ReturnType    Provides the return state
 *  @retval         E_OK              Clock was initialized successful
 *  @retval         E_NOT_OK          Clock was not initialized successful
 *
 *  @note           None
 *
 *  @pre            None
 */
Std_ReturnType Mcu_InitClock(
    Mcu_ClockType  ClockSetting
);
#endif /* (MCU_ENABLE_INIT_CLOCK == STD_OFF) */

#if (MCU_SUPPORT_PLL == STD_ON)
/**
 *  @brief          This service activates the Pll clock to the MCU clock distribution.
 *
 *  @details        Service ID:        0x03
 *                  SRS ID:            SWS_Mcu_00056,SWS_Mcu_00122,SWS_Mcu_00140,SWS_Mcu_00017,SWS_Mcu_00226
 *                                     SWS_Mcu_00141,SWS_Mcu_00142,SWS_Mcu_00156,SWS_Mcu_00163,SWS_Mcu_00152
 *                                     SWS_Mcu_00205
 *                  Design ID:         SDD_MCU_94
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         Std_ReturnType    Provides the return state
 *  @retval         E_OK              Distribute Pll Clock was successful
 *  @retval         E_NOT_OK          Distribute Pll Clock was not successful
 *
 *  @note           None
 *
 *  @pre            None
 */
Std_ReturnType Mcu_DistributePllClock(
    void
);
#endif /* (MCU_SUPPORT_PLL == STD_ON) */

#if (MCU_SUPPORT_PLL == STD_ON)
/**
 *  @brief          This service provides the lock status of the Pll.
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            SWS_Mcu_00008,SWS_Mcu_00132,SWS_Mcu_00157,SWS_Mcu_00226
 *                                     SWS_Mcu_00206,SWS_Mcu_00230,SWS_Mcu_00163
 *                  Design ID:         SDD_MCU_95
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         Mcu_PllStatusType           Provides the lock status of the Pll
 *  @retval         MCU_PLL_STATUS_UNDEFINED    Pll Status is unknown
 *  @retval         MCU_PLL_LOCKED              Pll is locked
 *  @retval         MCU_PLL_UNLOCKED            Pll is unlocked
 *
 *  @note           None
 *
 *  @pre            None
 */
Mcu_PllStatusType Mcu_GetPllStatus(
    void
);
#endif /* (MCU_SUPPORT_PLL == STD_ON) */

/**
 *  @brief          The service reads the reset type from the hardware, if supported.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            SWS_Mcu_00005,SWS_Mcu_00133,SWS_Mcu_00158,SWS_Mcu_00163,SWS_Mcu_00052,SWS_Mcu_00226
 *                  Design ID:         SDD_MCU_96
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         Mcu_ResetType          Reset reason from hardware
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
 *  @retval         MCU_RESET_UNDEFINED    Unknown.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Mcu_ResetType Mcu_GetResetReason(
    void
);

/**
 *  @brief          The service reads the reset type from the hardware register, if supported.
 *
 *  @details        Service ID:        0x06
 *                  SRS ID:            SWS_Mcu_00006,SWS_Mcu_00135,SWS_Mcu_00159,SWS_Mcu_00163,SWS_Mcu_00226
 *                  Design ID:         SDD_MCU_97
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         Mcu_RawResetType    Description of the returned value
 *  @retval         uint32_t              Code of the Raw reset value
 *
 *  @note           None
 *
 *  @pre            None
 */
Mcu_RawResetType Mcu_GetResetRawValue(
    void
);

#if (MCU_ENABLE_PERFORM_RESET_API == STD_ON)
/**
 *  @brief          The service performs a microcontroller reset.
 *
 *  @details        Service ID:        0x07
 *                  SRS ID:            SWS_Mcu_00055,SWS_Mcu_00143,SWS_Mcu_00144,SWS_Mcu_00226
 *                                     SWS_Mcu_00145,SWS_Mcu_00146,SWS_Mcu_00160,SWS_Mcu_00163
 *                                     SWS_Mcu_00246
 *                  Design ID:         SDD_MCU_98
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         void
 *
 *  @note           This function performs a microcontroller reset.
 *
 *  @pre            None
 *
 */
void Mcu_PerformReset(
    void
);
#endif /* (MCU_ENABLE_PERFORM_RESET_API == STD_ON) */

/**
 *  @brief          This service activates the MCU power modes.
 *
 *  @details        Service ID:        0x08
 *                  SRS ID:            SWS_Mcu_00147,SWS_Mcu_00148,SWS_Mcu_00161,SWS_Mcu_00163
 *                                     SWS_Mcu_00165,SWS_Mcu_00226,SWS_Mcu_00164
 *                  Design ID:         SDD_MCU_99
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      McuMode    Set different MCU power modes configured in the configuration set
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 */
void Mcu_SetMode(
    Mcu_ModeType  McuMode
);

#if (MCU_ENABLE_VERSION_INFO_API == STD_ON)
/**
 *  @brief          This service returns the version information of this module.
 *
 *  @details        Service ID:        0x09
 *                  SRS ID:            SWS_Mcu_00125,SWS_Mcu_00162,SWS_Mcu_00163,SWS_Mcu_00226
 *                  Design ID:         SDD_MCU_100
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      versioninfo    Pointer to where to store the version information of this module
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 */
void Mcu_GetVersionInfo(
    Std_VersionInfoType * versioninfo
);
#endif /* (MCU_ENABLE_VERSION_INFO_API == STD_ON) */

#if (MCU_ENABLE_GET_RAM_STATE_API == STD_ON)
/**
 *  @brief          This service provides the actual status of the microcontroller Ram.
 *
 *  @details        Service ID:        0x0A
 *                  SRS ID:            SWS_Mcu_00207,SWS_Mcu_00209,SWS_Mcu_00163,SWS_Mcu_00226
 *                  Design ID:         SDD_MCU_101
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @return         Mcu_RamStateType        Status of the Ram Content
 *  @retval         MCU_RAMSTATE_INVALID    Ram state is not valid or unknown (default)
 *  @retval         MCU_RAMSTATE_VALID      Ram state is valid
 *
 *  @note           None
 *
 *  @pre            None
 */
Mcu_RamStateType Mcu_GetRamState(
    void
);
#endif /* (MCU_ENABLE_GET_RAM_STATE_API == STD_ON) */

#if (STD_ON == MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API)
/**
 *  @brief          The service performs software module reset.
 *
 *  @details        Service ID:        0x0BU
 *                  SRS ID:            AR_RS_MCU_00001   
 *                  Design ID:         SDD_MCU_102
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      Module    Module to be reset
 *
 *  @return         Std_ReturnType    Status of software module reset operation
 *  @retval         E_OK              Software module reset was successful
 *  @retval         E_NOT_OK          Software module reset was not successful
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Mcu_PerformSoftwareModuleReset(
    Mcu_ModuleType Module
);
#endif /*!< (STD_ON == MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API) */

/**
 *  @brief          This service performs initializes the WKT Timer.
 * 
 *  @details        Service ID:        0x0CU
 *                  SRS ID:            AR_RS_MCU_00002
 *                  Design ID:         SDD_MCU_103
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non-Reentrant
 *
 *  @param[in]      WktClockSource_en    WKT Clock Source
 *  @param[in]      WktCmpVal_u16        WKT Comparison Value  
 *
 *  @return         Std_ReturnType    Status of WKT Timer initialization
 *  @retval         E_OK              WKT Timer initialization was successful
 *  @retval         E_NOT_OK          WKT Timer initialization was not successful
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Mcu_InitWktTimer(
    Mcu_WktClockSourceType WktClockSource_en,
    uint16_t WktCmpVal_u16
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* MCU_H */

/*---------------------- End of File ----------------------------------------*/
