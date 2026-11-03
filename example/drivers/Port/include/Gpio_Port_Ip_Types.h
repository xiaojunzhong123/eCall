/***********************************************************************************************************************
 * Project Name: XHSC AUTOSAR MCAL
 * File Name: Gpio_Port_Ip_Types.h
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
 *   @file       Gpio_Port_Ip_Types.h
 *   @version    V1.0.3
 *   @brief      Autosar 4.4.0 PORT Driver
 *   @addtogroup port_driver
 *   @{
 */

#ifndef GPIO_PORT_IP_TYPES_H
#define GPIO_PORT_IP_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "Std_Types.h"
#include "System_Hw_Reg.h"
#include "Port_VersionInfo.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES & MACROS
 **********************************************************************************************************************/
/**
 * @brief Peripheral GPIOA base address
 */
#define GPIOA_BASE                              (CM_GPIO_BASE)
/**
 * @brief Peripheral GPIOA base pointer
 */
#define GPIOA                                   ((GPIO_GeneralType *)GPIOA_BASE)

/**
 * @brief Peripheral GPIOB base address
 */
#define GPIOB_BASE                              (CM_GPIO_BASE + 0x00000010UL)
/**
 * @brief Peripheral GPIOB base pointer
 */
#define GPIOB                                   ((GPIO_GeneralType *)GPIOB_BASE)

/**
 * @brief Peripheral GPIOC base address
 */
#define GPIOC_BASE                              (CM_GPIO_BASE + 0x00000020UL)
/**
 * @brief Peripheral GPIOC base pointer
 */
#define GPIOC                                   ((GPIO_GeneralType *)GPIOC_BASE)

/**
 * @brief Peripheral GPIOD base address
 */
#define GPIOD_BASE                              (CM_GPIO_BASE + 0x00000030UL)
/**
 * @brief Peripheral GPIOD base pointer
 */
#define GPIOD                                   ((GPIO_GeneralType *)GPIOD_BASE)

/**
 * @brief Peripheral GPIOE base address
 */
#define GPIOE_BASE                              (CM_GPIO_BASE + 0x00000040UL)
/**
 * @brief Peripheral GPIOE base pointer
 */
#define GPIOE                                   ((GPIO_GeneralType *)GPIOE_BASE)

/**
 * @brief Peripheral GPIOF base address
 */
#define GPIOF_BASE                              (CM_GPIO_BASE + 0x00000050UL)
/**
 * @brief Peripheral GPIOF base pointer
 */
#define GPIOF                                   ((GPIO_GeneralType *)GPIOF_BASE)

/**
 * @brief Peripheral GPIOG base address
 */
#define GPIOG_BASE                              (CM_GPIO_BASE + 0x00000060UL)
/**
 * @brief Peripheral GPIOG base pointer
 */
#define GPIOG                                   ((GPIO_GeneralType *)GPIOG_BASE)

/**
 * @brief Peripheral GPIOH base address
 */
#define GPIOH_BASE                              (CM_GPIO_BASE + 0x00000070UL)
/**
 * @brief Peripheral GPIOH base pointer
 */
#define GPIOH                                   ((GPIO_GeneralType *)GPIOH_BASE)

/**
 * @brief Peripheral GPIOI base address
 */
#define GPIOI_BASE                              (CM_GPIO_BASE + 0x00000080UL)
/**
 * @brief Peripheral GPIOI base pointer
 */
#define GPIOI                                   ((GPIO_GeneralType *)GPIOI_BASE)

/**
 * @brief Array initializer of GPIO General peripheral base pointers
 */
#define GPIO_BASE_PTRS                          { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI }

/**
 * @brief Define as the same with det error code
 */
#define PORT_GPIO_PROCESS_SUCCESS                             (0x00U)
#define PORT_GPIO_PARAM_INVALID_MODE                          (0x0DU)
#define PORT_GPIO_MODE_UNCHANGEABLE                           (0x0EU)
#define PORT_GPIO_DIRECTION_UNCHANGEABLE                      (0x0BU)
#define PORT_GPIO_PARAM_PIN                                   (0x0AU)
#define PORT_GPIO_PARAM_INVALID_DIRECTION                     (0x12U)

#define PORT_GPIO_PIN_IN                                      0U
#define PORT_GPIO_PIN_OUT                                     1U
#define PORT_GPIO_PIN_INOUT                                   2U

