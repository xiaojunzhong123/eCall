/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Mode.c
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
 *   @file       Power_Ip_Mode.c
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
#include "Power_Ip_Mode.h"

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
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

static void Power_Ip_StartTimeout(
    uint32_t * para_StartTimeOut_ptr,
    uint32_t * para_TimeoutTicksOut_ptr,
    uint32_t para_TimeoutUs_u32,
    Osr_CounterType para_OsifCounter_en
);

static boolean Power_Ip_TimeoutExpired(
    uint32_t * para_StartTime_ptr,
    uint32_t * para_ElapsedTicks_ptr,
    uint32_t para_TimeoutTicks_u32,
    Osr_CounterType para_OsifCounter_en
);

static Std_ReturnType Power_Ip_CheckConditionLowPower(
    void
);

static Power_Ip_ModeType Power_Ip_CheckMode(
    void
);

static void Power_Ip_SetModeDelay(
    uint32_t  para_Delay_u32
);

static uint8_t Power_Ip_StopModeIsReady(
    void
);

static Std_ReturnType Power_Ip_CheckFlashStatus(
    void
);

static void Power_Ip_StopModeCfg(
    const Power_Ip_StopModeCfgType * para_StopModeCfgPtr
);

static void Power_Ip_ControlWakeupStopModeSrc(
    const Power_Ip_StopModeWKUSrcType * para_WKUSrc_st
);

static void Power_Ip_SelPWDWKUedge(
    const Power_Ip_PWDWKUEdgeSelType * para_WKUevent_ptr
);

static void Power_Ip_ControlPowerdownWakeUpSrc(
    const Power_Ip_WKUPWDEnableType * para_WKUSrc_str
);

static void Power_Ip_ClearPWDWakeUpFlag(
    void
);

static void Power_Ip_PowerDownCfg(
    const Power_Ip_PowerDownModeCfgType * para_PowerDownModeCfg_st
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 *  @brief Backup variable for registers of ADC and DAC
 *  @Design     None
 */
static Power_Ip_ADCDACBackuptype l_BackupADCADCStopmode_st = {0};
#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          Configure the timeout condition and start timeout.
 *
 *  @details        Design ID:         Power_Ip_StartTimeout_Activity
 * 
 *  @param[in]      para_StartTimeOut_ptr       Pointer to start timeout configuration structure
 *  @param[in]      para_TimeoutTicksOut_ptr    Pointer to elapsed timeout configuration structure
 *  @param[in]      para_TimeoutUs_u32          Pointer to  timeout ticksout structure
 *  @param[in]      para_OsifCounter_en         Type of timer counter
 *  @arg            OSR_COUNTER_DUMMY           Define dummy counter
 *  @arg            OSR_COUNTER_AUTOSAROS       Define dummy counter
 *  @arg            OSR_COUNTER_SYSTICK         Define system counter
 *  @arg            OSR_COUNTER_CUSTOM          Define custom counter
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Power_Ip_StartTimeout(
    uint32_t * para_StartTimeOut_ptr,
    uint32_t * para_TimeoutTicksOut_ptr,
    uint32_t para_TimeoutUs_u32,
    Osr_CounterType para_OsifCounter_en
)
{
    * para_StartTimeOut_ptr    = Osr_GetCounter(para_OsifCounter_en);
    * para_TimeoutTicksOut_ptr = Osr_UsToTicks(para_TimeoutUs_u32, para_OsifCounter_en);
}

/**
 *  @brief          Handles the timeout expiration.
 *
 *  @details        Design ID:         Power_Ip_TimeoutExpired_Activity
 * 
 *  @param[in]      para_StartTime_ptr       Pointer to start time configuration structure
 *  @param[in]      para_ElapsedTicks_ptr    Pointer to elapsed time configuration structure
 *  @param[in]      para_TimeoutTicks_u32    Timeout ticks value
 *  @param[in]      para_OsifCounter_en      Type of timer counter
 *  @arg            OSR_COUNTER_DUMMY        Define dummy counter
 *  @arg            OSR_COUNTER_AUTOSAROS    Define dummy counter
 *  @arg            OSR_COUNTER_SYSTICK      Define system counter
 *  @arg            OSR_COUNTER_CUSTOM       Define custom counter
 *
 *  @return         boolean    Timeout is happened or not
 *  @retval         TRUE       Timeout happened
 *  @retval         FALSE      Timeout has not happened
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static boolean Power_Ip_TimeoutExpired(
    uint32_t * para_StartTime_ptr,
    uint32_t * para_ElapsedTicks_ptr,
    uint32_t para_TimeoutTicks_u32,
    Osr_CounterType para_OsifCounter_en
)
{
    uint32_t CurrentElapsedTicks_u32;
    uint32_t TempElapsedTicks_u32;
    boolean IsEndTimeOut_bool = (boolean)FALSE;

    /* Get the current elapsed time since the start time */
    CurrentElapsedTicks_u32   = Osr_GetElapsed(para_StartTime_ptr, para_OsifCounter_en);
    
    /* Calculate remaining ticks before counter overflow (handles 32-bit counter wrap-around) */
    TempElapsedTicks_u32      = 0xFFFFFFFFU - (* para_ElapsedTicks_ptr);
    
    /* Handle counter overflow: if remaining ticks is greater than current elapsed, 
       we haven't overflowed yet, so add the full current elapsed time */
    if (TempElapsedTicks_u32 > CurrentElapsedTicks_u32)
    {
        * para_ElapsedTicks_ptr += CurrentElapsedTicks_u32;
    }
    else
    {
        /* Counter has overflowed, only add the remaining ticks before overflow */
        * para_ElapsedTicks_ptr += TempElapsedTicks_u32;
    }

    /* Check if total elapsed time has reached or exceeded the timeout value */
    if (* para_ElapsedTicks_ptr >= para_TimeoutTicks_u32)
    {
        IsEndTimeOut_bool = (boolean)TRUE;
    }

    return IsEndTimeOut_bool;
}

