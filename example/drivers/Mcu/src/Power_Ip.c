/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip.c
 *
 * Description: Implementation of POWER IP layer
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
 *   @file       Power_Ip.c
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
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
#include "Power_Ip.h"
#include "Power_Ip_Mode.h"
#include "Power_Ip_Pvd.h"
#include "Clock_Ip.h"
#include "Power_Ip_Types.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
#define PWC_WRITE_ENABLE                (0xA500U)
#define PWC_WKT_OFF                     (0x00U)
#define PWC_WKT_ON                      (PWC_WKTC2_WKTCE)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"
static void Power_Ip_SwitchClkToHighSpeed(
    const Clock_Ip_HsrunConfigType * para_ClockHsrunConfig_ptr
);

static void Power_Ip_SwitchClkToUltraLowSpeed(
    const Clock_Ip_UlsrunConfigType * para_ClockUlsrunConfig_ptr
);

static void Power_Ip_DisablePeripheral(
    const Power_Ip_ConfigPeripheralType * para_PowerPeripheral_ptr
);
#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  LOCAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"
/**
 *  @brief          This function will make the clock change to High Speed
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_SwitchClkToHighSpeed_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @return         void
 *
 *  @note           This function will be called by Power_Ip_SetMode().
 *
 *  @pre            None
 *
 */
static void Power_Ip_SwitchClkToHighSpeed(
    const Clock_Ip_HsrunConfigType * para_ClockHsrunConfig_ptr
)
{
  /* System clock divider config for HCLK */
  Clock_Ip_SetSysClockDiv(CLK_BUS_HCLK,  para_ClockHsrunConfig_ptr->HCLKClockDiv_u32);
  /* System clock divider config for EXCLK */
  Clock_Ip_SetSysClockDiv(CLK_BUS_EXCLK, para_ClockHsrunConfig_ptr->EXCLKClockDiv_u32);
  /* System clock divider config for PCLK0 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK0, para_ClockHsrunConfig_ptr->PCLK0ClockDiv_u32);
  /* System clock divider config for PCLK1 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK1, para_ClockHsrunConfig_ptr->PCLK1ClockDiv_u32);
  /* System clock divider config for PCLK2 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK2, para_ClockHsrunConfig_ptr->PCLK2ClockDiv_u32);
  /* System clock divider config for PCLK3 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK3, para_ClockHsrunConfig_ptr->PCLK3ClockDiv_u32);
  /* System clock divider config for PCLK4 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK4, para_ClockHsrunConfig_ptr->PCLK4ClockDiv_u32);
}

/**
 *  @brief          This function will make the clock change to Ultra Low Speed
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_SwitchClkToUltraLowSpeed_Activity
 *                  Sync/Async:        Synchronous
 *
 *  @return         void
 *
 *  @note           This function will be called by Power_Ip_SetMode().
 *
 *  @pre            None
 *
 */
static void Power_Ip_SwitchClkToUltraLowSpeed(
    const Clock_Ip_UlsrunConfigType * para_ClockUlsrunConfig_ptr
)
{
  /* System clock divider config for HCLK */
  Clock_Ip_SetSysClockDiv(CLK_BUS_HCLK,  para_ClockUlsrunConfig_ptr->HCLKClockDiv_u32);
  /* System clock divider config for EXCLK */
  Clock_Ip_SetSysClockDiv(CLK_BUS_EXCLK, para_ClockUlsrunConfig_ptr->EXCLKClockDiv_u32);
  /* System clock divider config for PCLK0 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK0, para_ClockUlsrunConfig_ptr->PCLK0ClockDiv_u32);
  /* System clock divider config for PCLK1 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK1, para_ClockUlsrunConfig_ptr->PCLK1ClockDiv_u32);
  /* System clock divider config for PCLK2 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK2, para_ClockUlsrunConfig_ptr->PCLK2ClockDiv_u32);
  /* System clock divider config for PCLK3 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK3, para_ClockUlsrunConfig_ptr->PCLK3ClockDiv_u32);
  /* System clock divider config for PCLK4 */
  Clock_Ip_SetSysClockDiv(CLK_BUS_PCLK4, para_ClockUlsrunConfig_ptr->PCLK4ClockDiv_u32);
}

/**
 *  @brief          This function will disable peripheral clocks and power down unused peripherals
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_DisablePeripheral_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_PowerPeripheral_ptr    Pointer to peripheral configuration structure
 *
 *  @return         void
 *
 *  @note           This function will be called by Power_Ip_SetMode().
 *
 *  @pre            None
 *
 */
static void Power_Ip_DisablePeripheral(
    const Power_Ip_ConfigPeripheralType * para_PowerPeripheral_ptr
)
{
  uint32_t  f_FCG0_u32         = 0x00U;
  uint32_t  f_FCG1_u32         = 0x00U;
  uint32_t  f_FCG2_u32         = 0x00U;
  uint32_t  f_FCG3_u32         = 0x00U;
  uint32_t  f_RAMPC0_u32       = 0x00U;
  uint32_t  f_PRAMLPC_u32      = 0x00U;
  uint8_t  f_SysClockSource_u8 = 0x00U;
  uint8_t  f_PLLClockSource_u8 = 0x00U;

  /* Stop functional clock */
  f_FCG0_u32 = (((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAMH   << PWC_FCG0_SRAMH_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAM1   << PWC_FCG0_SRAM1_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAM2   << PWC_FCG0_SRAM2_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAM3   << PWC_FCG0_SRAM3_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAM4   << PWC_FCG0_SRAM4_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SRAMB   << PWC_FCG0_SRAMB_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->KEY     << PWC_FCG0_KEY_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DMA1    << PWC_FCG0_DMA1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DMA2    << PWC_FCG0_DMA2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->FCM     << PWC_FCG0_FCM_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->AOS     << PWC_FCG0_AOS_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->CTC     << PWC_FCG0_CTC_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->MAU     << PWC_FCG0_MAU_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->HASH    << PWC_FCG0_HASH_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->TRNG    << PWC_FCG0_TRNG_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->CRC     << PWC_FCG0_CRC_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU1    << PWC_FCG0_DCU1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU2    << PWC_FCG0_DCU2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU3    << PWC_FCG0_DCU3_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU4    << PWC_FCG0_DCU4_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU5    << PWC_FCG0_DCU5_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU6    << PWC_FCG0_DCU6_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU7    << PWC_FCG0_DCU7_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->DCU8    << PWC_FCG0_DCU8_POS));

#ifdef PWC_FCG0_ERMU_POS
  f_FCG0_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->ERMU << PWC_FCG0_ERMU_POS);
#endif

#ifdef PWC_FCG0_PKE_POS
  f_FCG0_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->PKE << PWC_FCG0_PKE_POS);
#endif

#ifdef PWC_FCG0_SKE_POS
  f_FCG0_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SKE_AES << PWC_FCG0_SKE_POS);
