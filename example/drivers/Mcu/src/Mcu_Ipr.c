/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Mcu_Ipr.c
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
 *   @file       Mcu_Ipr.c
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
#include "Mcu.h"
#include "Mcu_Ipr.h"
#include "Power_Ip.h"
#include "Reset_Ip.h"
#include "Clock_Ip.h"
#include "Ram_Ip.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define MCU_START_SEC_CONST_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 *  @brief          Definition of  all status reset reason.
 */
static const Mcu_ResetType s_ResetReason_aa[] = 
{
  MCU_PO_RESET,
  MCU_PIN_RESET,
  MCU_BO_RESET,
  MCU_PVD1_RESET,
  MCU_PVD2_RESET,
  MCU_WD_RESET,
  MCU_SWD_RESET,
  MCU_PD_RESET,
  MCU_SW_RESET,
  MCU_MPUE_RESET,
  MCU_RAPE_RESET,
  MCU_RAEC_RESET,
  MCU_CKFE_RESET,
  MCU_XTALE_RESET,
  MCU_LKUP_RESET,
  MCU_FLEC_RESET,
  MCU_ERMU_RESET,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_RESET_UNDEFINED,
  MCU_MULTIPLE_RESET,
  MCU_RESET_UNDEFINED
};

#define MCU_STOP_SEC_CONST_UNSPECIFIED
#include "Mcu_MemMap.h"
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
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 *  @brief Local variable used for storing the MCU driver configures for Mcu clock set type.
 */
static const Mcu_ClockConfigSetType* l_McuClockConfig_ptr;
#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"
static void Mcu_Ipr_Unlock_Periph(
    void
);

static void Mcu_Ipr_Lock_Periph(
    void
);
#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function UnLock Periph
 * 
 *  @details        Design ID:         Mcu_Ipr_Unlock_Periph_Activity
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_Ipr_Init().
 *
 *  @pre            None
 *
 */
static void Mcu_Ipr_Unlock_Periph(
    void
)
{
  /* Unlock CMU Register */
  SET_REG16_BIT(CM_PWC->FPRC, (PWC_UNLOCK_CODE0 | PWC_UNLOCK_CODE1 | PWC_UNLOCK_CODE3));
  /* Unlock PWR_FCG0 register. */
  WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_UNLOCK_KEY);
  /* Unlock SRAM registers, write enable */
  WRITE_REG32(CM_SRAMC->WTPR, MCU_SRAM_REG_UNLOCK_KEY);
  WRITE_REG32(CM_SRAMC->CKPR, MCU_SRAM_REG_UNLOCK_KEY);
  /* Unlock all EFM registers */
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY1);
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY2);
  /* Unlock all GPIO registers */
  WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_UNLOCK_KEY);
  /* Unlock all MPU registers */
  WRITE_REG32(CM_MPU->WP, MPU_REG_UNLOCK_KEY);
}

/**
 *  @brief          This function Lock Periph
 * 
 *  @details        Design ID:         Mcu_Ipr_Lock_Periph_Activity
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_Ipr_Init().
 *
 *  @pre            None
 *
 */