/**
 *  @brief          this function to check condition before switch from Run Mode  to Low Power mode
 *
 *  @details        Design ID:         Power_Ip_CheckConditionLowPower_Activity
 * 
 *  @return         Std_ReturnType    Check condition before switching from Run Mode to Low Power mode
 *  @retval         E_OK              Ready enter to the low-power mode
 *  @retval         E_NOT_OK          Not ready enter to the low-power mode
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static Std_ReturnType Power_Ip_CheckConditionLowPower(
    void
)
{
  Std_ReturnType  f_return_en = (Std_ReturnType)E_NOT_OK;
  /* Get the XTAL oscillator's status */
  uint8_t  f_XTALSTP_u8  = READ_REG8_BIT(CM_CMU->XTALCR, CMU_XTALCR_XTALSTP);
  uint8_t  f_XTALSTB_u8  = READ_REG8_BIT(CM_CMU->OSCSTBSR, CLK_STB_FLAG_XTAL);
  /* Get the HRC oscillator's status */
  uint8_t  f_HRCSTP_u8   = READ_REG8_BIT(CM_CMU->HRCCR, CMU_HRCCR_HRCSTP);
  uint8_t  f_HRCSTB_u8   = READ_REG8_BIT(CM_CMU->OSCSTBSR, CLK_STB_FLAG_HRC);
  /* Get the PLLH status working */
  uint8_t  f_PLLHSTP_u8  = READ_REG8_BIT(CM_CMU->PLLHCR, CMU_PLLHCR_PLLHOFF);
  uint8_t  f_PLLHSTB_u8  = READ_REG8_BIT(CM_CMU->OSCSTBSR, CLK_STB_FLAG_PLLH);
  /* Get the PLLA status working */
  uint8_t  f_PLLASTP_u8  = READ_REG8_BIT(CM_CMU->PLLACR, CMU_PLLACR_PLLAOFF);
  uint8_t  f_PLLASTB_u8  = READ_REG8_BIT(CM_CMU->OSCSTBSR, CLK_STB_FLAG_PLLA);

  /**
   * Condition enter the stop mode or power-down mode:
   * The XTAL oscillator to oscillate, only after confirming that XTAL oscilator is stable through the XTALSTBF bit,
   *   it can enter the stop mode or power-down mode.
   * The software sets the XTAL oscillator to stop, only after confirming that XTAL is stopped
   *   by the XTALSTBF bit, it can enter the stop mode or power-down mode.
   * The HRC oscillator to oscillate, only after confirming that HRC oscilator is stable through the HRCSTBF bit,
   *   it can enter the stop mode or power-down mode.
   * The software sets the HRC oscillator to stop, only after confirming that HRC is stopped
   *   by the HRCSTBF bit, it can enter the stop mode or power-down mode.
   * The PLLH to start working, only after confirming that PLLH is stable through the PLLHSTBF bit,
   *   it can enter the stop mode or power-down mode.
   * The software sets the PLLH to stop, only after confirming that PLLH is stopped
   *   by the PLLHSTBF bit, it can enter the stop mode or power-down mode.
   * The PLLA to start working, only after confirming that PLLA is stable through the PLLASTBF bit,
   *   it can enter the stop mode or power-down mode.
   * The software sets the PLLA to stop, only after confirming that PLLA is stopped
   *   by the PLLASTBF bit, it can enter the stop mode or power-down mode.
   */
  if ((f_XTALSTP_u8 == ((uint8_t)((uint8_t)(~f_XTALSTB_u8) >> CMU_OSCSTBSR_XTALSTBF_POS) & CMU_XTALCR_XTALSTP)) && \
      (f_HRCSTP_u8 == ((uint8_t)((uint8_t)(~f_HRCSTB_u8) >> CMU_OSCSTBSR_HRCSTBF_POS) & CMU_HRCCR_HRCSTP)) && \
      (f_PLLHSTP_u8 == ((uint8_t)((uint8_t)(~f_PLLHSTB_u8) >> CMU_OSCSTBSR_PLLHSTBF_POS) & CMU_PLLHCR_PLLHOFF)) && \
      (f_PLLASTP_u8 == ((uint8_t)((uint8_t)(~f_PLLASTB_u8) >> CMU_OSCSTBSR_PLLASTBF_POS) & CMU_PLLACR_PLLAOFF))) 
      {
        f_return_en = E_OK;
      }
  return f_return_en;
}

/**
 *  @brief          This function get mode currently is High speed mode
 *                  or ultra low speed mode.
 *
 *  @details        Design ID:         Power_Ip_CheckMode_Activity
 * 
 *  @return         Power_Ip_ModeType    Current MCU mode (high speed or ultra low speed)
 *  @retval         MCU_HIGH_SPEED_MODE         The system in high speed mode
 *  @retval         MCU_ULTRA_LOW_SPEED_MODE    The system is ultra low speed mode
 *
 *  @note           This function will called by Mcu_SetMode()
 *
 *  @pre            None
 *
 */
static Power_Ip_ModeType Power_Ip_CheckMode(
    void
)
{
  Power_Ip_ModeType  f_CurrentMode_en;
  /*
  + PWC_PWRC2.DVS = b'11
  + PWC_PWRC2.DDAS = 0xF
  + PWC_PWRC3.DDAS = 0xFF
  => The system in High speed mode
  otherwise => In ultra low speed mode
  */
  if ((PWC_PWRC2_DVS  == READ_REG8_BIT(CM_PWC->PWRC2, PWC_PWRC2_DVS) ) && \
    (PWC_PWRC2_DDAS == READ_REG8_BIT(CM_PWC->PWRC2, PWC_PWRC2_DDAS)) && \
    (PWC_PWRC3_DDAS == READ_REG8_BIT(CM_PWC->PWRC3, PWC_PWRC3_DDAS)))
  {
    /* High speed mode */
    f_CurrentMode_en = (uint8_t)PWC_IP_HIGH_SPEED_MODE;
  }
  else
  {
  /* Ultra low speed mode */
  f_CurrentMode_en = (uint8_t)PWC_IP_ULTRA_LOW_SPEED_MODE;
  }
  return f_CurrentMode_en;
}

/**
 *  @brief          This function will create delay period for set mode
 *
 *  @details        Design ID:         Power_Ip_SetModeDelay_Activity
 * 
 *  @param[in]      para_Delay_u32    Time for delay in microseconds
 *
 *  @return         void
 *
 *  @note           This function implements a software delay using nested loops.
 *                  The delay is calculated based on the HCLK frequency to provide
 *                  approximately the requested delay time.
 *
 *  @pre            None
 *
 */
