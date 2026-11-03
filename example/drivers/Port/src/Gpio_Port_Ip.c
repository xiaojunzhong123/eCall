/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Gpio_Port_Ip.c
 *
 * Description: Implementation of GPIO IP layer
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
 *   @file       Gpio_Port_Ip.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Gpio_Port_Ip.h"
#include "SchM_Port.h"
#include "Port_VersionInfo.h"

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
#define PORT_START_SEC_CONST_UNSPECIFIED
#include "Port_MemMap.h"
static GPIO_Type * const s_GpioGeneralBase_ptr = (GPIO_Type *)CM_GPIO_BASE;
#define PORT_STOP_SEC_CONST_UNSPECIFIED
#include "Port_MemMap.h"

#define PORT_START_SEC_VAR_CLEARED_8
#include "Port_MemMap.h"
static uint8_t  s_GpioPinMode_aa[PORT_NUM_PINS];
#define PORT_STOP_SEC_VAR_CLEARED_8
#include "Port_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define PORT_START_SEC_CODE
#include "Port_MemMap.h"
/**
 *  @brief          Initializes the Port Driver module
 *
 *  @details        SRS ID:            None
 *                                     
 *                  Design ID:         Port_Gpio_Init_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            None
 *
 */
void Port_Gpio_Init(
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
)
{
  uint8_t  Index_u8;
  uint8_t  GroupIndex_u8;
  uint8_t  PinIndex_u8;
  uint16_t  pinPCR_u16;
  const Port_Gpio_PinConfigType  *PinConfig_ptr;

  /* Register write permission */
  s_GpioGeneralBase_ptr->PWPR |= (uint16_t)PORT_GPIO_PWPR_SET_VALUE;

  for (Index_u8 = 0; Index_u8 < para_GpioConfig_ptr->NumPins_u8; Index_u8++)
  {
    PinConfig_ptr = &para_GpioConfig_ptr->PortPinConfig_ptr[Index_u8];
    GroupIndex_u8 = PORT_GPIO_GROUP_INDEX((PinConfig_ptr->PinName_u8));
    PinIndex_u8 = PORT_GPIO_PIN_INDEX((PinConfig_ptr->PinName_u8));

    /* Get PCR value */
    pinPCR_u16 =  s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR;
    if(PORT_GPIO_PIN_IN == PinConfig_ptr->Direction_u8)
    {
      pinPCR_u16 &= (~(uint16_t)GPIO_PCR_POUTE);
    }
    else if((PORT_GPIO_PIN_INOUT == PinConfig_ptr->Direction_u8) || (PORT_GPIO_PIN_OUT == PinConfig_ptr->Direction_u8))
    {
      pinPCR_u16 |= (uint16_t)GPIO_PCR_POUTE;
    }
    else
    {
      /* Do nothing to avoid MISRA */
    }

    if(PORT_NJTRST_PIN == PinConfig_ptr->PinName_u8)
    {
      if(PORT_TRACE_JTAG_MODE == PinConfig_ptr->Mode_u8)
      {
        s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_4;
      }
      else
      {
        s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_4);
      }
    }
    else if(PORT_JTDI_PIN == PinConfig_ptr->PinName_u8)
    {
      if(PORT_TRACE_JTAG_MODE == PinConfig_ptr->Mode_u8)
      {
        s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_3;
      }
      else
      {
        s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_3);
      }
    }
    else if(PORT_JTDO_SWO_PIN == PinConfig_ptr->PinName_u8)
    {
      if(PORT_TRACE_JTAG_MODE == PinConfig_ptr->Mode_u8)
      {
        s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_2;
      }
      else
      {
        s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_2);
      }
    }
    else if(PORT_JTMS_SWDIO_PIN == PinConfig_ptr->PinName_u8)
    {
      if(PORT_TRACE_JTAG_MODE == PinConfig_ptr->Mode_u8)
      {
        s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_1;
      }
      else
      {
        s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_1);
      }
    }
    else if(PORT_JTCK_SWCLK_PIN == PinConfig_ptr->PinName_u8)
    {
      if(PORT_TRACE_JTAG_MODE == PinConfig_ptr->Mode_u8)
      {
        s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_0;
      }
      else
      {
        s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_0);
      }
    }
    else
    {
      /* Do nothing to avoid MISRA */
    }

    if(PORT_ANALOG_MODE == PinConfig_ptr->Mode_u8)
    {
      /* Disable digital function */
      pinPCR_u16 |= (uint16_t)GPIO_PCR_DDIS;

    }
    else
    {
      /* Enable digital function */
      pinPCR_u16 &= (~(uint16_t)GPIO_PCR_DDIS);

      if(PORT_EIRQ_WKUP_MODE == PinConfig_ptr->Mode_u8)
      {
        /* Enable external interrupt */
        pinPCR_u16 |= (uint16_t)GPIO_PCR_INTE;
      }
      else if (PORT_TRACE_JTAG_MODE != PinConfig_ptr->Mode_u8)
      {
        /* Initialize the Port Pin Mode */
        s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR &= (~(uint16_t)GPIO_PFSR_FSEL);
        s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR |= (((uint16_t)PinConfig_ptr->Mode_u8 << GPIO_PFSR_FSEL_POS) & (uint16_t)GPIO_PFSR_FSEL);
      }
      else
      {
        /* Do nothing to avoid MISRA */
      }

      /* Disable output state latch */
      pinPCR_u16 = pinPCR_u16 & (~(uint16_t)GPIO_PCR_LTE);

      /* Enable CMOS for group if PORT_GPIO_PIN_CMOS_INPUT_MODE */
      if(PORT_GPIO_PIN_CMOS_INPUT_MODE == PinConfig_ptr->InputMode_u8)
      {
        s_GpioGeneralBase_ptr->PINAER |= (((uint16_t)1U << GroupIndex_u8) & (uint16_t)GPIO_PINAER_PINAE);
      }

      /* CMOS/Schmitt input mode */
      pinPCR_u16 = ((PORT_GPIO_PIN_CMOS_INPUT_MODE == PinConfig_ptr->InputMode_u8)?(pinPCR_u16 | (uint16_t)GPIO_PCR_CINSEL):(pinPCR_u16 & ((uint16_t)(~(uint16_t)GPIO_PCR_CINSEL))));

      /* Enable/disable pull-up */
      pinPCR_u16 = ((PinConfig_ptr->PullUp_bool == (boolean)TRUE)?(pinPCR_u16 | (uint16_t)GPIO_PCR_PUU):(pinPCR_u16 & ((uint16_t)(~(uint16_t)GPIO_PCR_PUU))));

      /* Select driving force mode */
      pinPCR_u16 &= (~(uint16_t)GPIO_PCR_DRV);
      pinPCR_u16 |= (((uint16_t)PinConfig_ptr->DrivingMode_u8 << GPIO_PCR_DRV_POS) & (uint16_t)GPIO_PCR_DRV);

      /* Enable/disable open-drain */
      pinPCR_u16 = ((PinConfig_ptr->OpenDrain_bool == (boolean)TRUE)?(pinPCR_u16 | (uint16_t)GPIO_PCR_NOD):(pinPCR_u16 & (~(uint16_t)GPIO_PCR_NOD)));

      /* Set pin level value */
      pinPCR_u16 = (PORT_PIN_LEVEL_HIGH == PinConfig_ptr->LevelValue_u8)?(pinPCR_u16 | (uint16_t)GPIO_PCR_POUT):(pinPCR_u16 & (~(uint16_t)GPIO_PCR_POUT));
    }

    /* Set value for PCR register */
    s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR = pinPCR_u16;

    /* Save the current pin MODE */
    s_GpioPinMode_aa[PinConfig_ptr->GpioNumber_u8 - 1U] = PinConfig_ptr->Mode_u8;

    if((boolean)TRUE == PinConfig_ptr->DualMode_bool)
    {
      /* Set sub-function */
      s_GpioGeneralBase_ptr->PCCR |= (((uint16_t)PinConfig_ptr->SubMode_u8 << GPIO_PCCR_BFSEL_POS) & (uint16_t)GPIO_PCCR_BFSEL);
      /* Enable sub-function */
      s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR |= (uint16_t)GPIO_PFSR_BFE;
    }
    else
    {
      /* Disable sub-function */
      s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR &= (~(uint16_t)GPIO_PFSR_BFE);
    }
  }
}

