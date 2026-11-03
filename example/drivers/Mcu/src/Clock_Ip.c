/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip.c
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
 *   @file       Clock_Ip.c
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
#include "Ram_Ip.h"
#include "Clock_Ip.h"
#include "Clock_Ip_Irq.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define CMU_PLLH_CONFIG             (1U)
#define CMU_PLLA_CONFIG             (2U)
#define CMU_PLLA_PLLH_CONFIG        (3U)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 * @brief  g_SystemCoreClock_u32 No. of SystemCore configured
 */
/* System clock frequency (Core clock) */
uint32_t  g_SystemCoreClock_u32;
#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"
static void Clock_Ip_StartTimeout(
    uint32_t * para_StartTimeOut_ptr,
    uint32_t * para_TimeoutTicksOut_ptr,
    uint32_t para_TimeoutUs_u32,
    Osr_CounterType para_OsifCounter_en
);

static boolean Clock_Ip_TimeoutExpired(
    uint32_t * para_StartTime_ptr,
    uint32_t * para_ElapsedTicks_ptr,
    uint32_t para_TimeoutTicks_u32,
    Osr_CounterType para_OsifCounter_en
);

static void Clock_Ip_DistributePllA(
    void
);

static void Clock_Ip_DistributePllH(
    void
);

static Std_ReturnType Clock_Ip_WaitStable(
    const uint8_t  para_Flag_u8
);

static void Clock_Ip_ConfigPeriphClock(
    const Clock_Ip_PeriphClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigSystemClock(
    const Clock_Ip_SelSysClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigCan1Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigCan2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

#ifdef CM_MCAN1
static void Clock_Ip_ConfigMcan1Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);
#endif /* CM_MCAN1 */

#ifdef CM_MCAN1
static void Clock_Ip_SelMcan1PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);
#endif /* CM_MCAN1 */

#ifdef CM_MCAN2
static void Clock_Ip_ConfigMcan2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);
#endif /* CM_MCAN2 */

#ifdef CM_MCAN2
static void Clock_Ip_SelMcan2PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);
#endif /* CM_MCAN2 */

