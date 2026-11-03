/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Osr.h
 *
 * Description: Implementation of OSR High Level layer
 *              Compiler:                IAR GCC
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
 *   @file       Osr.h
 *   @version    1.0.3
 *   @brief      Osr Header File
 *   @addtogroup system_driver
 *   @{
 */

#ifndef OSR_H
#define OSR_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Osr_Types.h"
#include "Common.h"
#include "Osr_Internal.h"

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
 *  @brief          This function performs initialization of OSR.
 *
 *  @details        Service ID:        0x09
 *                  SRS ID:            AR_RS_System_00033,AR_RS_System_00034
 *                                     AR_RS_System_00035,AR_RS_System_00036   
 *                  Design ID:         SDD_System_9
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      OsrConfig    Point to Osr configuration.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Osr_Init(
    const Osr_ConfigType *const OsrConfig[]
);

/**
 *  @brief          This function get the current counter.
 *
 *  @details        Service ID:        0x0a
 *                  SRS ID:            AR_RS_System_00032,AR_RS_System_00037
 *                                     AR_RS_System_00037,AR_RS_System_00038   
 *                  Design ID:         SDD_System_10
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      ChosenCounter    choose 1 in 3 Counter type.
 *
 *  @return         uint32_t    Current counter value.
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Osr_GetCounter(
    Osr_CounterType ChosenCounter
);

/**
 *  @brief          This function get the elapsed counter.
 *
 *  @details        Service ID:        0x0b
 *                  SRS ID:            AR_RS_System_00032,AR_RS_System_00039
 *                                     AR_RS_System_00040   
 *                  Design ID:         SDD_System_11
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      CounterRef       Point to current counter - RefCounter to compute the Elapsed counter.
 *  @param[in]      ChosenCounter    choose 1 in 3 Counter type.
 *
 *  @return         uint32_t    Elapsed counter value.
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Osr_GetElapsed(
    uint32_t *CounterRef,
    Osr_CounterType ChosenCounter
);

/**
 *  @brief          This function converts microseconds to ticks.
 *
 *  @details        Service ID:        0x0c
 *                  SRS ID:            AR_RS_System_00041,AR_RS_System_00042 
 *                  Design ID:         SDD_System_12
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Reentrant
 *
 *  @param[in]      Micros           Value Microsecond that need to convert into Ticks.
 *  @param[in]      ChosenCounter    Choose 1 in 3 Counter type.
 *
 *  @return         uint32_t    Converted tick value.
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
uint32_t Osr_UsToTicks(
    uint32_t Micros,
    Osr_CounterType ChosenCounter
);

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /*!< OSR_H */