static void Power_Ip_SetModeDelay(
    uint32_t  para_Delay_u32
)
{
  uint32_t  f_Count_u32;
  uint32_t  f_Delay_u32 = para_Delay_u32;
  
  /* Calculate cycles per microsecond based on HCLK frequency 
     Formula: (HCLK_Freq + 10MHz - 1) / 10MHz to get cycles per 10us, then divide by 10 */
  const uint32_t  f_Cyc_u32 = (PWC_HCLK_VALUE + 10000000U - 1U) / 10000000U;
  
  /* Outer loop: count down microseconds */
  while (0U < f_Delay_u32)
  {
    f_Delay_u32 --;
    /* Inner loop: count down cycles for each microsecond */
    f_Count_u32 = f_Cyc_u32;
    while (0U < f_Count_u32)
    {
      f_Count_u32 --;
    }
  }
}

/**
 *  @brief          This function will check condition of Flash and DMA
 *                  before entering stop mode
 *
 *  @details        Design ID:         Power_Ip_StopModeIsReady_Activity
 * 
 *  @return         uint8_t    The System Status
 *  @retval         0U       The System is not ready to enter STOP mode
 *  @retval         1U       The System is ready to enter STOP mode
 *
 *  @note           This function checks if Flash is idle (RDY0/1 = 1) and DMA
 *                  has completed all transfers (DMAACT = 0) before allowing
 *                  entry into STOP mode.
 *
 *  @pre            None
 *
 */
static uint8_t Power_Ip_StopModeIsReady(
    void
)
{
  Std_ReturnType  f_CheckReady_u8 = NOT_READY;
  /**
  * Check Flash state:
  * EFR_FSR.RDY0/1 = 1 => The Flash state is idle and vice verse
  * before enter to STOP mode.
  * Check CM_DMA1->CHSTAT_DMA_CHSTAT_DMAACT = 0 to ensure DMA have
  * completed all transfer before enter to Stop mode
  */
  if (((CM_DMA1->CHSTAT & DMA_CHSTAT_DMAACT) == (uint32_t)READY) && \
    ((CM_DMA2->CHSTAT & DMA_CHSTAT_DMAACT) == (uint32_t)READY) && \
    ((CM_EFM->FSR & (EFM_FSR_RDY0 | EFM_FSR_RDY1)) == (EFM_FSR_RDY0 | EFM_FSR_RDY1)) )
  {
    /* ready */
    f_CheckReady_u8 = READY;
  }

  return f_CheckReady_u8;
}

/**
 *  @brief          This function will check Flash state
 *
 *  @details        Design ID:         Power_Ip_CheckFlashStatus_Activity
 * 
 *  @return         Std_ReturnType    Check Flash state for programming or erasing status
 *  @retval         E_NOT_OK          The Flash is programming or erasing, or timeout occurred
 *  @retval         E_OK              The Flash state is IDLE
 *
 *  @note           This function waits for Flash to become idle by checking RDY0/1 bits.
 *                  It uses a timeout mechanism to prevent infinite waiting.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Power_Ip_CheckFlashStatus(
    void
)
{
  Std_ReturnType  f_CheckFlash_u8 = NOT_READY;
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;

  /* Initialize timeout mechanism to prevent infinite waiting */
  ElapsedTicks_u32 = 0U;
  Power_Ip_StartTimeout(&StartTime_u32,
                       &TimeoutTicks_u32,
                       (uint32_t)POWER_IP_TIMEOUT_VALUE,
                       POWER_IP_TIMEOUT_TYPE);

  /* Wait for Flash to become idle by checking RDY0 and RDY1 bits
     Continue waiting until both bits are set (Flash is idle) OR timeout occurs */
  do
  {
      /* Check if timeout has expired */
      TimeoutStatus_bool = Power_Ip_TimeoutExpired(&StartTime_u32,
                                                  &ElapsedTicks_u32,
                                                  TimeoutTicks_u32,
                                                  POWER_IP_TIMEOUT_TYPE);
      /* Loop continues while Flash is busy (RDY0 or RDY1 not set) AND timeout hasn't occurred */
  } while (((EFM_FSR_RDY0 != READ_REG32_BIT(CM_EFM->FSR, EFM_FSR_RDY0)) || (EFM_FSR_RDY1 != READ_REG32_BIT(CM_EFM->FSR, EFM_FSR_RDY1))) && ((boolean)FALSE == TimeoutStatus_bool));
  
  /* Check if we exited due to timeout or Flash becoming ready */
  if ((boolean)TRUE == TimeoutStatus_bool)
  {
      /* Timeout occurred - Flash is still busy or error condition */
      f_CheckFlash_u8 = E_NOT_OK;
  }
  else
  {
      /* Flash became ready within timeout period */
      f_CheckFlash_u8 = READY;
  }

  return f_CheckFlash_u8;
}

/**
 *  @brief          This function will configure for Stop mode
 *
 *  @details        Design ID:         Power_Ip_StopModeCfg_Activity
 * 
 *  @param[in]      para_StopModeCfgPtr    Chip configuration before entering stop mode
 *  @arg            StopDrv_en,            MCU speed mode when entering stop mode
 *  @arg            Clock_en,              System clock setting after wake-up from stop mode
 *  @arg            FlashWait_en,          Whether to wait for flash to stabilize after wake-up
 *  @arg            ExBusHold_en,          External bus status in stop mode
 *
 *  @return         void
 *
 *  @note           This function configures the power management registers for stop mode
 *                  including LDO driver selection, external bus control, and clock settings.
 *
 *  @pre            None
 *
 */