static void Clock_Ip_ConfigTpiuClk(
    const Clock_Ip_TpiuConfigType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigPclk2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigPclk4Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigI2sPeriph(
    const Clock_Ip_I2sConfigType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigUsbPeriph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_ConfigMcoPeriph(
    const Clock_Ip_PinOutConfigType * para_ClockConfig_ptr
);

static void Clock_Ip_SramSetWaitCycle(
    uint32_t  para_SramSel_u32,
    uint32_t  para_WriteCycle_u32,
    uint32_t  para_ReadCycle_u32
);

static void Clock_Ip_EFMSetWaitCycle(
    uint32_t  para_WaitCycle_u32
);

static void Clock_Ip_GPIOSetWaitCycle(
    uint16_t  para_WaitCycle_u16
);

static void Clock_Ip_ConfigWaitCycle(
    const Clock_Ip_SelSysClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_XtalStopDetctInit(
    const Clock_Ip_XtalConfigType * para_XTALConfig_ptr
);

static void Clock_Ip_FCMMeasurement(
    CM_FCM_TypeDef * para_FCMx_ptr,
    const Clock_Ip_FcmConfigType * para_FCMConfig_ptr
);

static void Clock_SetSysClockSrc(
    const uint8_t  para_ClockSource_u8
);

static void Clock_SetSysClockDiv(
    const uint32_t  para_ClockSource_u32,
    const uint32_t  para_ClockDiv_u32
);

static void Clock_Ip_RtcConfig(
    const Clock_Ip_RtcLrcConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_WaitStopped(
    const uint8_t  para_Flag_u8
);

static Std_ReturnType Clock_Ip_ConfigHrcClk(
    const Clock_Ip_HrcConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_ConfigLrcClk(
    const Clock_Ip_LrcConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_ConfigMrcClk(
    const Clock_Ip_MrcConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_ConfigPllaClk(
    const Clock_Ip_PllaConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_ConfigXtalClk(
    const Clock_Ip_XtalConfigType * para_ClockConfig_ptr
);

static Std_ReturnType Clock_Ip_ConfigXtal32Clk(
    const Clock_Ip_Xtal32ConfigType * para_ClockConfig_ptr
);

static void Clock_Ip_SelCan1PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

static void Clock_Ip_SelCan2PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
);

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
static Std_ReturnType Clock_Ip_ConfigXtalDiv(
    const Clock_Ip_XtalDivConfigType * para_XtalDivConfig_ptr
);
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

static void Clock_Ip_SetSysClockSrc(
    const uint8_t  para_ClockSource_u8
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 * @brief  l_McuPllCfg_u8 No. of PLLs configured
 */
static uint8_t l_McuPllCfg_u8 = 0U;

/**
 * @brief  l_McuPllCfg_u8 Variable (pointer) used for storing the MCU driver
 *         PLLH clock
 */
static const Clock_Ip_PllhConfigType* l_McuPllhClockConfig_ptr = NULL_PTR;

/**
 * @brief  l_McuPllaClockConfig_ptr Variable (pointer) used for storing the MCU driver
 *         PLLA clock
 */
static const Clock_Ip_PllaConfigType* l_McuPllaClockConfig_ptr = NULL_PTR;
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
 *  @details        Design ID:         Clock_Ip_StartTimeout_Activity
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
static void Clock_Ip_StartTimeout(
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
 *  @details        Design ID:         Clock_Ip_TimeoutExpired_Activity
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
static boolean Clock_Ip_TimeoutExpired(
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
 *  @brief          This function wait for stable in period timeout
 *
 *  @details        Design ID:         Clock_Ip_WaitStable_Activity
 * 
 *  @param[in]      para_Flag_u8    Flag stable in CMU_OSCSTBSR register
 *
 *  @return         Std_ReturnType    Status of stability check
 *  @retval         E_NOT_OK          Unstable after the period timeout
 *  @retval         E_OK              Stable after the period timeout
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_WaitStable(
    const uint8_t  para_Flag_u8
)
{
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;
  Std_ReturnType  f_ReturnValue_en = E_NOT_OK;
  uint8_t StableFlag_u8 = 0U;

  /* Initialize timeout mechanism to prevent infinite waiting */
  ElapsedTicks_u32 = 0U;
  Clock_Ip_StartTimeout(&StartTime_u32,
                          &TimeoutTicks_u32,
                          (uint32_t)CLOCK_IP_TIMEOUT_VALUE,
                          CLOCK_IP_TIMEOUT_TYPE);
  
  /* Wait for oscillator to become stable by checking the stability flag
     Continue waiting until flag is set (oscillator is stable) OR timeout occurs */
  do
  {
      /* Check if timeout has expired */
      TimeoutStatus_bool = Clock_Ip_TimeoutExpired(&StartTime_u32,
                                                  &ElapsedTicks_u32,
                                                  TimeoutTicks_u32,
                                                  CLOCK_IP_TIMEOUT_TYPE);
      /* Read the stability flag from oscillator stability status register */
      StableFlag_u8   = (uint8_t)(CM_CMU->OSCSTBSR & para_Flag_u8);
      /* Loop continues while oscillator is not stable (flag not set) AND timeout hasn't occurred */
  }
  while ((0x00U == StableFlag_u8) && ((boolean)FALSE == TimeoutStatus_bool));
  
  /* Check if we exited due to timeout or oscillator becoming stable */
  if ((boolean)TRUE == TimeoutStatus_bool)
  {
      /* Timeout occurred - oscillator is still unstable */
      f_ReturnValue_en = E_NOT_OK;
  }
  else
  {
      /* Oscillator became stable within timeout period */
      f_ReturnValue_en = E_OK;
  }

  return f_ReturnValue_en;
}

/**
 *  @brief          This function wait for stopped in period timeout
 *
 *  @details        Design ID:         Clock_Ip_WaitStopped_Activity
 * 
 *  @param[in]      para_Flag_u8    Flag stopped in CMU_OSCSTBSR register
 *
 *  @return         Std_ReturnType    Status of stop check
 *  @retval         E_NOT_OK          Unstopped after the period timeout
 *  @retval         E_OK              stopped after the period timeout
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_WaitStopped(
    const uint8_t  para_Flag_u8
)
{
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;
  Std_ReturnType  f_ReturnValue_en = E_NOT_OK;
  uint8_t StoppedFlag_u8 = 0U;

  /* Initialize timeout mechanism to prevent infinite waiting */
  ElapsedTicks_u32 = 0U;
  Clock_Ip_StartTimeout(&StartTime_u32,
                          &TimeoutTicks_u32,
                          (uint32_t)CLOCK_IP_TIMEOUT_VALUE,
                          CLOCK_IP_TIMEOUT_TYPE);
  
  /* Wait for oscillator to stop by checking the stop flag
     Continue waiting until flag is cleared (oscillator has stopped) OR timeout occurs */
  do
  {
      /* Check if timeout has expired */
      TimeoutStatus_bool = Clock_Ip_TimeoutExpired(&StartTime_u32,
                                                  &ElapsedTicks_u32,
                                                  TimeoutTicks_u32,
                                                  CLOCK_IP_TIMEOUT_TYPE);
      /* Read the stop flag from oscillator stability status register */
      StoppedFlag_u8   = (uint8_t)(CM_CMU->OSCSTBSR & para_Flag_u8);
      /* Loop continues while oscillator is still running (flag is set) AND timeout hasn't occurred */
  }
  while ((0x00U != StoppedFlag_u8) && ((boolean)FALSE == TimeoutStatus_bool));
  
  /* Check if we exited due to timeout or oscillator stopping */
  if ((boolean)TRUE == TimeoutStatus_bool)
  {
      /* Timeout occurred - oscillator is still running */
      f_ReturnValue_en = E_NOT_OK;
  }
  else
  {
      /* Oscillator stopped within timeout period */
      f_ReturnValue_en = E_OK;
  }

  return f_ReturnValue_en;
}

 /**
 *  @brief          This function will configure for HRC to work
 *
 *  @details        Design ID:         Clock_Ip_ConfigHrcClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of HRC
 *
 *  @return         Std_ReturnType    Status of HRC configuration
 *  @retval         E_OK              HRC configuration was successful
 *  @retval         E_NOT_OK          HRC configuration failed (HRC as system clock source or PLL source)
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigHrcClk(
    const Clock_Ip_HrcConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* Set HRC trimming value. */
    CM_CMU->HRCTRM = (uint8_t)(para_ClockConfig_ptr->McuHrcTrim_s8);

    if ((uint8_t)MCU_DISABLE == para_ClockConfig_ptr->McuHrcState_u8)
    {
        if (CLK_SYSCLK_SRC_HRC == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        else if (CLK_PLL_SRC_HRC == ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS))
        {
            /* HRC as PLL clock source and PLL is working */
            if ((uint8_t)0U == CM_CMU->PLLHCR)
            {
                f_ReturnValue_en = E_NOT_OK;
            }
            else
            {
                /* HRC disable. */
                CM_CMU->HRCCR = CLK_HRC_OFF;
                /* wait stopped */
                f_ReturnValue_en = Clock_Ip_WaitStopped(CLK_STB_FLAG_HRC);
            }
        }
        else
        {
            /* HRC disable. */
            CM_CMU->HRCCR = CLK_HRC_OFF;
            /* wait stopped */
            f_ReturnValue_en = Clock_Ip_WaitStopped(CLK_STB_FLAG_HRC);
        }
    }
    else
    {
        /* HRC enable. */
        CM_CMU->HRCCR = CLK_HRC_ON;
        /* wait stable */
        f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_HRC);
    }
    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function configure for LRC to work.
 *
 *  @details        Design ID:         Clock_Ip_ConfigLrcClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of LRC
 *
 *  @return         Std_ReturnType    Status of LRC configuration
 *  @retval         E_NOT_OK          The driver state is did not allowed the execution (LRC as system clock source)
 *  @retval         E_OK              The driver state is allowed the execution (LRC as system clock source)
 *
 *  @note           This function will be called by Clock_Ip_InitClock
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigLrcClk(
    const Clock_Ip_LrcConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* Set LRC trimming value. */
    CM_CMU->LRCTRM = (uint8_t)para_ClockConfig_ptr->McuLrcTrim_s8;

    if ((uint8_t)MCU_DISABLE == para_ClockConfig_ptr->McuLrcState_u8)
    {
        if (CLK_SYSCLK_SRC_LRC == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        else
        {
            /* LRC disable. */
            CM_CMU->LRCCR = CLK_LRC_OFF;
            /* wait for 5 LRC cycles */
            Clock_Ip_Delay(CLK_LRC_TIMEOUT);
        }
    }
    else
    {
        /* LRC enable. */
        CM_CMU->LRCCR = CLK_LRC_ON;
        /* wait for 5 LRC cycles */
        Clock_Ip_Delay(CLK_LRC_TIMEOUT);
    }
    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function configure for MRC to work.
 *
 *  @details        Design ID:         Clock_Ip_ConfigMrcClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to struct configure of MRC
 *
 *  @return         Std_ReturnType    Status of MRC configuration
 *  @retval         E_NOT_OK          The driver state did not allowed the execution (MRC as system clock source)
 *                                    or MRC is unstable
 *  @retval         E_OK              The driver state allowed the execution and MRC is stable
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigMrcClk(
    const Clock_Ip_MrcConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* Set MRC trimming value. */
    CM_CMU->MRCTRM = (uint8_t)para_ClockConfig_ptr->McuMrcTrim_s8;

    if ((uint8_t)MCU_DISABLE == (para_ClockConfig_ptr->McuMrcState_u8))
    {
        if (CLK_SYSCLK_SRC_MRC == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        else
        {
            /* MRC disable. */
            CM_CMU->MRCCR = CLK_MRC_OFF;
            /* wait for 5 MRC cycles */
            Clock_Ip_Delay(CLK_MRC_TIMEOUT);
        }
    }
    else
    {
        /* MRC enable. */
        CM_CMU->MRCCR = CLK_MRC_ON;
        /* wait for 5 MRC cycles */
        Clock_Ip_Delay(CLK_MRC_TIMEOUT);
    }
    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function distribute PLLA
 *
 *  @details        Design ID:         Clock_Ip_DistributePllA_Activity
 * 
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Clock_Ip_DistributePllA(
    void
)
{
    uint32_t  f_PllR_u32;
    uint32_t  f_PllQ_u32;
    uint32_t  f_PllP_u32;
    uint32_t  f_PllCfg_u32;

    if (NULL_PTR != l_McuPllhClockConfig_ptr)
    {
        /* PLLHR frequency drvision factor selection */
        f_PllR_u32   = ((uint32_t)l_McuPllaClockConfig_ptr->McuPllaRDiv_u8 << CMU_PLLACFGR_PLLAR_POS);
        /* PLLHQ frequency drvision factor selection */
        f_PllQ_u32   = ((uint32_t)l_McuPllaClockConfig_ptr->McuPllaQDiv_u8 << CMU_PLLACFGR_PLLAQ_POS);
        /* PLLHP frequency drvision factor selection */
        f_PllP_u32   = ((uint32_t)l_McuPllaClockConfig_ptr->McuPllaPDiv_u8 << CMU_PLLACFGR_PLLAP_POS);

        f_PllCfg_u32 = (f_PllR_u32 | f_PllQ_u32 | f_PllP_u32);
        CM_CMU->PLLACFGR = (CM_CMU->PLLACFGR & ~(CMU_PLLACFGR_PLLAR|CMU_PLLACFGR_PLLAQ|CMU_PLLACFGR_PLLAP))|f_PllCfg_u32;
        /* wait stable */
        (void)Clock_Ip_WaitStable(CLK_STB_FLAG_PLLA);
    }
}

/**
 *  @brief          This function distribute PLLH
 *
 *  @details        Design ID:         Clock_Ip_DistributePllH_Activity
 * 
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Clock_Ip_DistributePllH(
    void
)
{
    uint32_t  f_PllR_u32;
    uint32_t  f_PllQ_u32;
    uint32_t  f_PllCfg_u32;

    if (NULL_PTR != l_McuPllhClockConfig_ptr)
    {
        /* PLLHR frequency drvision factor selection */
        f_PllR_u32  = ((uint32_t)l_McuPllhClockConfig_ptr->McuPllhRDiv_u8 << CMU_PLLHCFGR_PLLHR_POS);
        /* PLLHQ frequency drvision factor selection */
        f_PllQ_u32  = ((uint32_t)l_McuPllhClockConfig_ptr->McuPllhQDiv_u8 << CMU_PLLHCFGR_PLLHQ_POS);
        
        f_PllCfg_u32 = (f_PllR_u32 | f_PllQ_u32);
        /* Configuration register for CMU PLLH */
        CM_CMU->PLLHCFGR = (CM_CMU->PLLHCFGR & ~(CMU_PLLHCFGR_PLLHR|CMU_PLLHCFGR_PLLHQ))|f_PllCfg_u32;
        /* wait stable */
        (void)Clock_Ip_WaitStable(CLK_STB_FLAG_PLLH);
    }
}

/**
 *  @brief          This function configures peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigPeriphClock_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to peripheral clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock().
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigPeriphClock(
    const Clock_Ip_PeriphClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuPeriphClock_en)
  {
    case MCU_CAN1CLK:
      /* Configure the clock for Can1 */
      Clock_Ip_ConfigCan1Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_CAN2CLK:
      /* Configure the clock for Can2 */
      Clock_Ip_ConfigCan2Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_MCAN1CLK:
      #ifdef CM_MCAN1
      /* Configure the clock for Mcan1 */
      Clock_Ip_ConfigMcan1Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      #endif
      break;
    case MCU_MCAN2CLK:
      #ifdef CM_MCAN2
      /* Configure the clock for Mcan2 */
      Clock_Ip_ConfigMcan2Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      #endif
      break;
    case MCU_TPIUCLK:
      /* Set system clock source */
      Clock_Ip_ConfigTpiuClk((const Clock_Ip_TpiuConfigType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_HCLK:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_HCLK, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      break;
    case MCU_PCLK0:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK0, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      break;
    case MCU_PCLK1:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK1, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      break;
    case MCU_PCLK2:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK2, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      /* Configure the clock for Pclk2 */
      Clock_Ip_ConfigPclk2Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_PCLK3:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK3, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      break;
    case MCU_PCLK4:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK4, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      /* Configure the clock for Pclk2 */
      Clock_Ip_ConfigPclk4Periph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_EXCLK:
      /* System clock divider config. */
      Clock_Ip_SetSysClockDiv(CLK_BUS_EXCLK, ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->SysClockDiv_u32);
      break;
    case MCU_I2SCLK:
      /* Configure the clock for I2s */
      Clock_Ip_ConfigI2sPeriph((const Clock_Ip_I2sConfigType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_USBCLK:
      /* Configure the clock for Usb */
      Clock_Ip_ConfigUsbPeriph((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
    case MCU_RTCCLK:
      if (MCU_CMU_SELXTAL32_CLK == ((const Clock_Ip_SelClockSourceType *)para_ClockConfig_ptr->McuPeriphConfig_ptr)->McuClockSelect_en) {
        /* Configure the Xtal32 clock for Rtc */
        MODIFY_REG8(CM_RTC->CR3, (RTC_CR3_LRCEN | RTC_CR3_RCKSEL), RTC_CLK_SRC_XTAL32);
      } else {
        /* Configure the Lrc clock for Rtc */
        MODIFY_REG8(CM_RTC->CR3, (RTC_CR3_LRCEN | RTC_CR3_RCKSEL), RTC_CLK_SRC_LRC);
      }
      break;
    case MCU_STCLK:
      /* Configure the clock for St, nothing to do */
      break;
    case MCU_SWDTCLK:
      /* Configure the clock for Swdt, nothing to do */
      break;
    default:
      /* Configure the clock for Pin out */
      Clock_Ip_ConfigMcoPeriph((const Clock_Ip_PinOutConfigType *)para_ClockConfig_ptr->McuPeriphConfig_ptr);
      break;
  }
}

/**
 *  @brief          This function configures system clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigSystemClock_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to system clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigSystemClock(
    const Clock_Ip_SelSysClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuSelectSysClock_en)
  {
    case MCU_CMU_SELXTAL_CLK:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_XTAL);
      break;
    case MCU_CMU_SELXTAL32_CLK:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_XTAL32);
      break;
    case MCU_CMU_SELPLLHP_CLK:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
      break;
    case MCU_CMU_SELHRC_CLK:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_HRC);
      break;
    case MCU_CMU_SELLRC_CLK:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_LRC);
      break;
    default:
      /* Set system clock source */
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
      break;
  }
}

/**
 *  @brief          This function configures CAN1 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigCan1Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to CAN1 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigCan1Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELXTAL_CLK:
      /* Select XTAL clock */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_XTAL << CMU_CANCKCFGR_CAN1CKS_POS));
      break;
    case MCU_CMU_SELPLLHQ_CLK:
    case MCU_CMU_SELPLLHR_CLK:
    case MCU_CMU_SELPLLAP_CLK:
    case MCU_CMU_SELPLLAQ_CLK:
    case MCU_CMU_SELPLLAR_CLK:
      /* Select Pll clock */
      Clock_Ip_SelCan1PllClk(para_ClockConfig_ptr);
      break;
    default:
      /* Select system clock div */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(para_ClockConfig_ptr->SysClockDiv_u32 << CMU_CANCKCFGR_CAN1CKS_POS));
      break;
  }
}

/**
 *  @brief          This function configures CAN2 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigCan2Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to CAN2 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigCan2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELXTAL_CLK:
      /* Select XTAL clock */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_XTAL << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLHQ_CLK:
    case MCU_CMU_SELPLLHR_CLK:
    case MCU_CMU_SELPLLAP_CLK:
    case MCU_CMU_SELPLLAQ_CLK:
    case MCU_CMU_SELPLLAR_CLK:
      /* Select Pll clock */
      Clock_Ip_SelCan2PllClk(para_ClockConfig_ptr);
      break;
    default:
      /* Select system clock div */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(para_ClockConfig_ptr->SysClockDiv_u32 << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
  }
}

#ifdef CM_MCAN1
/**
 *  @brief          This function configures MCAN1 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigMcan1Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to MCAN1 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigMcan1Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELXTAL_CLK:
      /* Select XTAL clock */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_XTAL << CMU_CANCKCFGR_MCAN1CKS_POS));
      break;
    case MCU_CMU_SELPLLHQ_CLK:
    case MCU_CMU_SELPLLHR_CLK:
    case MCU_CMU_SELPLLAP_CLK:
    case MCU_CMU_SELPLLAQ_CLK:
    case MCU_CMU_SELPLLAR_CLK:
      /* Select Pll clock */
      Clock_Ip_SelMcan1PllClk(para_ClockConfig_ptr);
      break;
    default:
      /* Select system clock div */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)(para_ClockConfig_ptr->SysClockDiv_u32 << CMU_CANCKCFGR_MCAN1CKS_POS));
      break;
  }
}
#endif /* CM_MCAN1 */


