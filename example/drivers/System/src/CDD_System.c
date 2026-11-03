/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: CDD_System.c
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
 *   @file       CDD_System.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 SYSTEM Driver
 *   @addtogroup system_driver
 *   @{
 */

/***********************************************************************************************************************
 *  MISRA VIOLATIONS - Summary and analysis.
 **********************************************************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "CDD_System.h"
#include "System_Ipr.h"
#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)
  #include "Det.h"
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */

/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/
/**
 * @brief   Check static code version with generated code version
 */
#if (SYSTEM_VENDOR_ID != SYSTEM_CFG_VENDOR_ID)
    #error "static code and generated code have different vendor ids"
#endif

#if ((SYSTEM_SW_MAJOR_VERSION != SYSTEM_CFG_SW_MAJOR_VERSION) || \
     (SYSTEM_SW_MINOR_VERSION != SYSTEM_CFG_SW_MINOR_VERSION) || \
     (SYSTEM_SW_PATCH_VERSION != SYSTEM_CFG_SW_PATCH_VERSION) \
    )
    #error "static code and generated code have different software release version"
#endif

#ifndef DISABLE_EXTERNAL_VERSION_CHECK
    #if (STD_ON == SYSTEM_DEV_ERROR_DETECT)
        /* Check AutoSar Version Numbers of System and Det.h */
        #if ((SYSTEM_AR_RELEASE_MAJOR_VERSION != DET_AR_RELEASE_MAJOR_VERSION) || \
            (SYSTEM_AR_RELEASE_MINOR_VERSION != DET_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of System and Det.h are different"
        #endif
    #endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */
#endif

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief   Driver initialization status.
 * @details This enum contains the values for the driver initialization status.
 */
typedef enum
{
  SYSTEM_UNINIT = 0U, /*!< @brief Driver not initialized */
  SYSTEM_INITIALIZED  /*!< @brief Driver ready */
} System_DrvStatusType;
/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static const System_ConfigType * l_SystemConfig_ptr;

#define SYSTEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"


#define SYSTEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static System_DrvStatusType  l_SystemDrvStatus_en = SYSTEM_UNINIT;

#define SYSTEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"
/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
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
 *                                     AR_RS_ECUC_System_00001,AR_RS_ECUC_System_00002,AR_RS_ECUC_System_00003
 *                                     AR_RS_ECUC_System_00004,AR_RS_ECUC_System_00005,AR_RS_ECUC_System_00006
 *                                     AR_RS_ECUC_System_00007,AR_RS_ECUC_System_00008,AR_RS_ECUC_System_00009
 *                                     AR_RS_ECUC_System_00010,AR_RS_ECUC_System_00011,AR_RS_ECUC_System_00012
 *                                     AR_RS_ECUC_System_00013,AR_RS_ECUC_System_00014,AR_RS_ECUC_System_00015
 *                                     AR_RS_ECUC_System_00016,AR_RS_ECUC_System_00017,AR_RS_ECUC_System_00018
 *                                     AR_RS_ECUC_System_00019,AR_RS_ECUC_System_00020,AR_RS_ECUC_System_00021
 *                                     AR_RS_ECUC_System_00022,AR_RS_ECUC_System_00023,AR_RS_ECUC_System_00024
 *                                     AR_RS_ECUC_System_00025,AR_RS_ECUC_System_00026,AR_RS_ECUC_System_00027
 *                                     AR_RS_ECUC_System_00028,AR_RS_ECUC_System_00029,AR_RS_ECUC_System_00030
 *                                     AR_RS_ECUC_System_00031,AR_RS_ECUC_System_00032,AR_RS_ECUC_System_00033
 *                                     AR_RS_ECUC_System_00034,AR_RS_ECUC_System_00035,AR_RS_ECUC_System_00036
 *                                     AR_RS_ECUC_System_00037,AR_RS_ECUC_System_00038,AR_RS_ECUC_System_00039
 *                                     AR_RS_ECUC_System_00040,AR_RS_ECUC_System_00041,AR_RS_ECUC_System_00042
 *                                     AR_RS_ECUC_System_00043,AR_RS_ECUC_System_00044,AR_RS_ECUC_System_00045
 *                                     AR_RS_ECUC_System_00046,AR_RS_ECUC_System_00047,AR_RS_ECUC_System_00048     
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)  
#if (STD_ON == SYSTEM_PRECOMPILE_SUPPORT)
  if (NULL_PTR != SystemConfigPtr)