static void Power_Ip_StopModeCfg(
    const Power_Ip_StopModeCfgType * para_StopModeCfgPtr
)
{
  if (NULL_PTR != para_StopModeCfgPtr)
  {
    /**
    * Set reg PWC_PWRC1
    * Modify bit STPDAS[1:0]: STOP mode LDO driver selection
    * + 00 >> The drive capacity set when entering STOP mode
    *      from high-speed mode.
    * + 11 >> The drive capacity set when entering STOP mode
    *      from ultra-low-speed mode.
    * + 01/10 are prohibited.
    * */
    MODIFY_REG8(CM_PWC->PWRC1, PWC_PWRC1_STPDAS, para_StopModeCfgPtr->StopDrv_en);

    if (PWC_STOP_DRV_LOW == para_StopModeCfgPtr->StopDrv_en)
    {
      /**
      * Before WFI instruction need Change into Ultra-low-speed mode
      * to saved power consumption
      * */
      Power_Ip_ModeUltraLowSpeed();
    }
    /**
    * Set reg PWC_STMPMCR
    * modify bit: EXBUSOE, CKSMRC, FLNWT
    *
    *  EXBUSOE: state control for EXMC address bus and
    *           control signal.
    *      + 0: High impedance.
    *      + 1: Maintain the state before power-down mode
    *          or stop mode.
    *  CKSMRC: Clock switch to MRC control bit
    *      + 0: After the STOP mode wake up, the clock and
    *          frequency division maintain the settings
    *          before entering.
    *      + 1: After the STOP mode wake up, the system
    *          clock switch to MRC, SCKCFGR register are
    *          initialize.
    *  FLNWT: FLASH stability waiting control
    *      + 0: After wake up from stop mode, wait for
    *          FLASH to stabilize.
    *      + 1: After wake up from stop mode, don't wait
    *          for FLASH to stabilize.
    */
    MODIFY_REG16(CM_PWC->STPMCR, PWC_STPMCR_EXBUSOE, para_StopModeCfgPtr->ExBusHold_en);
    MODIFY_REG16(CM_PWC->STPMCR, PWC_STPMCR_EXBUSOE, para_StopModeCfgPtr->Clock_en);
    MODIFY_REG16(CM_PWC->STPMCR, PWC_STPMCR_EXBUSOE, para_StopModeCfgPtr->FlashWait_en);
  }
}

/**
 *  @brief          This function will be configure wake up source to wake up
 *                  wake up system from stop-mode
 *
 *  @details        Design ID:         Power_Ip_ControlWakeupStopModeSrc_Activity
 * 
 *  @param[in]      para_WKUSrc_st    configre Wake up soure to wake up proccessor
 *                                    from stop mode
 *
 *  @return         void
 *
 *  @note           This function will called by Mcu_SetMode()
 *
 *  @pre            None
 *
 */
static void Power_Ip_ControlWakeupStopModeSrc(
    const Power_Ip_StopModeWKUSrcType * para_WKUSrc_st
)
{
  uint32_t  f_WUPEN_u32 = (uint32_t)0U;
  uint16_t  f_EIRQ_u16  = (uint16_t)0U;

  f_EIRQ_u16  = (uint16_t)((para_WKUSrc_st->EIRQ0  << (uint32_t)EVT_SRC_PORT_EIRQ0)  | (para_WKUSrc_st->EIRQ1  << (uint32_t)EVT_SRC_PORT_EIRQ1)  | \
              (para_WKUSrc_st->EIRQ2  << (uint32_t)EVT_SRC_PORT_EIRQ2)  | (para_WKUSrc_st->EIRQ3  << (uint32_t)EVT_SRC_PORT_EIRQ3)  | \
              (para_WKUSrc_st->EIRQ4  << (uint32_t)EVT_SRC_PORT_EIRQ4)  | (para_WKUSrc_st->EIRQ5  << (uint32_t)EVT_SRC_PORT_EIRQ5)  | \
              (para_WKUSrc_st->EIRQ6  << (uint32_t)EVT_SRC_PORT_EIRQ6)  | (para_WKUSrc_st->EIRQ7  << (uint32_t)EVT_SRC_PORT_EIRQ7)  | \
              (para_WKUSrc_st->EIRQ8  << (uint32_t)EVT_SRC_PORT_EIRQ8)  | (para_WKUSrc_st->EIRQ9  << (uint32_t)EVT_SRC_PORT_EIRQ9)  | \
              (para_WKUSrc_st->EIRQ10 << (uint32_t)EVT_SRC_PORT_EIRQ10) | (para_WKUSrc_st->EIRQ11 << (uint32_t)EVT_SRC_PORT_EIRQ11) | \
              (para_WKUSrc_st->EIRQ12 << (uint32_t)EVT_SRC_PORT_EIRQ12) | (para_WKUSrc_st->EIRQ13 << (uint32_t)EVT_SRC_PORT_EIRQ13) | \
              (para_WKUSrc_st->EIRQ14 << (uint32_t)EVT_SRC_PORT_EIRQ14) | (para_WKUSrc_st->EIRQ15 << (uint32_t)EVT_SRC_PORT_EIRQ15));
  /* Wake-up source config */
  f_WUPEN_u32 = (((uint32_t)f_EIRQ_u16           << (uint32_t)INTC_WKEN_EIRQWKEN_POS)    | (para_WKUSrc_st->SWDT      << (uint32_t)INTC_WKEN_SWDTWKEN_POS)    | \
              (para_WKUSrc_st->PVD1    << (uint32_t)INTC_WKEN_PVD1WKEN_POS)    | (para_WKUSrc_st->PVD2      << (uint32_t)INTC_WKEN_PVD2WKEN_POS)    | \
              (para_WKUSrc_st->CMP     << (uint32_t)INTC_WKEN_CMPWKEN_POS)     | (para_WKUSrc_st->WKTM      << (uint32_t)INTC_WKEN_WKTMWKEN_POS)    | \
              (para_WKUSrc_st->RTCALM  << (uint32_t)INTC_WKEN_RTCALMWKEN_POS)  | (para_WKUSrc_st->RTCPRD    << (uint32_t)INTC_WKEN_RTCPRDWKEN_POS)  | \
              (para_WKUSrc_st->TMR0CMP << (uint32_t)INTC_WKEN_TMR0CMPWKEN_POS) | (para_WKUSrc_st->TMR2GCM   << (uint32_t)INTC_WKEN_TMR2GCMWKEN_POS) | \
              (para_WKUSrc_st->TMR2OVF << (uint32_t)INTC_WKEN_TMR2OVFWKEN_POS) | (para_WKUSrc_st->USART1RX << (uint32_t)INTC_WKEN_RXWKEN_POS)      | \
              (para_WKUSrc_st->USBHS   << (uint32_t)INTC_WKEN_USHWKEN_POS)     | (para_WKUSrc_st->USBFS     << (uint32_t)INTC_WKEN_USFWKEN_POS)     | \
              (para_WKUSrc_st->ETHPMT << (uint32_t)INTC_WKEN_ETHWKEN_POS));
  /* Stop mode wake-up source configure */
  SET_REG32_BIT(CM_INTC->WKEN, f_WUPEN_u32);
}