#ifdef CM_MCAN2
/**
 *  @brief          This function configures MCAN2 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigMcan2Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to MCAN2 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigMcan2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{ 
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELXTAL_CLK:
      /* Select XTAL clock */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_XTAL << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLHQ_CLK:
    case MCU_CMU_SELPLLHR_CLK:
    case MCU_CMU_SELPLLAP_CLK:
    case MCU_CMU_SELPLLAQ_CLK:
    case MCU_CMU_SELPLLAR_CLK:
      /* Select Pll clock */  
      Clock_Ip_SelMcan2PllClk(para_ClockConfig_ptr);
      break;
    default:
      /* Select system clock div */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)(para_ClockConfig_ptr->SysClockDiv_u32 << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
  }
}
#endif /* CM_MCAN2 */

/**
 *  @brief          This function configures TPIU clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigTpiuClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to TPIU clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigTpiuClk(
    const Clock_Ip_TpiuConfigType * para_ClockConfig_ptr
)
{
  /* Clock divider select */
  CM_CMU->TPIUCKCFGR = (CM_CMU->TPIUCKCFGR & (uint8_t)(~CMU_TPIUCKCFGR_TPIUCKS)) | \
  (CMU_TPIUCKCFGR_TPIUCKS & para_ClockConfig_ptr->McuSysClockDiv_u8);
  /* Enable or disable the TPIU clock. */
  CM_CMU->TPIUCKCFGR = (CM_CMU->TPIUCKCFGR & (uint8_t)(~CMU_TPIUCKCFGR_TPIUCKOE)) | \
    (CMU_TPIUCKCFGR_TPIUCKOE & (para_ClockConfig_ptr->McuTpiuState_u8 << CMU_TPIUCKCFGR_TPIUCKOE_POS));
}

/**
 *  @brief          This function configures PLCK2 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigPclk2Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to PLCK2 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigPclk2Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELSYSDIV_CLK:
      /* Set PCLK clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PCLK;
      break;
    case MCU_CMU_SELPLLHQ_CLK:
      /* Set PLLHQ clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLHQ;
      break;
    case MCU_CMU_SELPLLHR_CLK:
      /* Set PLLHR clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLHR;
      break;
    case MCU_CMU_SELPLLAP_CLK:
      /* Set PLLAP clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAP;
      break;
    case MCU_CMU_SELPLLAQ_CLK:
      /* Set PLLAQ clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAQ;
      break;
    default:
      /* Set PLLAR clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAR;
      break;
  }
}

/**
 *  @brief          This function configures PLCK4 peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigPclk4Periph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to PLCK4 clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigPclk4Periph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELSYSDIV_CLK:
      /* Set PCLK clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PCLK;
      break;
    case MCU_CMU_SELPLLHQ_CLK:
      /* Set PLLHQ clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLHQ;
      break;
    case MCU_CMU_SELPLLHR_CLK:
      /* Set PLLHR clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLHR;
      break;
    case MCU_CMU_SELPLLAP_CLK:
      /* Set PLLAP clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAP;
      break;
    case MCU_CMU_SELPLLAQ_CLK:
      /* Set PLLAQ clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAQ;
      break;
    default:
      /* Set PLLAR clock. */
      CM_CMU->PERICKSEL = CLK_PERIPHCLK_PLLAR;
      break;
  }
}

/**
 *  @brief          This function configures I2S peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigI2sPeriph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to I2S clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigI2sPeriph(
    const Clock_Ip_I2sConfigType * para_ClockConfig_ptr
)
{
  /* I2S clock source config. */
  MODIFY_REG16(CM_CMU->I2SCKSEL, (uint16_t)CMU_I2SCKSEL_I2S1CKSEL << (para_ClockConfig_ptr->McuI2sUnit_u8 * CMU_I2SCKSEL_I2S2CKSEL_POS), \
                 (uint16_t)para_ClockConfig_ptr->McuI2sSource_u8 << (para_ClockConfig_ptr->McuI2sUnit_u8 * CMU_I2SCKSEL_I2S2CKSEL_POS));
}

/**
 *  @brief          This function configures USB peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigUsbPeriph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to USB clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigUsbPeriph(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELPLLHQ_CLK:
      /* Select PLLHQ clock for Usb */
      CM_CMU->USBCKCFGR = CLK_USBCLK_PLLQ;
      break;
    case MCU_CMU_SELPLLHR_CLK:
      /* Select PLLHR clock for Usb */
      CM_CMU->USBCKCFGR = CLK_USBCLK_PLLR;
      break;
    case MCU_CMU_SELPLLAP_CLK:
      /* Select PLLAP clock for Usb */
      CM_CMU->USBCKCFGR = CLK_USBCLK_PLLAP;
      break;
    case MCU_CMU_SELPLLAQ_CLK:
      /* Select PLLAQ clock for Usb */
      CM_CMU->USBCKCFGR = CLK_USBCLK_PLLAQ;
      break;
    case MCU_CMU_SELPLLAR_CLK:
      /* Select PLLAR clock for Usb */
      CM_CMU->USBCKCFGR = CLK_USBCLK_PLLAR;
      break;
    default:
      /* Usb clock source selection */
      CM_CMU->USBCKCFGR = (uint8_t)(para_ClockConfig_ptr->SysClockDiv_u32);
      break;
  }
}

/**
 *  @brief          This function configures MCO peripheral clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigMcoPeriph_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to MCO clock configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *                  This API is available if only McuInitClock parameter is configured as true.
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigMcoPeriph(
    const Clock_Ip_PinOutConfigType * para_ClockConfig_ptr
)
{
    volatile uint8_t * f_MCOCFGRx_ptr;

    /* Use array indexing instead of pointer arithmetic */
    f_MCOCFGRx_ptr = &((volatile uint8_t *)&CM_CMU->MCO1CFGR)[para_ClockConfig_ptr->McuMcoChannel_u8];
    
    /* MCO frequency divison and output clock source selection */
    MODIFY_REG8(*f_MCOCFGRx_ptr, (CMU_MCOCFGR_MCOSEL | CMU_MCOCFGR_MCODIV), (para_ClockConfig_ptr->McuMcoSource_u8 | para_ClockConfig_ptr->McuMcoDivider_u8));
    /* Enable or disable clock output */
    MODIFY_REG8(*f_MCOCFGRx_ptr, CMU_MCOCFGR_MCOEN, (uint8_t)para_ClockConfig_ptr->McuMcoState_u8 << CMU_MCOCFGR_MCOEN_POS);
}

/**
 *  @brief          Specifies access wait cycle for SRAM
 *
 *  @details        Design ID:               Clock_Ip_SramSetWaitCycle_Activity
 * 
 *  @param[in]      para_SramSel_u32         The SRAM selection, this parameter can be values of SRAM_Sel
 *  @param[in]      para_WriteCycle_u32      The write access wait cycle for the specified SRAM, this parameter
 *                                           can be a value of SRAM_Access_Wait_Cycle
 *  @param[in]      para_ReadCycle_u32       The read access wait cycle for the specified SRAM, this parameter can
 *                                           be a value of SRAM_Access_Wait_Cycle
 *  @arg            MCU_SRAM_WAIT_CYCLE0     Wait 0 CPU cycle
 *  @arg            MCU_SRAM_WAIT_CYCLE1     Wait 1 CPU cycle
 *  @arg            MCU_SRAM_WAIT_CYCLE2     Wait 2 CPU cycles
 *  @arg            MCU_SRAM_WAIT_CYCLE3     Wait 3 CPU cycles
 *  @arg            MCU_SRAM_WAIT_CYCLE4     Wait 4 CPU cycles
 *  @arg            MCU_SRAM_WAIT_CYCLE5     Wait 5 CPU cycles
 *  @arg            MCU_SRAM_WAIT_CYCLE6     Wait 6 CPU cycles
 *  @arg            MCU_SRAM_WAIT_CYCLE7     Wait 7 CPU cycles
 *
 *  @return         void
 *
 *  @note           Call SRAM_REG_Unlock to unlock register WTCR first
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SramSetWaitCycle(
    uint32_t  para_SramSel_u32,
    uint32_t  para_WriteCycle_u32,
    uint32_t  para_ReadCycle_u32
)
{
    uint8_t  f_Count_u8 = (uint8_t)0U;
    uint8_t  f_FsWt_u8;
    uint8_t  f_FsRd_u8;
    uint32_t  f_SramSel_u32 = para_SramSel_u32;

    while ((uint32_t)0U != f_SramSel_u32)
    {
        if ((f_SramSel_u32 & 0x1U) != (uint32_t)0U)
        {
          f_FsWt_u8 = f_Count_u8 * (uint8_t)8U;
          f_FsRd_u8 = f_FsWt_u8 + 4U;
          MODIFY_REG32(CM_SRAMC->WTCR, \
          ((MCU_SRAM_CYCLE_MASK << f_FsWt_u8) | (MCU_SRAM_CYCLE_MASK << f_FsRd_u8)), \
          ((para_WriteCycle_u32 << f_FsWt_u8) | (para_ReadCycle_u32 << f_FsRd_u8)));
        }
        f_SramSel_u32 = f_SramSel_u32 >> 1U;
        f_Count_u8++;
    }
}

/**
 *  @brief          Set the efm read wait cycles
 *
 *  @details        Design ID:         Clock_Ip_EFMSetWaitCycle_Activity
 * 
 *  @param[in]      para_WaitCycle_u32      Specifies the efm read wait cycles
 *  @arg            MCU_EFM_WAIT_CYCLEx     Wait x CPU cycle (x: 0 ~ 15)
 *
 *  @return         void
 *
 *  @note           unlock EFM register first
 *
 *  @pre            None
 *
 */
