/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu.c
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
 *   @file       Mcu.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

/***********************************************************************************************************************
 *  MISRA VIOLATIONS - Summary and analysis.
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/**
 * @brief Mcu.c shall include Mcu.h
 */
#include "Mcu.h"
/* Retrieve the prototypes of IPR functions. */
#include "Mcu_Ipr.h"
#include "SchM_Mcu.h"

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

#if (MCU_CLOCK_SOURCE_FAILURE_NOTIFICATION == STD_ON)
/* Included for the declaration of Dem Report Error */
#include "Dem.h"
#endif

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/
/* Check static code version with generated code version */
#if ((MCU_SW_MAJOR_VERSION != MCU_CFG_SW_MAJOR_VERSION) || \
     (MCU_SW_MINOR_VERSION != MCU_CFG_SW_MINOR_VERSION) || \
     (MCU_SW_PATCH_VERSION != MCU_CFG_SW_PATCH_VERSION) \
    )
    #error "static code and generated code have different software release version"
#endif

#ifndef DISABLE_EXTERNAL_VERSION_CHECK
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  /* Check AutoSar Version Numbers of Mcu.c and Det.h */
  #if ((MCU_AR_RELEASE_MAJOR_VERSION != DET_AR_RELEASE_MAJOR_VERSION) || \
   (MCU_AR_RELEASE_MINOR_VERSION != DET_AR_RELEASE_MINOR_VERSION))
    #error "Inconsistent AUTOSAR Version Numbers of Det.h and Mcu.c"
#endif
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

#if (MCU_CLOCK_SOURCE_FAILURE_NOTIFICATION == STD_ON)
  /* Check AutoSar Version Numbers of Mcu.c and Dem.h */
  #if ((MCU_AR_RELEASE_MAJOR_VERSION != DEM_AR_RELEASE_MAJOR_VERSION) || \
   (MCU_AR_RELEASE_MINOR_VERSION != DEM_AR_RELEASE_MINOR_VERSION))
    #error "Inconsistent AUTOSAR Version Numbers of Dem.h and Mcu.c"
  #endif
#endif

  /* Check AutoSar Version Numbers Mcu.c and SchM_Mcu.h */
  #if ((MCU_AR_RELEASE_MAJOR_VERSION != SCHM_MCU_AR_RELEASE_MAJOR_VERSION) || \
       (MCU_AR_RELEASE_MINOR_VERSION != SCHM_MCU_AR_RELEASE_MINOR_VERSION) \
      )
    #error "AutoSar Version Numbers of Mcu.c and SchM_Mcu.h are not consistent"
  #endif
#endif /* DISABLE_EXTERNAL_VERSION_CHECK */

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

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 *  @brief Local variable used for storing the MCU driver configuration data.
 */
static const Mcu_ConfigType * l_McuConfig_ptr;
#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

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
 *
 */
void Mcu_Init(
    const Mcu_ConfigType * ConfigPtr
)
{

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    /* When PostBuild is used and #(Variants) > 1, the input parameter 'ConfigPtr' is mandatory
    * to be different than NULL_PTR.
    * In case of error, return immediately and report DET errors. */
  #if (MCU_PRECOMPILE_ENABLE == STD_OFF)
    if (NULL_PTR != ConfigPtr)
  #else
    if (NULL_PTR == ConfigPtr)
  #endif /* MCU_PRECOMPILE_ENABLE == STD_OFF */
    {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

#if (MCU_PRECOMPILE_ENABLE == STD_ON)
      l_McuConfig_ptr = &Mcu_PreCompileConfig;
      (void)(ConfigPtr);
#else
      l_McuConfig_ptr = ConfigPtr;
#endif  /* (MCU_PRECOMPILE_ENABLE == STD_ON) */
      /* Init the Mcu module */
      Mcu_Ipr_Init(l_McuConfig_ptr->Mcu_HwIPsConfig_Ptr);
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    }
    else
    {
      /* Clear the initialization pointer in the event of an error */
      (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT, MCU_E_INIT_FAILED);
    }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
}

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
 *
 */