/**
 *  @brief          This function will select edge for power-down wake-up.
 *
 *  @details        Design ID:         Power_Ip_SelPWDWKUedge_Activity
 * 
 *  @param[in]      para_WKUevent_ptr    Power-down wake-up edge selection configuration
 *
 *  @return         void
 *
 *  @note           This function configures the edge detection for power-down wake-up sources
 *                  including PVD1, PVD2, and external wake-up pins (WKUP0-WKUP3).
 *                  This function will be called by Power_Ip_ModePwd().
 *
 *  @pre            None
 *
 */
static void Power_Ip_SelPWDWKUedge(
    const Power_Ip_PWDWKUEdgeSelType * para_WKUevent_ptr
)
{
  uint8_t  f_PWDKES_u8 = (uint8_t)0U;

  f_PWDKES_u8 = (uint8_t)((para_WKUevent_ptr->PWC_PD_WKUP_TRIG_PVD1  << PWC_PDWKES_VD1EGS_POS) | \
              (para_WKUevent_ptr->PWC_PD_WKUP_TRIG_PVD2  << PWC_PDWKES_VD2EGS_POS) | \
              (para_WKUevent_ptr->PWC_PD_WKUP_TRIG_WKUP0 << PWC_PDWKES_WK0EGS_POS) | \
              (para_WKUevent_ptr->PWC_PD_WKUP_TRIG_WKUP1 << PWC_PDWKES_WK1EGS_POS) | \
              (para_WKUevent_ptr->PWC_PD_WKUP_TRIG_WKUP2 << PWC_PDWKES_WK2EGS_POS) | \
              (para_WKUevent_ptr->PWC_PD_WKUP_TRIG_WKUP3 << PWC_PDWKES_WK3EGS_POS));
  /* Power down mode wake up event trigger config. */
  SET_REG8_BIT(CM_PWC->PDWKES, f_PWDKES_u8);
}

/**
 *  @brief          This function will control Power-down wake-up source
 *
 *  @details        Design ID:         Power_Ip_ControlPowerdownWakeUpSrc_Activity
 * 
 *  @param[in]      para_WKUSrc_str    Power-down wake-up source configuration
 *
 *  @return         void
 *
 *  @note           This function configures which events can wake up the system from
 *                  power-down mode, including external pins, timers, RTC, and PVD events.
 *                  This function will be called by Power_Ip_ModePwd().
 *
 *  @pre            None
 *
 */
static void Power_Ip_ControlPowerdownWakeUpSrc(
    const Power_Ip_WKUPWDEnableType * para_WKUSrc_str
)
{
  uint8_t  f_PDWKE0_u8 = (uint8_t)0U;
  uint8_t  f_PDWKE1_u8 = (uint8_t)0U;
  uint8_t  f_PDWKE2_u8 = (uint8_t)0U;
  /* Power down mode wake up event config 0. */
  f_PDWKE0_u8 = (uint8_t)((para_WKUSrc_str->PWC_PD_WKUP_WKUP13 << PWC_PDWKE0_WKE13_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP12 << PWC_PDWKE0_WKE12_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP11 << PWC_PDWKE0_WKE11_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP10 << PWC_PDWKE0_WKE10_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP03 << PWC_PDWKE0_WKE03_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP02 << PWC_PDWKE0_WKE02_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP01 << PWC_PDWKE0_WKE01_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP00 << PWC_PDWKE0_WKE00_POS));
  SET_REG8_BIT(CM_PWC->PDWKE0, f_PDWKE0_u8);
  /* Power down mode wake up event config 1. */
  f_PDWKE1_u8 = (uint8_t)((para_WKUSrc_str->PWC_PD_WKUP_WKUP33 << PWC_PDWKE1_WKE33_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP32 << PWC_PDWKE1_WKE32_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP31 << PWC_PDWKE1_WKE31_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP30 << PWC_PDWKE1_WKE30_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP23 << PWC_PDWKE1_WKE23_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP22 << PWC_PDWKE1_WKE22_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP21 << PWC_PDWKE1_WKE21_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_WKUP20 << PWC_PDWKE1_WKE20_POS));
  SET_REG8_BIT(CM_PWC->PDWKE1, f_PDWKE1_u8);
  /* Power down mode wake up event config 2. */
  f_PDWKE2_u8 = (uint8_t)((para_WKUSrc_str->PWC_PD_WKUP_WKTM   << PWC_PDWKE2_WKTMWKE_POS)   | \
              (para_WKUSrc_str->PWC_PD_WKUP_RTCALM << PWC_PDWKE2_RTCALMWKE_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_RTCPRD << PWC_PDWKE2_RTCPRDWKE_POS) | \
              (para_WKUSrc_str->PWC_PD_WKUP_PVD2   << PWC_PDWKE2_VD2WKE_POS)    | \
              (para_WKUSrc_str->PWC_PD_WKUP_PVD1   << PWC_PDWKE2_VD1WKE_POS));
  SET_REG8_BIT(CM_PWC->PDWKE2, f_PDWKE2_u8);
}

/**
 *  @brief          This function will clear power-down wake-up flag
 *
 *  @details        Design ID:         Power_Ip_ClearPWDWakeUpFlag_Activity
 * 
 *  @return         void
 *
 *  @note           This function clears all power-down wake-up flags to ensure
 *                  the system can enter power-down mode again. If flags are not cleared,
 *                  the chip cannot enter power-down mode. This function will be called by Power_Ip_ModePwd().
 *
 *  @pre            None
 *
 */
static void Power_Ip_ClearPWDWakeUpFlag(
    void
)
{
  /* Clear wake up event flag. */
  CLR_REG8_BIT(CM_PWC->PDWKF0, PWC_PD_WKUP_FLAG_ALL);
  CLR_REG8_BIT(CM_PWC->PDWKF1, PWC_PD_WKUP_FLAG_ALL >> PWC_PD_WKUP_FLAG1_POS);
}

