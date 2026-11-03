/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: system_hc32a4xx.h
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
 *   @file       system_hc32a4xx.h
 *   @version    V1.0.2
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */
#ifndef SYSTEM_HC32A4XX_H
#define SYSTEM_HC32A4XX_H

#ifdef __cplusplus
extern "C"{
#endif

/******************************************************************************
 *  INCLUDES
 *****************************************************************************/

/******************************************************************************
 *   HEADER FILE VERSION INFORMATION
 ******************************************************************************/
#define SYSTEM_HC32A4XX_AR_RELEASE_MAJOR_VERSION        4
#define SYSTEM_HC32A4XX_AR_RELEASE_MINOR_VERSION        4
#define SYSTEM_HC32A4XX_AR_RELEASE_REVISION_VERSION     0
/******************************************************************************
 * VERSION CHECKS
 ******************************************************************************/
 
/******************************************************************************
 *  DEFINES & MACROS
 *****************************************************************************/

#if !defined (MRC_VALUE)
#define MRC_VALUE                       (8000000UL)     /*!< Internal middle speed RC freq. */
#endif

#if !defined (LRC_VALUE)
#define LRC_VALUE                       (32768UL)       /*!< Internal low speed RC freq. */
#endif

#if !defined (RTCLRC_VALUE)
#define RTCLRC_VALUE                    (32768UL)       /*!< Internal RTC low speed RC freq. */
#endif

#if !defined (SWDTLRC_VALUE)
#define SWDTLRC_VALUE                   (10000UL)       /*!< Internal SWDT low speed RC freq. */
#endif

#if !defined (XTAL_VALUE)
#define XTAL_VALUE                      (8000000UL)     /*!< External high speed OSC freq. */
#endif

#if !defined (XTAL32_VALUE)
#define XTAL32_VALUE                    (32768UL)       /*!< External low speed OSC freq. */
#endif

#if !defined (HCLK_VALUE)
#define HCLK_VALUE                      (SystemCoreClock >> ((CM_CMU->SCFGR & CMU_SCFGR_HCLKS) >> CMU_SCFGR_HCLKS_POS))
#endif
/**
 * @brief Compiler_Macros Compiler Macros
 * 
 */
#if defined (__ICCARM__)              /*!< IAR Compiler */
#ifndef NO_INIT
#define NO_INIT                   __no_init
#endif /* NO_INIT */

#elif defined ( __GNUC__ )
#ifndef NO_INIT
#define NO_INIT
#endif /* NO_INIT */
#endif

/******************************************************************************
 *  TYPEDEFS
 *****************************************************************************/

/******************************************************************************
 *  CONSTANTS
 *****************************************************************************/
 
/******************************************************************************
 *  GLOBAL VARIABLES
 *****************************************************************************/

/*******************************************************************************
 *  GLOBAL FUNCTION
 ******************************************************************************/
/** 
 * @brief NMI IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void NMI_Handler(void);

/** 
 * @brief HardFault IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void HardFault_Handler(void);

/** 
 * @brief MemManage IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void MemManage_Handler(void);

/** 
 * @brief BusFault IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void BusFault_Handler(void);

/** 
 * @brief UsageFault IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void UsageFault_Handler(void);

/** 
 * @brief DebugMon IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void DebugMon_Handler(void);

/** 
 * @brief PendSV IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void PendSV_Handler(void);

/** 
 * @brief SysTick IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void SysTick_Handler(void);

/** 
 * @brief SVC IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void SVC_Handler(void)                  __attribute__((naked));


/** 
 * @brief undefined IRQ handler
 * @param[in]  void
 * 
 * @return     void
 */

void UnKnown_IRQhandler(void);


extern void SystemInit(void);             /*!< Initialize the system */

#if defined (ROM_EXT_QSPI)
void SystemInit_QspiMem(void);
#endif

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_HC32A4XX_H */

/*---------------------- End of File ----------------------------------------*/