static void Clock_Ip_EFMSetWaitCycle(
    uint32_t  para_WaitCycle_u32
)
{
    uint32_t StartTime_u32                  = 0U;
    uint32_t TimeoutTicks_u32               = 0U;
    uint32_t ElapsedTicks_u32               = 0U;
    boolean TimeoutStatus_bool            = (boolean)FALSE;

    MODIFY_REG32(CM_EFM->FRMC, EFM_FRMC_FLWT, para_WaitCycle_u32);

    /* Initialize timeout */
    ElapsedTicks_u32 = 0U;
    Clock_Ip_StartTimeout(&StartTime_u32,
                         &TimeoutTicks_u32,
                         (uint32_t)CLOCK_IP_TIMEOUT_VALUE,
                         CLOCK_IP_TIMEOUT_TYPE);

    do
    {
        TimeoutStatus_bool = Clock_Ip_TimeoutExpired(&StartTime_u32,
                                                   &ElapsedTicks_u32,
                                                   TimeoutTicks_u32,
                                                   CLOCK_IP_TIMEOUT_TYPE);
    } while ((para_WaitCycle_u32 != READ_REG32_BIT(CM_EFM->FRMC, EFM_FRMC_FLWT)) && 
             ((boolean)FALSE == TimeoutStatus_bool));
}

/**
 *  @brief          Set the GPIO read wait cycles
 *
 *  @details        Design ID:         Clock_Ip_GPIOSetWaitCycle_Activity
 * 
 *  @param[in]      para_WaitCycle_u32    Specifies the GPIO read wait cycles
 *  @arg            MCU_GPIO_RD_WAITx     Wait x CPU cycle (x: 0 ~ 7)
 *
 *  @return         void
 *
 *  @note           unlock GPIO register first
 *
 *  @pre            None
 *
 */
static void Clock_Ip_GPIOSetWaitCycle(
    uint16_t  para_WaitCycle_u16
)
{
  MODIFY_REG16(CM_GPIO->PCCR, GPIO_PCCR_RDWT, para_WaitCycle_u16);
}

/**
 *  @brief          This function shall set the wait cycle for SRAM, FLASH, GPIO
 *
 *  @details        Design ID:         Clock_Ip_ConfigWaitCycle_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    The pointer to Wait Cycle configuration
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Clock_Ip_ConfigWaitCycle(
    const Clock_Ip_SelSysClockSourceType * para_ClockConfig_ptr
)
{
    const Clock_Ip_WaitCycleConfigType * f_WaitCycleConfig_ptr;
    f_WaitCycleConfig_ptr = (const Clock_Ip_WaitCycleConfigType*)para_ClockConfig_ptr->McuWaitCycleConfig_ptr;

    /* RAMPDC1_3_4_backup set to 1 Read/Write wait cycle */
    Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAM_ALL, f_WaitCycleConfig_ptr->McuSramWaitCycle_u32, \
                            f_WaitCycleConfig_ptr->McuSramWaitCycle_u32);
    /* Highspeed SRAM set to 0 Read/Write wait cycle */
    Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAMH, f_WaitCycleConfig_ptr->McuSramHWaitCycle_u32, \
                            f_WaitCycleConfig_ptr->McuSramHWaitCycle_u32);
    /* FLASH read wait cycle setting */
    Clock_Ip_EFMSetWaitCycle(f_WaitCycleConfig_ptr->McuFlashWaitCycle_u32);
    /* GPIO wait cycle setting */
    Clock_Ip_GPIOSetWaitCycle(f_WaitCycleConfig_ptr->McuGPIOWaitCycle_u16);
}

/**
 *  @brief          This function Initialize Xtal stop detect
 *
 *  @details        Design ID:         Clock_Ip_XtalStopDetctInit_Activity
 * 
 *  @param[in]      para_XTALConfig_ptr    Pointer to XTAL configuration structure
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Clock_Ip_XtalStopDetctInit(
    const Clock_Ip_XtalConfigType * para_XTALConfig_ptr
)
{
  /* Get stable status */
  if (0x00U != (CM_CMU->OSCSTBSR & CLK_STB_FLAG_XTAL))
  {
    /* Configure and enable XTALSTD */
    WRITE_REG8(CM_CMU->XTALSTDCR, (para_XTALConfig_ptr->McuXtalStdState_u8 | \
                                   para_XTALConfig_ptr->McuXtalStdMode_u8   | \
                                   para_XTALConfig_ptr->McuXtalStdInt_u8    | \
                                   para_XTALConfig_ptr->McuXtalStdReset_u8));
  }
}

/**
 *  @brief          This function Initialize Fcm Measurement
 *
 *  @details        Design ID:         Clock_Ip_FCMMeasurement_Activity
 * 
 *  @param[in]      para_FCMx_ptr         Pointer to FCM module structure
 *  @param[in]      para_FCMConfig_ptr    Pointer to FCM configuration structure
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Clock_Ip_FCMMeasurement(
    CM_FCM_TypeDef * para_FCMx_ptr,
    const Clock_Ip_FcmConfigType * para_FCMConfig_ptr
)
{
  /* Enable function clock */
  CLR_REG32_BIT(CM_PWC->FCG0, MCU_FCG0_PERIPH_FCM);

  /* Stop FCM measurement */
  if (CM_FCM1 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START1, (uint32_t)(MCU_DISABLE << FCM_COM_STR_START1_POS));
  }
  #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
  else if (CM_FCM2 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START2, (uint32_t)(MCU_DISABLE << FCM_COM_STR_START2_POS));
  }
  else if (CM_FCM3 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START3, (uint32_t)(MCU_DISABLE << FCM_COM_STR_START3_POS));
  }
  else if (CM_FCM4 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START4, (uint32_t)(MCU_DISABLE << FCM_COM_STR_START4_POS));
  }
  else
  {
    /* Do nothing */
  }
  #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

  /* Set FCM lower limit compare value */
  WRITE_REG32(para_FCMx_ptr->LVR,   para_FCMConfig_ptr->McuFcmLowerLimit_u16);
  /* Set FCM upper limit compare value */
  WRITE_REG32(para_FCMx_ptr->UVR,   para_FCMConfig_ptr->McuFcmUpperLimit_u16);
  /* Config FCM measurement object */
  WRITE_REG32(para_FCMx_ptr->MCCR, (para_FCMConfig_ptr->McuFcmTargetClock_u32 | \
                             para_FCMConfig_ptr->McuFcmTargetClockDiv_u32));
  /* Config FCM measurement reference */
  WRITE_REG32(para_FCMx_ptr->RCCR, (para_FCMConfig_ptr->McuFcmExtRefClockEnable_u32 | \
                             para_FCMConfig_ptr->McuFcmRefClockEdge_u32      | \
                             para_FCMConfig_ptr->McuFcmDigitalFilter_u32     | \
                             para_FCMConfig_ptr->McuFcmRefClock_u32          | \
                             para_FCMConfig_ptr->McuFcmRefClockDiv_u32));
  /* Config FCM Interrupt/Reset */
  MODIFY_REG32(para_FCMx_ptr->RIER, FCM_RIER_ERRINTRS, para_FCMConfig_ptr->McuFcmExceptionType_u32);
  /* Enable or disable the FCM reset */
  if (FCM_EXP_TYPE_RST == para_FCMConfig_ptr->McuFcmExceptionType_u32)
  {
    SET_REG32_BIT(para_FCMx_ptr->RIER, 0x80U);
  }
  else
  {
    CLR_REG32_BIT(para_FCMx_ptr->RIER, 0x80U);
  }
  /* FCM overflow interrupt */
  if ((uint8_t)MCU_ENABLE == para_FCMConfig_ptr->McuFcmIntOvf_u8)
  {
    SET_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_OVF);
  }
  else
  {
    CLR_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_OVF);
  }
  /* FCM calculate end interrupt */
  if ((uint8_t)MCU_ENABLE == para_FCMConfig_ptr->McuFcmIntEnd_u8)
  {
    SET_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_END);
  }
  else
  {
    CLR_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_END);
  }
  /* FCM frequency abnormal interrupt */
  if ((uint8_t)MCU_ENABLE == para_FCMConfig_ptr->McuFcmIntErr_u8)
  {
    SET_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_ERR);
  }
  else
  {
    CLR_REG32_BIT(para_FCMx_ptr->RIER, FCM_INT_ERR);
  }

  /* Start FCM measurement */
  if (CM_FCM1 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START1, (uint32_t)(MCU_ENABLE << FCM_COM_STR_START1_POS));
  }
  #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
  else if (CM_FCM2 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START2, (uint32_t)(MCU_ENABLE << FCM_COM_STR_START2_POS));
  }
  else if (CM_FCM3 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START3, (uint32_t)(MCU_ENABLE << FCM_COM_STR_START3_POS));
  }
  else if (CM_FCM4 == para_FCMx_ptr)
  {
    MODIFY_REG(CM_FCM_COM->STR, FCM_COM_STR_START4, (uint32_t)(MCU_ENABLE << FCM_COM_STR_START4_POS));
  }
  else
  {
    /* Do nothing */
  }
  #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
}

