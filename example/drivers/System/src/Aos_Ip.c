/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Aos_Ip.c
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
 *   @file       Aos_Ip.c
 *   @version    V1.0.3
 *   @brief      System Driver
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
#include "Aos_Ip.h"
/***********************************************************************************************************************
 *   SOURCE FILE VERSION INFORMATION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * VERSION CHECKS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/

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

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static const System_AosConfigSetType * l_SystemAOSConfig_ptr;

#define SYSTEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static void Aos_Ip_ImplementAOSTriggerTarget(
    const System_AosTriggerTargetConfigType * para_AOSTriggerTarget_ptr
);
/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
 /**
 *  @brief          This function implements AOS trigger target configuration.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_AOSTriggerTarget_ptr    Pointer to AOS trigger target configuration.
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static void Aos_Ip_ImplementAOSTriggerTarget(
    const System_AosTriggerTargetConfigType * para_AOSTriggerTarget_ptr
)
{
  /* Initialize return value */
  uint32_t  f_RegisterValue_u32 = 0U;
  uint32_t  f_RegisterAddr_u32 = (uint32_t)(CM_AOS);

  /*Get Target selection Register base on user configuration */
  f_RegisterAddr_u32 = ((uint32_t)f_RegisterAddr_u32 + (uint32_t)(para_AOSTriggerTarget_ptr->SystemAOSTrigerSel_u32));
  /*Calculate register value */
  f_RegisterValue_u32 = f_RegisterValue_u32 | ((uint32_t)(para_AOSTriggerTarget_ptr->SystemAosCommTrigger1Status_u8) << AOS_TRIGGER_TRGSEL_COMEN_0_POS);
  f_RegisterValue_u32 = f_RegisterValue_u32 | ((uint32_t)(para_AOSTriggerTarget_ptr->SystemAosCommTrigger2Status_u8) << AOS_TRIGGER_TRGSEL_COMEN_1_POS);
  f_RegisterValue_u32 = f_RegisterValue_u32 | (uint32_t)(para_AOSTriggerTarget_ptr->SystemAosEventSrc_en);

  /* Write configuration calculated into register */
  *(uint32_t*)f_RegisterAddr_u32 = f_RegisterValue_u32;
}

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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;  

  if(NULL_PTR != para_AOS_ptr)
  {
    l_SystemAOSConfig_ptr = para_AOS_ptr;
    f_ReturnValue_dd = Aos_Ip_UpdateAOSConfigSet(0U);
  }
  else
  {
    f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK; 
  }
  
  return f_ReturnValue_dd;
}

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
)
{
  /*Generate Software Trigger */
  CM_AOS->INTSFTTRG = 0x01;
}

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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;
  en_event_src_t  f_CommonSrcTrigger1_en;
  en_event_src_t  f_CommonSrcTrigger2_en;
  uint8_t  f_Loop_u8 = 0U;

  /*Reset all register*/
  Aos_Ip_ResetAllAosTriggerTarget();

  if(NULL_PTR != l_SystemAOSConfig_ptr)
  {
    /*Initialize Common Trigger Source 1 */
    f_CommonSrcTrigger1_en = l_SystemAOSConfig_ptr[para_AOSConfigSetIndex_u32].SystemAosCommonTrigger1_en;
    CM_AOS->COMTRG1 = (uint32_t)f_CommonSrcTrigger1_en;

    /*Initialize Common Trigger Source 2 */
    f_CommonSrcTrigger2_en = l_SystemAOSConfig_ptr[para_AOSConfigSetIndex_u32].SystemAosCommonTrigger2_en;
    CM_AOS->COMTRG2 = (uint32_t)f_CommonSrcTrigger2_en;

    /*Config all Tringger target configured in AOS ConfigSet*/
    for(f_Loop_u8 = 0U; f_Loop_u8 < l_SystemAOSConfig_ptr[para_AOSConfigSetIndex_u32].SystemAosNumberOfAosTargetConfig_u8; f_Loop_u8++)
    {
      Aos_Ip_ImplementAOSTriggerTarget(&(l_SystemAOSConfig_ptr[para_AOSConfigSetIndex_u32].SystemAosTriggerTargetConfigList_ptr[f_Loop_u8]));
    }
    /*Return status succesfully */
    f_ReturnValue_dd = (Std_ReturnType)E_OK;    
  }

  return f_ReturnValue_dd;
}

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
)
{
  uint8_t  f_Loop_u8 = 0U;
  uint32_t  f_RegisterAddr_u32 = (uint32_t)(CM_AOS);

  for(f_Loop_u8 = 0U; f_Loop_u8 < SYSTEM_AOS_TOTAL_TRIGGER_TARGET_REGISTER; f_Loop_u8++)
  {
    /*Get Register address */
    f_RegisterAddr_u32 = (uint32_t)(f_RegisterAddr_u32 + 0x04UL);
    /*Reset default value of register */
    *(uint32_t*)f_RegisterAddr_u32 = 0x1FF;
  }
}

#define SYSTEM_STOP_SEC_CODE
#include "System_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