#define PORT_GPIO_A_IDX                                       0U
#define PORT_GPIO_B_IDX                                       1U
#define PORT_GPIO_C_IDX                                       2U
#define PORT_GPIO_D_IDX                                       3U
#define PORT_GPIO_E_IDX                                       4U
#define PORT_GPIO_F_IDX                                       5U
#define PORT_GPIO_G_IDX                                       6U
#define PORT_GPIO_H_IDX                                       7U
#define PORT_GPIO_I_IDX                                       8U

#define PORT_GPIO_PA0                                              ((PORT_GPIO_A_IDX*16U) + 0U)
#define PORT_GPIO_PA1                                              ((PORT_GPIO_A_IDX*16U) + 1U)
#define PORT_GPIO_PA2                                              ((PORT_GPIO_A_IDX*16U) + 2U)
#define PORT_GPIO_PA3                                              ((PORT_GPIO_A_IDX*16U) + 3U)
#define PORT_GPIO_PA4                                              ((PORT_GPIO_A_IDX*16U) + 4U)
#define PORT_GPIO_PA5                                              ((PORT_GPIO_A_IDX*16U) + 5U)
#define PORT_GPIO_PA6                                              ((PORT_GPIO_A_IDX*16U) + 6U)
#define PORT_GPIO_PA7                                              ((PORT_GPIO_A_IDX*16U) + 7U)
#define PORT_GPIO_PA8                                              ((PORT_GPIO_A_IDX*16U) + 8U)
#define PORT_GPIO_PA9                                              ((PORT_GPIO_A_IDX*16U) + 9U)
#define PORT_GPIO_PA10                                             ((PORT_GPIO_A_IDX*16U) + 10U)
#define PORT_GPIO_PA11                                             ((PORT_GPIO_A_IDX*16U) + 11U)
#define PORT_GPIO_PA12                                             ((PORT_GPIO_A_IDX*16U) + 12U)
#define PORT_GPIO_PA13                                             ((PORT_GPIO_A_IDX*16U) + 13U)
#define PORT_GPIO_PA14                                             ((PORT_GPIO_A_IDX*16U) + 14U)
#define PORT_GPIO_PA15                                             ((PORT_GPIO_A_IDX*16U) + 15U)

#define PORT_GPIO_PB0                                              ((PORT_GPIO_B_IDX*16U) + 0U)
#define PORT_GPIO_PB1                                              ((PORT_GPIO_B_IDX*16U) + 1U)
#define PORT_GPIO_PB2                                              ((PORT_GPIO_B_IDX*16U) + 2U)
#define PORT_GPIO_PB3                                              ((PORT_GPIO_B_IDX*16U) + 3U)
#define PORT_GPIO_PB4                                              ((PORT_GPIO_B_IDX*16U) + 4U)
#define PORT_GPIO_PB5                                              ((PORT_GPIO_B_IDX*16U) + 5U)
#define PORT_GPIO_PB6                                              ((PORT_GPIO_B_IDX*16U) + 6U)
#define PORT_GPIO_PB7                                              ((PORT_GPIO_B_IDX*16U) + 7U)
#define PORT_GPIO_PB8                                              ((PORT_GPIO_B_IDX*16U) + 8U)
#define PORT_GPIO_PB9                                              ((PORT_GPIO_B_IDX*16U) + 9U)
#define PORT_GPIO_PB10                                             ((PORT_GPIO_B_IDX*16U) + 10U)
#define PORT_GPIO_PB11                                             ((PORT_GPIO_B_IDX*16U) + 11U)
#define PORT_GPIO_PB12                                             ((PORT_GPIO_B_IDX*16U) + 12U)
#define PORT_GPIO_PB13                                             ((PORT_GPIO_B_IDX*16U) + 13U)
#define PORT_GPIO_PB14                                             ((PORT_GPIO_B_IDX*16U) + 14U)
#define PORT_GPIO_PB15                                             ((PORT_GPIO_B_IDX*16U) + 15U)

