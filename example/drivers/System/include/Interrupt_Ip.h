/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Interrupt_Ip.h
 *
 * Description: Implementation of INTERRUPT IP layer
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
 *   @file       Interrupt_Ip.h
 *   @version    V1.0.3
 *   @brief      System Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef INTERRUPT_IP_H
#define INTERRUPT_IP_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "System_Types.h"

/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define WRITE_REG32(REG, VAL)           ((REG) = ((uint32_t)(VAL)))

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/
/**
 * @brief   Maximum IRQ handler number
 */
#define EXCEPTION_NUM_MAX       (159U)
#define IRQn_OFFSET             (0U)
#define EXTINT_CH_NUM_MAX       (16U)
#define EIRQCFR_REG             (CM_INTC->EIFCR)
#define NMICFR_REG              (CM_INTC->NMIFCR)
#define INTSEL_REG              (uint32_t)(&CM_INTC->INTSEL0)
#define INTSEL_RST_VALUE        (0x1FFUL)

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

/**
 *  @brief          Initialize intc module via configurations.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr    Intc configurations.
 *
 *  @return         Std_ReturnType    Initialize interrupt controller.
 *  @retval         E_OK              Interrupt initialized successfully.
 *  @retval         E_NOT_OK          Interrupt initialized failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_Init(
    const System_IntConfigSetType * para_IntConfig_ptr
);

/**
 *  @brief          Update intc configuration set.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr      Intc configurations.
 *  @param[in]      para_IntCfgSetIdx_u8    Interrupt setting ID from config structure to be used.
 *
 *  @return         Std_ReturnType    Update interrupt configuration set.
 *  @retval         E_OK              Interrupt updated successfully.
 *  @retval         E_NOT_OK          Interrupt updated failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_UpdateINTCConfig(
    const System_IntConfigSetType * para_IntConfig_ptr,
    uint32_t para_IntCfgSetIdx_u8
);

/**
 *  @brief          IRQ enable function
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr    Intc configurations.
 *
 *  @return         Std_ReturnType    Enable IRQ function.
 *  @retval         E_OK              IRQ register successfully.
 *  @retval         E_NOT_OK          IRQ No. and Peripheral Int source are not match or Specified IRQ entry was
 *                                    signed before.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_EnableIRQ(
    const System_IntConfigType * para_IntConfig_ptr
);

/**
 *  @brief          IRQ sign out (release) function
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IrqNumber_en    can be any value from INT000_IRQn ~  for different product
 *
 *  @return         Std_ReturnType    Disable IRQ function.
 *  @retval         E_OK              IRQ disabled successfully.
 *  @retval         E_NOT_OK          IRQ disable failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_DisableIRQ(
    IRQn_Type  para_IrqNumber_en
);

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* INTERRUPT_IP_H */

/*---------------------- End of File ----------------------------------------*/