/**
 *  @brief          This function will be configure for power-down mode.
 *                  Consists of: Control IO state hold, Select power-down mode:
 *                  + Power-down mode 1.
 *                  + Power-down mode 2.
 *                  + Power-down mode 3.
 *                  + Power-down mode 4.
 *
 *  @details        Design ID:         Power_Ip_PowerDownCfg_Activity
 * 
 *  @param[in]      para_PowerDownModeCfg_st    Struct configure for power-down mode
 *
 *  @return         void
 *
 *  @note           This function will be called by Power_Ip_ModePwd().
 *
 *  @pre            None
 *
 */
static void Power_Ip_PowerDownCfg(
    const Power_Ip_PowerDownModeCfgType * para_PowerDownModeCfg_st
)
{
  /* Configure IO state hold and power-down mode in PWRC0 register*/
  MODIFY_REG8(CM_PWC->PWRC0, PWC_PWRC0_IORTN, para_PowerDownModeCfg_st->IOState_en);
  MODIFY_REG8(CM_PWC->PWRC0, PWC_PWRC0_PDMDS, para_PowerDownModeCfg_st->Mode_en);
  /* Control power-down wake-up time in PWRC1 register */
  MODIFY_REG8(CM_PWC->PWRC1, PWC_PWRC1_PDTS, ((uint8_t)(para_PowerDownModeCfg_st->VcapCtrl_en) << PWC_PWRC1_PDTS_POS));
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
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
)
{
  Power_Ip_ModeType  f_CurrentMode_en;
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;

  /* Check current power mode - only switch if not already in high speed mode */
  f_CurrentMode_en = Power_Ip_CheckMode();
  if ((Power_Ip_ModeType)PWC_IP_HIGH_SPEED_MODE != f_CurrentMode_en)
  {
    /* Switch from ultra-low-speed mode to high-speed mode
       Configure power control registers for high speed operation:
       - PWC_PWRC2.DVS = b'11 (high speed mode)
       - PWC_PWRC2.DDAS = 0xF (high speed drive strength)
       - PWC_PWRC3.DDAS = 0xFF (high speed drive strength) */
    MODIFY_REG8(CM_PWC->PWRC2, PWC_PWRC2_DDAS, PWC_PWRC2_DDAS);
    WRITE_REG8(CM_PWC->PWRC3, PWC_PWRC3_DDAS);
    SET_REG8_BIT(CM_PWC->PWRC2, PWC_PWRC2_DVS);
    
    /* Wait for mode switch delay (TSWMD2 = 30us) */
    Power_Ip_SetModeDelay(POWER_IP_SWITCH_TIMEOUT);
    
    /* Configure Flash for high speed mode (LVM = 0) */
    WRITE_REG32(bCM_EFM->FRMC_b.LVM, 0U);
    
    /* Configure RAM for high speed operation */
    WRITE_REG16(CM_PWC->RAMOPM, POWER_RAMOPM_HIGH_SPEED);
    
    /* Wait for RAM operation mode to be set correctly */
    ElapsedTicks_u32 = 0U;
    Power_Ip_StartTimeout(&StartTime_u32,
                        &TimeoutTicks_u32,
                        (uint32_t)POWER_IP_TIMEOUT_VALUE,
                        POWER_IP_TIMEOUT_TYPE);
    do
    {
        TimeoutStatus_bool = Power_Ip_TimeoutExpired(&StartTime_u32,
                                                    &ElapsedTicks_u32,
                                                    TimeoutTicks_u32,
                                                    POWER_IP_TIMEOUT_TYPE);
    } while (((uint16_t)POWER_RAMOPM_HIGH_SPEED != READ_REG16(CM_PWC->RAMOPM)) && ((boolean)FALSE == TimeoutStatus_bool));
    
    /* Wait for Flash low voltage mode to be disabled */
    ElapsedTicks_u32 = 0U;
    Power_Ip_StartTimeout(&StartTime_u32,
                        &TimeoutTicks_u32,
                        (uint32_t)POWER_IP_TIMEOUT_VALUE,
                        POWER_IP_TIMEOUT_TYPE);
    do
    {
        TimeoutStatus_bool = Power_Ip_TimeoutExpired(&StartTime_u32,
                                                    &ElapsedTicks_u32,
                                                    TimeoutTicks_u32,
                                                    POWER_IP_TIMEOUT_TYPE);
    } while ((0U != READ_REG32(bCM_EFM->FRMC_b.LVM)) && ((boolean)FALSE == TimeoutStatus_bool));
  }
}

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
)
{
  Power_Ip_ModeType  f_CurrentMode_en = (uint8_t)PWC_IP_HIGH_SPEED_MODE;
  Std_ReturnType  f_CheckFlash_u8 = NOT_READY;
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;

  /* Check current power mode - only switch if not already in ultra low speed mode */
  f_CurrentMode_en = Power_Ip_CheckMode();
  if ((uint8_t)PWC_IP_ULTRA_LOW_SPEED_MODE != f_CurrentMode_en)
  {
    /* Ensure Flash is ready before switching modes */
    f_CheckFlash_u8 = Power_Ip_CheckFlashStatus();
    if ((uint8_t)READY == f_CheckFlash_u8)
    {
      /* Configure Flash and RAM for ultra low speed mode:
         - Set FRMC.LVM=1 (Flash low voltage mode enabled)
         - Set RAM operation mode to ultra low speed configuration */
      WRITE_REG32(bCM_EFM->FRMC_b.LVM, (uint32_t)1U);
      WRITE_REG16(CM_PWC->RAMOPM, POWER_RAMOPM_ULTRALOWSPEED);
      
      /* Wait for RAM operation mode to be set correctly */
      ElapsedTicks_u32 = 0U;
      Power_Ip_StartTimeout(&StartTime_u32,
                          &TimeoutTicks_u32,
                          (uint32_t)POWER_IP_TIMEOUT_VALUE,
                          POWER_IP_TIMEOUT_TYPE);
      do
      {
          TimeoutStatus_bool = Power_Ip_TimeoutExpired(&StartTime_u32,
                                                      &ElapsedTicks_u32,
                                                      TimeoutTicks_u32,
                                                      POWER_IP_TIMEOUT_TYPE);
      } while (((POWER_RAMOPM_ULTRALOWSPEED == CM_PWC->RAMOPM)) && ((boolean)FALSE == TimeoutStatus_bool));

      /* Wait for Flash low voltage mode to be enabled */
      ElapsedTicks_u32 = 0U;
      Power_Ip_StartTimeout(&StartTime_u32,
                          &TimeoutTicks_u32,
                          (uint32_t)POWER_IP_TIMEOUT_VALUE,
                          POWER_IP_TIMEOUT_TYPE);
      do
      {
        TimeoutStatus_bool = Power_Ip_TimeoutExpired(&StartTime_u32,
                                                      &ElapsedTicks_u32,
                                                      TimeoutTicks_u32,
                                                      POWER_IP_TIMEOUT_TYPE);
      } while (((uint32_t)1U != bCM_EFM->FRMC_b.LVM) && ((boolean)FALSE == TimeoutStatus_bool));

      /* Configure power control registers for ultra low speed mode:
         - Set PWC_PWRC2.DDAS[3:0] to 0x0 (low drive strength)
         - Set PWC_PWRC3.DDAS[7:0] to 0x00 (low drive strength)
         - Set PWC_PWRC2.DVS to b'10 (ultra low speed mode) */
      MODIFY_REG8(CM_PWC->PWRC2, PWC_PWRC2_DDAS, 0x00U);
      WRITE_REG8(CM_PWC->PWRC3, 0x00U);
      MODIFY_REG8(CM_PWC->PWRC2, PWC_PWRC2_DVS, PWC_PWRC2_DVS_1);
      
      /* Wait for mode switch delay (TSWMD1 = 30us) */
      Power_Ip_SetModeDelay(POWER_IP_SWITCH_TIMEOUT);
    }
  }
}

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
)
{
  /* Disable STOP mode and Power-down mode to enter sleep mode:
     - Clear STOP mode control bit (0: STOP mode is invalid, 1: STOP mode is valid)
     - Clear Power-down mode control bit */
  CLR_REG16_BIT(CM_PWC->STPMCR, PWC_STPMCR_STOP);
  CLR_REG8_BIT(CM_PWC->PWRC0, PWC_PWRC0_PWDN);
  
  /* Clear all non-maskable interrupt flags before entering sleep mode
     This ensures clean state for wake-up events */
  #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
  SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                          INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                          INTC_NMIFCR_RECCERRCLR | INTC_NMIFCR_BUSERRCLR | \
                                          INTC_NMIFCR_WDTCLR | INTC_NMIFCR_FECCERRCLR |\
                                          INTC_NMIFCR_ERMUCLR));
  #else
  SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                          INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                          INTC_NMIFCR_REPCLR | INTC_NMIFCR_RECCERRCLR | \
                                          INTC_NMIFCR_BUSERRCLR | INTC_NMIFCR_WDTCLR));
  #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
  
  /* Execute Wait For Interrupt (WFI) instruction to enter sleep mode
     The system will wake up when any interrupt or reset occurs */
  ASM_WFI();
}