#define PORT_GPIO_PC0                                              ((PORT_GPIO_C_IDX*16U) + 0U)
#define PORT_GPIO_PC1                                              ((PORT_GPIO_C_IDX*16U) + 1U)
#define PORT_GPIO_PC2                                              ((PORT_GPIO_C_IDX*16U) + 2U)
#define PORT_GPIO_PC3                                              ((PORT_GPIO_C_IDX*16U) + 3U)
#define PORT_GPIO_PC4                                              ((PORT_GPIO_C_IDX*16U) + 4U)
#define PORT_GPIO_PC5                                              ((PORT_GPIO_C_IDX*16U) + 5U)
#define PORT_GPIO_PC6                                              ((PORT_GPIO_C_IDX*16U) + 6U)
#define PORT_GPIO_PC7                                              ((PORT_GPIO_C_IDX*16U) + 7U)
#define PORT_GPIO_PC8                                              ((PORT_GPIO_C_IDX*16U) + 8U)
#define PORT_GPIO_PC9                                              ((PORT_GPIO_C_IDX*16U) + 9U)
#define PORT_GPIO_PC10                                             ((PORT_GPIO_C_IDX*16U) + 10U)
#define PORT_GPIO_PC11                                             ((PORT_GPIO_C_IDX*16U) + 11U)
#define PORT_GPIO_PC12                                             ((PORT_GPIO_C_IDX*16U) + 12U)
#define PORT_GPIO_PC13                                             ((PORT_GPIO_C_IDX*16U) + 13U)
#define PORT_GPIO_PC14                                             ((PORT_GPIO_C_IDX*16U) + 14U)
#define PORT_GPIO_PC15                                             ((PORT_GPIO_C_IDX*16U) + 15U)

#define PORT_GPIO_PD0                                              ((PORT_GPIO_D_IDX*16U) + 0U)
#define PORT_GPIO_PD1                                              ((PORT_GPIO_D_IDX*16U) + 1U)
#define PORT_GPIO_PD2                                              ((PORT_GPIO_D_IDX*16U) + 2U)
#define PORT_GPIO_PD3                                              ((PORT_GPIO_D_IDX*16U) + 3U)
#define PORT_GPIO_PD4                                              ((PORT_GPIO_D_IDX*16U) + 4U)
#define PORT_GPIO_PD5                                              ((PORT_GPIO_D_IDX*16U) + 5U)
#define PORT_GPIO_PD6                                              ((PORT_GPIO_D_IDX*16U) + 6U)
#define PORT_GPIO_PD7                                              ((PORT_GPIO_D_IDX*16U) + 7U)
#define PORT_GPIO_PD8                                              ((PORT_GPIO_D_IDX*16U) + 8U)
#define PORT_GPIO_PD9                                              ((PORT_GPIO_D_IDX*16U) + 9U)
#define PORT_GPIO_PD10                                             ((PORT_GPIO_D_IDX*16U) + 10U)
#define PORT_GPIO_PD11                                             ((PORT_GPIO_D_IDX*16U) + 11U)
#define PORT_GPIO_PD12                                             ((PORT_GPIO_D_IDX*16U) + 12U)
#define PORT_GPIO_PD13                                             ((PORT_GPIO_D_IDX*16U) + 13U)
#define PORT_GPIO_PD14                                             ((PORT_GPIO_D_IDX*16U) + 14U)
#define PORT_GPIO_PD15                                             ((PORT_GPIO_D_IDX*16U) + 15U)

#define PORT_GPIO_PE0                                              ((PORT_GPIO_E_IDX*16U) + 0U)
#define PORT_GPIO_PE1                                              ((PORT_GPIO_E_IDX*16U) + 1U)
#define PORT_GPIO_PE2                                              ((PORT_GPIO_E_IDX*16U) + 2U)
#define PORT_GPIO_PE3                                              ((PORT_GPIO_E_IDX*16U) + 3U)
#define PORT_GPIO_PE4                                              ((PORT_GPIO_E_IDX*16U) + 4U)
#define PORT_GPIO_PE5                                              ((PORT_GPIO_E_IDX*16U) + 5U)
#define PORT_GPIO_PE6                                              ((PORT_GPIO_E_IDX*16U) + 6U)
#define PORT_GPIO_PE7                                              ((PORT_GPIO_E_IDX*16U) + 7U)
#define PORT_GPIO_PE8                                              ((PORT_GPIO_E_IDX*16U) + 8U)
#define PORT_GPIO_PE9                                              ((PORT_GPIO_E_IDX*16U) + 9U)
#define PORT_GPIO_PE10                                             ((PORT_GPIO_E_IDX*16U) + 10U)
#define PORT_GPIO_PE11                                             ((PORT_GPIO_E_IDX*16U) + 11U)
#define PORT_GPIO_PE12                                             ((PORT_GPIO_E_IDX*16U) + 12U)
#define PORT_GPIO_PE13                                             ((PORT_GPIO_E_IDX*16U) + 13U)
#define PORT_GPIO_PE14                                             ((PORT_GPIO_E_IDX*16U) + 14U)
#define PORT_GPIO_PE15                                             ((PORT_GPIO_E_IDX*16U) + 15U)

