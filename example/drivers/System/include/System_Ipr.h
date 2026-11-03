/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: System_Ipr.h
 *
 * Description: Implementation of SYSTEM IP Router layer
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
 *   @file       System_Ipr.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef SYSTEM_IPR_H
#define SYSTEM_IPR_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Interrupt_Ip.h"
#include "System_Types.h"
#include "Aos_Ip.h"
#include "CDD_System_Cfg.h"
/***********************************************************************************************************************
 *   HEADER FILE VERSION INFORMATION
 **********************************************************************************************************************/
#define SYSTEM_IPR_AR_RELEASE_MAJOR_VERSION        4
#define SYSTEM_IPR_AR_RELEASE_MINOR_VERSION        4
#define SYSTEM_IPR_AR_RELEASE_REVISION_VERSION     0
/***********************************************************************************************************************
 * VERSION CHECKS
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
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

/**
 *  @brief          System driver initialization function.
 *
 *  @param[in]      para_ConfigPtr_ptr    Pointer to configuration structure.
 *
 *  @return         Std_ReturnType    Perform initialization of the System Driver component.
 *  @retval         E_OK              Initialize successfully.
 *  @retval         E_NOT_OK          Initialize unsuccessfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType System_Ipr_Init(
    const System_ConfigType * para_ConfigPtr_ptr
);

/**
 *  @brief          AOS software trigger function.
 *                  This function generates a software trigger event.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void System_Ipr_GenerateSwTrigger(
    void
);

#if(STD_ON == SYSTEM_AOS_ENABLE)
/**
 *  @brief          System driver AOS Implementation function.
 *                  This function intializes the AOS configuration.
 *
 *  @param[in]      para_AosConfigSetIdx_u32    AOS setting ID from config structure to be used.
 *
 *  @return         Std_ReturnType    Update AOS configuration set.
 *  @retval         E_OK              Update Configures successfully.
 *  @retval         E_NOT_OK          Update Configures unsuccessfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType System_Ipr_UpdateAosConfigSet(
    uint32_t   para_AosConfigSetIdx_u32
);

/**
 *  @brief          System driver AOS Reset function.
 *                  This function reset all AOS register to default value.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void System_Ipr_ResetAllAOSTriggerTarget(
    void
);
#endif /* STD_ON == SYSTEM_AOS_ENABLE */

/**
 *  @brief          System driver INTC Implementation function.
 *                  This function intializes the INTC configuration.
 *
 *  @param[in]      para_ConfigPtr_ptr      Interrupt config set.
 *  @param[in]      para_IntCfgSetIdx_u8    Interrupt setting ID from config structure to be used.
 *
 *  @return         Std_ReturnType    Update interrupt configuration set.
 *  @retval         E_OK              Update Configures successfully.
 *  @retval         E_NOT_OK          Update Configures unsuccessfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType System_Ipr_UpdateIntConfigSet(
    const System_IntConfigSetType * para_ConfigPtr_ptr,
    uint8_t para_IntCfgSetIdx_u8
);

/**
 *  @brief          IRQ enable function
 *
 *  @param[in]      para_IntcConfig_ptr    Intc configurations.
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
Std_ReturnType System_Ipr_EnableIrq(
    const System_IntConfigType * para_IntcConfig_ptr
);

/**
 *  @brief          IRQ sign out (release) function
 *
 *  @param[in]      para_IrqNumber_en    - can be any value from INT000_IRQn ~  for different product
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
Std_ReturnType System_Ipr_DisableIrq(
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

#endif /* SYSTEM_IPR_H */

/*---------------------- End of File ----------------------------------------*/