#endif

#ifdef PWC_FCG0_AES_POS
  f_FCG0_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG0_ptr)->SKE_AES << PWC_FCG0_AES_POS);
#endif

  MODIFY_REG32(CM_PWC->FCG0, 0xFFFFFFFFU, f_FCG0_u32);

  f_FCG1_u32 = (((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->CAN1   << PWC_FCG1_CAN1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->CAN2   << PWC_FCG1_CAN2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->ETHMAC << PWC_FCG1_ETHMAC_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->QSPI   << PWC_FCG1_QSPI_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C1   << PWC_FCG1_I2C1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C2   << PWC_FCG1_I2C2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C3   << PWC_FCG1_I2C3_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C4   << PWC_FCG1_I2C4_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C5   << PWC_FCG1_I2C5_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2C6   << PWC_FCG1_I2C6_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SDIOC1 << PWC_FCG1_SDIOC1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SDIOC2 << PWC_FCG1_SDIOC2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2S1   << PWC_FCG1_I2S1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2S2   << PWC_FCG1_I2S2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2S3   << PWC_FCG1_I2S3_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->I2S4   << PWC_FCG1_I2S4_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI1   << PWC_FCG1_SPI1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI2   << PWC_FCG1_SPI2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI3   << PWC_FCG1_SPI3_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI4   << PWC_FCG1_SPI4_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI5   << PWC_FCG1_SPI5_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->SPI6   << PWC_FCG1_SPI6_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->USBFS  << PWC_FCG1_USBFS_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->USBHS  << PWC_FCG1_USBHS_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->FMAC1  << PWC_FCG1_FMAC1_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->FMAC2  << PWC_FCG1_FMAC2_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->FMAC3  << PWC_FCG1_FMAC3_POS)   | \
                ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->FMAC4  << PWC_FCG1_FMAC4_POS));