#define PORT_GPIO_PF0                                              ((PORT_GPIO_F_IDX*16U) + 0U)
#define PORT_GPIO_PF1                                              ((PORT_GPIO_F_IDX*16U) + 1U)
#define PORT_GPIO_PF2                                              ((PORT_GPIO_F_IDX*16U) + 2U)
#define PORT_GPIO_PF3                                              ((PORT_GPIO_F_IDX*16U) + 3U)
#define PORT_GPIO_PF4                                              ((PORT_GPIO_F_IDX*16U) + 4U)
#define PORT_GPIO_PF5                                              ((PORT_GPIO_F_IDX*16U) + 5U)
#define PORT_GPIO_PF6                                              ((PORT_GPIO_F_IDX*16U) + 6U)
#define PORT_GPIO_PF7                                              ((PORT_GPIO_F_IDX*16U) + 7U)
#define PORT_GPIO_PF8                                              ((PORT_GPIO_F_IDX*16U) + 8U)
#define PORT_GPIO_PF9                                              ((PORT_GPIO_F_IDX*16U) + 9U)
#define PORT_GPIO_PF10                                             ((PORT_GPIO_F_IDX*16U) + 10U)
#define PORT_GPIO_PF11                                             ((PORT_GPIO_F_IDX*16U) + 11U)
#define PORT_GPIO_PF12                                             ((PORT_GPIO_F_IDX*16U) + 12U)
#define PORT_GPIO_PF13                                             ((PORT_GPIO_F_IDX*16U) + 13U)
#define PORT_GPIO_PF14                                             ((PORT_GPIO_F_IDX*16U) + 14U)
#define PORT_GPIO_PF15                                             ((PORT_GPIO_F_IDX*16U) + 15U)

#define PORT_GPIO_PG0                                              ((PORT_GPIO_G_IDX*16U) + 0U)
#define PORT_GPIO_PG1                                              ((PORT_GPIO_G_IDX*16U) + 1U)
#define PORT_GPIO_PG2                                              ((PORT_GPIO_G_IDX*16U) + 2U)
#define PORT_GPIO_PG3                                              ((PORT_GPIO_G_IDX*16U) + 3U)
#define PORT_GPIO_PG4                                              ((PORT_GPIO_G_IDX*16U) + 4U)
#define PORT_GPIO_PG5                                              ((PORT_GPIO_G_IDX*16U) + 5U)
#define PORT_GPIO_PG6                                              ((PORT_GPIO_G_IDX*16U) + 6U)
#define PORT_GPIO_PG7                                              ((PORT_GPIO_G_IDX*16U) + 7U)
#define PORT_GPIO_PG8                                              ((PORT_GPIO_G_IDX*16U) + 8U)
#define PORT_GPIO_PG9                                              ((PORT_GPIO_G_IDX*16U) + 9U)
#define PORT_GPIO_PG10                                             ((PORT_GPIO_G_IDX*16U) + 10U)
#define PORT_GPIO_PG11                                             ((PORT_GPIO_G_IDX*16U) + 11U)
#define PORT_GPIO_PG12                                             ((PORT_GPIO_G_IDX*16U) + 12U)
#define PORT_GPIO_PG13                                             ((PORT_GPIO_G_IDX*16U) + 13U)
#define PORT_GPIO_PG14                                             ((PORT_GPIO_G_IDX*16U) + 14U)
#define PORT_GPIO_PG15                                             ((PORT_GPIO_G_IDX*16U) + 15U)