/**
 *  @brief          This function configure stop mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModeStop_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerStopModeCfg_ptr    Struct configure for Stop mode
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
)
{
  Std_ReturnType  f_CheckReady_u8 = NOT_READY;
  Std_ReturnType  f_CheckCondition_u8 = E_NOT_OK;
  /* Clear data backup */
  l_BackupADCADCStopmode_st.DAC1_DAE  = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.DAC2_DAE  = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.ADC1_STRT = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.ADC2_STRT = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.ADC3_STRT = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.FCG3_ADC1 = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.FCG3_ADC2 = (uint8_t)(0U);
  l_BackupADCADCStopmode_st.FCG3_ADC3 = (uint8_t)(0U);

  /* Config for operation after wake up from Stop mode */
  Power_Ip_StopModeCfg(para_PowerStopModeCfg_ptr);
  /**
  * Before executing the WFI instruction to enter the stop mode, the digital
  * filtering of EIRQ needs to be set to invalid, ortherwise the interrupt
  * can't be used for STOP wake-up.
  */
  CM_INTC->EIRQCR1  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR2  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR3  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR4  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR5  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR6  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR7  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR8  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR9  &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR10 &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR11 &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR12 &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR13 &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR14 &= (~INTC_EIRQCR_EFEN);
  CM_INTC->EIRQCR15 &= (~INTC_EIRQCR_EFEN);
  /**
  * In the STOP mode, the ADC and DAC also consume power, unless
  * disabled it before enter to STOP mode.
  * To disabled DAC need to clear DACR.DAE,DAOE0,DAOE1 to "0"
  * To disable ADC needs to clear the ADC_STR.STRT bit to "0",
  * Set PWC_FCG3.ADC3, PWC_FCG3.ADC2 and PWC_FCG3.ADC1 as "1"
  * Backup register control ADC and DAC
  * This value wil be use in ISR-Stop mode Wake-up
  */
  l_BackupADCADCStopmode_st.DAC1_DAE  = (uint8_t)(bCM_DAC1->DACR_b.DAE);
  l_BackupADCADCStopmode_st.DAC2_DAE  = (uint8_t)(bCM_DAC2->DACR_b.DAE);
  l_BackupADCADCStopmode_st.ADC1_STRT = (uint8_t)(bCM_ADC1->STR_b.STRT);
  l_BackupADCADCStopmode_st.ADC2_STRT = (uint8_t)(bCM_ADC2->STR_b.STRT);
  l_BackupADCADCStopmode_st.ADC3_STRT = (uint8_t)(bCM_ADC3->STR_b.STRT);
  l_BackupADCADCStopmode_st.FCG3_ADC1 = (uint8_t)(bCM_PWC->FCG3_b.ADC1);
  l_BackupADCADCStopmode_st.FCG3_ADC2 = (uint8_t)(bCM_PWC->FCG3_b.ADC2);
  l_BackupADCADCStopmode_st.FCG3_ADC3 = (uint8_t)(bCM_PWC->FCG3_b.ADC3);
  /* Disable DAC */
  bCM_DAC1->DACR_b.DAE = (uint32_t)0U;
  bCM_DAC2->DACR_b.DAE = (uint32_t)0U;
  /* Disable ADC*/
  bCM_ADC1->STR_b.STRT = (uint32_t)0U;
  bCM_ADC2->STR_b.STRT = (uint32_t)0U;
  bCM_ADC3->STR_b.STRT = (uint32_t)0U;
  bCM_PWC->FCG3_b.ADC1 = (uint32_t)1U;
  bCM_PWC->FCG3_b.ADC2 = (uint32_t)1U;
  bCM_PWC->FCG3_b.ADC3 = (uint32_t)1U;
  /* Enable source wake-up system from stop-mode */
  Power_Ip_ControlWakeupStopModeSrc(para_PowerStopModeCfg_ptr->StopMode_Wkusrc_ptr);
  /**
  * Check Flash and DMA ready or not
  * if ready then config and enter to stop_mode
  */
  f_CheckReady_u8 = Power_Ip_StopModeIsReady();
  if ((Std_ReturnType)READY == f_CheckReady_u8)
  {
    #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
    /* Clear lpm flag */
    WRITE_REG16(CM_PWC->LPMCSCR, PWC_LPMCSCR_LPMFC);
    #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

    SET_REG16_BIT(CM_PWC->STPMCR, PWC_STPMCR_STOP);
    if (1U == para_PowerStopModeCfg_ptr->McuStopModeState_u8)
    {
      /* Mode control is enable */
      CLR_REG8_BIT(CM_PWC->PWRC0, PWC_PWRC0_PWDN);
    }
    /* Before executing the WFI instruction, make sure that all bit of the non-maskable
    interrupt flag register (INTC_NMIFR) are 0 */
    #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
    SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                            INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                            INTC_NMIFCR_RECCERRCLR | INTC_NMIFCR_BUSERRCLR | \
                                            INTC_NMIFCR_WDTCLR | INTC_NMIFCR_FECCERRCLR |\
                                            INTC_NMIFCR_ERMUCLR));
    #else
    SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                            INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                            INTC_NMIFCR_REPCLR | INTC_NMIFCR_RECCERRCLR | \
                                            INTC_NMIFCR_BUSERRCLR | INTC_NMIFCR_WDTCLR));
    #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
    /* Checking the XTAL, HRC, PLL status before enter to low-power mode */
    f_CheckCondition_u8 = Power_Ip_CheckConditionLowPower();
    if (E_OK == f_CheckCondition_u8)
    {
      /* Wait for LPM status */
      #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
      while (0U == READ_REG32(CM_PWC->LPMCSR)) 
      {
      #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

        ASM_WFI();
        
      #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
      }
      #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
    }
  }
}