#if (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)
/**
 *  @brief          Set Pin Direction for Port Pin
 *
 *  @details        SRS ID:            None
 *                                     
 *                  Design ID:         Port_Gpio_SetPinDirection_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_Pin_u8            Pin index
 *  @param[in]      para_Direction_u8      Direction want to change
 *  @param[in]      para_GpioConfig_ptr    Specifies the pointer to the configuration set
 *
 *  @return         uint8_t
 *  @retval         PORT_GPIO_PARAM_INVALID_DIRECTION   Set Pin Direction Fail cause Expected Direction was wrong
 *  @retval         PORT_GPIO_DIRECTION_UNCHANGEABLE    Set Pin Direction Fail cause Direction Can't be changed
 *  @retval         PORT_GPIO_PROCESS_SUCCESS           Set Pin Direction Success

 * 
 *  @note           None
 *
 *  @pre            None
 *
 */
uint8_t Port_Gpio_SetPinDirection
(
  uint8_t  para_Pin_u8,
  uint8_t  para_Direction_u8,
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
)
{
  uint8_t  Index_u8;
  uint8_t  GroupIndex_u8;
  uint8_t  PinIndex_u8;
  uint8_t  result_u8 = (uint8_t)PORT_GPIO_PARAM_PIN;
  const Port_Gpio_PinConfigType  *PinConfig_ptr;

  SchM_Enter_Port_PORT_EXCLUSIVE_AREA_00();
  for (Index_u8 = 0; Index_u8 < para_GpioConfig_ptr->NumPins_u8; Index_u8++)
  {
    PinConfig_ptr= &para_GpioConfig_ptr->PortPinConfig_ptr[Index_u8];
    if(PinConfig_ptr->GpioNumber_u8 == para_Pin_u8)
    {
      result_u8 = (uint8_t)PORT_GPIO_DIRECTION_UNCHANGEABLE;
      if((boolean)TRUE == PinConfig_ptr->DirectionChangeable_bool)
      {
        result_u8 = (uint8_t)PORT_GPIO_PARAM_INVALID_DIRECTION;
        GroupIndex_u8 = PORT_GPIO_GROUP_INDEX((PinConfig_ptr->PinName_u8));
        PinIndex_u8 = PORT_GPIO_PIN_INDEX((PinConfig_ptr->PinName_u8));
        /* Set direction for PORT_GPIO_MODE */
        if(PORT_GPIO_MODE == s_GpioPinMode_aa[PinConfig_ptr->GpioNumber_u8 - 1U])
        {
          if(PORT_GPIO_PIN_IN == para_Direction_u8)
          {
            s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR &= (~(uint16_t)GPIO_PCR_POUTE);
            result_u8 = (uint8_t)PORT_GPIO_PROCESS_SUCCESS;
          }
          else if((PORT_GPIO_PIN_INOUT == para_Direction_u8) || (PORT_GPIO_PIN_OUT == para_Direction_u8))
          {
            s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR |= (uint16_t)GPIO_PCR_POUTE;
            result_u8 = (uint8_t)PORT_GPIO_PROCESS_SUCCESS;
          }
          else
          {
            /* Do nothing to avoid MISRA */
          }
        }
      }
      break;
    }
  }
  SchM_Exit_Port_PORT_EXCLUSIVE_AREA_00();
  return result_u8;
}
#endif /*!< End of (STD_ON == PORT_SET_PIN_DIRECTION_API_ENABLE)*/

