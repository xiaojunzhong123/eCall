/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Clock_Ip_Types.h
 *
 * Description: Implementation of CLOCK IP layer
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
 *   @file       Clock_Ip_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef CLOCK_IP_TYPES_H
#define CLOCK_IP_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "StandardTypes.h"
#include "System_Hw_Reg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief  FCGx Group Peripheral.
 */
#define MCU_FCG_FCG0_PERIPH_GROUP       (0U)
#define MCU_FCG_FCG1_PERIPH_GROUP       (1U)
#define MCU_FCG_FCG2_PERIPH_GROUP       (2U)
#define MCU_FCG_FCG3_PERIPH_GROUP       (3U)

/**
 * @brief  Switch clock stable time (30us).
 */
#define CMU_HCLK_VALUE                  (g_SystemCoreClock_u32 >> ((CM_CMU->SCFGR & CMU_SCFGR_HCLKS) >> CMU_SCFGR_HCLKS_POS))
#define CLK_SYSCLK_SW_STB               (CMU_HCLK_VALUE / 50000U)

/**
 * @brief  FCM Status Flag Selection.
 */
#define FCM_FLAG_ERR                    (FCM_SR_ERRF)
#define FCM_FLAG_END                    (FCM_SR_MENDF)
#define FCM_FLAG_OVF                    (FCM_SR_OVF)

/**
 * @brief  GPIO Registers Protect Keys.
 */
#define GPIO_REG_LOCK_KEY               (0xA500U)
#define GPIO_REG_UNLOCK_KEY             (0xA501U)

/**
 * @brief  EFM Registers Protect Keys.
 */
#define EFM_REG_UNLOCK_KEY1             (0x0123U)
#define EFM_REG_UNLOCK_KEY2             (0x3210U)
#define EFM_REG_LOCK_KEY                (0x0000U)

/**
 * @brief  INTC Registers Protect Key.
 */
#define MPU_REG_LOCK_KEY                (0x96A4U)
#define MPU_REG_UNLOCK_KEY              (0x96A5U)

/**
 * @brief  This is return type for functions GetStableStatus.
 */
#define RESET                           (0x00U)
#define SET                             (0x01U)

/**
 * @brief  Functional state.
 */
#define MCU_DISABLE                     (0x00U)
#define MCU_ENABLE                      (0x01U)

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ENUMS
 **********************************************************************************************************************/
/**
 * @brief  These are the different types of  configurations for clock , configured by Mcu_InitClock API of MCU module.
 */
typedef enum {
  MCU_CMU_XTAL_CLK   = 0U,                                    /*!< The External high-speed oscillator.            */
  MCU_CMU_HRC_CLK    = 1U,                                    /*!< The Internal high-speed oscillator.            */
  MCU_CMU_SYS_CLK    = 2U,                                    /*!< The system clock.                              */
  MCU_CMU_FCM1_CLK    = 3U,                                   /*!< The clock frequency measurement circuit.       */
  MCU_CMU_FCM2_CLK    = 4U,                                   /*!< The clock frequency measurement circuit.       */
  MCU_CMU_FCM3_CLK    = 5U,                                   /*!< The clock frequency measurement circuit.       */
  MCU_CMU_FCM4_CLK    = 6U,                                   /*!< The clock frequency measurement circuit.       */
  MCU_CMU_XTAL32_CLK = 7U,                                    /*!< The external low-speed oscillator.             */
  MCU_CMU_PLLH_CLK   = 8U,                                    /*!< The PLLH clock circuit.                        */
  MCU_CMU_PLLA_CLK   = 9U,                                    /*!< The PLLA clock circuit.                        */
  MCU_CMU_MRC_CLK    = 10U,                                   /*!< The internal medium speed oscillator.          */
  MCU_CMU_LRC_CLK    = 11U,                                   /*!< The internal low-speed oscillator.             */
  MCU_CMU_RTCLRC_CLK = 12U,                                   /*!< The low-speed oscillator for RTC.              */
  MCU_CMU_PERIPH_CLK = 13U,                                   /*!< The peripheral clock.                          */
  MCU_CMU_XTALDIV_CLK = 14U                                   /*!< The XTAL Div clock.                            */
}Clock_Ip_ConfigClocksType;

/**
 * @brief  These are select source clock by Mcu_InitClock API of MCU module.
 */