#define PORT_GPIO_PH0                                              ((PORT_GPIO_H_IDX*16U) + 0U)
#define PORT_GPIO_PH1                                              ((PORT_GPIO_H_IDX*16U) + 1U)
#define PORT_GPIO_PH2                                              ((PORT_GPIO_H_IDX*16U) + 2U)
#define PORT_GPIO_PH3                                              ((PORT_GPIO_H_IDX*16U) + 3U)
#define PORT_GPIO_PH4                                              ((PORT_GPIO_H_IDX*16U) + 4U)
#define PORT_GPIO_PH5                                              ((PORT_GPIO_H_IDX*16U) + 5U)
#define PORT_GPIO_PH6                                              ((PORT_GPIO_H_IDX*16U) + 6U)
#define PORT_GPIO_PH7                                              ((PORT_GPIO_H_IDX*16U) + 7U)
#define PORT_GPIO_PH8                                              ((PORT_GPIO_H_IDX*16U) + 8U)
#define PORT_GPIO_PH9                                              ((PORT_GPIO_H_IDX*16U) + 9U)
#define PORT_GPIO_PH10                                             ((PORT_GPIO_H_IDX*16U) + 10U)
#define PORT_GPIO_PH11                                             ((PORT_GPIO_H_IDX*16U) + 11U)
#define PORT_GPIO_PH12                                             ((PORT_GPIO_H_IDX*16U) + 12U)
#define PORT_GPIO_PH13                                             ((PORT_GPIO_H_IDX*16U) + 13U)
#define PORT_GPIO_PH14                                             ((PORT_GPIO_H_IDX*16U) + 14U)
#define PORT_GPIO_PH15                                             ((PORT_GPIO_H_IDX*16U) + 15U)

#define PORT_GPIO_PI0                                              ((PORT_GPIO_I_IDX*16U) + 0U)
#define PORT_GPIO_PI1                                              ((PORT_GPIO_I_IDX*16U) + 1U)
#define PORT_GPIO_PI2                                              ((PORT_GPIO_I_IDX*16U) + 2U)
#define PORT_GPIO_PI3                                              ((PORT_GPIO_I_IDX*16U) + 3U)
#define PORT_GPIO_PI4                                              ((PORT_GPIO_I_IDX*16U) + 4U)
#define PORT_GPIO_PI5                                              ((PORT_GPIO_I_IDX*16U) + 5U)
#define PORT_GPIO_PI6                                              ((PORT_GPIO_I_IDX*16U) + 6U)
#define PORT_GPIO_PI7                                              ((PORT_GPIO_I_IDX*16U) + 7U)
#define PORT_GPIO_PI8                                              ((PORT_GPIO_I_IDX*16U) + 8U)
#define PORT_GPIO_PI9                                              ((PORT_GPIO_I_IDX*16U) + 9U)
#define PORT_GPIO_PI10                                             ((PORT_GPIO_I_IDX*16U) + 10U)
#define PORT_GPIO_PI11                                             ((PORT_GPIO_I_IDX*16U) + 11U)
#define PORT_GPIO_PI12                                             ((PORT_GPIO_I_IDX*16U) + 12U)
#define PORT_GPIO_PI13                                             ((PORT_GPIO_I_IDX*16U) + 13U)
#define PORT_GPIO_PI14                                             ((PORT_GPIO_I_IDX*16U) + 14U)
#define PORT_GPIO_PI15                                             ((PORT_GPIO_I_IDX*16U) + 15U)

#define PORT_GPIO_GROUP_INDEX(portPinId)                           (portPinId/16U)
#define PORT_GPIO_PIN_INDEX(portPinId)                             (portPinId%16U)

#define PORT_GPIO_PWPR_SET_VALUE                                   (0xA501U)

#define PORT_GPIO_PIN_SCHMITT_INPUT_MODE                           (0U)
#define PORT_GPIO_PIN_CMOS_INPUT_MODE                              (1U)

#define PORT_GPIO_PIN_LOW_DRIVING_FORCE_MODE                       (0U)
#define PORT_GPIO_PIN_MEDIUM_DRIVING_FORCE_MODE                    (1U)
#define PORT_GPIO_PIN_HIGH_DRIVING_FORCE_MODE                      (2U)

#define PORT_PIN_LEVEL_LOW                                         (0U)
#define PORT_PIN_LEVEL_HIGH                                        (1U)


/**
 * @brief Port GPIO Mode
 */
#define PORT_GPIO_MODE                                            (0U)
/**
 * @brief Port Func 1 Mode
 */