#if (STD_ON == PORT_SET_PIN_MODE_API_ENABLE)
/**
 *  @brief          Set Pin Mode for Port Pin.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Port_Gpio_SetPinMode_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @param[in]      para_PinId_u8    Pin index
 *  @param[in]      para_Mode_u32    Port pin mode
 *
 *  @return         uint8_t
 *  @retval         PORT_GPIO_MODE_UNCHANGEABLE       Set Pin Mode Fail cause Mode Can't be changed
 *  @retval         PORT_GPIO_IP_PROCESS_SUCCESS      Set Pin Mode Success
 *
 *  @note           None
 *
 *  @pre            Driver must be initialized before
 *
 */
uint8_t Port_Gpio_SetPinMode
(
  uint8_t  para_Pin_u8,
  uint8_t  para_Mode_u8,
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
)
{
  uint8_t  Index_u8;
  uint8_t  GroupIndex_u8;
  uint8_t  PinIndex_u8;
  uint16_t  pinPCR_u16;
  uint8_t  result_u8 = (uint8_t)PORT_GPIO_PARAM_PIN;
  const Port_Gpio_PinConfigType  *PinConfig_ptr;

  SchM_Enter_Port_PORT_EXCLUSIVE_AREA_01();
  for (Index_u8 = 0; Index_u8 < para_GpioConfig_ptr->NumPins_u8; Index_u8++)
  {
    PinConfig_ptr= &para_GpioConfig_ptr->PortPinConfig_ptr[Index_u8];
    if(PinConfig_ptr->GpioNumber_u8 == para_Pin_u8)
    {
      result_u8 = (uint8_t)PORT_GPIO_MODE_UNCHANGEABLE;
      if((boolean)TRUE == PinConfig_ptr->ModeChangeable_bool)
      {
          GroupIndex_u8 = PORT_GPIO_GROUP_INDEX((PinConfig_ptr->PinName_u8));
          PinIndex_u8 = PORT_GPIO_PIN_INDEX((PinConfig_ptr->PinName_u8));
          if((boolean)TRUE == PinConfig_ptr->OutputLatch_bool)
          {
            /* Enable output state latch */
            s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR |= (uint16_t)GPIO_PCR_LTE;
          }
          /* Get PCR register */
          pinPCR_u16 = s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR;

          if(PORT_NJTRST_PIN == PinConfig_ptr->PinName_u8)
          {
            if(PORT_TRACE_JTAG_MODE == para_Mode_u8)
            {
              s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_4;
            }
            else
            {
              s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_4);
            }
          }
          else if(PORT_JTDI_PIN == PinConfig_ptr->PinName_u8)
          {
            if(PORT_TRACE_JTAG_MODE == para_Mode_u8)
            {
              s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_3;
            }
            else
            {
              s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_3);
            }
          }
          else if(PORT_JTDO_SWO_PIN == PinConfig_ptr->PinName_u8)
          {
            if(PORT_TRACE_JTAG_MODE == para_Mode_u8)
            {
              s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_2;
            }
            else
            {
              s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_2);
            }
          }
          else if(PORT_JTMS_SWDIO_PIN == PinConfig_ptr->PinName_u8)
          {
            if(PORT_TRACE_JTAG_MODE == para_Mode_u8)
            {
              s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_1;
            }
            else
            {
              s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_1);
            }
          }
          else if(PORT_JTCK_SWCLK_PIN == PinConfig_ptr->PinName_u8)
          {
            if(PORT_TRACE_JTAG_MODE == para_Mode_u8)
            {
              s_GpioGeneralBase_ptr->PSPCR |= (uint16_t)GPIO_PSPCR_SPFE_0;
            }
            else
            {
              s_GpioGeneralBase_ptr->PSPCR &= (~(uint16_t)GPIO_PSPCR_SPFE_0);
            }
          }
          else
          {
            /* Do nothing to avoid MISRA */
          }

          if(PORT_ANALOG_MODE == para_Mode_u8)
          {
            /* Disable digital function */
            pinPCR_u16 |= (uint16_t)GPIO_PCR_DDIS;

          }
          else if (PORT_TRACE_JTAG_MODE != para_Mode_u8)
          {
            /* Enable digital function */
            pinPCR_u16 &= (~(uint16_t)GPIO_PCR_DDIS);

            if(PORT_EIRQ_WKUP_MODE == para_Mode_u8)
            {
              /* Enable external interrupt */
              pinPCR_u16 |= (uint16_t)GPIO_PCR_INTE;
              /* Reset Port Pin Mode */
              s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR &= (~(uint16_t)GPIO_PFSR_FSEL);
            }
            else
            {
              /* Initialize the Port Pin Mode */
              s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR &= (~(uint16_t)GPIO_PFSR_FSEL);
              s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PFSR |= (((uint16_t)para_Mode_u8 << GPIO_PFSR_FSEL_POS) & (uint16_t)GPIO_PFSR_FSEL);
            }
          }
          else
          {
            /* Do nothing to avoid MISRA */
          }

          /* Set value for PCR register */
          s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR = pinPCR_u16;

          /* Save the current pin MODE */
          s_GpioPinMode_aa[PinConfig_ptr->GpioNumber_u8 - 1U] = para_Mode_u8;

          /* Disable output state latch */
          s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR &= (~(uint16_t)GPIO_PCR_LTE);

          result_u8 = (uint8_t)PORT_GPIO_PROCESS_SUCCESS;
      }
      break;
    }
  }
  SchM_Exit_Port_PORT_EXCLUSIVE_AREA_01();
  return result_u8;
}
#endif /*!< End of (STD_ON == PORT_SET_PIN_MODE_API_ENABLE) */