#ifdef PWC_FCG1_MCAN1_POS
  f_FCG1_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->MCAN1 << PWC_FCG1_MCAN1_POS);
#endif

#ifdef PWC_FCG1_MCAN2_POS
  f_FCG1_u32 |= ((para_PowerPeripheral_ptr->FuncClockFCG1_ptr)->MCAN2 << PWC_FCG1_MCAN2_POS);
#endif

  MODIFY_REG32(CM_PWC->FCG1, 0xFFFFFFFFU, f_FCG1_u32);

  f_FCG2_u32 = (((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_1 << PWC_FCG2_TMR6_1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_2 << PWC_FCG2_TMR6_2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_3 << PWC_FCG2_TMR6_3_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_4 << PWC_FCG2_TMR6_4_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_5 << PWC_FCG2_TMR6_5_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_6 << PWC_FCG2_TMR6_6_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_7 << PWC_FCG2_TMR6_7_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR6_8 << PWC_FCG2_TMR6_8_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR4_1 << PWC_FCG2_TMR4_1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR4_2 << PWC_FCG2_TMR4_2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR4_3 << PWC_FCG2_TMR4_3_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR0_1 << PWC_FCG2_TMR0_1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR0_2 << PWC_FCG2_TMR0_2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->EMB    << PWC_FCG2_EMB_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR2_1 << PWC_FCG2_TMR2_1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR2_2 << PWC_FCG2_TMR2_2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR2_3 << PWC_FCG2_TMR2_3_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMR2_4 << PWC_FCG2_TMR2_4_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_1  << PWC_FCG2_TMRA_1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_2  << PWC_FCG2_TMRA_2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_3  << PWC_FCG2_TMRA_3_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_4  << PWC_FCG2_TMRA_4_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_5  << PWC_FCG2_TMRA_5_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_6  << PWC_FCG2_TMRA_6_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_7  << PWC_FCG2_TMRA_7_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_8  << PWC_FCG2_TMRA_8_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_9  << PWC_FCG2_TMRA_9_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_10 << PWC_FCG2_TMRA_10_POS) | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_11 << PWC_FCG2_TMRA_11_POS) | \
                ((para_PowerPeripheral_ptr->FuncClockFCG2_ptr)->TMRA_12 << PWC_FCG2_TMRA_12_POS));
  MODIFY_REG32(CM_PWC->FCG2, 0xFFFFFFFFU, f_FCG2_u32);

  f_FCG3_u32 = (((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->ADC1     << PWC_FCG3_ADC1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->ADC2     << PWC_FCG3_ADC2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->ADC3     << PWC_FCG3_ADC3_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->CMBIAS   << PWC_FCG3_CMBIAS_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->DAC1     << PWC_FCG3_DAC1_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->DAC2     << PWC_FCG3_DAC2_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->CMP12    << PWC_FCG3_CMP12_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->CMP34    << PWC_FCG3_CMP34_POS)    | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->OTS      << PWC_FCG3_OTS_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->DVP      << PWC_FCG3_DVP_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->SMC      << PWC_FCG3_SMC_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->DMC      << PWC_FCG3_DMC_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->NFC      << PWC_FCG3_NFC_POS)     | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART1   << PWC_FCG3_USART1_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART2   << PWC_FCG3_USART2_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART3   << PWC_FCG3_USART3_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART4   << PWC_FCG3_USART4_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART5   << PWC_FCG3_USART5_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART6   << PWC_FCG3_USART6_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART7   << PWC_FCG3_USART7_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART8   << PWC_FCG3_USART8_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART9   << PWC_FCG3_USART9_POS)  | \
                ((para_PowerPeripheral_ptr->FuncClockFCG3_ptr)->USART10  << PWC_FCG3_USART10_POS));
  MODIFY_REG32(CM_PWC->FCG3, 0xFFFFFFFFU, f_FCG3_u32);

  /* Turn off unused RAM: SRAMH->SRAM1*/
  f_RAMPC0_u32 =(((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC10 << PWC_RAMPC0_RAMPDC10_POS) | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC9 << PWC_RAMPC0_RAMPDC9_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC8 << PWC_RAMPC0_RAMPDC8_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC7 << PWC_RAMPC0_RAMPDC7_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC6   << PWC_RAMPC0_RAMPDC6_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC5 << PWC_RAMPC0_RAMPDC5_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC4 << PWC_RAMPC0_RAMPDC4_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC3 << PWC_RAMPC0_RAMPDC3_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC2 << PWC_RAMPC0_RAMPDC2_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC1 << PWC_RAMPC0_RAMPDC1_POS)  | \
                 ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->RAMPDC0 << PWC_RAMPC0_RAMPDC0_POS));
  MODIFY_REG32(CM_PWC->RAMPC0, 0xFFFFFFFFU, f_RAMPC0_u32);

  /* Turn off backup RAM */
  MODIFY_REG8(CM_PWC->VBATCR, PWC_VBATCR_VBTRSD, (para_PowerPeripheral_ptr->RamTurnoff_ptr)->SRAMB);

  /**
   * Turn off unused RAM:
   */

  f_PRAMLPC_u32 = (((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC9     << PWC_PRAMLPC_PRAMPDC9_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC8     << PWC_PRAMLPC_PRAMPDC8_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC7     << PWC_PRAMLPC_PRAMPDC7_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC6     << PWC_PRAMLPC_PRAMPDC6_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC5     << PWC_PRAMLPC_PRAMPDC5_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC4     << PWC_PRAMLPC_PRAMPDC4_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC3     << PWC_PRAMLPC_PRAMPDC3_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC2     << PWC_PRAMLPC_PRAMPDC2_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC1     << PWC_PRAMLPC_PRAMPDC1_POS) | \
                   ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC0     << PWC_PRAMLPC_PRAMPDC0_POS));

