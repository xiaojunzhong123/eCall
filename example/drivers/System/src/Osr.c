/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Osr.c
 *
 * Description: Implementation of OSR High Level layer
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
 *   @file       Osr.c
 *   @version    1.0.3
 *   @brief      Osr Source File
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Osr.h"
#include "Systick_Ip.h"
#if (STD_ON == OSR_VALIDATE_PARAMS)
#include "Det.h"
#endif /*!< (STD_ON == OSR_VALIDATE_PARAMS) */

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define OsrCoreID()        (0U)

#ifndef SYSTEM_MODULE_ID
#define SYSTEM_MODULE_ID 255
#endif

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
#if defined(AUTOSAR_OS_USED)
#define SYSTEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static const Osr_ConfigType *s_OsrOsCounterCfg_aa[OSR_MAX_COREIDX_SUPPORTED];

#define SYSTEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"
#endif

#define SYSTEM_START_SEC_VAR_CLEARED_32
#include "System_MemMap.h"

static uint32_t s_OsrClockFrequency_aa[OSR_MAX_COREIDX_SUPPORTED];

#define SYSTEM_STOP_SEC_VAR_CLEARED_32
#include "System_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

LOCAL_INLINE uint32_t Osr_GetDummyCounter(
    void
);

LOCAL_INLINE uint32_t Osr_GetDummyElapsed(
    uint32_t * para_CounterRef_ptr
);

LOCAL_INLINE uint32_t Osr_DummyUsToTick(
    uint32_t para_Micros_u32
);

#if (STD_ON == OSR_VALIDATE_PARAMS)

LOCAL_INLINE Std_ReturnType Osr_ValidateInitPtr(
    const Osr_ConfigType *const para_OsrConfig_ptr[],
    uint8_t para_ServiceId_u8,
    uint8_t para_ErrorId_u8
);
#endif

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

/**
 *  @brief          Osr_GetDummyCounter.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None   
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         uint32_t    Dummy counter value (always 0).
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
LOCAL_INLINE uint32_t Osr_GetDummyCounter(
    void
)
{
    return 0u;
}

/**
 *  @brief          Osr_GetDummyElapsed.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None   
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_CounterRef_ptr    Pointer to counter reference value.
 *
 *  @return         uint32_t    Dummy elapsed value (always 1).
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
LOCAL_INLINE uint32_t Osr_GetDummyElapsed(
    uint32_t * para_CounterRef_ptr
)
{
    (void)para_CounterRef_ptr;
    return 1u;
}

/**
 *  @brief          Osr_DummyUsToTick.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None   
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Micros_u32    Microsecond value to convert to ticks.
 *
 *  @return         uint32_t    Converted tick value (same as input).
 *  @retval         None
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
LOCAL_INLINE uint32_t Osr_DummyUsToTick(
    uint32_t para_Micros_u32
)
{
    return para_Micros_u32;
}

#if (STD_ON == OSR_VALIDATE_PARAMS)
/**
 *  @brief          Osr_ValidateInitPtr.
 *
 *  @details        Service ID:        None
 *                  SRS ID:            None   
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_OsrConfig_ptr    Pointer to OSR configuration array.
 *  @param[in]      para_ServiceId_u8     Service ID for error reporting.
 *  @param[in]      para_ErrorId_u8       Error ID for error reporting.
 *
 *  @return         Std_ReturnType    Validate initialization pointer.
 *  @retval         E_OK              Validation successful.
 *  @retval         E_NOT_OK          Validation failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
LOCAL_INLINE Std_ReturnType Osr_ValidateInitPtr(
    const Osr_ConfigType *const para_OsrConfig_ptr[],
    uint8_t para_ServiceId_u8,
    uint8_t para_ErrorId_u8
)
{
    Std_ReturnType Ret_en = (Std_ReturnType)E_NOT_OK;
    if (NULL_PTR == para_OsrConfig_ptr)
    {
#if defined(AUTOSAR_OS_USED)
        (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, (uint8_t)0, (uint8_t)para_ServiceId_u8, (uint8_t)para_ErrorId_u8);
#else
        (void)para_ServiceId_u8;
        (void)para_ErrorId_u8;
#endif
    }
    else
    {
        Ret_en = (Std_ReturnType)E_OK;
    }
    return Ret_en;
}
#endif

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

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
)
{
    uint32_t CoreId_u32 = OsrCoreID();

#if (STD_ON == OSR_VALIDATE_PARAMS)
    Std_ReturnType Ret_en = (Std_ReturnType)E_OK;

    Ret_en = Osr_ValidateInitPtr(OsrConfig, OSR_INIT_ID, OSR_E_INIT_FAILED);

    if (E_OK == Ret_en)
    {
#endif        
        s_OsrClockFrequency_aa[CoreId_u32] = OsrConfig[CoreId_u32]->ClockFrequency_u32;

#if defined(AUTOSAR_OS_USED)
        s_OsrOsCounterCfg_aa[CoreId_u32] = OsrConfig[CoreId_u32];
#endif

        SysTick_Ip_Init(OsrConfig[CoreId_u32]->SystickReloadValue_u32, OsrConfig[CoreId_u32]->SystickClkSrc_u8);

#if (STD_ON == OSR_VALIDATE_PARAMS)
    }
#endif    
}

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
)
{
    uint32_t Counter_u32 = 0u;
#if defined(AUTOSAR_OS_USED)
    TickType Value_en = 0u;
    StatusType Status_en = 0u;
    uint32_t CoreId_u32  = OsrCoreID();
#endif

    if (OSR_COUNTER_DUMMY == ChosenCounter)
    {
        Counter_u32 = Osr_GetDummyCounter();
    }
    else if (OSR_COUNTER_AUTOSAROS == ChosenCounter)
    {
#if defined(AUTOSAR_OS_USED)
        Status_en = GetCounterValue(s_OsrOsCounterCfg_aa[CoreId_u32]->CounterIdx_u8, &Value_en);
        (void)Status_en; 
        Counter_u32 = (uint32_t)Value_en;
#else
        ; /* Do nothing */
