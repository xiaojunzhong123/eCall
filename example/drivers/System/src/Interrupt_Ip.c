/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Interrupt_Ip.c
 *
 * Description: Implementation of INTERRUPT IP layer
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
 *   @file       Interrupt_Ip.c
 *   @version    V1.0.3
 *   @brief      System Driver
 *   @addtogroup system_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif
/***********************************************************************************************************************
 *  MISRA VIOLATIONS - Summary and analysis.
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Interrupt_Ip.h"

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

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define SYSTEM_START_SEC_CODE
#include "System_MemMap.h"

static inline void NVIC_EnableIRQ(IRQn_Type  IRQn);
static inline void NVIC_DisableIRQ(IRQn_Type  IRQn);
static inline void NVIC_ClearPendingIRQ(IRQn_Type  IRQn);
static inline void NVIC_SetPriority(IRQn_Type  IRQn, System_IntPriorityType  priority);
static inline uint32_t Scb_GetVtor(void);

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Enable Interrupt
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_para_para_para_para_para_para_para_IRQn    Device specific interrupt number.
 *
 *  @return         void
 *
 *  @note           para_para_para_para_para_para_para_para_IRQn must not be negative.
 *
 *  @pre            None
 *
 */
static inline void NVIC_EnableIRQ(IRQn_Type  IRQn)
{
  NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
}

/**
 *  @brief          Disable Interrupt
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_para_para_para_para_para_para_para_IRQn    Device specific interrupt number.
 *
 *  @return         void
 *
 *  @note           para_para_para_para_para_para_para_para_IRQn must not be negative.
 *
 *  @pre            None
 *
 */
static inline void NVIC_DisableIRQ(IRQn_Type  IRQn)
{
  NVIC->ICER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
}

/**
 *  @brief          Clear Pending Interrupt
 *                  Clears the pending bit of a device specific interrupt in the NVIC pending register.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_para_para_para_para_para_para_para_IRQn    Device specific interrupt number.
 *
 *  @return         void
 *
 *  @note           para_para_para_para_para_para_para_para_IRQn must not be negative.
 *
 *  @pre            None
 *
 */