/**
 *  @brief          This function will configure clock source for system clock
 *
 *  @details        Design ID:         Clock_SetSysClockSrc_Activity
 * 
 *  @param[in]      para_ClockSource_u8    Choose clock source for system clock
 *
 *  @return         void
 *
 *  @note           Only current system clock source or target system clock
 *                  source is PLLH need to close fcg0~fcg3 and open fcg0~fcg3
 *                  during switch system clock source.
 *                  Need to backup fcg0~fcg3 before close them.
 *
 *  @pre            None
 *
 */
static void Clock_SetSysClockSrc(
    const uint8_t  para_ClockSource_u8
)
{
    /* Backup FCGx setting */
    uint32_t  f_Fcg0_u32 = CM_PWC->FCG0;
    uint32_t  f_Fcg1_u32 = CM_PWC->FCG1;
    uint32_t  f_Fcg2_u32 = CM_PWC->FCG2;
    uint32_t  f_Fcg3_u32 = CM_PWC->FCG3;

    if (CLK_SYSCLK_SRC_PLL == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
    {
        /* Turn off the peripheral module clock */
        CM_PWC->FCG0 = CLK_FCG0_DEFAULT;
        CM_PWC->FCG1 = CLK_FCG1_DEFAULT;
        CM_PWC->FCG2 = CLK_FCG2_DEFAULT;
        CM_PWC->FCG3 = CLK_FCG3_DEFAULT;
        /* Wait stable after close FCGx. */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        /* Set system clock source */
        CM_CMU->CKSWR = para_ClockSource_u8;
        /* Wait stable after setting system clock source */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        /* Turn on the peripheral module clock */
        CM_PWC->FCG0 = f_Fcg0_u32;
        CM_PWC->FCG1 = f_Fcg1_u32;
        CM_PWC->FCG2 = f_Fcg2_u32;
        CM_PWC->FCG3 = f_Fcg3_u32;
        /* Wait stable after open fcg. */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
    }
    else
    {
        if (CLK_SYSCLK_SRC_PLL == para_ClockSource_u8)
        {
            /* Turn off the peripheral module clock */
            CM_PWC->FCG0 = CLK_FCG0_DEFAULT;
            CM_PWC->FCG1 = CLK_FCG1_DEFAULT;
            CM_PWC->FCG2 = CLK_FCG2_DEFAULT;
            CM_PWC->FCG3 = CLK_FCG3_DEFAULT;
            /* Wait stable after close FCGx. */
            Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
            /* Set system clock source */
            CM_CMU->CKSWR = para_ClockSource_u8;
            /* Wait stable after setting system clock source */
            Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
            /* Turn on the peripheral module clock */
            CM_PWC->FCG0 = f_Fcg0_u32;
            CM_PWC->FCG1 = f_Fcg1_u32;
            CM_PWC->FCG2 = f_Fcg2_u32;
            CM_PWC->FCG3 = f_Fcg3_u32;
            /* Wait stable after open fcg. */
            Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        }
        else
        {
            CM_CMU->CKSWR = para_ClockSource_u8;
            /* Wait stable after setting system clock source */
            Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        }
    }
}


/**
 *  @brief          This function will set frequency divisor of system clock
 *
 *  @details        Design ID:         Clock_SetSysClockDiv_Activity
 * 
 *  @param[in]      para_ClockSource_u32    Choose clock source
 *  @param[in]      para_ClockDiv_u32       choose the divisor
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_SetSysClockDiv.
 *
 *  @pre            None
 *
 */
static void Clock_SetSysClockDiv(
    const uint32_t  para_ClockSource_u32,
    const uint32_t  para_ClockDiv_u32
)
{
    /* Backup FCGx setting */
    uint32_t  f_Fcg0_u32 = CM_PWC->FCG0;
    uint32_t  f_Fcg1_u32 = CM_PWC->FCG1;
    uint32_t  f_Fcg2_u32 = CM_PWC->FCG2;
    uint32_t  f_Fcg3_u32 = CM_PWC->FCG3;

    if (CLK_SYSCLK_SRC_PLL == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
    {
        /* Turn off the peripheral module clock */
        CM_PWC->FCG0 = CLK_FCG0_DEFAULT;
        CM_PWC->FCG1 = CLK_FCG1_DEFAULT;
        CM_PWC->FCG2 = CLK_FCG2_DEFAULT;
        CM_PWC->FCG3 = CLK_FCG3_DEFAULT;
        /* Wait stable after close FCGx. */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        /* Set clock source Div */
        MODIFY_REG32(CM_CMU->SCFGR, para_ClockSource_u32, para_ClockDiv_u32);
        /* Wait stable after setting clock source Div */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
        /* Turn on the peripheral module clock */
        CM_PWC->FCG0 = f_Fcg0_u32;
        CM_PWC->FCG1 = f_Fcg1_u32;
        CM_PWC->FCG2 = f_Fcg2_u32;
        CM_PWC->FCG3 = f_Fcg3_u32;
        /* Wait stable after open fcg. */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
    }
    else
    {
        /* Set clock source Div */
        MODIFY_REG32(CM_CMU->SCFGR, para_ClockSource_u32, para_ClockDiv_u32);
        /* Wait stable after setting clock source Div */
        Clock_Ip_Delay(CLK_SYSCLK_SW_STB);
    }
}

/**
 *  @brief          This function will calibration frequency for RTCLRC
 *
 *  @details        Design ID:         Clock_Ip_RtcConfig_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of RTCLRC
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_InitClock
 *
 *  @pre            None
 *
 */
static void Clock_Ip_RtcConfig(
    const Clock_Ip_RtcLrcConfigType * para_ClockConfig_ptr
)
{
    CM_CMU->RTCLRCTRM = (uint8_t)para_ClockConfig_ptr->McuRtcLrcTrim_s8;
}

/**
 *  @brief          This function will configure for PLLH to work
 *
 *  @details        Design ID:         Clock_Ip_ConfigPllhClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to struct configure of PLL
 *
 *  @return         Std_ReturnType    Status of PLLH configuration
 *  @retval         E_OK              PLLH configuration was successful
 *  @retval         E_NOT_OK          PLLH configuration failed (PLLH as system clock source)
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigPllhClk(
    const Clock_Ip_PllhConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* Initialize pllh value */
    uint32_t  f_PllSrc_u32;
    uint32_t  f_PllHm_u32;
    uint32_t  f_PllHn_u32;
    uint32_t  f_PllHp_u32;
    uint32_t  f_PllHcfg_u32;

    /* Select input clock source */
    f_PllSrc_u32  = ((uint32_t)para_ClockConfig_ptr->McuSrcClock_u8 << CMU_PLLHCFGR_PLLSRC_POS);
    /* Select input clock frequency drvision factor */
    f_PllHm_u32   = ((uint32_t)para_ClockConfig_ptr->McuPllhMDiv_u8 << CMU_PLLHCFGR_PLLHM_POS);
    /* Select frequency multiplication factor */
    f_PllHn_u32   = ((uint32_t)para_ClockConfig_ptr->McuPllhNMul_u8 << CMU_PLLHCFGR_PLLHN_POS);
    /* PLLHP frequency drvision factor selection */
    f_PllHp_u32   = ((uint32_t)para_ClockConfig_ptr->McuPllhPDiv_u8 << CMU_PLLHCFGR_PLLHP_POS);

    f_PllHcfg_u32 = (f_PllSrc_u32 | f_PllHm_u32 | f_PllHn_u32 | f_PllHp_u32);

    /* Configuration register for CMU PLLH */
    CM_CMU->PLLHCFGR = (CM_CMU->PLLHCFGR & ~(CMU_PLLHCFGR_PLLSRC | CMU_PLLHCFGR_PLLHM | CMU_PLLHCFGR_PLLHN | CMU_PLLHCFGR_PLLHP)) | f_PllHcfg_u32;

    if ((uint8_t)MCU_DISABLE == (para_ClockConfig_ptr->McuPllState_u8))
    {
        /* Check the PLLH clock is used as the system clock or not */
        if (CLK_SYSCLK_SRC_PLL == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        else
        {
            CM_CMU->PLLHCR = CLK_PLL_OFF;
            /* Wait PLLH clock is stopped */
            f_ReturnValue_en = Clock_Ip_WaitStopped(CLK_STB_FLAG_PLLH);
        }
    }
    else
    {
        /* Check the XTAL clock as the input clock of PLLH/PLLA or HRC clock */
        if (CLK_PLL_SRC_XTAL == ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS))
        {
            /* Wait XTAL clock is stable */
            f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_XTAL);
        }
        else
        {
            /* wait HRC clock is stable */
            f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_HRC);
        }
        if ((Std_ReturnType)E_OK == f_ReturnValue_en)
        {
            /* Active PLLH */
            CM_CMU->PLLHCR = CLK_PLL_ON;
            /* Wait PLLH clock is stable */
            f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_PLLH);
        }
    }

    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function will configure for PLLA to work
 *
 *  @details        Design ID:         Clock_Ip_ConfigPllaClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to struct configure of PLL
 *
 *  @return         Std_ReturnType    Status of PLLA configuration
 *  @retval         E_OK              PLLA configuration was successful
 *  @retval         E_NOT_OK          PLLA configuration failed (clock source not stable)
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigPllaClk(
    const Clock_Ip_PllaConfigType * para_ClockConfig_ptr
)
{
    Std_ReturnType  f_ReturnValue_en = E_OK;

    uint32_t  f_PllAm_u32;
    uint32_t  f_PllAn_u32;
    uint32_t  f_PllAcfg_u32;

    /* Select input clock frequency drvision factor */
    f_PllAm_u32   = ((uint32_t)para_ClockConfig_ptr->McuPllaMDiv_u8 << CMU_PLLHCFGR_PLLHM_POS);
    /* Select frequency multiplication factor */
    f_PllAn_u32   = ((uint32_t)para_ClockConfig_ptr->McuPllaNMul_u16 << CMU_PLLHCFGR_PLLHN_POS);

    f_PllAcfg_u32 = (f_PllAm_u32 | f_PllAn_u32);
    /* Configuration register for CMU PLLA */
    CM_CMU->PLLACFGR = (CM_CMU->PLLACFGR & ~(CMU_PLLACFGR_PLLAM | CMU_PLLACFGR_PLLAN)) | f_PllAcfg_u32;

    if ((uint8_t)MCU_DISABLE == (para_ClockConfig_ptr->McuPllState_u8))
    {
        CM_CMU->PLLACR = CLK_PLL_OFF;
        /* Wait PLLA clock is stopped */
        (void)Clock_Ip_WaitStopped(CLK_STB_FLAG_PLLA);
    }
    else
    {
        /* Check the XTAL clock as the input clock of PLLH/PLLA or HRC clock */
        if (CLK_PLL_SRC_XTAL == ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS))
        {
            /* wait XTAL clock is stable */
            f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_XTAL);
        }
        else
        {
            /* Wait HRC clock is stable */
            f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_HRC);
        }

        if ((Std_ReturnType)E_OK == f_ReturnValue_en)
        {
            /* Active PLLA */
            CM_CMU->PLLACR = CLK_PLL_ON;
            /* Wait PLLA clock is stable */
            (void)Clock_Ip_WaitStable(CLK_STB_FLAG_PLLA);
        }
    }
    return f_ReturnValue_en;
}