#else
  if (NULL_PTR == SystemConfigPtr)
#endif /*!<!< (STD_ON == SYSTEM_PRECOMPILE_SUPPORT) */
  {
    #if(STD_ON == SYSTEM_DEV_ERROR_DETECT)
    /*Raise Det Error INVALID_POINTER*/
    /* Invalid pointer */
    (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                          (uint8_t)0U, \
                          (uint8_t)SYSTEM_INIT_ID, \
                          (uint8_t)SYSTEM_E_INVALID_POINTER);
    #endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */
  }
  else
  {
#endif /*!< (STD_ON == SYSTEM_DEV_ERROR_DETECT) */      
    /* Set system driver status as SYSTEM_INITIALIZED*/
    l_SystemDrvStatus_en = SYSTEM_INITIALIZED;    
#if (STD_ON == SYSTEM_PRECOMPILE_SUPPORT)
    f_ReturnValue_dd = System_Ipr_Init(&System_Config);
    l_SystemConfig_ptr = &System_Config;
    (void)SystemConfigPtr;
#else
    f_ReturnValue_dd = System_Ipr_Init(SystemConfigPtr);
    l_SystemConfig_ptr = SystemConfigPtr;
#endif
#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)     
  }
#endif /*!< (STD_ON == SYSTEM_DEV_ERROR_DETECT) */  

  return f_ReturnValue_dd;
}

/**
 *  @brief          AOS software trigger function.
 *
 *  @details        Service ID:        0X02
 *                  SRS ID:            AR_RS_System_00012,AR_RS_System_00013
 *                                     AR_RS_System_00002,AR_RS_System_00003,AR_RS_System_00004   
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
)
{
  /*Generate Software Trigger */
  System_Ipr_GenerateSwTrigger();
}

#if(STD_ON == SYSTEM_AOS_ENABLE)
 /*
 *   @brief      This function initializes the AOS settings selected by AosSetting parameter.
 *
 *   @details       Service ID:        0x03
 *                  SRS ID:            AR_RS_System_00014,AR_RS_System_00015,AR_RS_System_00016
 *                                     AR_RS_System_00017
 *                                     AR_RS_System_00002,AR_RS_System_00003,AR_RS_System_00004 
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)  
  if(SYSTEM_UNINIT == l_SystemDrvStatus_en)
  {
    /*Raise Det Error AOS_NOT_CONFIGURED*/
    /* AOS has not been configured */
    (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                          (uint8_t)0U, \
                          (uint8_t)SYSTEM_UPDATEAOSCONFIGSET_ID, \
                          (uint8_t)SYSTEM_E_NOT_CONFIGURED);
  }
  else
  {
    if(l_SystemConfig_ptr->SystemAosNumberOfAosSetConfig_u32 <= AosConfigSetIdx)
    {
      /* Invalid pointer */
      (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                            (uint8_t)0U, \
                            (uint8_t)SYSTEM_UPDATEAOSCONFIGSET_ID, \
                            (uint8_t)SYSTEM_E_INVALID_CHANNEL);
    }
    else
    {
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */      
      f_ReturnValue_dd = System_Ipr_UpdateAosConfigSet(AosConfigSetIdx);
#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)      
    }
  }
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */ 

  return f_ReturnValue_dd;
}