#ifdef PWC_PRAMLPC_PRAMPDC11_POS
  f_PRAMLPC_u32 |= ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC11 << PWC_PRAMLPC_PRAMPDC11_POS);
#endif

#ifdef PWC_PRAMLPC_PRAMPDC10_POS
  f_PRAMLPC_u32 |= ((para_PowerPeripheral_ptr->RamTurnoff_ptr)->PRAMPDC10 << PWC_PRAMLPC_PRAMPDC10_POS);
#endif

  MODIFY_REG32(CM_PWC->PRAMLPC, 0xFFFFFFFFU, f_PRAMLPC_u32);
  /* Get the system clock source and PLL clock source */
  f_SysClockSource_u8 = (CM_CMU->CKSWR & CMU_CKSWR_CKSW);
  f_PLLClockSource_u8 = (uint8_t)((CM_CMU->PLLHCFGR & CMU_PLLHCFGR_PLLSRC) >> CMU_PLLHCFGR_PLLSRC_POS);

  /**
   * When XTAL is selected as the system clock or PLLA/H clock source, prohibit writing "1"
   * to XTALSTP to stop the XTAL oscillator.
   */
  if ((CLK_SYSCLK_SRC_XTAL != f_SysClockSource_u8) && (CLK_PLL_SRC_XTAL != f_PLLClockSource_u8))
  {
    MODIFY_REG8(CM_CMU->XTALCR, CMU_XTALCR_XTALSTP, (para_PowerPeripheral_ptr->ClockSource_ptr)->XtalControl);
  }

  /**
   * When XTAL32 is selected as the system clock or PLLA/H clock source, prohibit writing "1"
   * to XTAL32STP to stop the XTAL32 oscillator.
   */
  if (CLK_SYSCLK_SRC_XTAL32 != f_SysClockSource_u8)
  {
    /* Enable/Disable XTAL32 */
    MODIFY_REG8(CM_CMU->XTAL32CR, CMU_XTAL32CR_XTAL32STP, (para_PowerPeripheral_ptr->ClockSource_ptr)->Xtal32Control);
    /* wait for 5 XTAL32 cycles */
    Clock_Ip_Delay(CLK_XTAL32_TIMEOUT);
  }

  /**
   * When MRC is selected as the system clock clock source, prohibit writing "1"
   * to MRCSTP to stop the MRC oscillator.
   */
  if (CLK_SYSCLK_SRC_MRC != f_SysClockSource_u8)
  {
    /* Enable/Disable MRC */
    MODIFY_REG8(CM_CMU->MRCCR, CMU_MRCCR_MRCSTP, (para_PowerPeripheral_ptr->ClockSource_ptr)->MRCControl);
    /* wait for 5 MRC cycles */
    Clock_Ip_Delay(CLK_MRC_TIMEOUT);
  }

  /**
   * When LRC is selected as the system clock clock source, prohibit writing "1"
   * to LRCSTP to stop the LRC oscillator.
   */
  if (CLK_SYSCLK_SRC_LRC != f_SysClockSource_u8)
  {
    /* Enable/Disable LRC */
    MODIFY_REG8(CM_CMU->LRCCR, CMU_LRCCR_LRCSTP, (para_PowerPeripheral_ptr->ClockSource_ptr)->LRCControl);
    /* wait for 5 LRC cycles */
    Clock_Ip_Delay(CLK_LRC_TIMEOUT);
  }

  /**
   * When HRC is selected as the system clock or PLLA/H clock source, prohibit writing "1"
   * to HRCSTP to stop the HRC oscillator.
   */
  if ((CLK_SYSCLK_SRC_HRC != f_SysClockSource_u8) && (CLK_PLL_SRC_HRC != f_PLLClockSource_u8))
  {
    /* Stop HRC */
    MODIFY_REG8(CM_CMU->HRCCR, CMU_HRCCR_HRCSTP, (para_PowerPeripheral_ptr->ClockSource_ptr)->HRCControl & (0x01U));
    /* HRC power-down */
    if (PWC_POWERDOWN == (para_PowerPeripheral_ptr->ClockSource_ptr)->HRCControl)
    {
      SET_REG8_BIT(CM_PWC->PWRC1, PWC_PWRC1_VHRCSD);
    }
  }

  /**
   * When PLLH is selected as the system clock clock source, prohibit writing "1"
   * to PLLHOFF to stop the PLL.
   */
  if (CLK_SYSCLK_SRC_PLL != f_SysClockSource_u8)
  {
    MODIFY_REG8(CM_CMU->PLLHCR, CMU_PLLHCR_PLLHOFF, (para_PowerPeripheral_ptr->ClockSource_ptr)->PLLHControl & (0x01U));
  }

  /**
   * When PLLA is selected clock source of I2S/TRNG/ADC/USB/CAN,
   * write "1" to PLLAOFF to stop PLLA clock is forbiden.
   */
  if (((uint8_t)0x0AU > (uint8_t)((CM_CMU->I2SCKSEL & CMU_I2SCKSEL_I2S1CKSEL) >> CMU_I2SCKSEL_I2S1CKSEL_POS)) && \
      ((uint8_t)0x0AU > (uint8_t)((CM_CMU->I2SCKSEL & CMU_I2SCKSEL_I2S2CKSEL) >> CMU_I2SCKSEL_I2S2CKSEL_POS)) && \
      ((uint8_t)0x0AU > (uint8_t)((CM_CMU->I2SCKSEL & CMU_I2SCKSEL_I2S3CKSEL) >> CMU_I2SCKSEL_I2S3CKSEL_POS)) && \
      ((uint8_t)0x0AU > (uint8_t)((CM_CMU->I2SCKSEL & CMU_I2SCKSEL_I2S4CKSEL) >> CMU_I2SCKSEL_I2S4CKSEL_POS)) && \
      ((uint8_t)0x0AU > (uint8_t)(CM_CMU->PERICKSEL & CMU_PERICKSEL_PERICKSEL)) && \
      ((uint8_t)0x0AU > (uint8_t)((CM_CMU->USBCKCFGR & CMU_USBCKCFGR_USBCKS) >> CMU_USBCKCFGR_USBCKS_POS)) && \
      (((uint8_t)0x0AU > (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_CAN1CKS) >> CMU_CANCKCFGR_CAN1CKS_POS)) || \
       ((uint8_t)0x0DU == (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_CAN1CKS) >> CMU_CANCKCFGR_CAN1CKS_POS))) && \
      (((uint8_t)0x0AU > (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_CAN2CKS) >> CMU_CANCKCFGR_CAN2CKS_POS)) || \
       ((uint8_t)0x0DU == (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_CAN1CKS) >> CMU_CANCKCFGR_CAN1CKS_POS)))