typedef enum {
  MCU_CMU_SELXTAL_CLK   = 0U,                                 /*!< Select the External high-speed clock.          */
  MCU_CMU_SELXTAL32_CLK = 1U,                                 /*!< Select the External low-speed clock.           */
  MCU_CMU_SELPLLHP_CLK  = 2U,                                 /*!< Select the PLLH-P clock.                       */
  MCU_CMU_SELHRC_CLK    = 3U,                                 /*!< Select the internal high-speed clock.          */
  MCU_CMU_SELMRC_CLK    = 4U,                                 /*!< Select the internal medium-speed clock.        */
  MCU_CMU_SELLRC_CLK    = 5U,                                 /*!< Select the internal low-speed clock.           */
  MCU_CMU_SELPLLHQ_CLK  = 6U,                                 /*!< Select the PLLH-Q clock.                       */
  MCU_CMU_SELPLLHR_CLK  = 7U,                                 /*!< Select the PLLH-R clock.                       */
  MCU_CMU_SELPLLAP_CLK  = 8U,                                 /*!< Select the PLLA-P clock.                       */
  MCU_CMU_SELPLLAQ_CLK  = 9U,                                 /*!< Select the PLLA-Q clock.                       */
  MCU_CMU_SELPLLAR_CLK  = 10U,                                /*!< Select the PLLA-R clock.                       */
  MCU_CMU_SELRTCLRC_CLK = 11U,                                /*!< Select the LRCRTC clock.                       */
  MCU_CMU_SELSYSDIV_CLK = 12U,                                /*!< Select the system clock.                       */
  MCU_CMU_DEFAULT_CLK   = 13U                                 /*!< select default clock.                          */
}Clock_Ip_ClocksSelectType;

/**
 * @brief  These are the different types of peripheral clock , configured by Mcu_InitClock API of MCU module.
 */
typedef enum {
  MCU_CAN1CLK = 0U,                                           /*!< The CAN1 clock.                                */
  MCU_CAN2CLK = 1U,                                           /*!< The CAN2 clock.                                */
  MCU_MCAN1CLK = 2U,                                          /*!< The MCAN1 clock.                               */
  MCU_MCAN2CLK = 3U,                                          /*!< The MCAN2 clock.                               */
  MCU_TPIUCLK = 4U,                                           /*!< The TPIU clock.                                */
  MCU_HCLK    = 5U,                                           /*!< The system clock.                              */
  MCU_PCLK0   = 6U,                                           /*!< The peripheral clock 0.                        */
  MCU_PCLK1   = 7U,                                           /*!< The peripheral clock 1.                        */
  MCU_PCLK2   = 8U,                                           /*!< The peripheral clock 2.                        */
  MCU_PCLK3   = 9U,                                           /*!< The peripheral clock 3.                        */
  MCU_PCLK4   = 10U,                                          /*!< The peripheral clock 4.                        */
  MCU_EXCLK   = 11U,                                          /*!< The external bus clock.                        */
  MCU_I2SCLK  = 12U,                                          /*!< The I2S clock.                                 */
  MCU_USBCLK  = 13U,                                          /*!< The USB clock.                                 */
  MCU_RTCCLK  = 14U,                                          /*!< The RTC clock.                                 */
  MCU_STCLK   = 15U,                                          /*!< The system clock clock.                        */
  MCU_SWDTCLK = 16U,                                          /*!< The watchdog clock.                            */
  MCU_MCO1CLK = 17U,                                          /*!< The MCO1 clock.                                */
  MCU_MCO2CLK = 18U                                           /*!< The MCO2 clock.                                */
}Clock_Ip_PeriphClocksType;

/**
 * @brief  This is status returned by the function Clock_Ip_GetPllStatus of the MCU module.
 */
typedef enum {
  CLOCK_IP_PLL_LOCKED           = 0x00U,                      /*!< PLL is locked.                                 */
  CLOCK_IP_PLL_UNLOCKED         = 0x01U,                      /*!< PLL is unlocked.                               */
  CLOCK_IP_PLL_STATUS_UNDEFINED = 0x02U                       /*!< PLL Status is unknown.                         */
} Clock_Ip_PllStatusType;

/**
 * @brief     Type of value of the function Mcu_GetPllStatus.
 * @details   The type of Clock_Ip_PllStatus is an enumeration with the following values: MCU_PLL_LOCKED, MCU_PLL_UNLOCKED, MCU_PLL_STATUS_UNDEFINED.
 */
typedef enum {
  MCU_PLL_LOCKED                = 0x00U,                      /*!< PLL is locked.                                 */
  MCU_PLL_UNLOCKED              = 0x01U,                      /*!< PLL is unlocked.                               */
  MCU_PLL_STATUS_UNDEFINED      = 0x02U                       /*!< PLL Status is unknown.                         */
} Clock_Ip_PllStatus;

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief  Mcu Clock data types to saving ID for a clock setting.
 */