/**
 *  @brief          Refresh Port Direction.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Port_Gpio_RefreshPortDirection_Activity
 *                  Sync/Async:        None
 *                  Reentrancy:        None
 *
 *  @return         void
 *
 *  @note           None
 *
 *  @pre            Driver must be initialized before
 *
 */
void Port_Gpio_RefreshPortDirection
(
  const Port_Gpio_ConfigType * para_GpioConfig_ptr
)
{
  uint8_t  Index_u8;
  uint8_t  GroupIndex_u8;
  uint8_t  PinIndex_u8;
  const Port_Gpio_PinConfigType  *PinConfig_ptr;

  SchM_Enter_Port_PORT_EXCLUSIVE_AREA_02();
  for (Index_u8 = 0; Index_u8 < para_GpioConfig_ptr->NumPins_u8; Index_u8++)
  {
    PinConfig_ptr = &para_GpioConfig_ptr->PortPinConfig_ptr[Index_u8];
    if((boolean)FALSE == PinConfig_ptr->DirectionChangeable_bool)
    {
        if((s_GpioPinMode_aa[PinConfig_ptr->GpioNumber_u8 - 1U] == PinConfig_ptr->Mode_u8) && (PORT_GPIO_MODE == PinConfig_ptr->Mode_u8))
        {
          GroupIndex_u8 = PORT_GPIO_GROUP_INDEX((PinConfig_ptr->PinName_u8));
          PinIndex_u8 = PORT_GPIO_PIN_INDEX((PinConfig_ptr->PinName_u8));
          /* Set direction for PORT_GPIO_MODE */
          if(PORT_GPIO_PIN_IN == PinConfig_ptr->Direction_u8)
          {
            s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR &= (~(uint16_t)GPIO_PCR_POUTE);
          }
          else if((PORT_GPIO_PIN_INOUT == PinConfig_ptr->Direction_u8) || (PORT_GPIO_PIN_OUT == PinConfig_ptr->Direction_u8))
          {
            s_GpioGeneralBase_ptr->PIN[GroupIndex_u8][PinIndex_u8].PCR |= (uint16_t)GPIO_PCR_POUTE;
          }
          else
          {
            /* Do nothing to avoid MISRA */
          }
        }
    }

  }
  SchM_Exit_Port_PORT_EXCLUSIVE_AREA_02();
}
#define PORT_STOP_SEC_CODE
#include "Port_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