static inline void NVIC_ClearPendingIRQ(IRQn_Type  IRQn)
{
  NVIC->ICPR[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
}

/**
 *  @brief          Set Interrupt Priority
 *                  Sets the para_para_priority of a device specific interrupt or a processor exception.
 *                  The interrupt number can be positive to specify a device specific interrupt,
 *                  or negative to specify a processor exception.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_para_para_para_para_para_para_para_IRQn    Interrupt number.
 *  @param[in]      para_para_priority                              Priority to set.
 *
 *  @return         void
 *
 *  @note           The para_para_priority cannot be set for every processor exception.
 *
 *  @pre            None
 *
 */
static inline void NVIC_SetPriority(IRQn_Type  IRQn, System_IntPriorityType  priority)
{
  NVIC->IP[((uint32_t)IRQn)] = (uint8_t)(((uint32_t)priority << (8U - (uint8_t)NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
}

/**
 *  @brief          Get vector table
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         uint32_t    Vector table address.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
static inline uint32_t Scb_GetVtor(
    void
)
{
  return ((uint32_t)(SCB->VTOR));
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
/**
 *  @brief          Initialize intc module via configurations.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr    Intc configurations.
 *
 *  @return         Std_ReturnType    Initialize interrupt controller.
 *  @retval         E_OK              Interrupt initialized successfully.
 *  @retval         E_NOT_OK          Interrupt initialized failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_Init(
  const System_IntConfigSetType * para_IntConfig_ptr
)
{
  Std_ReturnType f_ReturnValue_dd = E_OK;

  if (NULL_PTR != para_IntConfig_ptr)
  {
    f_ReturnValue_dd = Int_Ip_UpdateINTCConfig(para_IntConfig_ptr, 0U);
  }
  else
  {
    f_ReturnValue_dd = E_NOT_OK;
  }

  return f_ReturnValue_dd;
}

/**
 *  @brief          Update intc configuration set.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr      Intc configurations.
 *  @param[in]      para_IntCfgSetIdx_u8    Interrupt setting ID from config structure to be used.
 *
 *  @return         Std_ReturnType    Update interrupt configuration set.
 *  @retval         E_OK              Interrupt updated successfully.
 *  @retval         E_NOT_OK          Interrupt updated failed.
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
Std_ReturnType Int_Ip_UpdateINTCConfig(
  const System_IntConfigSetType *para_IntConfig_ptr,
  uint32_t para_IntCfgSetIdx_u8
)
{
  Std_ReturnType f_ReturnValue_dd = E_OK;
  uint8_t Index_u8       = 0U;

  for (Index_u8 = 0U; Index_u8 < para_IntConfig_ptr[para_IntCfgSetIdx_u8].SystemIntNumberOfIntcConfig_u8; Index_u8++)
  {
    f_ReturnValue_dd |= Int_Ip_EnableIRQ(&(para_IntConfig_ptr[para_IntCfgSetIdx_u8].SystemIntListConfig_ptr[Index_u8]));
  }

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ enable function
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IntConfig_ptr    Intc configurations.
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
Std_ReturnType Int_Ip_EnableIRQ(
  const System_IntConfigType *para_IntConfig_ptr
)
{
  volatile uint32_t * INTC_SELx;
  volatile func_ptr_t *VectorRam_pfn = (volatile func_ptr_t*)(Scb_GetVtor());
  Std_ReturnType   f_ReturnValue_dd = E_OK;

  if (NULL_PTR != para_IntConfig_ptr)
  {
    /* Check if IntSource out of range */
    if (((uint16_t)INTC_SRC_MAX <= (uint16_t)(para_IntConfig_ptr->SystemIntReqSource_en)) || ((uint16_t)MAX_IRQn <= (uint16_t)para_IntConfig_ptr->SystemIntIrqNumber_en) || \
        ((sint16)INT000_IRQn > (sint16)para_IntConfig_ptr->SystemIntIrqNumber_en) || ((uint16_t)PRIORITY_MAX <= (uint16_t)para_IntConfig_ptr->SystemIntPriority_en)  || \
        (NULL_PTR == para_IntConfig_ptr->SystemIntIrqHandler_dd))
    {
      f_ReturnValue_dd = E_NOT_OK;
    }
    else
    {
      INTC_SELx = (volatile uint32_t *)(INTSEL_REG + (0x4UL * (uint32_t)(para_IntConfig_ptr->SystemIntIrqNumber_en)));
      if (INTSEL_RST_VALUE == ((*INTC_SELx) & INTSEL_RST_VALUE))
      {
        *INTC_SELx = (uint32_t)para_IntConfig_ptr->SystemIntReqSource_en;
        VectorRam_pfn[(sint32)(para_IntConfig_ptr->SystemIntIrqNumber_en)+16] = para_IntConfig_ptr->SystemIntIrqHandler_dd;
      }
      else if ((uint32_t)(para_IntConfig_ptr->SystemIntReqSource_en) == ((*INTC_SELx) & INTSEL_RST_VALUE))
      {
        VectorRam_pfn[(sint32)(para_IntConfig_ptr->SystemIntIrqNumber_en)+16] = para_IntConfig_ptr->SystemIntIrqHandler_dd;
      }
      else
      {
        f_ReturnValue_dd = E_NOT_OK;
      }
    }

    /* Clear pending */
    NVIC_ClearPendingIRQ(para_IntConfig_ptr->SystemIntIrqNumber_en);
    /* Set priority */
    NVIC_SetPriority(para_IntConfig_ptr->SystemIntIrqNumber_en, para_IntConfig_ptr->SystemIntPriority_en);
    /* Enable NVIC */
    NVIC_EnableIRQ(para_IntConfig_ptr->SystemIntIrqNumber_en);
  }
  else
  {
    f_ReturnValue_dd = E_NOT_OK;
  }

  return f_ReturnValue_dd;
}

/**
 *  @brief          IRQ sign out (release) function
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         None
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_IrqNumber_en    can be any value from INT000_IRQn ~  for different product
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
Std_ReturnType Int_Ip_DisableIRQ(
  IRQn_Type  para_IrqNumber_en
)
{
  volatile uint32_t * INTC_SELx;
  Std_ReturnType  f_ReturnValue_dd = E_OK;

  INTC_SELx = (volatile uint32_t *)(INTSEL_REG + (0x4UL * (uint32_t)para_IrqNumber_en));
  WRITE_REG32(*INTC_SELx, INTSEL_RST_VALUE);
  NVIC_DisableIRQ(para_IrqNumber_en);

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