/**
 *  @brief          System driver AOS Reset function.
 *                  This function reset all AOS register to default value.
 *
 *  @details        Service ID:        0x04
 *                  SRS ID:            AR_RS_System_00018,AR_RS_System_00019
  *                                     AR_RS_System_00002,AR_RS_System_00003,AR_RS_System_00004   
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
)
{
  System_Ipr_ResetAllAOSTriggerTarget();
}
#endif

/**
 *  @brief          This service performs initialization of the System Driver component.
 *
 *  @details        Service ID:        0x05
 *                  SRS ID:            AR_RS_System_00005,AR_RS_System_00020
 *                                     AR_RS_System_00021,AR_RS_System_00022,AR_RS_System_00001  
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)
  if (NULL_PTR == IntConfig)
  {
    /* Raise Det Error INVALID POINTER */
    (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID,
                          (uint8_t)0U,
                          (uint8_t)SYSTEM_INTUPDATECONFIG_ID,
                          (uint8_t)SYSTEM_E_INVALID_POINTER);
    (void)IntCfgSetIdx;
  }
  else
  {
#endif /*!< (STD_ON == SYSTEM_DEV_ERROR_DETECT) */
    /* Updating configuraitons for INTC */
    f_ReturnValue_dd = System_Ipr_UpdateIntConfigSet(IntConfig, IntCfgSetIdx);
#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)        
  }    
#endif /*!< (STD_ON == SYSTEM_DEV_ERROR_DETECT) */

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ enable function
 *
 *  @details        Service ID:        0x06
 *                  SRS ID:            AR_RS_System_00005,AR_RS_System_00023
 *                                     AR_RS_System_00024,AR_RS_System_00025,AR_RS_System_00001     
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
)
{
  /* Initial return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)
  if (NULL_PTR != IntcConfig)
  {
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */
    f_ReturnValue_dd = System_Ipr_EnableIrq(IntcConfig);
#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)
  }
  else
  {
    /* Invalid pointer */
    (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                          (uint8_t)0U, \
                          (uint8_t)SYSTEM_ENABLEIRQ_ID, \
                          (uint8_t)SYSTEM_E_INVALID_POINTER);
  }
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ sign out (release) function
 *
 *  @details        Service ID:        0x07
 *                  SRS ID:            AR_RS_System_00026,AR_RS_System_00027
 *                                     AR_RS_System_00028,AR_RS_System_00001   
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
)
{
  /* Initial return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)
  if (((sint32)INT000_IRQn <= (sint32)IrqNumber) && ((sint32)IrqNumber <= (sint32)INT143_IRQn))
  {
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */    
    f_ReturnValue_dd = System_Ipr_DisableIrq(IrqNumber);
#if(STD_ON == SYSTEM_DEV_ERROR_DETECT)    
  }
  else
  {

      /* Invalid pointer */
    (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                          (uint8_t)0U, \
                          (uint8_t)SYSTEM_DISABLEIRQ_ID, \
                          (uint8_t)SYSTEM_E_INVALID_PARAMETER);
  }
#endif /* (STD_ON == SYSTEM_DEV_ERROR_DETECT) */

  return f_ReturnValue_dd;
}

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
)
{
#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)  
  if (NULL_PTR != SystemVersionInfo)
  {
#endif /* STD_ON == SYSTEM_DEV_ERROR_DETECT */    
    SystemVersionInfo->vendorID = (uint16_t) SYSTEM_VENDOR_ID;
    SystemVersionInfo->moduleID = (uint8_t) SYSTEM_MODULE_ID;
    SystemVersionInfo->sw_major_version = (uint8_t) SYSTEM_SW_MAJOR_VERSION;
    SystemVersionInfo->sw_minor_version = (uint8_t) SYSTEM_SW_MINOR_VERSION;
    SystemVersionInfo->sw_patch_version = (uint8_t) SYSTEM_SW_PATCH_VERSION;
#if (STD_ON == SYSTEM_DEV_ERROR_DETECT)    
  }
  else
  {

      (void)Det_ReportError((uint16_t)SYSTEM_MODULE_ID, \
                          (uint8_t)0U, \
                          (uint8_t)SYSTEM_VERSIONINFO_ID,
                          (uint8_t)SYSTEM_E_INVALID_POINTER);
  }
#endif /* STD_ON == SYSTEM_DEV_ERROR_DETECT */  
}
#endif /* STD_ON == SYSTEM_VERSION_INFO_API */

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