#define PORT_FUNC_1_MODE                                          (1U)
/**
 * @brief Port Func 2 Mode
 */
#define PORT_FUNC_2_MODE                                          (2U)
/**
 * @brief Port Func 3 Mode
 */
#define PORT_FUNC_3_MODE                                          (3U)
/**
 * @brief Port Func 4 Mode
 */
#define PORT_FUNC_4_MODE                                          (4U)
/**
 * @brief Port Func 5 Mode
 */
#define PORT_FUNC_5_MODE                                          (5U)
/**
 * @brief Port Func 6 Mode
 */
#define PORT_FUNC_6_MODE                                          (6U)
/**
 * @brief Port Func 7 Mode
 */
#define PORT_FUNC_7_MODE                                          (7U)
/**
 * @brief Port Func 8 Mode
 */
#define PORT_FUNC_8_MODE                                          (8U)
/**
 * @brief Port Func 9 Mode
 */
#define PORT_FUNC_9_MODE                                          (9U)
/**
 * @brief Port Func 10 Mode
 */
#define PORT_FUNC_10_MODE                                         (10U)
/**
 * @brief Port Func 11 Mode
 */
#define PORT_FUNC_11_MODE                                         (11U)
/**
 * @brief Port Func 12 Mode
 */
#define PORT_FUNC_12_MODE                                         (12U)
/**
 * @brief Port Func 13 Mode
 */
#define PORT_FUNC_13_MODE                                         (13U)
/**
 * @brief Port Func 14 Mode
 */
#define PORT_FUNC_14_MODE                                         (14U)
/**
 * @brief Port Func 15 Mode
 */
#define PORT_FUNC_15_MODE                                         (15U)
/**
 * @brief Port Func 16 Mode
 */
#define PORT_FUNC_16_MODE                                         (16U)
/**
 * @brief Port Func 17 Mode
 */
#define PORT_FUNC_17_MODE                                         (17U)
/**
 * @brief Port Func 18 Mode
 */ 
#define PORT_FUNC_18_MODE                                         (18U)
/**
 * @brief Port Func 19 Mode
 */
#define PORT_FUNC_19_MODE                                         (19U)
/**
 * @brief Port Func 20 Mode
 */
#define PORT_FUNC_20_MODE                                         (20U)
/**
 * @brief Port Func 21 Mode
 */
#define PORT_FUNC_21_MODE                                         (21U)
/**
 * @brief Port Func 22 Mode
 */
#define PORT_FUNC_22_MODE                                         (22U)
/**
 * @brief Port Func 23 Mode
 */
#define PORT_FUNC_23_MODE                                         (23U)
/**
 * @brief Port Func 24 Mode
 */
#define PORT_FUNC_24_MODE                                         (24U)
/**
 * @brief Port Func 25 Mode
 */
#define PORT_FUNC_25_MODE                                         (25U)
/**
 * @brief Port Func 26 Mode
 */
#define PORT_FUNC_26_MODE                                         (26U)
/**
 * @brief Port Func 27 Mode
 */
#define PORT_FUNC_27_MODE                                         (27U)
/**
 * @brief Port Func 28 Mode
 */
#define PORT_FUNC_28_MODE                                         (28U)
/**
 * @brief Port Func 29 Mode
 */
#define PORT_FUNC_29_MODE                                         (29U)
/**
 * @brief Port Func 30 Mode
 */
#define PORT_FUNC_30_MODE                                         (30U)
/**
 * @brief Port Func 31 Mode
 */
#define PORT_FUNC_31_MODE                                         (31U)
/**
 * @brief Port Func 32 Mode
 */
#define PORT_FUNC_32_MODE                                         (32U)
/**
 * @brief Port Func 33 Mode
 */
#define PORT_FUNC_33_MODE                                         (33U)
/**
 * @brief Port Func 34 Mode
 */
#define PORT_FUNC_34_MODE                                         (34U)
/**
 * @brief Port Func 35 Mode
 */
#define PORT_FUNC_35_MODE                                         (35U)
/**
 * @brief Port Func 36 Mode
 */
#define PORT_FUNC_36_MODE                                         (36U)
/**
 * @brief Port Func 37 Mode
 */
#define PORT_FUNC_37_MODE                                         (37U)
/**
 * @brief Port Func 38 Mode
 */
#define PORT_FUNC_38_MODE                                         (38U)
/**
 * @brief Port Func 39 Mode
 */