Std_ReturnType Mcu_InitRamSection(
  Mcu_RamSectionType  RamSection
)
{
Std_ReturnType  f_RamStatus_u8 = (Std_ReturnType)E_NOT_OK;
#ifndef MCU_MAX_NORAMCONFIGS
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
if (NULL_PTR != l_McuConfig_ptr)
{
  /* Check if Ram configuration is valid. */
  if (RamSection < l_McuConfig_ptr->McuNumberOfRamConfigs_u32)
  {
    if (NULL_PTR != l_McuConfig_ptr->McuRamSectorConfigList_ptr)
    {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
      f_RamStatus_u8 = Mcu_Ipr_InitRamSection(&(*l_McuConfig_ptr->McuRamSectorConfigList_ptr)[RamSection]);
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    }
    else
    {
      (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_RAMSECTION, MCU_E_PARAM_RAMSECTION);
    }
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_RAMSECTION, MCU_E_PARAM_RAMSECTION);
  }
}
else
{
  (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_RAMSECTION, MCU_E_UNINIT);
}
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
#endif /* #ifndef MCU_MAX_NORAMCONFIGS */
return f_RamStatus_u8;
}

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
 *
 */
Std_ReturnType Mcu_InitClock(
    Mcu_ClockType  ClockSetting
)
{
  /* Return the success status of the clock initialization operation */
  Std_ReturnType  f_ClockStatus_u8 = (Std_ReturnType)E_NOT_OK;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
    /* Check if input is in range. */
    if (ClockSetting < l_McuConfig_ptr->McuNumberOfClockConfig_u8)
    {
#endif /* (MCU_PARAM_CHECK == STD_ON) */
      f_ClockStatus_u8 = Mcu_Ipr_InitClock(&(*l_McuConfig_ptr->McuClockConfigSet_ptr)[ClockSetting]);
      #if (MCU_CLOCK_SOURCE_FAILURE_NOTIFICATION == STD_ON)
      if (E_NOT_OK == f_ClockStatus_u8)
      {
        (void)Dem_SetEventStatus(MCU_E_CLOCK_FAILURE, DEM_EVENT_STATUS_FAILED);
      }
      #endif /* #if (MCU_CLOCK_SOURCE_FAILURE_NOTIFICATION == STD_ON) */
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    }
    else
    {
      (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_CLOCK, MCU_E_PARAM_CLOCK);
    }
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_CLOCK, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  return f_ClockStatus_u8;
}
#endif /* (MCU_ENABLE_INIT_CLOCK == STD_ON) */

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
 *
 */
Std_ReturnType Mcu_DistributePllClock(
    void
)
{
  Std_ReturnType  f_PllStatus_u8 = (Std_ReturnType)E_NOT_OK;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
    /* Verify if the PLL is in a locked state */
    if (MCU_PLL_LOCKED == Mcu_Ipr_GetPllStatus())
    {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

      /* distribute pll clock */
      Mcu_Ipr_DistributePllClock();
      f_PllStatus_u8 = (Std_ReturnType)E_OK;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    }
    else
    {
      (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_DISTRIBUTE_PLLCLOCK, MCU_E_PLL_NOT_LOCKED);
    }
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_DISTRIBUTE_PLLCLOCK, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  /* Return value */
  return f_PllStatus_u8;
}
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
 *
 */
Mcu_PllStatusType Mcu_GetPllStatus(
    void
)
{
  Mcu_PllStatusType  f_PllStatus_en = MCU_PLL_STATUS_UNDEFINED;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

    /* Return MCU_PLL_STATUS_UNDEFINED in case PLL is not used. */
#if (MCU_SUPPORT_PLL == STD_ON)
    /* Get status of the PLL. */
    f_PllStatus_en = Mcu_Ipr_GetPllStatus();
#endif /* (MCU_SUPPORT_PLL == STD_ON) */

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_GET_PLLSTATUS, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  return f_PllStatus_en;
}
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
)
{
Mcu_ResetType  f_ResetReason_en = MCU_RESET_UNDEFINED;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
if (NULL_PTR != l_McuConfig_ptr)
{
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  /* Retrieve the reset reason */
  f_ResetReason_en = Mcu_Ipr_GetResetReason();

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
}
else
{
  (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_GET_RESET_REASON, MCU_E_UNINIT);
}
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */

return f_ResetReason_en;
}

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
*
*/
Mcu_RawResetType Mcu_GetResetRawValue(
  void
)
{
Mcu_RawResetType  f_RawResetValue_u32 = MCU_INVALID_RAW_RESET_VALUE;

#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
if (NULL_PTR != l_McuConfig_ptr)
{
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  f_RawResetValue_u32 = (Mcu_RawResetType)Mcu_Ipr_GetResetRawValue();
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
}
else
{
  (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_GET_RESET_RAWVALUE, MCU_E_UNINIT);
}
#endif /*( MCU_ENABLE_DEV_ERROR_DETECT == STD_ON )*/

return f_RawResetValue_u32;
}

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
)
{
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
    Mcu_Ipr_PerformReset();
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_PERFORM_RESET, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
}
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
 *
 */
