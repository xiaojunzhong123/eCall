/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Power_Ip_Types.h
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
 *   @file       Power_Ip_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 MCU Driver
 *   @addtogroup mcu_driver
 *   @{
 */

#ifndef POWER_IP_TYPES_H
#define POWER_IP_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "Power_Ip_Mode_Types.h"
#include "Clock_Ip_Types.h"
#include "Clock_Ip_Cfg.h"

/***********************************************************************************************************************
 *   VERSION CHECK
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief PWC WKT Clock Source
 */
#define PWC_WKT_CLK_SRC_64HZ            (0x00U << PWC_WKTC2_WKCKS_POS)      /*!< 64Hz Clock   */
#define PWC_WKT_CLK_SRC_XTAL32          (0x01U << PWC_WKTC2_WKCKS_POS)      /*!< XTAL32 Clock */
#define PWC_WKT_CLK_SRC_RTCLRC          (0x02U << PWC_WKTC2_WKCKS_POS)      /*!< RTCLRC Clock */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 * @brief hardware IP Configuration for.
 */
typedef struct {
  const Power_Ip_PWC_PVD1ConfigType  *McuPVD1Config_ptr;        /*!< Configuration for PWC (Power controller Unit) hardware IP.               */
  const Power_Ip_PWC_PVD2ConfigType  *McuPVD2Config_ptr;        /*!< Configuration for PWC (Power controller Unit) hardware IP.               */
  const Power_Ip_PowerMonitorConfigType  *McuPowerMonitorConfig_ptr;        /*!< Configuration for PWC (Power controller Unit) hardware IP.               */
} Power_Ip_HwConfigType;

/**
 * @brief The config structure to the peripheral before entering to low-power mode.
 */
typedef struct {
  const Power_Ip_TurnOffClockSourceType *ClockSource_ptr;       /*!< The structure to turn-ofF the clock source don't use.                    */
  const Power_Ip_FCG0Type               *FuncClockFCG0_ptr;     /*!< The structure to stop the peripheral clock in the FCG0 register.         */
  const Power_Ip_FCG1Type               *FuncClockFCG1_ptr;     /*!< The structure to stop the peripheral clock in the FCG1 register.         */
  const Power_Ip_FCG2Type               *FuncClockFCG2_ptr;     /*!< The structure to stop the peripheral clock in the FCG2 register.         */
  const Power_Ip_FCG3Type               *FuncClockFCG3_ptr;     /*!< The structure to stop the peripheral clock in the FCG3 register.         */
  const Power_Ip_TurnoffRamType         *RamTurnoff_ptr;        /*!< The structure to turn off the RAM that doesn't use.                      */
} Power_Ip_ConfigPeripheralType;

/**
 * @brief The config structure of the peripheral before entering to the sleep mode.
 */
typedef struct {
  const Power_Ip_ConfigPeripheralType * Peripheral_ptr;
} Power_Ip_SleepModeCfgType;

/**
 * @brief PWC Stop mode Initialization.
 */
typedef struct {
  uint8_t                               McuStopModeState_u8;       /*!< On/Off Stop mode.                                                       */
  Power_Ip_ClkAfterStopWkuType        Clock_en;                  /*!< System clock setting after wake-up from stop mode.                      */
  Power_Ip_StopDrvType                StopDrv_en;                /*!< Stop mode drive capacity.                                               */
  Power_Ip_ExBusHoldType              ExBusHold_en;              /*!< Expos status in stop mode.                                              */
  Power_Ip_FlashWaitType              FlashWait_en;              /*!< Waiting flash stable after wake-up from stop mode.                      */
  const Power_Ip_StopModeWKUSrcType   *StopMode_Wkusrc_ptr;      /*!< Choose source wake-up system from stop mode.                            */
  const Power_Ip_ConfigPeripheralType * Peripheral_ptr;           /*!< The structure to configure the peripheral before entering to stop mode. */
} Power_Ip_StopModeCfgType;

/**
 * @brief PWC Power-down Initialization.
 */
typedef struct {
  uint8_t                               McuPwdState_u8;             /*!< On/Off power-down mode.                                                */
  Power_Ip_PowerDownSelTypes          Mode_en;                    /*!< Power-down mode sub-mode.                                              */
  Power_Ip_PowerDownIOSelType         IOState_en;                 /*!< The IO state hold control in power-down mode.                          */
  Power_Ip_VCAPSelType                VcapCtrl_en;                /*!< The total capacitance of VCAP1/VCAP2.                                  */
  const Power_Ip_PWDWKUEdgeSelType    * PWDWakeupTrigger_ptr;      /*!< The event that can be used for wake-up from power-down mode.           */
  const Power_Ip_WKUPWDEnableType     * PWDWKUSrc_ptr;             /*!< The edge trigger of power-down wake-up source.                         */
  const Power_Ip_ConfigPeripheralType * Peripheral_ptr;            /*!< The structure to configure the peripheral before entering to PWD mode. */
} Power_Ip_PowerDownModeCfgType;


/**
 * @brief  Power_Ip_ModeType Specifies the identification (ID) for a MCU mode,
 *         configured via configuration structure.
 *         The type shall be uint8_t, uint16_t or uint32_t.
 */
typedef uint8_t Power_Ip_ModeType;

/**
 * @brief  Power_Ip_PowerModeType Modes encoding in enum type.
 *         Supported power all modes for hw IP.
 */
typedef enum {
  PWC_IP_HIGH_SPEED_MODE      = 0x00U,                            /*!< High Speed Run Mode.                                                   */
  PWC_IP_ULTRA_LOW_SPEED_MODE = 0x01U,                            /*!< Ultra Low Speed Run Mode.                                              */
  PWC_IP_SLEEP_MODE           = 0x02U,                            /*!< Sleep Mode.                                                            */
  PWC_IP_STOP_MODE            = 0x03U,                            /*!< Stop Mode.                                                             */
  PWC_IP_PWD_MODE             = 0x04U                             /*!< Power-Down Mode.                                                       */
} Power_Ip_PowerModeType;

/**
 * @brief  Power_Ip_HwModeConfigType Definition of a MCU mode section in the structure
 *         The system behaviour during the selected target mode.
 */
typedef struct {
  Power_Ip_ModeType ModeConfigId_dd;                              /*!< The ID for Power Mode configuration.                                   */
  Power_Ip_PowerModeType PowerMode_en;                            /*!< Power modes control configuration.                                     */
  const void* McuSetModeConfig_ptr;   /*!< The power-mode control configuration.                                  */
} Power_Ip_HwModeConfigType;

/***********************************************************************************************************************
 *  CONSTANTS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERN
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* POWER_IP_TYPES_H */

/*---------------------- End of File ----------------------------------------*/
