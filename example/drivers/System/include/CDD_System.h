/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: CDD_System.h
 *
 * Description: Implementation of CDD SYSTEM High Level layer
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
 *   @file       CDD_System.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "System_Types.h"
#include "System_Ipr.h"
#include "CDD_System_Cfg.h"
#include "System_VersionInfo.h"

/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define SYSTEM_MODULE_ID     255
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
 *  @brief          This service performs initialization of the System Driver component.
 *
 *  @details        Service ID:        0x01
 *                  SRS ID:            AR_RS_System_00007,AR_RS_System_00008,AR_RS_System_00009
 *                                     AR_RS_System_00010,AR_RS_System_00011   
 *                  Design ID:         SDD_System_1
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      SystemConfigPtr    Pointer to a selected configuration structure.
 *  @arg            None               
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
Std_ReturnType System_Init(
    const System_ConfigType * SystemConfigPtr
);

/**
 *  @brief          AOS software trigger function.
 *
 *  @details        Service ID:        0X02
 *                  SRS ID:            AR_RS_System_00012,AR_RS_System_00013   
 *                  Design ID:         SDD_System_2
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         void
 *
 *  @note           This function generates a software trigger event.
 *
 *  @pre            None
 *
 */
void System_GenerateSwTrigger(
    void
);

#if(STD_ON == SYSTEM_AOS_ENABLE)
 /*
 *   @brief      This function initializes the AOS settings selected by AosSetting parameter.
 *
 *   @details       Service ID:        0x03
 *                  SRS ID:            AR_RS_System_00014,AR_RS_System_00015,AR_RS_System_00016
 *                                     AR_RS_System_00017
 *                  Design ID:         SDD_System_3
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 * 
 *   @param[in]  AosConfigSetIdx   Index of AOS setting from configuration structure to be initialized.
 *   @arg
 *
 *   @return     Std_ReturnType    Update AOS configuration set.
 *   @retval     E_OK              AOS configuration updated successfully.
 *   @retval     E_NOT_OK          AOS configuration update failed.
 *   @note
 *
 *
 */
Std_ReturnType System_UpdateAosConfigSet(
    uint32_t   AosConfigSetIdx
);

/**
 *  @brief          System driver AOS Reset function.
 *                  This function reset all AOS register to default value.
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            AR_RS_System_00018,AR_RS_System_00019  
 *                  Design ID:         SDD_System_4
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
void System_ResetAllAOSTriggerTarget(
    void
);
#endif /* STD_ON == SYSTEM_AOS_ENABLE */

/**
 *  @brief          This service performs initialization of the System Driver component.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            AR_RS_System_00005,AR_RS_System_00020
 *                                     AR_RS_System_00021,AR_RS_System_00022  
 *                  Design ID:         SDD_System_5
 *                  Sync/Async:        Synchronous
 *                  Reentrancy:        Non Reentrant
 *
 *  @param[in]      IntcConfig       Pointer to configuration structure.
 *  @param[in]      IntcCfgSetIdx    Intc configuration set index.
 *
 *  @return         Std_ReturnType    Perform initialization of the System Driver component.
 *  @retval         E_OK              Intc update configurations successfully.
 *  @retval         E_NOT_OK          Intc update configurations unsuccessfully.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType System_UpdateIntConfigSet(
    const System_IntConfigSetType *IntConfig,
    uint8_t IntCfgSetIdx
);

/**
 *  @brief          IRQ enable function
 *
 *  @details        Service ID:        0x06
 *                  SRS ID:            AR_RS_System_00005,AR_RS_System_00023
 *                                     AR_RS_System_00024,AR_RS_System_00025   
 *                  Design ID:         SDD_System_6
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      IntcConfig    Intc configurations.
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
Std_ReturnType System_EnableIrq(
    const System_IntConfigType *IntcConfig
);

/**
 *  @brief          IRQ sign out (release) function
 *
 *  @details        Service ID:        0x07
 *                  SRS ID:            AR_RS_System_00026,AR_RS_System_00027
 *                                     AR_RS_System_00028   
 *                  Design ID:         SDD_System_7
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      IrqNumber    - can be any value from INT000_IRQn ~  MAX_IRQn
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
Std_ReturnType System_DisableIrq(
    IRQn_Type  IrqNumber
);

#if (STD_ON == SYSTEM_VERSION_INFO_API)
/**
 *  @brief          This function returns the Version Information (vendor id, module id, major, minor and patch version).
 *
 *  @details        Service ID:        0x08
 *                  SRS ID:            AR_RS_System_00029,AR_RS_System_00030
 *                                     AR_RS_System_00031    
 *                  Design ID:         SDD_System_8
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      SystemVersionInfo    Pointer to version information structure.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void System_GetVersionInfo(
    Std_VersionInfoType * SystemVersionInfo
);
#endif /* STD_ON == SYSTEM_VERSION_INFO_API */

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SYSTEM_H */

/*---------------------- End of File ----------------------------------------*/