/**
 *  @brief          This function configures XTAL clock.
 *
 *  @details        Design ID:         Clock_Ip_ConfigXtalClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of Xtal
 *
 *  @return         Std_ReturnType    Status of XTAL configuration
 *  @retval         E_NOT_OK          The driver state did not allowed the execution (the Xtal as system clock
 *                                    source or The Xtal as the clock source and PLL is working) or The Xtal state is unstable
 *  @retval         E_OK              The driver state allowed the execution and the Xtal state is stable
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigXtalClk(
    const Clock_Ip_XtalConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* XTAL stability time selection */
    CM_CMU->XTALSTBCR = para_ClockConfig_ptr->McuXtalStableTime_u8;
    /* XTAL mode and driving ability selection */
    CM_CMU->XTALCFGR  = (para_ClockConfig_ptr->McuXtalMode_u8 | para_ClockConfig_ptr->McuXtalDriv_u8);
    /* XTAL enable/disable. */
    if ((uint8_t)MCU_DISABLE == (para_ClockConfig_ptr->McuXtalState_u8))
    {
        if (CLK_SYSCLK_SRC_XTAL == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        /* XTAL as PLL clock source and PLL is working */
        else if (CLK_PLL_SRC_XTAL  == ((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS))
        {
            if (0U == CM_CMU->PLLHCR)
            {
                f_ReturnValue_en = E_NOT_OK;
            }
            else
            {
                /* XTAL disable */
                CM_CMU->XTALCR = CLK_XTAL_OFF;
                /* wait stopped */
                f_ReturnValue_en = Clock_Ip_WaitStopped(CLK_STB_FLAG_XTAL);
            }
        }
        else
        {
            /* XTAL disable */
            CM_CMU->XTALCR = CLK_XTAL_OFF;
            /* wait stopped */
            f_ReturnValue_en = Clock_Ip_WaitStopped(CLK_STB_FLAG_XTAL);
        }
    }
    else
    {
        /* XTAL enable */
        CM_CMU->XTALCR = CLK_XTAL_ON;
        /* wait stable */
        f_ReturnValue_en = Clock_Ip_WaitStable(CLK_STB_FLAG_XTAL);
    }
    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function configures XTAL32 clock
 *
 *  @details        Design ID:         Clock_Ip_ConfigXtal32Clk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to Xtal32 clock configuration structure
 *
 *  @return         Std_ReturnType    Return config status (success or fail)
 *  @retval         E_OK              Config Xtal32 clock was successful
 *  @retval         E_NOT_OK          Config Xtal32 clock was NOT successful
 *
 *  @note           This function will be called by Clock_Ip_InitClock()
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigXtal32Clk(
    const Clock_Ip_Xtal32ConfigType * para_ClockConfig_ptr
)
{
    /* Initialize return value */
    Std_ReturnType  f_ReturnValue_en = E_OK;

    /* XTAL32 disable */
    CM_CMU->XTAL32CR = CLK_XTAL32_OFF;
    /* Reset the VBAT area */
    WRITE_REG8(CM_PWC->VBATRSTR, 0xA5U);
    /* XTAL32 drive capability selection */
    CM_CMU->XTAL32CFGR = para_ClockConfig_ptr->McuXtal32Driv_u8;
    /* XTAL32 oscilllater filter selection */
    CM_CMU->XTAL32NFR  = para_ClockConfig_ptr->McuXtal32Filter_u8;

    if ((uint8_t)MCU_DISABLE == (para_ClockConfig_ptr->McuXtal32State_u8))
    {
        if (CLK_SYSCLK_SRC_XTAL32 == (CM_CMU->CKSWR & CMU_CKSWR_CKSW))
        {
            f_ReturnValue_en = E_NOT_OK;
        }
        else
        {
            /* XTAL32 disable */
            CM_CMU->XTAL32CR = CLK_XTAL32_OFF;
            /* wait for 5 XTAL32 cycles */
            Clock_Ip_Delay(CLK_XTAL32_TIMEOUT);
        }
    }
    else
    {
        /* XTAL32 enable */
        CM_CMU->XTAL32CR = CLK_XTAL32_ON;
        /* wait for 5 XTAL32 cycles */
        Clock_Ip_Delay(CLK_XTAL32_TIMEOUT);
    }
    /* Return value */
    return f_ReturnValue_en;
}

/**
 *  @brief          This function will update value of system clock
 *
 *  @details        Design ID:         Clock_SystemCoreClockUpdate_Activity
 * 
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_SetSysClockDiv.
 *
 *  @pre            None
 *
 */
static void Clock_SystemCoreClockUpdate(
    void
)
{
    uint32_t  f_Plln_u32;
    uint32_t  f_Pllp_u32;
    uint32_t  f_Pllm_u32;
    uint32_t  f_PllSrcFreq_u32 = CMU_XTAL_VALUE;
    uint32_t  f_HrcValue_u32 = CMU_HRC_20MHz_VALUE;

    /* Select proper HRC_VALUE according to ICG1.HRCFREQSEL bit */
    if (1U == (CMU_HRC_FREQ_MON & 1U))
    {
        f_HrcValue_u32 = CMU_HRC_16MHz_VALUE;
    }

    switch (CM_CMU->CKSWR & CMU_CKSWR_CKSW)
    {
        case CLK_SYSCLK_SRC_HRC: /* use internal high speed RC */
            g_SystemCoreClock_u32 = f_HrcValue_u32;
            break;
        case CLK_SYSCLK_SRC_MRC: /* use internal middle speed RC */
            g_SystemCoreClock_u32 = CMU_MRC_VALUE;
            break;
        case CLK_SYSCLK_SRC_LRC: /* use internal low speed RC */
            g_SystemCoreClock_u32 = CMU_LRC_VALUE;
            break;
        case CLK_SYSCLK_SRC_XTAL: /* use external high speed OSC */
            g_SystemCoreClock_u32 = CMU_XTAL_VALUE;
            break;
        case CLK_SYSCLK_SRC_XTAL32: /* use external low speed OSC */
            g_SystemCoreClock_u32 = CMU_XTAL32_VALUE;
            break;
        default:
            /* PLLCLK = ((pllsrc / pllm) * plln) / pllp */
            f_Plln_u32 = (CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLHN) >> CMU_PLLHCFGR_PLLHN_POS;
            f_Pllp_u32 = (CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLHP) >> CMU_PLLHCFGR_PLLHP_POS;
            f_Pllm_u32 = (CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLHM) >> CMU_PLLHCFGR_PLLHM_POS;
            if (0U != bCM_CMU->PLLHCFGR_b.PLLSRC)
            {  /* use internal high RC as PLL source */
                f_PllSrcFreq_u32 = f_HrcValue_u32;
            }
            g_SystemCoreClock_u32 = f_PllSrcFreq_u32 / (f_Pllm_u32 + 1U) * (f_Plln_u32 + 1U) / (f_Pllp_u32 + 1U);
            break;
    }
}

/**
 *  @brief          This function will select PLL clock for CAN1
 *
 *  @details        Design ID:         Clock_Ip_SelCan1PllClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of CAN1
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_ConfigCan1Periph.
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SelCan1PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
    switch(para_ClockConfig_ptr->McuClockSelect_en)
    {
        case MCU_CMU_SELPLLHQ_CLK:
            /* Select PLLHQ clock for can1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_PLLQ << CMU_CANCKCFGR_CAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLHR_CLK:
            /* Select PLLHR clock for can1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_PLLR << CMU_CANCKCFGR_CAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLAP_CLK:
            /* Select PLLAP clock for can1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_PLLAP << CMU_CANCKCFGR_CAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLAQ_CLK:
            /* Select PLLAQ clock for can1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_PLLAQ << CMU_CANCKCFGR_CAN1CKS_POS));
            break;
        default:
            /* Select PLLAR clock for can1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN1CKS, (uint32_t)(CLK_CANCLK_PLLAR << CMU_CANCKCFGR_CAN1CKS_POS));
            break;
    }
}

/**
 *  @brief          This function will select PLL clock for CAN2
 *
 *  @details        Design ID:         Clock_Ip_SelCan2PllClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of CAN2
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_ConfigCan2Periph
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SelCan2PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELPLLHQ_CLK:
      /* Select PLLHQ clock for can2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_PLLQ << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLHR_CLK:
      /* Select PLLHR clock for can2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_PLLR << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLAP_CLK:
      /* Select PLLAP clock for can2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_PLLAP << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLAQ_CLK:
      /* Select PLLAQ clock for can2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_PLLAQ << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
    default:
      /* Select PLLAR clock for can2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_CAN2CKS, (uint32_t)(CLK_CANCLK_PLLAR << CMU_CANCKCFGR_CAN2CKS_POS));
      break;
  }
}

#ifdef CM_MCAN1
/**
 *  @brief          This function will select PLL clock for MCAN1
 *
 *  @details        Design ID:         Clock_Ip_SelMcan1PllClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of MCAN1
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_ConfigMcan1Periph.
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SelMcan1PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
    switch(para_ClockConfig_ptr->McuClockSelect_en)
    {
        case MCU_CMU_SELPLLHQ_CLK:
            /* Select PLLHQ clock for mcan1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLQ << CMU_CANCKCFGR_MCAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLHR_CLK:
            /* Select PLLHR clock for mcan1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLR << CMU_CANCKCFGR_MCAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLAP_CLK:
            /* Select PLLAP clock for mcan1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAP << CMU_CANCKCFGR_MCAN1CKS_POS));
            break;
        case MCU_CMU_SELPLLAQ_CLK:
            /* Select PLLAQ clock for mcan1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAQ << CMU_CANCKCFGR_MCAN1CKS_POS));
            break;
        default:
            /* Select PLLAR clock for mcan1 */
            MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN1CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAR << CMU_CANCKCFGR_MCAN1CKS_POS));
            break;
    }
}
#endif /* CM_MCAN1 */