#ifdef PWC_CANCKCFGR_MCAN1CKS_POS
      && (((uint8_t)0x0AU > (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_MCAN1CKS) >> CMU_CANCKCFGR_MCAN1CKS_POS)) || \
          ((uint8_t)0x0DU == (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_MCAN1CKS) >> CMU_CANCKCFGR_MCAN1CKS_POS)))
#endif
#ifdef PWC_CANCKCFGR_MCAN2CKS_POS
      && (((uint8_t)0x0AU > (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_MCAN2CKS) >> CMU_CANCKCFGR_MCAN2CKS_POS)) || \
          ((uint8_t)0x0DU == (uint8_t)((CM_CMU->CANCKCFGR & CMU_CANCKCFGR_MCAN2CKS) >> CMU_CANCKCFGR_MCAN2CKS_POS)))
#endif
      )
  {
    /* Stop PLLA */
    MODIFY_REG8(CM_CMU->PLLACR, CMU_PLLACR_PLLAOFF, (para_PowerPeripheral_ptr->ClockSource_ptr)->PLLAControl & (0x01U));
  }

  /**
   * The PLLA or PLLH selected for the system clock or the peripheral clock source,
   * then can't power-down PLL.
   */
  if ((CMU_PLLHCR_PLLHOFF == READ_REG8_BIT(CM_CMU->PLLHCR, CMU_PLLHCR_PLLHOFF)) && \
     (CMU_PLLACR_PLLAOFF == READ_REG8_BIT(CM_CMU->PLLACR, CMU_PLLACR_PLLAOFF)))
  {
    if (PWC_POWERDOWN == (para_PowerPeripheral_ptr->ClockSource_ptr)->PLLHControl)
    {
      /* Power-down the PLL */
      SET_REG8_BIT(CM_PWC->PWRC1, PWC_PWRC1_VPLLSD);
    }
  }
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/
#define MCU_START_SEC_CODE
#include "Mcu_MemMap.h"