#endif
    }
    else
    {
        Counter_u32 = SysTick_Ip_GetCounter();
    }

    return Counter_u32;
}

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
)
{
    uint32_t ElapsedTime_u32 = 0u;
#if defined(AUTOSAR_OS_USED)
    TickType ElapsedTickType_en = 0u;
    StatusType Status_en = 0u;
    uint32_t CoreId_u32  = OsrCoreID();
#endif  

    if (NULL_PTR != CounterRef)
    {
        if (OSR_COUNTER_DUMMY == ChosenCounter)
        {
            ElapsedTime_u32 = Osr_GetDummyElapsed(CounterRef);
        }
        else if (OSR_COUNTER_AUTOSAROS == ChosenCounter)
        {
#if defined(AUTOSAR_OS_USED)
            Status_en = GetElapsedValue(s_OsrOsCounterCfg_aa[CoreId_u32]->CounterIdx_u8, (TickType *)CounterRef, &ElapsedTickType_en);
            (void)Status_en; 
            ElapsedTime_u32 = (uint32_t)ElapsedTickType_en;
#else
            ; /* Do nothing */
#endif
        }
        else
        {
            ElapsedTime_u32 = SysTick_Ip_GetElapsed(CounterRef);            
        }
    }

    return ElapsedTime_u32;
}

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
)
{
    uint64_t InterTick_64;
    uint32_t Ticks_u32  = 0u;
    uint32_t CoreId_u32 = OsrCoreID();

    if (OSR_COUNTER_DUMMY == ChosenCounter)
    {
        Ticks_u32 = Osr_DummyUsToTick(Micros);
    }
    else
    {
        InterTick_64  = Micros * (uint64_t)s_OsrClockFrequency_aa[CoreId_u32];
        InterTick_64 /= OSR_USVAL;
        Ticks_u32 = (uint32_t)(InterTick_64 & OSR_64BIT_MASK);
    }

    return Ticks_u32;
}

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
