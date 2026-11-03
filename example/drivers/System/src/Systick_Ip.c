/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Systick_Ip.c
 *
 * Description: Implementation of SYSTICK IP layer
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
 *   @file       Systick_Ip.c
 *   @version    1.0.3
 *   @brief      Systick Ip Layer Driver
 *   @addtogroup systick_ip
 *   @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Systick_Ip.h"
#include "System_Hw_Reg.h"

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/* SYST_CSR Register */
#define SCB_SYST_CSR_ENABLE_MASK                                         (0x00000001U)
#define SCB_SYST_CSR_TICKINT_MASK                                        (0x00000002U)
#define SCB_SYST_CSR_CLKSOURCE_MASK                                      (0x00000004U)
/* SYST_RVR Register */
#define SCB_SYST_RVR_RELOAD_MASK                                         (0x00FFFFFFU)
#define SCB_SYST_RVR_RELOAD_POS                                          (0U)
#define SCB_SYST_RVR_RELOAD(x)                                           (((uint32_t)(((uint32_t)(x)) << SCB_SYST_RVR_RELOAD_POS)) &  SCB_SYST_RVR_RELOAD_MASK)

/* SYST_CVR Register */
#define SCB_SYST_CVR_CURRENT_MASK                                        (0xFFFFFFFFU)
#define SCB_SYST_CVR_CURRENT_POS                                         (0U)
#define SCB_SYST_CVR_CURRENT(x)                                          (((uint32_t)(((uint32_t)(x)) << SCB_SYST_CVR_CURRENT_POS)) &  SCB_SYST_CVR_CURRENT_MASK)


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

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

/**
 *  @brief          This function Initialize the Systick
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Value_u32    Systick reload value
 *  @param[in]      para_SrcClk_u8    Systick source clock selection
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void SysTick_Ip_Init(
    uint32_t para_Value_u32,
    uint8_t para_SrcClk_u8
)
{
    /* Disable the SysTick counter */
    SCB->SYST_CSR &= ~(uint32_t)SCB_SYST_CSR_ENABLE_MASK;
    /* Load the value into the SYST_CVR */
    SCB->SYST_RVR = SCB_SYST_RVR_RELOAD((para_Value_u32 & SCB_SYST_RVR_RELOAD_MASK));
    /* Clears the current counter value */
    SCB->SYST_CVR = SCB_SYST_CVR_CURRENT(0U);
    /* Changes the SysTick exception status */
    SCB->SYST_CSR &= ~(uint32_t)SCB_SYST_CSR_TICKINT_MASK;
    if (0U == para_SrcClk_u8)
    {
        /* SysTick uses the IMPLEMENTATION DEFINED external reference clock */
        SCB->SYST_CSR &= ~(uint32_t)SCB_SYST_CSR_CLKSOURCE_MASK;
    }
    else
    {
        /* SysTick uses the processor clock */
        SCB->SYST_CSR |= (uint32_t)SCB_SYST_CSR_CLKSOURCE_MASK;
    }
    /* Enable the SysTick counter */
    SCB->SYST_CSR |= (uint32_t)SCB_SYST_CSR_ENABLE_MASK;
}

/**
 *  @brief          This function get the currrent counter.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         uint32_t    Counter_u32
 *  @retval         None
 *
 *  @note           None.
 *
 *  @pre            None
 *
 */
uint32_t SysTick_Ip_GetCounter(
    void
)
{
    /* Reads the current counter value */
    return (uint32_t)((SCB->SYST_CVR) & SCB_SYST_CVR_CURRENT_MASK);
}
/**
 *  @brief          This function get the eplapsed counter.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[inout]   para_CounterRef_ptr    Systick counter refrence
 *
 *  @return         uint32_t    Elapsed_u32
 *  @retval         None
 *
 *  @note           None.
 *
 *  @pre            None
 *
 */
uint32_t SysTick_Ip_GetElapsed(
    uint32_t * para_CounterRef_ptr
)
{
    uint32_t ElapsedVal_u32 = 0U;
    uint32_t CurrentVal_u32 = SysTick_Ip_GetCounter();
    uint32_t RefVal_u32 = * para_CounterRef_ptr;
    uint32_t GetRvrVal_u32 = 0U;
    
    if (CurrentVal_u32 > RefVal_u32)
    {
        GetRvrVal_u32 = ((uint32_t)(SCB->SYST_RVR & SCB_SYST_RVR_RELOAD_MASK));     
        ElapsedVal_u32 = GetRvrVal_u32 - (CurrentVal_u32 - RefVal_u32);
    }
    else
    {
        ElapsedVal_u32 = (RefVal_u32 - CurrentVal_u32);
    }
    * para_CounterRef_ptr = CurrentVal_u32;

    return ElapsedVal_u32;
}

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