/**
 *  @brief          This function initializes the module Reset control
 *
 *  @details        SRS ID:            None 
 *                  Design ID:         Power_Ip_Init_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_HwIPsConfigPtr_ptr    Pointer to hardware IP configuration structure.
 *
 *  @return         void
 *
 *  @note           This function will be called by Mcu_Ipr_Init().
 *
 *  @pre            None
 *
 */
void Power_Ip_Init(
    const Power_Ip_HwConfigType * para_HwIPsConfigPtr_ptr
)
{
    /* Low voltage down 1 initialize */
    Power_Ip_PVD1Init(para_HwIPsConfigPtr_ptr->McuPVD1Config_ptr);
    /* Low voltage down 2 initialize */
    Power_Ip_PVD2Init(para_HwIPsConfigPtr_ptr->McuPVD2Config_ptr);
    /* Power Monitor initialize */
    Power_Ip_PowerMonitorInit(para_HwIPsConfigPtr_ptr->McuPowerMonitorConfig_ptr);
}

/**
 *  @brief          This function make pointer to the mode structure.
 *
 *  @details        SRS ID:            None
 *                  Design ID:         Power_Ip_SetMode_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ModeConfigPtr_ptr        Pointer to mode configuration structure.
 *  @param[in]      para_McuClockConfigptr_ptr    Pointer to clock configuration structure.
 *
 *  @return         void
 *
 *  @note           This function initializes the mode structure by configuring
 *                  the MC_ME module.
 *                  This function will called by Mcu_Ipr_SetMode().
 *
 *  @pre            None
 *
 */