#define PORT_FUNC_39_MODE                                         (39U)
/**
 * @brief Port Func 40 Mode
 */
#define PORT_FUNC_40_MODE                                         (40U)
/**
 * @brief Port Func 41 Mode
 */
#define PORT_FUNC_41_MODE                                         (41U)
/**
 * @brief Port Func 42 Mode
 */
#define PORT_FUNC_42_MODE                                         (42U)
/**
 * @brief Port Func 43 Mode
 */
#define PORT_FUNC_43_MODE                                         (43U)
/**
 * @brief Port Func 44 Mode
 */
#define PORT_FUNC_44_MODE                                         (44U)
/**
 * @brief Port Func 45 Mode
 */
#define PORT_FUNC_45_MODE                                         (45U)
/**
 * @brief Port Func 46 Mode
 */
#define PORT_FUNC_46_MODE                                         (46U)
/**
 * @brief Port Func 47 Mode
 */
#define PORT_FUNC_47_MODE                                         (47U)
/**
 * @brief Port Func 48 Mode
 */
#define PORT_FUNC_48_MODE                                         (48U)
/**
 * @brief Port Func 49 Mode
 */
#define PORT_FUNC_49_MODE                                         (49U)
/**
 * @brief Port Func 50 Mode
 */
#define PORT_FUNC_50_MODE                                         (50U)
/**
 * @brief Port Func 51 Mode
 */
#define PORT_FUNC_51_MODE                                         (51U)
/**
 * @brief Port Func 52 Mode
 */
#define PORT_FUNC_52_MODE                                         (52U)
/**
 * @brief Port Func 53 Mode
 */
#define PORT_FUNC_53_MODE                                         (53U)
/**
 * @brief Port Func 54 Mode
 */
#define PORT_FUNC_54_MODE                                         (54U)
/**
 * @brief Port Func 55 Mode
 */
#define PORT_FUNC_55_MODE                                         (55U)
/**
 * @brief Port Func 56 Mode
 */
#define PORT_FUNC_56_MODE                                         (56U)
/**
 * @brief Port Func 57 Mode
 */
#define PORT_FUNC_57_MODE                                         (57U)
/**
 * @brief Port Func 58 Mode
 */
#define PORT_FUNC_58_MODE                                         (58U)
/**
 * @brief Port Func 59 Mode
 */
#define PORT_FUNC_59_MODE                                         (59U)
/**
 * @brief Port Func 60 Mode
 */
#define PORT_FUNC_60_MODE                                         (60U)
/**
 * @brief Port Func 61 Mode
 */
#define PORT_FUNC_61_MODE                                         (61U)
/**
 * @brief Port Func 62 Mode
 */
#define PORT_FUNC_62_MODE                                         (62U)
/**
 * @brief Port Func 63 Mode
 */
#define PORT_FUNC_63_MODE                                         (63U)
/**
 * @brief Port Analog Mode
 */
#define PORT_ANALOG_MODE                                          (64U)
/**
 * @brief Port EIRQ_WKUP Mode
 */
#define PORT_EIRQ_WKUP_MODE                                       (65U)
/**
 * @brief Port TRACE/JTAG Mode
 */
#define PORT_TRACE_JTAG_MODE                                      (66U)

/**
 * @brief Bit definition for special control of PSPCR
 */
#define GPIO_PSPCR_SPFE                                           (0x001FU)
/**
 * @brief Bit definition for JTCK_SWCLK of PSPCR
 */
#define GPIO_PSPCR_SPFE_0                                         (0x0001U)
/**
 * @brief Bit definition for JTMS_SWDIO of PSPCR
 */
#define GPIO_PSPCR_SPFE_1                                         (0x0002U)
/**
 * @brief Bit definition for JTDO_TRACESWO of PSPCR
 */
#define GPIO_PSPCR_SPFE_2                                         (0x0004U)
/**
 * @brief Bit definition for JTDI of PSPCR
 */
#define GPIO_PSPCR_SPFE_3                                         (0x0008U)
/**
 * @brief Bit definition for NJTRST of PSPCR
 */
#define GPIO_PSPCR_SPFE_4                                         (0x0010U)
/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/**
 *  @brief   GPIO - Register Layout Typedef
 *  @details SRS ID:     None
 */
