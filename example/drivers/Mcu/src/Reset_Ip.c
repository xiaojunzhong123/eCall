/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Reset_Ip.c
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
 *   @file       Reset_Ip.c
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
#include "Reset_Ip.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define RESET_IP_RESET_FLAG_NUMBER (17U)

/**
 * @brief  Define Reset Mask
 */
#define RESET_IP_PO_RESET_MASK (0x00000001U)
#define RESET_IP_PIN_RESET_MASK (0x00000002U)
#define RESET_IP_BO_RESET_MASK (0x00000004U)
#define RESET_IP_PVD1_RESET_MASK (0x00000008U)
#define RESET_IP_PVD2_RESET_MASK (0x00000010U)
#define RESET_IP_WD_RESET_MASK (0x00000020U)
#define RESET_IP_SWD_RESET_MASK (0x00000040U)
#define RESET_IP_PD_RESET_MASK (0x00000080U)
#define RESET_IP_SW_RESET_MASK (0x00000100U)
#define RESET_IP_MPUE_RESET_MASK (0x00000200U)
#define RESET_IP_RAPE_RESET_MASK (0x00000400U)
#define RESET_IP_RAEC_RESET_MASK (0x00000800U)
#define RESET_IP_CKFE_RESET_MASK (0x00001000U)
#define RESET_IP_XTALE_RESET_MASK (0x00002000U)
#define RESET_IP_LKUP_RESET_MASK (0x00004000U)
#define RESET_IP_FLEC_RESET_MASK (0x00008000U)
#define RESET_IP_ERMU_RESET_MASK (0x00020000U)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/
#define MCU_START_SEC_CONST_UNSPECIFIED
#include "Mcu_MemMap.h"

static const uint32_t s_ResetFlagMask_aa[RESET_IP_RESET_FLAG_NUMBER] = 
{ 
  RESET_IP_PO_RESET_MASK,
  RESET_IP_PIN_RESET_MASK,
  RESET_IP_BO_RESET_MASK,
  RESET_IP_PVD1_RESET_MASK,
  RESET_IP_PVD2_RESET_MASK,
  RESET_IP_WD_RESET_MASK,
  RESET_IP_SWD_RESET_MASK,
  RESET_IP_PD_RESET_MASK,
  RESET_IP_SW_RESET_MASK,
  RESET_IP_MPUE_RESET_MASK,
  RESET_IP_RAPE_RESET_MASK,
  RESET_IP_RAEC_RESET_MASK,
  RESET_IP_CKFE_RESET_MASK,
  RESET_IP_XTALE_RESET_MASK,
  RESET_IP_LKUP_RESET_MASK,
  RESET_IP_FLEC_RESET_MASK,
  RESET_IP_ERMU_RESET_MASK
};

static const Reset_Ip_ResetReasonType s_ResetFlag_aa[] =
{
  RESET_IP_PO_RESET,
  RESET_IP_PIN_RESET,
  RESET_IP_BO_RESET,
  RESET_IP_PVD1_RESET,
  RESET_IP_PVD2_RESET,
  RESET_IP_WD_RESET,
  RESET_IP_SWD_RESET,
  RESET_IP_PD_RESET,
  RESET_IP_SW_RESET,
  RESET_IP_MPUE_RESET,
  RESET_IP_RAPE_RESET,
  RESET_IP_RAEC_RESET,
  RESET_IP_CKFE_RESET,
  RESET_IP_XTALE_RESET,
  RESET_IP_LKUP_RESET,
  RESET_IP_FLEC_RESET,
  RESET_IP_ERMU_RESET,
  RESET_IP_RESET_MULTIPLE,
  RESET_IP_RESET_UNDEFINED
};
#define MCU_STOP_SEC_CONST_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

static void Reset_Ip_StartTimeout(
    uint32_t * para_StartTimeOut_ptr,
    uint32_t * para_TimeoutTicksOut_ptr,
    uint32_t para_TimeoutUs_u32,
    Osr_CounterType para_OsifCounter_en
);