typedef uint8_t Clock_Ip_ClockType;

/**
 * @brief  Mcu Clock data types to saving ID for a function clock.
 */
typedef uint8_t Clock_Ip_FuncClockType;

/**
 * @brief  This configuration type configure for XTAL Oscillator.
 */
typedef struct {
  uint8_t McuXtalState_u8;                                      /*!< Xtal oscillator start/stop.                    */
  uint8_t McuXtalMode_u8;                                       /*!< Xtal mode selection.                           */
  uint8_t McuXtalDriv_u8;                                       /*!< Xtal driving ability selection.                */
  uint8_t McuXtalStableTime_u8;                                 /*!< Xtal stability time selection.                 */
  uint8_t McuXtalStdState_u8;                                   /*!< Specifies the new state of XTALSTD.            */
  uint8_t McuXtalStdMode_u8;                                    /*!< Specifies the XTALSTD mode.                    */
  uint8_t McuXtalStdInt_u8;                                     /*!< Specifies the XTALSTD interrupt on or off.     */
  uint8_t McuXtalStdReset_u8;                                   /*!< Specifies the XTALSTD reset on or off.         */
} Clock_Ip_XtalConfigType;

/**
 * @brief  FCM Types.
 */
typedef struct {
  uint8_t McuFcmState_u8;                                       /*!< FCM state.                                     */
  uint16_t McuFcmLowerLimit_u16;                                /*!< FCM lower limit value.                         */
  uint16_t McuFcmUpperLimit_u16;                                /*!< FCM upper limit value.                         */
  uint32_t McuFcmTargetClock_u32;                               /*!< FCM target clock source selection.             */
  uint32_t McuFcmTargetClockDiv_u32;                            /*!< FCM target clock source division selection.    */
  uint32_t McuFcmExtRefClockEnable_u32;                         /*!< FCM external reference clock function config.  */
  uint32_t McuFcmRefClockEdge_u32;                              /*!< FCM reference clock trigger edge selection.    */
  uint32_t McuFcmDigitalFilter_u32;                             /*!< FCM digital filter function config.            */
  uint32_t McuFcmRefClock_u32;                                  /*!< FCM reference clock source selection.          */
  uint32_t McuFcmRefClockDiv_u32;                               /*!< FCM reference clock source division selection. */
  uint32_t McuFcmExceptionType_u32;                             /*!< FCM exception type select.                     */
  uint8_t  McuFcmIntOvf_u8;                                     /*!< ENABLE/DISABLE overflow interrupt.             */
  uint8_t  McuFcmIntEnd_u8;                                     /*!< ENABLE/DISABLE end of measurement interrupt.   */
  uint8_t  McuFcmIntErr_u8;                                     /*!< ENABLE/DISABLE frequency abnormal interrupt.   */
} Clock_Ip_FcmConfigType;

/**
 * @brief  This configuration type configure frequency division of system clock for peripheral.
 */
typedef struct {
  Clock_Ip_ClocksSelectType McuClockSelect_en;                /*!< Configure the clocks selection type.           */
  uint32_t SysClockDiv_u32;                                     /*!< System clock divided.                          */
} Clock_Ip_SelClockSourceType;

/**
 * @brief  This configuration type configure to wait cycle clock for SRAM, Flash and GPIO.
 */
typedef struct {
  uint32_t McuSysClockFrequency_u32;                            /*!< Mcu System clock frequency value.              */
  uint32_t McuSramWaitCycle_u32;                                /*!< Read/Write wait cycle configures for Sram.     */
  uint32_t McuSramHWaitCycle_u32;                               /*!< Read/Write wait cycle configures for SramH.    */
  uint32_t McuFlashWaitCycle_u32;                               /*!< Read wait cycle configures for Flash.          */
  uint16_t McuGPIOWaitCycle_u16;                                /*!< Read wait cycle configures for GPIO.           */
} Clock_Ip_WaitCycleConfigType;

/**
 * @brief  The Structure select source clock  for system clock.
 */
typedef struct {
  Clock_Ip_ClocksSelectType McuSelectSysClock_en;                         /*!< Configure the clocks selection type.               */
  const Clock_Ip_WaitCycleConfigType*  McuWaitCycleConfig_ptr;            /*!< Pointer to the structure configure the wait cycle. */
} Clock_Ip_SelSysClockSourceType;