void Mcu_SetMode(
    Mcu_ModeType  McuMode
)
{
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
    /* Verify if the mode configuration is valid */
    if (McuMode < l_McuConfig_ptr->McuNumberOfMcuModes_u8)
    {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
      SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_00();
      Mcu_Ipr_SetMode(&(*l_McuConfig_ptr->McuModeConfig_Ptr)[McuMode]);
      SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_00();
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
    }
    else
    {
      (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_SET_MODE, MCU_E_PARAM_MODE);
    }
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_SET_MODE, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
}

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
 *
 */
void Mcu_GetVersionInfo(
    Std_VersionInfoType * versioninfo
)
{
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != versioninfo)
  {
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
    versioninfo->vendorID         = (uint16_t)MCU_VENDOR_ID;
    versioninfo->moduleID         = (uint8_t) MCU_MODULE_ID;
    versioninfo->sw_major_version = (uint8_t) MCU_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8_t) MCU_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8_t) MCU_SW_PATCH_VERSION;
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  }
  else
  {
    (void)Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_GET_VERSIONINFO, MCU_E_PARAM_POINTER);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
}
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
 *
 */
Mcu_RamStateType Mcu_GetRamState(
    void
)
{
  Mcu_RamStateType  f_RamStatus_en = MCU_RAMSTATE_INVALID;
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR != l_McuConfig_ptr)
  {
#endif /*( MCU_ENABLE_DEV_ERROR_DETECT == STD_ON )*/
    f_RamStatus_en = (Mcu_RamStateType) Mcu_Ipr_GetRamState();
#if (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON)
  }
  else
  {
    (void) Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_GET_RAMSTATE, MCU_E_UNINIT);
  }
#endif /* (MCU_ENABLE_DEV_ERROR_DETECT == STD_ON) */
  return f_RamStatus_en;
}
#endif /* #if (MCU_ENABLE_GET_RAM_STATE_API == STD_ON) */

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
)
{
    /* Declare and initialize Ram status variable */
    Std_ReturnType RetStatus_en = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT)
    if ((uint8_t)MCU_PERIPH_USART10 >= (uint8_t)Module)
    {
#endif /*!< (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT )*/
        /* Call to lower layer to perform software module reset */
        RetStatus_en = Mcu_Ipr_PerformSoftwareModuleReset(Module);
#if (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT)
    }
    else
    {
        (void)Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_PERFORM_SOFTWARE_MODULE_RESET, MCU_E_PARAM_CONFIG);
    }
#endif /*!< (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT )*/
    /* Return status */
    return RetStatus_en;
}
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
)
{
    Std_ReturnType RetStatus_en = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT)
    if (((uint8_t)MCU_WKT_CLK_SRC_RTCLRC >= (uint8_t)WktClockSource_en) && ((uint16_t)0xFFF >= WktCmpVal_u16))
    {
#endif /*!< (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT )*/
        Mcu_Ipr_InitWktTimer(WktClockSource_en, WktCmpVal_u16);

#if (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT)
    }
    else
    {
        (void)Det_ReportError((uint16_t)MCU_MODULE_ID, MCU_INSTANCE_ID, MCU_SID_INIT_WKT_TIMER, MCU_E_PARAM_CONFIG);
    }
#endif /*!< (STD_ON == MCU_ENABLE_DEV_ERROR_DETECT )*/

    return RetStatus_en;
}


#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