static boolean Reset_Ip_TimeoutExpired(
    uint32_t * para_StartTime_ptr,
    uint32_t * para_ElapsedTicks_ptr,
    uint32_t para_TimeoutTicks_u32,
    Osr_CounterType para_OsifCounter_en
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_32
#include "Mcu_MemMap.h"

static uint32_t s_StoredResetReason_u32;

#define MCU_STOP_SEC_VAR_CLEARED_32
#include "Mcu_MemMap.h"

#define MCU_START_SEC_VAR_CLEARED_BOOLEAN
#include "Mcu_MemMap.h"

static boolean s_IsResetReasonRead_bool;

#define MCU_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Configure the timeout condition and start timeout.
 *
 *  @details        Design ID:         Reset_Ip_StartTimeout_Activity
 * 
 *  @param[in]      para_StartTimeOut_ptr       Pointer to store the start time value
 *  @param[in]      para_TimeoutTicksOut_ptr    Pointer to store the calculated timeout ticks
 *  @param[in]      para_TimeoutUs_u32          Timeout value in microseconds
 *  @param[in]      para_OsifCounter_en         Type of timer counter to use
 *  @arg            OSR_COUNTER_DUMMY           Define dummy counter
 *  @arg            OSR_COUNTER_AUTOSAROS       Define dummy counter
 *  @arg            OSR_COUNTER_SYSTICK         Define system counter
 *  @arg            OSR_COUNTER_CUSTOM          Define custom counter
 *
 *  @return         void
 *
 *  @note           This function initializes timeout tracking by getting current counter value
 *                  and converting timeout from microseconds to ticks.
 *
 *  @pre            None
 *
 */
static void Reset_Ip_StartTimeout(
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
 *  @details        Design ID:         Reset_Ip_TimeoutExpired_Activity
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
static boolean Reset_Ip_TimeoutExpired(
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

/***********************************************************************************************************************
 *  GLOBAL FUNCTION DEFINITIONS
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
)
{
  /* Unlock RMU registers */
  SET_REG16_BIT(CM_PWC->FPRC, PWC_UNLOCK_CODE1);
  /* Lockup reset enable */
  bCM_RMU->PRSTCR0_b.LKUPREN = (uint32_t)1;
  /* Lock RMU registers */
  CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~(PWC_UNLOCK_CODE1)) & (CM_PWC->FPRC)));
}

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
)
{
    uint8_t Count_u8;
    uint32_t ResetFlag_u32                     = 0U;
    uint32_t RegValue_u32                      = 0U;
    Reset_Ip_ResetReasonType ResetFlagStatus_en = RESET_IP_RESET_UNDEFINED;

    /* Get the raw reset flags from hardware register */
    RegValue_u32 = Reset_Ip_GetResetRawValue();
    
    /* Iterate through all possible reset flag types */
    for (Count_u8 = 0U; Count_u8 < RESET_IP_RESET_FLAG_NUMBER; Count_u8++)
    {
        /* Check if the current reset flag is set in the register */
        if (0U != (RegValue_u32 & (uint32_t)s_ResetFlagMask_aa[Count_u8]))
        {
            /* Store the first detected reset reason */
            ResetFlagStatus_en = s_ResetFlag_aa[Count_u8];
            ResetFlag_u32++;
            
            /* If more than one reset flag is set, indicate multiple reset reasons */
            if (2U <= ResetFlag_u32)
            {
                ResetFlagStatus_en = RESET_IP_RESET_MULTIPLE;
                break;
            }
        }
    }
    /* Return the detected reset reason (single, multiple, or undefined) */
    return ResetFlagStatus_en;
}

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
)
{
    /* Check if reset reason has been read before */
    if(FALSE == s_IsResetReasonRead_bool)
    {
        /* Store reset flags from hardware register to avoid multiple reads */
        s_StoredResetReason_u32 = (uint32_t)CM_RMU->RSTF0;
        /* Mark reset reason as read to use cached value next time */
        s_IsResetReasonRead_bool = TRUE;
    }
    /* Return stored reset flags value */
    return s_StoredResetReason_u32;
}