/**
 * @brief  This configuration type for peripheral clock source.
 */
typedef struct {
  Clock_Ip_PeriphClocksType McuPeriphClock_en;                /*!< Configure the peripheral selection type.       */
  const void*  McuPeriphConfig_ptr;                           /*!< Pointer to peripheral config.                  */
} Clock_Ip_PeriphClockSourceType;

/**
 * @brief  This configuration type for XTAL32 Oscillator.
 */
typedef struct {
  uint8_t McuXtal32State_u8;                                    /*!< Xtal32 oscillator start/stop.                  */
  uint8_t McuXtal32Driv_u8;                                     /*!< Xtal32 drive capability selection.             */
  uint8_t McuXtal32Filter_u8;                                   /*!< Xtal32 oscillator filter selection.            */
} Clock_Ip_Xtal32ConfigType;

/**
 * @brief  This configuration type for HRC Oscillator.
 */
typedef struct {
  uint8_t McuHrcState_u8;                                       /*!< Hrc start/stop.                                */
  sint8 McuHrcTrim_s8;                                        /*!< Hrc frequency calibration.                     */
} Clock_Ip_HrcConfigType;

/**
 * @brief  This configuration type for MRC Clock.
 */
typedef struct {
  uint8_t McuMrcState_u8;                                       /*!< Mrc start/stop.                                */
  sint8 McuMrcTrim_s8;                                        /*!< Mrc frequency calibration.                     */
} Clock_Ip_MrcConfigType;

/**
 * @brief  This configuration type for I2s Peripheral Clock.
 */
typedef struct {
  uint8_t McuI2sUnit_u8;                                        /*!< I2S unit.                                      */
  uint8_t McuI2sSource_u8;                                      /*!< I2S clock source.                              */
} Clock_Ip_I2sConfigType;

/**
 * @brief  This configuration type for Clock Generator PinOut.
 */
typedef struct {
  uint8_t McuMcoState_u8;                                       /*!< Mco start/stop.                                */
  uint8_t McuMcoChannel_u8;                                     /*!< Mco Chanel.                                    */
  uint8_t McuMcoSource_u8;                                      /*!< Mco Source clock.                              */
  uint8_t McuMcoDivider_u8;                                     /*!< Mco Source clock Div.                          */
} Clock_Ip_PinOutConfigType;

/**
 * @brief  This configuration type for LRC Clock.
 */
typedef struct {
  uint8_t McuLrcState_u8;                                       /*!< Lrc start/stop.                                */
  sint8 McuLrcTrim_s8;                                        /*!< Lrc frequency calibration.                     */
} Clock_Ip_LrcConfigType;

/**
 * @brief  This configuration type for peripheral RTCLRC Clock.
 */
typedef struct {
  sint8 McuRtcLrcTrim_s8;                                     /*!< Rtc Lrc frequency calibration.                 */
} Clock_Ip_RtcLrcConfigType;

/**
 * @brief  This configuration type configure for XTAL Div.
 */
typedef struct {
  uint32_t McuXtalDivNumer_u32;                                      /*!< XTAL Div numerator. */
  uint32_t McuXtalDivDenom_u32;                                      /*!< XTAL Div denominator. */
  uint32_t McuXtalDivState_u32;                                      /*!< XTAL Div state. */
} Clock_Ip_XtalDivConfigType;

/**
 * @brief  This configuration type for peripheral TPIU Clock.
 */
typedef struct {
  uint8_t McuTpiuState_u8;                                      /*!< TPIU clock supply enable.                      */
  uint8_t McuSysClockDiv_u8;                                    /*!< System clock divided.                          */
} Clock_Ip_TpiuConfigType;

/**
 * @brief  This configuration type for PLLH Clock.
 */
typedef struct {
  uint8_t McuPllState_u8;                                       /*!< LPPH start/stop.                               */
  uint8_t McuPllhMDiv_u8;                                       /*!< PLLH M divide.                                 */
  uint8_t McuPllhNMul_u8;                                       /*!< PLLH N multi.                                  */
  uint8_t McuPllhRDiv_u8;                                       /*!< PLLH R divide.                                 */
  uint8_t McuPllhQDiv_u8;                                       /*!< PLLH Q divide.                                 */
  uint8_t McuPllhPDiv_u8;                                       /*!< PLLH P divide.                                 */
  uint8_t McuSrcClock_u8;                                       /*!< PLLH/PLLA source clock select.                 */
} Clock_Ip_PllhConfigType;

/**
 * @brief  This configuration type for PLLA Clock.
 */
