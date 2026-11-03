/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: system_hc32a4xx.c
 *
 * Description: Implementation of SYSTEM
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
 *   @file       system_hc32a4xx.c
 *   @version    V1.0.2
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

/******************************************************************************
 *  MISRA VIOLATIONS - Summary and analysis.
 *****************************************************************************/
 
#ifdef __cplusplus
extern "C"{
#endif

/******************************************************************************
 *  INCLUDES
 *****************************************************************************/
#include "system_hc32a4xx.h"
#include "Interrupt_Ip.h"
 
/******************************************************************************
 * VERSION CHECKS
 ******************************************************************************/
 
/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

/******************************************************************************
 *  EXTERN
 *****************************************************************************/

/******************************************************************************
 *  LOCAL TYPEDEFS
 *****************************************************************************/

/******************************************************************************
 *  LOCAL CONSTANTS
 *****************************************************************************/

/******************************************************************************
 *  LOCAL VARIABLES
 *****************************************************************************/
 
/******************************************************************************
 *  GLOBAL VARIABLES
 *****************************************************************************/

 /******************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *  LOCAL FUNCTION
 ******************************************************************************/

/******************************************************************************
 *  GLOBAL FUNCTION
 ******************************************************************************/
/**
 * @brief NMI IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void NMI_Handler(void)
{
    while(1){;};
}

/**
 * @brief HardFault IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void HardFault_Handler(void)
{
    while(1){;};
}

/**
 * @brief MemManage IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void MemManage_Handler(void)
{
    while(1){;};
}

/**
 * @brief BusFault IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void BusFault_Handler(void)
{
    while(1){;};
}

/**
 * @brief UsageFault IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void UsageFault_Handler(void)
{
    while(1){;};
}

/**
 * @brief DebugMon IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void DebugMon_Handler(void)
{
    while(1){;};
}

/**
 * @brief PendSV IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void PendSV_Handler(void)
{
    while(1){;};
}

/**
 * @brief SysTick IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

__attribute__ ((weak)) void SysTick_Handler(void)
{
    while(1){;};
}

/**
 * @brief undefined IRQ handler
 * @param[in]  void
 *
 * @return     void
 */

void UnKnown_IRQhandler(void)
{
   while(1){;};
}


void SVC_Handler(void)
{
    while(1){;};
}

/**
 *   @brief        Setup the microcontroller system. Initialize the System and update the SystemCoreClock variable.
 *
 *   @param[in]    None
 *   @arg          None
 *
 *   @return       void
 *   @retval       None
 *
 *   @note
 */
void SystemInit(void)
{
#if defined(FLASH_ICACHE_ENABLE) || defined(FLASH_DCACHE_ENABLE)
    CM_EFM->FAPRT = 0x0123U;
    CM_EFM->FAPRT = 0x3210U;

/* Eanble Icode cache function */    
#if defined(FLASH_ICACHE_ENABLE)
    CM_EFM->FRMC |= EFM_FRMC_PREFE;
    CM_EFM->FRMC |= EFM_FRMC_ICACHE;
#endif /* (FLASH_ICACHE_ENABLE) */

/* Eanble Dcode cache function */
#if defined(FLASH_DCACHE_ENABLE)    
    CM_EFM->FRMC |= EFM_FRMC_DCACHE;
#endif /* (FLASH_DCACHE_ENABLE) */
    CM_EFM->FAPRT = 0x0U;
#endif

/* FPU settings */
#ifdef USE_FPU
    SCB->CPACR |= ((3UL << 20) | (3UL << 22)); /* set CP10 and CP11 Full Access */
#endif /* (USE_FPU) */

}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
