/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip_Irq.h
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
 *   @file       Clock_Ip_Irq.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef CLOCK_IP_IRQ_H
#define CLOCK_IP_IRQ_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Mcu_Cfg.h"
#include "Osr_Internal.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function handles XTAL oscillator stop detection interrupt.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_CMU_Xtal_ISR_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @note           This ISR is called when XTAL oscillator stop is detected.
 *                  It calls Clock_Ip_XTAL_STOP() to handle the XTAL stop event.
 *
 *  @pre            None
 *
 */
ISR(
    Clock_Ip_CMU_Xtal_ISR
);

/**
 *  @brief          This function handles FCM (Frequency Control Monitor) overflow interrupt.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_CMU_FCM_OVF_ISR_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @note           This ISR is called when FCM overflow occurs.
 *                  It calls Clock_Ip_FCM_INT_OVF() to handle the FCM overflow event.
 *
 *  @pre            None
 *
 */
ISR(
    Clock_Ip_CMU_FCM_OVF_ISR
);

/**
 *  @brief          This function handles FCM (Frequency Control Monitor) measurement end interrupt.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_CMU_FCM_END_ISR_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @note           This ISR is called when FCM measurement completes.
 *                  It calls Clock_Ip_FCM_INT_END() to handle the FCM measurement end event.
 *
 *  @pre            None
 *
 */
ISR(
    Clock_Ip_CMU_FCM_END_ISR
);

/**
 *  @brief          This function handles FCM (Frequency Control Monitor) error interrupt.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Clock_Ip_CMU_FCM_ERR_ISR_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @note           This ISR is called when FCM error occurs.
 *                  It calls Clock_Ip_FCM_INT_ERR() to handle the FCM error event.
 *
 *  @pre            None
 *
 */
ISR(
    Clock_Ip_CMU_FCM_ERR_ISR
);

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* CLOCK_IP_IRQ_H */

/*---------------------- End of File ----------------------------------------*/
