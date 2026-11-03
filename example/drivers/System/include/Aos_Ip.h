/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Aos_Ip.h
 *
 * Description: Implementation of AOS IP layer
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
 *   @file       Aos_Ip.h
 *   @version    V1.0.3
 *   @brief      System Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef AOS_IP_H
#define AOS_IP_H

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
#define SYSTEM_AOS_TOTAL_TRIGGER_TARGET_REGISTER        44U
#define AOS_TRIGGER_TRGSEL_COMEN_0_POS                  30U
#define AOS_TRIGGER_TRGSEL_COMEN_1_POS                  31U
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
 *  @brief          This service performs initialization AOS
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_AOS_ptr    Pointer to configuration structure.
 *
 *  @return         Std_ReturnType    Perform initialization of AOS.
 *  @retval         E_OK              AOS initialized successfully.
 *  @retval         E_NOT_OK          AOS initialization failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Aos_Ip_Init(
    const System_AosConfigSetType * para_AOS_ptr
);

/**
 *  @brief          AOS software trigger function.
 *                  This function generates a software trigger event.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Aos_Ip_GenerateSwTrigger(
    void
);

/**
 *  @brief          System driver AOS Implementation function.
 *                  This function intializes the AOS configuration.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_AOSConfigSetIndex_u32    AOS setting ID from config structure to be used.
 *
 *  @return         Std_ReturnType    Update AOS configuration set.
 *  @retval         E_OK              Update configures is successfully.
 *  @retval         E_NOT_OK          Update configures is unsucessfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Aos_Ip_UpdateAOSConfigSet(
    uint32_t   para_AOSConfigSetIndex_u32
);

/**
 *  @brief          System driver AOS Reset function.
 *                  This function reset all AOS register to default value.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Aos_Ip_ResetAllAosTriggerTarget(
    void
);

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* AOS_IP_H */

/*---------------------- End of File ----------------------------------------*/