#ifdef CM_MCAN2
/**
 *  @brief          This function will select PLL clock for MCAN2
 *
 *  @details        Design ID:         Clock_Ip_SelMcan2PllClk_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of MCAN2
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_ConfigMcan2Periph
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SelMcan2PllClk(
    const Clock_Ip_SelClockSourceType * para_ClockConfig_ptr
)
{
  switch(para_ClockConfig_ptr->McuClockSelect_en)
  {
    case MCU_CMU_SELPLLHQ_CLK:
      /* Select PLLHQ clock for mcan2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLQ << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLHR_CLK:
      /* Select PLLHR clock for mcan2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLR << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLAP_CLK:
      /* Select PLLAP clock for mcan2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAP << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
    case MCU_CMU_SELPLLAQ_CLK:
      /* Select PLLAQ clock for mcan2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAQ << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
    default:
      /* Select PLLAR clock for mcan2 */
      MODIFY_REG(CM_CMU->CANCKCFGR, CMU_CANCKCFGR_MCAN2CKS, (uint32_t)((uint32_t)CLK_CANCLK_PLLAR << CMU_CANCKCFGR_MCAN2CKS_POS));
      break;
  }
}
#endif /* CM_MCAN2 */

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          This function will configure for Xtal Div to work
 *
 *  @details        Design ID:         Clock_Ip_ConfigXtalDiv_Activity
 * 
 *  @param[in]      para_ClockConfig_ptr    Pointer to configuration structure of Xtal Div
 *
 *  @return         Std_ReturnType    Status of XTAL divider configuration
 *  @retval         E_OK              XTAL divider configuration was successful
 *  @retval         E_NOT_OK          XTAL divider configuration failed (NULL pointer)
 *
 *  @note           This function will be called by Clock_Ip_InitClock.
 *
 *  @pre            None
 *
 */
static Std_ReturnType Clock_Ip_ConfigXtalDiv(
    const Clock_Ip_XtalDivConfigType * para_XtalDivConfig_ptr
)
{
    Std_ReturnType RetValue = E_OK;

    /* Check parameter */
    if (NULL_PTR == para_XtalDivConfig_ptr) {
        RetValue = E_NOT_OK;
    } else {
        /* Configure XTAL divider numerator and denominator */
        WRITE_REG32(CM_CMU->XTALDIVR, ((uint32_t)para_XtalDivConfig_ptr->McuXtalDivNumer_u32 << CMU_XTALDIVR_NUMER_POS) | 
                                      para_XtalDivConfig_ptr->McuXtalDivDenom_u32);

        /* Configure XTAL divider state */
        WRITE_REG32(CM_CMU->XTALDIVCR, para_XtalDivConfig_ptr->McuXtalDivState_u32);
    }

    return RetValue;
}
#endif  /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

/**
 *  @brief          This function will configure and update clock source for
 *                  system clock
 *
 *  @details        Design ID:         Clock_Ip_SetSysClockSrc_Activity
 * 
 *  @param[in]      para_ClockSource_u8    Choose clock source for system clock
 *
 *  @return         void
 *
 *  @note           This function will be called by Clock_Ip_ConfigSystemClock.
 *
 *  @pre            None
 *
 */