void Power_Ip_SetMode(
    const Power_Ip_HwModeConfigType * para_ModeConfigPtr_ptr,
    const Clock_Ip_ClockConfigSetType * para_McuClockConfigptr_ptr
)
{
    /* Cast the configuration pointer to appropriate mode-specific structures */
    const Power_Ip_SleepModeCfgType * f_Sleep_ptr   = \
                      (const Power_Ip_SleepModeCfgType *)para_ModeConfigPtr_ptr->McuSetModeConfig_ptr;
    const Power_Ip_StopModeCfgType * f_Stop_ptr     = \
                      (const Power_Ip_StopModeCfgType *)para_ModeConfigPtr_ptr->McuSetModeConfig_ptr;
    const Power_Ip_PowerDownModeCfgType * f_PWD_ptr = \
                      (const Power_Ip_PowerDownModeCfgType *)para_ModeConfigPtr_ptr->McuSetModeConfig_ptr;

    /* Switch to the requested power mode */
    switch(para_ModeConfigPtr_ptr->PowerMode_en)
    {
        case PWC_IP_HIGH_SPEED_MODE:
            /* Configure clock dividers for high speed operation */
            Power_Ip_SwitchClkToHighSpeed(para_McuClockConfigptr_ptr->McuHsrunConfig_ptr);
            /* Switch to high speed power mode */
            Power_Ip_ModeHighSpeed();
            break;
            
        case PWC_IP_ULTRA_LOW_SPEED_MODE:
            /* Configure clock dividers for ultra low speed operation */
            Power_Ip_SwitchClkToUltraLowSpeed(para_McuClockConfigptr_ptr->McuUlsrunConfig_ptr);
            /* Switch to ultra low speed power mode */
            Power_Ip_ModeUltraLowSpeed();
            break;
            
        case PWC_IP_SLEEP_MODE:
            /* Disable unused peripherals to reduce power consumption */
            Power_Ip_DisablePeripheral((const Power_Ip_ConfigPeripheralType *)(f_Sleep_ptr->Peripheral_ptr));
            /* Enter sleep mode (CPU stops, peripherals continue running) */
            Power_Ip_ModeSleep();
            break;
            
        case PWC_IP_STOP_MODE:
            /* Disable XTAL fault detection to prevent unwanted resets during stop mode */
            WRITE_REG8(CM_CMU->XTALSTDCR, (CLK_XTALSTD_OFF | CLK_XTALSTD_MD_RST | CLK_XTALSTD_INT_OFF | CLK_XTALSTD_RST_OFF));
            /* Disable unused peripherals to reduce power consumption */
            Power_Ip_DisablePeripheral((const Power_Ip_ConfigPeripheralType *)(f_Stop_ptr->Peripheral_ptr));
            /* Enter stop mode (CPU and most peripherals stop) */
            Power_Ip_ModeStop((const Power_Ip_StopModeCfgType *)(para_ModeConfigPtr_ptr->McuSetModeConfig_ptr));
            break;
            
        case PWC_IP_PWD_MODE:
            /* Disable XTAL fault detection to prevent unwanted resets during power-down mode */
            WRITE_REG8(CM_CMU->XTALSTDCR, (CLK_XTALSTD_OFF | CLK_XTALSTD_MD_RST | CLK_XTALSTD_INT_OFF | CLK_XTALSTD_RST_OFF));
            /* Disable unused peripherals to reduce power consumption */
            Power_Ip_DisablePeripheral((const Power_Ip_ConfigPeripheralType *)(f_PWD_ptr->Peripheral_ptr));
            /* Enter power-down mode (deepest sleep, only specific wake-up sources active) */
            Power_Ip_ModePwd((const Power_Ip_PowerDownModeCfgType *)(para_ModeConfigPtr_ptr->McuSetModeConfig_ptr));
            break;
            
        default:
            /* Invalid power mode - no action taken */
            break;
    }
}

#if !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI)
/**
 *  @brief          This function checks if backup domain power-on occurred
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_CheckBackupDomainPowerOn_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         boolean    TRUE: Backup domain power-on occurred                
 *                             FALSE: Backup domain power-on did not occur
 *
 *  @note           This function reads VBATPORF flag and clears it if set
 *
 *  @pre            None
 *
 */
boolean Power_Ip_CheckBackupDomainPowerOn(
    void
)
{
    boolean f_BackupReset_b = FALSE;
    uint8_t f_VbatPorf_u8;
    
    /* Read VBATPORF flag */
    f_VbatPorf_u8 = READ_REG8_BIT(CM_PWC->VBATPORF, PWC_VBATPORF_VBATPORF);
    
    if (0U != f_VbatPorf_u8)
    {
        f_BackupReset_b = TRUE;
        
        /* Clear VBATPORF flag by writing 1 to VBATPORFCLR */
        SET_REG8_BIT(CM_PWC->VBATPORFCLR, PWC_VBATPORFCLR_VBATPORFCLR);
    }
    
    return f_BackupReset_b;
}
#endif /* !defined(HC32A4A0SITI) && !defined(HC32A4A0PITI) */

/**
 *  @brief          This function initializes WKT Timer
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_WktTimerConfig_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_ClkSrc_u16               Clock source for WKT timer
 *  @param[in]      para_CmpVal_u16               Comparison value of the Counter (0-0xFFF)
 * 
 *  @return         void
 *
 *  @note           This function configures the Wake-up Timer (WKT) with the specified
 *                  clock source and comparison value. The comparison value can be a number
 *                  between Min_Data = 0 and Max_Data = 0xFFF.
 *
 *  @pre            None
 *
 */