static void Mcu_Ipr_Lock_Periph(
    void
)
{
  /* Lock CMU Register */
  CM_PWC->FPRC = (PWC_WRITE_ENABLE | (~(uint16_t)(PWC_UNLOCK_CODE0 | \
          PWC_UNLOCK_CODE1 | PWC_UNLOCK_CODE3) & CM_PWC->FPRC));
  /* Lock PWC_FCG0 register. */
  WRITE_REG32(CM_PWC->FCG0PC, PWC_FCG0_REG_LOCK_KEY);
  /* Lock SRAM registers, write protect */
  WRITE_REG32(CM_SRAMC->WTPR, MCU_SRAM_REG_LOCK_KEY);
  WRITE_REG32(CM_SRAMC->CKPR, MCU_SRAM_REG_LOCK_KEY);
  /* Lock all EFM registers */
  WRITE_REG32(CM_EFM->FAPRT, EFM_REG_LOCK_KEY);
  /* Lock all GPIO registers */
  WRITE_REG16(CM_GPIO->PWPR, GPIO_REG_LOCK_KEY);
  /* Lock all MPU registers */
  WRITE_REG32(CM_MPU->WP, MPU_REG_LOCK_KEY);
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"
/***********************************************************************************************************************
 *  GLOBAL FUNCTION
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
)
{
  /* Reset initialize */
#if (MCU_LOOKUP_RESET_ENABLE == STD_ON)
  Reset_Ip_Init();
#endif

  /* Unlock all register */
  Mcu_Ipr_Unlock_Periph();
  /* Initialize power */
  Power_Ip_Init(para_HwIPsConfig_ptr);

  /* Lock all register */
  Mcu_Ipr_Lock_Periph();
}

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
)
{
  uint8_t  f_ClocksToBeConfigCnt_u8;
  const Mcu_ClockConfigType * f_SysClockConfig_ptr;
  const Mcu_FuncClockControlType * f_FuncClockConfig_ptr;
  uint8_t  f_CtrlIndex_u8;
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_u8 = E_OK;

  /* Unlock all register */
  Mcu_Ipr_Unlock_Periph();
  l_McuClockConfig_ptr = para_ClockConfig_ptr;

  /* System clock backup */
  (void) Clock_Ip_PreInitClock();

  f_ClocksToBeConfigCnt_u8 = para_ClockConfig_ptr->McuClocksConfigCount_dd;
  /* Configure all the clock setting */
  for(f_CtrlIndex_u8 = (uint8_t)0U; f_CtrlIndex_u8 < f_ClocksToBeConfigCnt_u8; f_CtrlIndex_u8++)
  {
    /* Pointer to the Clock Configurations */
    f_SysClockConfig_ptr = (const Mcu_ClockConfigType *)(&para_ClockConfig_ptr->McuClockConfigList_ptr[f_CtrlIndex_u8]);
    /* Init clock */
    f_ReturnValue_u8 = Clock_Ip_InitClock(f_SysClockConfig_ptr);

    if ((Std_ReturnType)E_NOT_OK == f_ReturnValue_u8)
    {
      break;
    }
  }
  f_ClocksToBeConfigCnt_u8 = para_ClockConfig_ptr->McuFuncClocksCount_dd;
  for(f_CtrlIndex_u8 = (uint8_t)0U; f_CtrlIndex_u8 < f_ClocksToBeConfigCnt_u8; f_CtrlIndex_u8++)
  {
    /* Pointer to the Clock Configurations */
    f_FuncClockConfig_ptr = (const Mcu_FuncClockControlType *)(&para_ClockConfig_ptr->McuFuncClockControlList_ptr[f_CtrlIndex_u8]);
    /* Enable peripheral clock */
    Clock_Ip_FcgPeriphClock(f_FuncClockConfig_ptr);
  }

  /* Lock all register */
  Mcu_Ipr_Lock_Periph();
  /* Return value */
  return f_ReturnValue_u8;
}
#endif /* (MCU_ENABLE_INIT_CLOCK == STD_ON) */

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
)
{
  Mcu_PllStatusType  f_PllStatus_en ;

  switch(Clock_Ip_GetPllStatus())
  {
    case CLOCK_IP_PLL_LOCKED:
      f_PllStatus_en  = MCU_PLL_LOCKED;
      break;
    case CLOCK_IP_PLL_UNLOCKED:
      f_PllStatus_en  = MCU_PLL_UNLOCKED;
      break;
    case CLOCK_IP_PLL_STATUS_UNDEFINED:
    default:
      f_PllStatus_en  = MCU_PLL_STATUS_UNDEFINED;
      break;
  }

  return f_PllStatus_en;
}
#endif /* (MCU_SUPPORT_PLL == STD_ON) */

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
)
{
  /* Unlock all register */
  Mcu_Ipr_Unlock_Periph();
  /* Activates the PLL clock */
  Clock_Ip_DistributePll();
  /* Lock all register */
  Mcu_Ipr_Lock_Periph();
}
#endif /* (MCU_SUPPORT_PLL == STD_ON) */

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
)
{
  /* Unlock all register */
  Mcu_Ipr_Unlock_Periph();

  /* Initializes the mode */
  Power_Ip_SetMode(para_ModeConfig_ptr, l_McuClockConfig_ptr);

  /* Lock all register */
  Mcu_Ipr_Lock_Periph();
}

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
)
{
  /* Disable WKT Timer */
  Power_Ip_WktTimerEnableConfig(FALSE);

  /* Configure WKT Timer */ 
  if (para_WktClockSource_en == MCU_WKT_CLK_SRC_64HZ)
  {
    Power_Ip_WktTimerConfig(PWC_WKT_CLK_SRC_64HZ, para_WktCmpVal_u16);
  }
  else if (para_WktClockSource_en == MCU_WKT_CLK_SRC_XTAL32)
  {
    Power_Ip_WktTimerConfig(PWC_WKT_CLK_SRC_XTAL32, para_WktCmpVal_u16);
  }
  else if (para_WktClockSource_en == MCU_WKT_CLK_SRC_RTCLRC)
  {
    Power_Ip_WktTimerConfig(PWC_WKT_CLK_SRC_RTCLRC, para_WktCmpVal_u16);
  }
  else
  {
    /* Do nothing */
  }

  /* Enable WKT Timer */
  Power_Ip_WktTimerEnableConfig(TRUE);
}

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
)
{
  return (Std_ReturnType)Ram_Ip_InitRamSection(para_RamConfig_ptr);
}

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
)
{
  return (Mcu_RamStateType) Ram_Ip_GetRamState(MCU_SRAM_FLAG_ALL);
}

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
)
{
  /*
  *  Initiates a microcontroller reset.
  *  A RESET mode requested through the ME_MCTL register is forwarded to the MC_RGM,
  *  which triggers a global system reset and initiates the reset sequence
  */
  Reset_Ip_PerformReset();
}

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
)
{
    uint32_t ResetFlag_u32 = 0U;

    ResetFlag_u32        = (uint32_t)Reset_Ip_GetResetReason();

    /* Clear reset reasons */
    (void)Reset_Ip_ClearResetReason();

    return s_ResetReason_aa[ResetFlag_u32];
}

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
)
{
    uint32_t ResetReason_u32 = 0U;

    ResetReason_u32        = Reset_Ip_GetResetRawValue();

    /* Clear reset reasons */
    (void)Reset_Ip_ClearResetReason();

    return (Mcu_RawResetType)ResetReason_u32;
}

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
)
{
    uint32_t TempVal_u32 = 0U;
    uint32_t * PerResetModule_ptr;
    Reset_Ip_SoftwareModuleResetConfigType PeripheralReset_st;
    Reset_Ip_StatusType ReturnStatus_en = RESET_IP_STATUS_SUCCESS;

    Std_ReturnType StdReturn_en      = (Std_ReturnType)E_NOT_OK;

    uint32_t RcRegisterIdx_u32         = (uint32_t)((uint32_t)SwModuleReset / (uint32_t)32U);
    uint32_t IndexBitReset_u32         = (uint32_t)((uint32_t)SwModuleReset % (uint32_t)32U);

    /* Clear reset memory */
    PeripheralReset_st.SwModuleReset0_u32 = (uint32_t)0x0U;
    PeripheralReset_st.SwModuleReset1_u32 = (uint32_t)0x0U;
    PeripheralReset_st.SwModuleReset2_u32 = (uint32_t)0x0U;
    PeripheralReset_st.SwModuleReset3_u32 = (uint32_t)0x0U;

    PerResetModule_ptr                    = (uint32_t *)&(PeripheralReset_st.SwModuleReset0_u32);

    /* Setup peripheral will performs reset */
    TempVal_u32                           = (uint32_t)((uint32_t)1U << IndexBitReset_u32);
    PerResetModule_ptr[RcRegisterIdx_u32] = (uint32_t)TempVal_u32;

    ReturnStatus_en                       = Reset_Ip_SetSoftwareModuleReset(&PeripheralReset_st);

    if (RESET_IP_STATUS_SUCCESS == ReturnStatus_en)
    {
        StdReturn_en = E_OK;
    }
    else
    {
        StdReturn_en = E_NOT_OK;
    }

    return StdReturn_en;
}
#endif /*!< (STD_ON == MCU_SUPPORT_PERFORM_SOFTWARE_MODELE_RESET_API) */

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