/**
 *  @brief          Clear reset reason.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Reset_Ip_ClearResetReason_Activity
 *                  Sync/Async:        Synchronous
 * 
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
)
{
  Std_ReturnType f_ReturnValue_en = E_OK;
  uint32_t StartTime_u32 = 0U;
  uint32_t TimeoutTicks_u32 = 0U;
  uint32_t ElapsedTicks_u32 = 0U;
  boolean TimeoutStatus_bool = (boolean)FALSE;
  
  /* Unlock RMU registers */
  CM_PWC->FPRC |= PWC_UNLOCK_CODE1;
  /* Clear all reset Flag*/
  CM_RMU->RSTF0 |= (uint32_t)RMU_RSTF0_CLRF;
  /* Lock RMU registers */
  CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~(PWC_UNLOCK_CODE1)) & (CM_PWC->FPRC)));

  /* Initialize timeout */
  ElapsedTicks_u32 = 0U;
  Reset_Ip_StartTimeout(&StartTime_u32,
                       &TimeoutTicks_u32,
                       (uint32_t)RESET_IP_TIMEOUT_VALUE,
                       RESET_IP_TIMEOUT_TYPE);

  /* Wait for reset to complete */
  do
  {
      TimeoutStatus_bool = Reset_Ip_TimeoutExpired(&StartTime_u32,
                                                 &ElapsedTicks_u32,
                                                 TimeoutTicks_u32,
                                                 RESET_IP_TIMEOUT_TYPE);
  } while ((0U != READ_REG32_BIT(CM_RMU->RSTF0, 0x4001FFFF)) && ((boolean)FALSE == TimeoutStatus_bool));

  if ((boolean)TRUE == TimeoutStatus_bool)
  {
      f_ReturnValue_en = E_NOT_OK;
  }
  else
  {
      f_ReturnValue_en = E_OK;
  }

  return f_ReturnValue_en;
}

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
)
{
  uint32_t  f_TmpValue_u8;
  Reset_Ip_CM4ResetType * f_CM4ResetCtrl_ptr = (Reset_Ip_CM4ResetType*)CM4_AIRCR_BASEADDR;

  f_TmpValue_u8 = (f_CM4ResetCtrl_ptr->AIRCR) & (uint32_t)CM4_AIRCR_PRIGROUP_MASK;
  f_TmpValue_u8 |= (uint32_t)CM4_AIRCR_SYSRESETREQ_MASK;
  f_TmpValue_u8 |= (uint32_t)CM4_AIRCR_VECTKEY(0x5FAU);
  /* All memory accesses have to be completed before reset */
  ASM_DSB();
  /* Performs a micro controller reset. */
  f_CM4ResetCtrl_ptr->AIRCR = f_TmpValue_u8;
}

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
)
{
  Std_ReturnType f_ReturnValue_en = E_OK;
  uint32_t StartTime_u32;
  uint32_t TimeoutTicks_u32;
  uint32_t ElapsedTicks_u32 = 0U;
  boolean TimeoutStatus_bool = (boolean)FALSE;
  
  /* Unlock RMU registers */
  CM_PWC->FPRC |= PWC_UNLOCK_CODE1;

  /* Reset the peripheral */
  CLR_REG32_BIT(CM_RMU->FRST0, para_SwModuleReset_ptr->SwModuleReset0_u32);
  CLR_REG32_BIT(CM_RMU->FRST1, para_SwModuleReset_ptr->SwModuleReset1_u32);
  CLR_REG32_BIT(CM_RMU->FRST2, para_SwModuleReset_ptr->SwModuleReset2_u32);
  CLR_REG32_BIT(CM_RMU->FRST3, para_SwModuleReset_ptr->SwModuleReset3_u32);

  /* Lock RMU registers */
  CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~(PWC_UNLOCK_CODE1)) & (CM_PWC->FPRC)));
  
  /* Initialize timeout */
  ElapsedTicks_u32 = 0U;
  Reset_Ip_StartTimeout(&StartTime_u32,
                       &TimeoutTicks_u32,
                       (uint32_t)RESET_IP_TIMEOUT_VALUE,
                       RESET_IP_TIMEOUT_TYPE);

  /* Wait for reset to complete */
  do
  {
      TimeoutStatus_bool = Reset_Ip_TimeoutExpired(&StartTime_u32,
                                                 &ElapsedTicks_u32,
                                                 TimeoutTicks_u32,
                                                 RESET_IP_TIMEOUT_TYPE);
  } while (((uint32_t)0U == READ_REG32_BIT(CM_RMU->FRST0, para_SwModuleReset_ptr->SwModuleReset0_u32)) &&
           ((uint32_t)0U == READ_REG32_BIT(CM_RMU->FRST1, para_SwModuleReset_ptr->SwModuleReset1_u32)) &&
           ((uint32_t)0U == READ_REG32_BIT(CM_RMU->FRST2, para_SwModuleReset_ptr->SwModuleReset2_u32)) &&
           ((uint32_t)0U == READ_REG32_BIT(CM_RMU->FRST3, para_SwModuleReset_ptr->SwModuleReset3_u32)) &&
           ((boolean)FALSE == TimeoutStatus_bool));

  if ((boolean)TRUE == TimeoutStatus_bool)
  {
      f_ReturnValue_en = E_NOT_OK;
  }
  else
  {
      f_ReturnValue_en = E_OK;
  }

  return f_ReturnValue_en;
}
#endif  /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
