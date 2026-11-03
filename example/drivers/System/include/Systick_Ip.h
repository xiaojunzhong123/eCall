/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Systick_Ip.h
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
 *   @file       Systick_Ip.h
 *   @version    1.0.3
 *   @brief      Systick Ip Layer Driver
 *   @addtogroup systick_ip
 *   @{
 */

#ifndef SYSTICK_IP
#define SYSTICK_IP

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"

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
);

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
);

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
);

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< SYSTICK_IP */