/**
 *  @brief          This function configure power down mode
 *                  for microcontroller
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ModePwd_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerDownModeCfg_ptr    Struct configure for power-down mode.
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
)
{
  Std_ReturnType  f_CheckFlash_u8 = NOT_READY;
  Std_ReturnType  f_CheckCondition_u8 = E_NOT_OK;
  /**
  * Before executing WFI instruction to enter the power down
  * mode, It's necessary to ensure that the FLASH is not in
  * programming or erasing state, and the oscillation stop
  * detection function is invalid, otherwise the chip will
  * enter to sleep mode instead of the power down mode.
  */
  f_CheckFlash_u8 = Power_Ip_CheckFlashStatus();
  if ((Std_ReturnType)READY == f_CheckFlash_u8)
  {
    #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
    /* Clear lpm flag */
    WRITE_REG16(CM_PWC->LPMCSCR, PWC_LPMCSCR_LPMFC);
    #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

    /* confiure for power-down mode: choose PWD mode(1-2-3-4) */
    Power_Ip_PowerDownCfg(para_PowerDownModeCfg_ptr);
    /**
    * When the power-down wake-up event occurs, the correspondings
    * power-down wake-up flag(PWC_PWDKF0>>PWDKF1) will be set.
    * Clear the power-down wake-up flag, if Flag is not clear
    * The chip cannot enter power-down mode again.
    */
    Power_Ip_ClearPWDWakeUpFlag();
    /**
    * The power-down wake up event is controlled by the
    * power-down wake-up enbale register (PWC_PDWKE0-PDWKE3)
    * and the power-down wake-up event edge selection register
    * (PWC_PDWKES).
    */
    /* Control source wake-up*/
    Power_Ip_ControlPowerdownWakeUpSrc(para_PowerDownModeCfg_ptr->PWDWKUSrc_ptr);
    /* Select edge */
    Power_Ip_SelPWDWKUedge(para_PowerDownModeCfg_ptr->PWDWakeupTrigger_ptr);
    /**
    * The Interrupt is generated when the VCC change meets
    * The PVD1/PVD2 detection condition
    */
    CLR_REG8_BIT(CM_PWC->PVDCR1, PWC_PVDCR1_PVD1IRS | PWC_PVDCR1_PVD2IRS);
    /* Set Stop-mode control is valid */
    SET_REG16_BIT(CM_PWC->STPMCR, PWC_STPMCR_STOP);
    
    if (1U == para_PowerDownModeCfg_ptr->McuPwdState_u8)
    {
      /* Set Power-down mode control is enable */
      SET_REG8_BIT(CM_PWC->PWRC0, PWC_PWRC0_PWDN);
    }
    /**
    * Before executing the WFI instruction, make sure that all bit of the non-maskable
    * interrupt flag register (INTC_NMIFR) are 0
    */
    #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
    SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                            INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                            INTC_NMIFCR_RECCERRCLR | INTC_NMIFCR_BUSERRCLR | \
                                            INTC_NMIFCR_WDTCLR | INTC_NMIFCR_FECCERRCLR |\
                                            INTC_NMIFCR_ERMUCLR));
    #else
    SET_REG32_BIT(CM_INTC->NMIFCR, (uint32_t)(INTC_NMIFCR_SWDTCLR | INTC_NMIFCR_PVD1CLR | \
                                            INTC_NMIFCR_PVD2CLR | INTC_NMIFCR_XTALSTPCLR | \
                                            INTC_NMIFCR_REPCLR | INTC_NMIFCR_RECCERRCLR | \
                                            INTC_NMIFCR_BUSERRCLR | INTC_NMIFCR_WDTCLR));
    #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

    /* Checking the XTAL, HRC, PLL status before enter to low-power mode */
    f_CheckCondition_u8 = Power_Ip_CheckConditionLowPower();
    if (E_OK == f_CheckCondition_u8)
    {
      /* Wait for LPM status */
      #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
      while (0U == READ_REG32(CM_PWC->LPMCSR)) 
      {
      #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

        ASM_WFI();
        
      #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
      }
      #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
    }
  }
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
