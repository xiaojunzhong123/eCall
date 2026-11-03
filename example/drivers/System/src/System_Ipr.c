/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: System_Ipr.c
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
 *   @file       System_Ipr.c
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
#include "System_Ipr.h"

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
#define SYSTEM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

static uint32_t l_SystemIntNumberOfIntcConfigSet_u32;

#define SYSTEM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "System_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_OK;
  
  l_SystemIntNumberOfIntcConfigSet_u32 = para_ConfigPtr_ptr->SystemIntNumberOfIntcConfigSet_u32;
  /* Initialize AOS */
  f_ReturnValue_dd |= Aos_Ip_Init(para_ConfigPtr_ptr->SystemAosConfigSet_ptr);
  /* Initialize Interrupt */
  f_ReturnValue_dd |= Int_Ip_Init(para_ConfigPtr_ptr->SystemIntConfigSet_ptr); 

  return f_ReturnValue_dd;
}

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
)
{
  /*Generate Software Trigger */
  Aos_Ip_GenerateSwTrigger();
}
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

  f_ReturnValue_dd = Aos_Ip_UpdateAOSConfigSet(para_AosConfigSetIdx_u32);

  return f_ReturnValue_dd;
}

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
)
{
  Aos_Ip_ResetAllAosTriggerTarget();
}
#endif

/**
 *  @brief          System driver INTC Implementation function.
 *                  This function intializes the INTC configuration.
 *  @details        Design ID:         None
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
)
{
  /* Initialize return value */
  Std_ReturnType  f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;

  /* The update index of configurations set should be smaller than the number of INTC configuration sets  */
  if (l_SystemIntNumberOfIntcConfigSet_u32 > para_IntCfgSetIdx_u8)
  {
    f_ReturnValue_dd = Int_Ip_UpdateINTCConfig(para_ConfigPtr_ptr, para_IntCfgSetIdx_u8);
  }
  else
  {
    f_ReturnValue_dd = (Std_ReturnType)E_NOT_OK;
  }

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ enable function
 *  @details        SRS ID:            None 
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
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
)
{
  /* Initial Return value */
  Std_ReturnType  f_ReturnValue_dd;

  f_ReturnValue_dd = Int_Ip_EnableIRQ(para_IntcConfig_ptr);

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ sign out (release) function
 *  @details        Design ID:         None
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
)
{
  /* Initial Return value */
  Std_ReturnType  f_ReturnValue_dd;

  f_ReturnValue_dd = Int_Ip_DisableIRQ(para_IrqNumber_en);

  return f_ReturnValue_dd;
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