void Power_Ip_WktTimerConfig(
    uint16_t para_ClkSrc_u16, 
    uint16_t para_CmpVal_u16
)
{
    /* Unlock PWC registers for write access */
    SET_REG16_BIT(CM_PWC->FPRC, PWC_UNLOCK_CODE1);
    
    /* Configure WKT timer:
       - Set clock source and clear overflow flag in WKTC2 register
       - Write lower 8 bits of comparison value to WKTC0 register
       - Write upper 4 bits of comparison value to WKTC1 register */
    MODIFY_REG8(CM_PWC->WKTC2, PWC_WKTC2_WKCKS | PWC_WKTC2_WKOVF, para_ClkSrc_u16 | PWC_WKTC2_WKOVF);
    WRITE_REG8(CM_PWC->WKTC0, (para_CmpVal_u16 & PWC_WKTC0_WKTMCMP));
    WRITE_REG8(CM_PWC->WKTC1, ((para_CmpVal_u16 >> 8U) & PWC_WKTC1_WKTMCMP));
    
    /* Lock PWC registers to prevent unauthorized access */
    CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~PWC_UNLOCK_CODE1) & (CM_PWC->FPRC)));
}

/**
 *  @brief          This function enables or disables WKT Timer.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_WktTimerEnableConfig_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @param[in]      para_WKTTimerEnable_bool               TRUE to enable, FALSE to disable WKT Timer
 * 
 *  @return         void
 *
 *  @note           This function controls the enable/disable state of the Wake-up Timer (WKT).
 *                  When enabled, the timer will count and can wake up the system from low-power modes.
 *
 *  @pre            None
 *
 */
void Power_Ip_WktTimerEnableConfig(
    boolean para_WKTTimerEnable_bool
)
{
    /* Unlock PWC registers for write access */
    SET_REG16_BIT(CM_PWC->FPRC, PWC_UNLOCK_CODE1);
    
    /* Enable or disable WKT timer based on parameter */
    if ((boolean)TRUE == para_WKTTimerEnable_bool) 
    {
        /* Enable WKT timer and clear overflow flag */
        MODIFY_REG8(CM_PWC->WKTC2, PWC_WKTC2_WKTCE | PWC_WKTC2_WKOVF, PWC_WKT_ON | PWC_WKTC2_WKOVF);
    } 
    else 
    {
        /* Disable WKT timer */
        MODIFY_REG8(CM_PWC->WKTC2, PWC_WKTC2_WKTCE, PWC_WKT_OFF);
    }
    
    /* Lock PWC registers to prevent unauthorized access */
    CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~PWC_UNLOCK_CODE1) & (CM_PWC->FPRC)));
}

/**
 *  @brief          This function gets WKT Timer count match flag.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_GetWktTimerStatus_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         boolean    TRUE if WKT timer count match occurred, FALSE otherwise
 *
 *  @note           This function reads the WKOVF (Wake-up Timer Overflow) flag to determine
 *                  if the WKT timer has reached its comparison value and generated a match event.
 *
 *  @pre            None
 *
 */
boolean Power_Ip_GetWktTimerStatus(
    void
)
{
    boolean f_WKTTimerStatus_bool = FALSE;

    f_WKTTimerStatus_bool = (0U != READ_REG8_BIT(CM_PWC->WKTC2, PWC_WKTC2_WKOVF)) ? (boolean)TRUE : (boolean)FALSE;

    return f_WKTTimerStatus_bool;
}

/**
 *  @brief          This function clears WKT Timer count match flag.
 *
 *  @details        SRS ID:            None  
 *                  Design ID:         Power_Ip_ClearWktTimerStatus_Activity
 *                  Sync/Async:        Synchronous
 * 
 *  @return         void
 *
 *  @note           This function clears the WKOVF (Wake-up Timer Overflow) flag by writing
 *                  to the appropriate register bit. This should be called after handling
 *                  a WKT timer match event.
 *
 *  @pre            None
 *
 */
void Power_Ip_ClearWktTimerStatus(
    void
)
{
    SET_REG16_BIT(CM_PWC->FPRC, PWC_UNLOCK_CODE1);
    CLR_REG8_BIT(CM_PWC->WKTC2, PWC_WKTC2_WKOVF);
    CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~PWC_UNLOCK_CODE1) & (CM_PWC->FPRC)));
}

#define MCU_STOP_SEC_CODE
#include "Mcu_MemMap.h"

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/*---------------------- End of File ----------------------------------------*/