typedef struct {
  uint8_t McuPllState_u8;                                       /*!< LPPA start/stop.                               */
  uint8_t McuPllaMDiv_u8;                                       /*!< PLLA M divide.                                 */
  uint16_t McuPllaNMul_u16;                                     /*!< PLLA N multi.                                  */
  uint8_t McuPllaRDiv_u8;                                       /*!< PLLA R divide.                                 */
  uint8_t McuPllaQDiv_u8;                                       /*!< PLLA Q divide.                                 */
  uint8_t McuPllaPDiv_u8;                                       /*!< PLLA P divide.                                 */
} Clock_Ip_PllaConfigType;

/**
 * @brief  This structure contains configuration for clock IP.
 */
typedef struct {
  Clock_Ip_ConfigClocksType McuClockSource_en;          /*!< These are the different types of clock configurations */
  const void*  McuClockConfig_ptr;                      /*!< This configuration structure */
} Clock_Ip_ClockHwConfigType;

/**
 * @brief  This structure contains configuration for HSRUN.
 */
typedef struct {
  uint32_t HCLKClockDiv_u32;                                    /*!< HCLK clock divided.  */
  uint32_t EXCLKClockDiv_u32;                                   /*!< EXCLK clock divided. */
  uint32_t PCLK0ClockDiv_u32;                                   /*!< PCLK0 clock divided. */
  uint32_t PCLK1ClockDiv_u32;                                   /*!< PCLK1 clock divided. */
  uint32_t PCLK2ClockDiv_u32;                                   /*!< PCLK2 clock divided. */
  uint32_t PCLK3ClockDiv_u32;                                   /*!< PCLK3 clock divided. */
  uint32_t PCLK4ClockDiv_u32;                                   /*!< PCLK4 clock divided. */
} Clock_Ip_HsrunConfigType;

/**
 * @brief  This structure contains configuration for ULSRUN.
 */
typedef struct {
  uint32_t HCLKClockDiv_u32;                                    /*!< HCLK clock divided.   */
  uint32_t EXCLKClockDiv_u32;                                   /*!< EXCLK clock divided.  */
  uint32_t PCLK0ClockDiv_u32;                                   /*!< PCLK0 clock divided.  */
  uint32_t PCLK1ClockDiv_u32;                                   /*!< PCLK1 clock divided.  */
  uint32_t PCLK2ClockDiv_u32;                                   /*!< PCLK2 clock divided.  */
  uint32_t PCLK3ClockDiv_u32;                                   /*!< PCLK3 clock divided.  */
  uint32_t PCLK4ClockDiv_u32;                                   /*!< PCLK4 clock divided.  */
} Clock_Ip_UlsrunConfigType;

/**
 * @brief  This configuration type for function clock controller.
 */
typedef struct {
  uint8_t McuPeripheralState_u8;                                /*!< Peripheral state.    */
  uint8_t McuPeripheralGroup_u8;                                /*!< Group peripheranl.   */
  uint32_t McuPeripheralName_u32;                               /*!< Peripheral name.     */
} Clock_Ip_FuncClockControlType;

/**
 * @brief  Mcu Clock Configuration set structure.
 */
typedef struct {
  uint32_t McuClockConfigId_u32;                                       /*!< Mcu clock ID.                                                   */
  Clock_Ip_ClockType McuClocksConfigCount_dd;                        /*!< Mcu Clock Specifies the ID for a clock setting.                 */
  const Clock_Ip_ClockHwConfigType*  McuClockConfigList_ptr;         /*!< This is a pointer to array of "Mcu_ClockConfigType" structures. */
  Clock_Ip_FuncClockType McuFuncClocksCount_dd;                      /*!< Mcu Clock controller the ID for the function clock setting.     */
  const Clock_Ip_FuncClockControlType*  McuFuncClockControlList_ptr; /*!< Pointer to List of Mcu Function Clock controllers.              */
  const Clock_Ip_HsrunConfigType*  McuHsrunConfig_ptr;               /*!< Pointer to High speed run mode configuration.                   */
  const Clock_Ip_UlsrunConfigType*  McuUlsrunConfig_ptr;             /*!< Pointer to Ultra low speed run mode configuration.              */
} Clock_Ip_ClockConfigSetType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/
#define MCU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"
/**
 * @brief System clock frequency (Core clock).
 */
extern uint32_t g_SystemCoreClock_u32;
#define MCU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mcu_MemMap.h"

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 *
 * @}
 */

#endif /* CLOCK_IP_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