static void Clock_Ip_SetSysClockSrc(
    const uint8_t  para_ClockSource_u8
)
{
    /* Set system clock source */
    Clock_SetSysClockSrc(para_ClockSource_u8);
    /* Update system clock */
    Clock_SystemCoreClockUpdate();
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

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
)
{
  switch(para_FuncClockConfig_ptr->McuPeripheralGroup_u8)
  {
    case MCU_FCG_FCG0_PERIPH_GROUP:
      /* Enable or disable the FCG0 peripheral clock. */
      if ((uint8_t)MCU_ENABLE == para_FuncClockConfig_ptr->McuPeripheralState_u8)
      {
        CLR_REG32_BIT(CM_PWC->FCG0, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      else
      {
        SET_REG32_BIT(CM_PWC->FCG0, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      break;
    case MCU_FCG_FCG1_PERIPH_GROUP:
      /* Enable or disable the FCG1 peripheral clock. */
      if ((uint8_t)MCU_ENABLE == para_FuncClockConfig_ptr->McuPeripheralState_u8)
      {
        CLR_REG32_BIT(CM_PWC->FCG1, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      else
      {
        SET_REG32_BIT(CM_PWC->FCG1, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      break;
    case MCU_FCG_FCG2_PERIPH_GROUP:
      /* Enable or disable the FCG2 peripheral clock. */
      if ((uint8_t)MCU_ENABLE == para_FuncClockConfig_ptr->McuPeripheralState_u8)
      {
        CLR_REG32_BIT(CM_PWC->FCG2, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      else
      {
        SET_REG32_BIT(CM_PWC->FCG2, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      break;
    default:
      /* Enable or disable the FCG3 peripheral clock. */
      if ((uint8_t)MCU_ENABLE == para_FuncClockConfig_ptr->McuPeripheralState_u8)
      {
        CLR_REG32_BIT(CM_PWC->FCG3, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      else
      {
        SET_REG32_BIT(CM_PWC->FCG3, para_FuncClockConfig_ptr->McuPeripheralName_u32);
      }
      break;
  }
}

/**
 *  @brief          This function will set frequency divisor of system clock
 *                  and update system clock
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_SetSysClockDiv_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ClockSource_u32    Choose clock source
 *  @param[in]      para_ClockDiv_u32       choose the divisor
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
)
{
    /* Set clock source Div */
    Clock_SetSysClockDiv(para_ClockSource_u32, para_ClockDiv_u32);
    /* Update system clock */
    Clock_SystemCoreClockUpdate();
}

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
)
{
    const Clock_Ip_MrcConfigType  f_ClockMrcConfig_st =
    {
        (uint8_t)MCU_ENABLE,
        (sint8)0
    };
    Clock_SystemCoreClockUpdate();
    /* Configure the MRC clock */
    (void)Clock_Ip_ConfigMrcClk((const Clock_Ip_MrcConfigType *) &f_ClockMrcConfig_st);
    if (CMU_MRC_VALUE >= g_SystemCoreClock_u32)
    {
        /* RAMPDC1_3_4_backup set to 1 Read/Write wait cycle */
        Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAM_ALL, MCU_SRAM_WAIT_CYCLE1, MCU_SRAM_WAIT_CYCLE1);
        /* Highspeed SRAM set to 0 Read/Write wait cycle */
        Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAMH, MCU_SRAM_WAIT_CYCLE0, MCU_SRAM_WAIT_CYCLE0);
        /* FLASH read wait cycle setting */
        Clock_Ip_EFMSetWaitCycle(MCU_EFM_WAIT_CYCLE0);
        /* GPIO wait cycle setting */
        Clock_Ip_GPIOSetWaitCycle(MCU_SRAM_WAIT_CYCLE1);
        /* Set system clock source */
        Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
    }
    else
    {
        /* Set system clock source */
        Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
        /* RAMPDC1_3_4_backup set to 1 Read/Write wait cycle */
        Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAM_ALL, MCU_SRAM_WAIT_CYCLE1, MCU_SRAM_WAIT_CYCLE1);
        /* Highspeed SRAM set to 0 Read/Write wait cycle */
        Clock_Ip_SramSetWaitCycle(MCU_SRAM_SRAMH, MCU_SRAM_WAIT_CYCLE0, MCU_SRAM_WAIT_CYCLE0);
        /* FLASH read wait cycle setting */
        Clock_Ip_EFMSetWaitCycle(MCU_EFM_WAIT_CYCLE0);
        /* GPIO wait cycle setting */
        Clock_Ip_GPIOSetWaitCycle(MCU_SRAM_WAIT_CYCLE1);
    }
}

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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_en = E_OK;
  uint32_t  f_SysClockToBeUpdate_u32 = 0U;

  switch(para_ClockConfig_ptr->McuClockSource_en)
  {
    case MCU_CMU_XTAL_CLK:
      /* Disable XTAL fault detection */
      WRITE_REG8(CM_CMU->XTALSTDCR, (CLK_XTALSTD_OFF | CLK_XTALSTD_MD_RST | CLK_XTALSTD_INT_OFF | CLK_XTALSTD_RST_OFF));
      /* Configure the XTAL clock */
      f_ReturnValue_en = Clock_Ip_ConfigXtalClk((const Clock_Ip_XtalConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      /* Xtal stop detect initialize */
      Clock_Ip_XtalStopDetctInit((const Clock_Ip_XtalConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_HRC_CLK:
      /* Configure the HRC clock */
      f_ReturnValue_en = Clock_Ip_ConfigHrcClk((const Clock_Ip_HrcConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_SYS_CLK:
      f_SysClockToBeUpdate_u32 = \
      (((const Clock_Ip_SelSysClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr)->McuWaitCycleConfig_ptr)->McuSysClockFrequency_u32;
      if (g_SystemCoreClock_u32 <= f_SysClockToBeUpdate_u32)
      {
        /* Configure the wait cycle */
        Clock_Ip_ConfigWaitCycle((const Clock_Ip_SelSysClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr);
        /* Configure the system clock */
        Clock_Ip_ConfigSystemClock((const Clock_Ip_SelSysClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      }
      else
      {
        /* Configure the system clock */
        Clock_Ip_ConfigSystemClock((const Clock_Ip_SelSysClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr);
        /* Configure the wait cycle */
        Clock_Ip_ConfigWaitCycle((const Clock_Ip_SelSysClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      }
      break;
    case MCU_CMU_XTAL32_CLK:
      /* Configure the XTAL32 clock */
      f_ReturnValue_en = Clock_Ip_ConfigXtal32Clk((const Clock_Ip_Xtal32ConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_PLLH_CLK:
      /* Configure the PLLH clock */
      f_ReturnValue_en = Clock_Ip_ConfigPllhClk((const Clock_Ip_PllhConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      l_McuPllCfg_u8 |= CMU_PLLH_CONFIG;
      /* Variable (pointer) used for storing the MCU driver PLLH clock */
      l_McuPllhClockConfig_ptr = (const Clock_Ip_PllhConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr;
      break;
    case MCU_CMU_PLLA_CLK:
      /* Configure the PLLA clock */
      f_ReturnValue_en = Clock_Ip_ConfigPllaClk((const Clock_Ip_PllaConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      l_McuPllCfg_u8 |= CMU_PLLA_CONFIG;
      /* Variable (pointer) used for storing the MCU driver PLLA clock */
      l_McuPllaClockConfig_ptr = (const Clock_Ip_PllaConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr;
      break;
    case MCU_CMU_MRC_CLK:
      /* Configure the MRC clock */
      f_ReturnValue_en = Clock_Ip_ConfigMrcClk((const Clock_Ip_MrcConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_LRC_CLK:
      /* Configure the LRC clock */
      f_ReturnValue_en = Clock_Ip_ConfigLrcClk((const Clock_Ip_LrcConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_RTCLRC_CLK:
      /* Configure the RTCLRC clock */
      Clock_Ip_RtcConfig((const Clock_Ip_RtcLrcConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_FCM1_CLK:
      /* Configure clock frequency measurement */
      Clock_Ip_FCMMeasurement(CM_FCM1, (const Clock_Ip_FcmConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
    case MCU_CMU_FCM2_CLK:
      #ifdef CM_FCM2
      /* Configure clock frequency measurement */
      Clock_Ip_FCMMeasurement(CM_FCM2, (const Clock_Ip_FcmConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      #endif
      break;
    case MCU_CMU_FCM3_CLK:
      #ifdef CM_FCM3
      /* Configure clock frequency measurement */
      Clock_Ip_FCMMeasurement(CM_FCM3, (const Clock_Ip_FcmConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      #endif
      break;
    case MCU_CMU_FCM4_CLK:
      #ifdef CM_FCM2
      /* Configure clock frequency measurement */
      Clock_Ip_FCMMeasurement(CM_FCM4, (const Clock_Ip_FcmConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      #endif
      break;
    case MCU_CMU_XTALDIV_CLK:
      #if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
      /* Configure the XTAL Div clock */
      f_ReturnValue_en = Clock_Ip_ConfigXtalDiv((const Clock_Ip_XtalDivConfigType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      #endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */
      break;
    default:
      /* Configure the Peripheral clock */
      Clock_Ip_ConfigPeriphClock((const Clock_Ip_PeriphClockSourceType *)para_ClockConfig_ptr->McuClockConfig_ptr);
      break;
  }
  return f_ReturnValue_en;
}

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
)
{
  /* PLL status variable. */
  Clock_Ip_PllStatusType f_PllStatus_en;

  switch(l_McuPllCfg_u8) {
    case CMU_PLLA_PLLH_CONFIG:
      if (0U == (CM_CMU->OSCSTBSR & CLK_STB_FLAG_PLLH))
      {
        f_PllStatus_en = CLOCK_IP_PLL_UNLOCKED;
      }
      else if (0U == (CM_CMU->OSCSTBSR & CLK_STB_FLAG_PLLA))
      {
        f_PllStatus_en = CLOCK_IP_PLL_UNLOCKED;
      }
      else
      {
        f_PllStatus_en = CLOCK_IP_PLL_LOCKED;
      }
      break;
    default:
      f_PllStatus_en = CLOCK_IP_PLL_STATUS_UNDEFINED;
      break;
  }
  /* Return value */
  return f_PllStatus_en;
}

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
)
{
  /* Active PLL clock source */
  switch(l_McuPllCfg_u8)
  {
    case CMU_PLLA_PLLH_CONFIG:
        Clock_Ip_DistributePllH();
        Clock_Ip_DistributePllA();
      break;
    default:
      /* Empty without process here! */
      break;
  }
}

/**
 *  @brief          This function will create delay period
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_Delay_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_Delay_u32    Time for delay
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
)
{
  uint32_t  f_Timeout_u32 = 0U;
  while (f_Timeout_u32 < para_Delay_u32)
  {
    f_Timeout_u32++;
  }
}

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
)
{
  Std_ReturnType  f_ReturnWaitStable_u8   = E_OK;
  uint32_t          f_SystemClockBackup_u32 = 0U;
  uint8_t           f_HRCStable_u8          = 0U;
  uint32_t StartTime_u32                  = 0U;
  uint32_t TimeoutTicks_u32               = 0U;
  uint32_t ElapsedTicks_u32               = 0U;
  boolean TimeoutStatus_bool            = (boolean)FALSE;

  /* Back up the system clock */
  f_SystemClockBackup_u32 = g_SystemCoreClock_u32;

  /* Unlock CMU registers */
  SET_REG16_BIT(CM_PWC->FPRC, RMU_UNLOCK_CODE0);
  if (0x01U == READ_REG8(CM_CMU->XTALSTDSR))
  {
    /* Clear the XTAL error flag */
    WRITE_REG8(CM_CMU->XTALSTDSR, 0x00U);
  }

  /* Switch The system clock to MRC or HRC */
  f_HRCStable_u8 = (READ_REG8_BIT(CM_CMU->OSCSTBSR, CLK_STB_FLAG_HRC) >> CMU_OSCSTBSR_HRCSTBF_POS);
  if((uint8_t)SET == f_HRCStable_u8)
  {
    Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_HRC);
  }
  else
  {
    Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_MRC);
  }

  /* Set XTALSTDIE to 0*/
  CM_CMU->XTALSTDCR = (uint8_t)(CM_CMU->XTALSTDCR & (~CMU_XTALSTDCR_XTALSTDIE));

  /* Clear XTALSTDF to 0 */
  CM_CMU->XTALSTDSR = (uint8_t)(CM_CMU->XTALSTDSR & (~CMU_XTALSTDSR_XTALSTDF));

  /* Initialize timeout */
  ElapsedTicks_u32 = 0U;
  Clock_Ip_StartTimeout(&StartTime_u32,
                       &TimeoutTicks_u32,
                       (uint32_t)CLOCK_IP_TIMEOUT_VALUE,
                       CLOCK_IP_TIMEOUT_TYPE);

  /* Loop for checking delay timeout */
  do
  {
    TimeoutStatus_bool = Clock_Ip_TimeoutExpired(&StartTime_u32,
                                               &ElapsedTicks_u32,
                                               TimeoutTicks_u32,
                                               CLOCK_IP_TIMEOUT_TYPE);
  } while ((0x00U != (CM_CMU->XTALSTDSR & CMU_XTALSTDSR_XTALSTDF)) && ((boolean)FALSE == TimeoutStatus_bool));

  /* Disable XTAL Oscillation Fault detection function */
  CM_CMU->XTALSTDCR = (uint8_t)(CM_CMU->XTALSTDCR & (~CMU_XTALSTDCR_XTALSTDE));
  /* Enable XTAL again, wait XTAL stable */
  CM_CMU->XTALCR = CLK_XTAL_ON;
  /* wait stable */
  f_ReturnWaitStable_u8 = Clock_Ip_WaitStable(CLK_STB_FLAG_XTAL);
  /* Switch the desired clock to XTAL */
  if ((CMU_XTAL_VALUE == f_SystemClockBackup_u32))
  {
    if (E_OK == f_ReturnWaitStable_u8)
    {
      Clock_Ip_SetSysClockSrc(CLK_SYSCLK_SRC_XTAL);
    }
  }
#ifdef CLOCK_IP_CMU_XTAL_NOTIFICATION
    CLOCK_IP_CMU_XTAL_NOTIFICATION(CLOCK_IP_ISR_CLOCK_FAILURE);
#endif
  /* Lock RMU registers */
  CM_PWC->FPRC = (RMU_WRITE_ENABLE | (uint16_t)((uint16_t)(~(RMU_UNLOCK_CODE0)) & (CM_PWC->FPRC)));
}

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
)
{
    /* Stop FCM measurement */
    WRITE_REG32(CM_FCM_COM->STR, MCU_DISABLE);

    /* Clear FCM overflow */
    #ifdef CM_FCM1
      SET_REG32_BIT(CM_FCM1->CLR, FCM_CLR_OVFCLR);
    #endif /* CM_FCM1 */
    #ifdef CM_FCM2
      SET_REG32_BIT(CM_FCM2->CLR, FCM_CLR_OVFCLR);
    #endif /* CM_FCM2 */
    #ifdef CM_FCM3
      SET_REG32_BIT(CM_FCM3->CLR, FCM_CLR_OVFCLR);
    #endif /* CM_FCM3 */
    #ifdef CM_FCM4
      SET_REG32_BIT(CM_FCM4->CLR, FCM_CLR_OVFCLR);
    #endif /* CM_FCM4 */
#ifdef CLOCK_IP_CMU_FCM_NOTIFICATION
    CLOCK_IP_CMU_FCM_NOTIFICATION(CLOCK_IP_ISR_CLOCK_FREQUENCY_MEASUREMENT);
#endif
}

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
)
{
    /* Stop FCM measurement */
    WRITE_REG32(CM_FCM_COM->STR, MCU_DISABLE);

    /* Clear FCM overflow */
    #ifdef CM_FCM1
      SET_REG32_BIT(CM_FCM1->CLR, FCM_CLR_MENDFCLR);
    #endif /* CM_FCM1 */
    #ifdef CM_FCM2
      SET_REG32_BIT(CM_FCM2->CLR, FCM_CLR_MENDFCLR);
    #endif /* CM_FCM2 */
    #ifdef CM_FCM3
      SET_REG32_BIT(CM_FCM3->CLR, FCM_CLR_MENDFCLR);
    #endif /* CM_FCM3 */
    #ifdef CM_FCM4
      SET_REG32_BIT(CM_FCM4->CLR, FCM_CLR_MENDFCLR);
    #endif /* CM_FCM4 */

#ifdef CLOCK_IP_CMU_FCM_NOTIFICATION
    CLOCK_IP_CMU_FCM_NOTIFICATION(CLOCK_IP_ISR_CLOCK_FREQUENCY_MEASUREMENT);
#endif
}

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
)
{
    /* Stop FCM measurement */
    WRITE_REG32(CM_FCM_COM->STR, MCU_DISABLE);
    /* Clear FCM overflow */
    #ifdef CM_FCM1
      SET_REG32_BIT(CM_FCM1->CLR, FCM_CLR_ERRFCLR);
    #endif /* CM_FCM1 */
    #ifdef CM_FCM2
      SET_REG32_BIT(CM_FCM2->CLR, FCM_CLR_ERRFCLR);
    #endif /* CM_FCM2 */
    #ifdef CM_FCM3
      SET_REG32_BIT(CM_FCM3->CLR, FCM_CLR_ERRFCLR);
    #endif /* CM_FCM3 */
    #ifdef CM_FCM4
      SET_REG32_BIT(CM_FCM4->CLR, FCM_CLR_ERRFCLR);
    #endif /* CM_FCM4 */
    
#ifdef CLOCK_IP_CMU_FCM_NOTIFICATION
    CLOCK_IP_CMU_FCM_NOTIFICATION(CLOCK_IP_ISR_CLOCK_FREQUENCY_MEASUREMENT);
#endif
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