typedef struct
{
  volatile const uint16_t                   PIDR;               /*!< (R/   16) General input data register*/
  uint8_t                                   RESERVED_0[2];      /*!< (R/W  16) Reserved register*/
  volatile uint16_t                         PODR;               /*!< (R/W  16) General output data register*/
  volatile uint16_t                         POER;               /*!< (R/W  16) General output register*/
  volatile uint16_t                         POSR;               /*!< (R/W  16) General output set register*/
  volatile uint16_t                         PORR;               /*!< (R/W  16) General output reset register*/
  volatile uint16_t                         POTR;               /*!< (R/W  16) General output flip register*/
  uint8_t                                   RESERVED_1[2];      /*!< (R/W  16) Reserved register*/
} GPIO_GroupType;

/**
 *  @brief   GPIO - Register Layout Typedef
 *  @details SRS ID:     None
 */
typedef struct
{
  volatile uint16_t                         PCR;                /*!< (R/W  16) General control register*/
  volatile uint16_t                         PFSR;               /*!< (R/W  16) Function select register*/
} GPIO_PinType;

/**
 *  @brief   GPIO - Register Layout Typedef
 *  @details SRS ID:     None
 */
typedef struct
{
  GPIO_GroupType                          PORT[9];           /*!< (R/W  16) Port General control register*/                     
  uint8_t                                   RESERVED_0[868];   /*!< (R/W  16) Reserved register*/
  volatile uint16_t                         PSPCR;             /*!< (R/W  16) Special control register*/
  uint8_t                                   RESERVED_1[2];     /*!< (R/W  16) Reserved register*/
  volatile uint16_t                         PCCR;              /*!< (R/W  16) Common control register*/
  volatile uint16_t                         PINAER;            /*!< (R/W  16) Input control register*/
  volatile uint16_t                         PWPR;              /*!< (R/W  16) Writable protect register*/
  uint8_t                                   RESERVED_2[2];     /*!< (R/W  16) Reserved register*/
  GPIO_PinType                            PIN[9][16];        /*!<  Pin operation register */
} GPIO_Type;

/**
 *  @brief   This type contain data of all pin config in Port driver
 *  @details SRS ID:     SWS_Port_00072, SWS_Port_00228
 */
typedef struct
{
  uint8_t InitialMode_u8;             /*!< Port pin mode from mode list for use with Port_Init() function - (Port_PinModeType) */
  uint8_t Mode_u8;                    /*!< Port pin mode from mode list - (Port_PinModeType) */
  uint8_t SubMode_u8;                 /*!< Port pin sub-mode from mode list - (Port_PinModeType) */
  uint8_t GpioNumber_u8;              /*!< Pin Id of the GPIO pin */
  uint8_t PinName_u8;                 /*!< Pin name of the GPIO pin */
  boolean DualMode_bool;            /*!< Parameter to indicate if the dual mode enable */
  uint8_t Direction_u8;               /*!< The initial direction of the pin (IN or OUT) - (Port_PinDirectionType) */
  boolean DirectionChangeable_bool; /*!< Parameter to indicate if the direction is changeable on a port pin during runtime */
  uint8_t LevelValue_u8;              /*!< Port Pin Level value from Port pin list */
  boolean ModeChangeable_bool;      /*!< Parameter to indicate if the mode is changeable on a port pin during runtime */
  boolean OutputLatch_bool;         /*!< Output state latch enable/disable */
  uint8_t InputMode_u8;               /*!< Input mode Schmitt/CMOS */
  boolean PullUp_bool;              /*!< Pull-up enable/disable */
  uint8_t DrivingMode_u8;             /*!< Select driving force mode */
  boolean OpenDrain_bool;           /*!< Open-drain enable/disable */
} Port_Gpio_PinConfigType;

/**
 *  @brief   Type of the external data structure containing the initialization data for Port module
 *  @details SRS ID:     None
 */
typedef struct
{
  uint8_t NumPins_u8;                                 /*!< The number of configured pins */
  const Port_Gpio_PinConfigType *PortPinConfig_ptr; /*!< Pointer to the list of Port configured pins */
} Port_Gpio_ConfigType;

/***********************************************************************************************************************
 *  CONSTANTS
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
#endif /* _GPIO_PORT_IP_TYPES_H_ */
/**
 * @}
 */

/*---------------------- End of File -----------------------------------------*/
