/**
 *******************************************************************************
 * @file  ev_hc32a4a8_lqfp176.h
 * @brief This file contains all the functions prototypes of the
 *        EV_HC32A4A8_LQFP176 BSP driver library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-09-13       CDT             First version
   2025-01-20       CDT             Modify XTAL/XTAL32 pins definition
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef __EV_HC32A4A8_LQFP176_H__
#define __EV_HC32A4A8_LQFP176_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_aos.h"
#include "hc32_ll_clk.h"
#include "hc32_ll_dma.h"
#include "hc32_ll_efm.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_fcm.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_i2c.h"
#include "hc32_ll_i2s.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_keyscan.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll_spi.h"
#include "hc32_ll_sram.h"
#include "hc32_ll_usart.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176_BASE
 * @{
 */

#if (BSP_EV_HC32A4A8_LQFP176 == BSP_EV_HC32A4XX)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_Global_Macros EV_HC32A4A8_LQFP176 Global Macros
 * @{
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_I2C_Configuration EV_HC32A4A8_LQFP176 I2C Configuration
 * @{
 */
#define BSP_I2C_BAUDRATE                (100000UL)
#define BSP_I2C_TIMEOUT                 (0x40000U)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_LED_PortPin_Sel EV_HC32A4A8_LQFP176 LED port/pin definition
 * @{
 */
#define LED_RED_PORT                    (GPIO_PORT_I)
#define LED_RED_PIN                     (GPIO_PIN_08)
#define LED_YELLOW_PORT                 (GPIO_PORT_H)
#define LED_YELLOW_PIN                  (GPIO_PIN_05)
#define LED_BLUE_PORT                   (GPIO_PORT_A)
#define LED_BLUE_PIN                    (GPIO_PIN_06)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_LED_Sel EV_HC32A4A8_LQFP176 LED definition
 * @{
 */
#define LED_RED                         (0x01U)
#define LED_YELLOW                      (0x02U)
#define LED_BLUE                        (0x04U)
#define LED_ALL                         (LED_RED | LED_YELLOW | LED_BLUE)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_LED_Ctrl EV_HC32A4A8_LQFP176 LED control
 * @{
 */
#define LED_RED_ON()                    WRITE_REG32(bCM_GPIO->POSRI_b.POS08, 1U)
#define LED_YELLOW_ON()                 WRITE_REG32(bCM_GPIO->POSRH_b.POS05, 1U)
#define LED_BLUE_ON()                   WRITE_REG32(bCM_GPIO->POSRA_b.POS06, 1U)

#define LED_RED_OFF()                   WRITE_REG32(bCM_GPIO->PORRI_b.POR08, 1U)
#define LED_YELLOW_OFF()                WRITE_REG32(bCM_GPIO->PORRH_b.POR05, 1U)
#define LED_BLUE_OFF()                  WRITE_REG32(bCM_GPIO->PORRA_b.POR06, 1U)

#define LED_RED_TOGGLE()                WRITE_REG32(bCM_GPIO->POTRI_b.POT08, 1U)
#define LED_YELLOW_TOGGLE()             WRITE_REG32(bCM_GPIO->POTRH_b.POT05, 1U)
#define LED_BLUE_TOGGLE()               WRITE_REG32(bCM_GPIO->POTRA_b.POT06, 1U)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_KEY_Sel EV_HC32A4A8_LQFP176 KEY definition
 * @{
 */
#define BSP_KEY_1                       (0x0001UL)    /*!< BSP KEY 1 */
#define BSP_KEY_2                       (0x0002UL)    /*!< BSP KEY 2 */
#define BSP_KEY_3                       (0x0010UL)    /*!< BSP KEY 3 */
#define BSP_KEY_4                       (0x0020UL)    /*!< BSP KEY 4 */
#define BSP_KEY_5                       (0x0800UL)    /*!< BSP KEY 5. Independent key. */
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_KEY_Number EV_HC32A4A8_LQFP176 KEY Number
 * @{
 */
#define BSP_KEY_ROW_NUM                 (2U)
#define BSP_KEY_COL_NUM                 (2U)
#define BSP_KEY_INDEPENDENT_NUM         (1U)
/* KEY Dithering Elimination */
#define BSP_KEY_DELAY_MS                (10UL)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_KEY_PortPin EV_HC32A4A8_LQFP176 KEY port/pin definition
 * @{
 */
#define BSP_KEY_KEY5_PORT               (GPIO_PORT_A)
#define BSP_KEY_KEY5_PIN                (GPIO_PIN_00)
#define BSP_KEY_KEY5_EXTINT             (EXTINT_CH00)
#define BSP_KEY_KEY5_INT_SRC            (INT_SRC_PORT_EIRQ0)
#define BSP_KEY_KEY5_IRQn               (INT025_IRQn)
#define BSP_KEY_KEY5_WAKEUP             (INTC_STOP_WKUP_EXTINT_CH0)
#define BSP_KEY_KEY5_EVT                (EVT_SRC_PORT_EIRQ0)

#define BSP_KEYOUT0_PORT                (GPIO_PORT_C)
#define BSP_KEYOUT0_PIN                 (GPIO_PIN_11)
#define BSP_KEYOUT1_PORT                (GPIO_PORT_C)
#define BSP_KEYOUT1_PIN                 (GPIO_PIN_08)

#define BSP_KEYIN0_PORT                 (GPIO_PORT_H)
#define BSP_KEYIN0_PIN                  (GPIO_PIN_06)
#define BSP_KEY_ROW0_EXTINT             (EXTINT_CH06)
#define BSP_KEY_ROW0_INT_SRC            (INT_SRC_PORT_EIRQ6)
#define BSP_KEY_ROW0_IRQn               (INT029_IRQn)

#define BSP_KEYIN1_PORT                 (GPIO_PORT_H)
#define BSP_KEYIN1_PIN                  (GPIO_PIN_07)
#define BSP_KEY_ROW1_EXTINT             (EXTINT_CH07)
#define BSP_KEY_ROW1_INT_SRC            (INT_SRC_PORT_EIRQ7)
#define BSP_KEY_ROW1_IRQn               (INT030_IRQn)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_KEYSCAN_CONFIG EV_HC32A4A8_LQFP176 KEYSCAN Configure definition
 * @{
 */
#define BSP_KEYOUT_SELECT               (KEYSCAN_OUT_0T1)
#define BSP_KEYIN_SELECT                (KEYSCAN_IN_6 | KEYSCAN_IN_7)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_PRINT_CONFIG EV_HC32A4A8_LQFP176 PRINT Configure definition
 * @{
 */
#define BSP_PRINTF_DEVICE               (CM_USART1)
#define BSP_PRINTF_DEVICE_FCG           (FCG3_PERIPH_USART1)

#define BSP_PRINTF_BAUDRATE             (115200UL)
#define BSP_PRINTF_BAUDRATE_ERR_MAX     (0.025F)

#define BSP_PRINTF_PORT                 (GPIO_PORT_H)
#define BSP_PRINTF_PIN                  (GPIO_PIN_15)
#define BSP_PRINTF_PORT_FUNC            (GPIO_FUNC_32)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_XTAL_CONFIG EV_HC32A4A8_LQFP176 XTAL port/pin definition
 * @{
 */
#define BSP_XTAL_PORT                   (GPIO_PORT_H)
#define BSP_XTAL_PIN                    (GPIO_PIN_00 | GPIO_PIN_01)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_XTAL32_CONFIG EV_HC32A4A8_LQFP176 XTAL32 port/pin definition
 * @{
 */
#define BSP_XTAL32_PORT                 (GPIO_PORT_C)
#define BSP_XTAL32_PIN                  (GPIO_PIN_14 | GPIO_PIN_15)
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_FCM_CONFIG EV_HC32A4A8_LQFP176 FCM definition
 * @{
 */
#define BSP_FCM_UNIT                    (CM_FCM1)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup EV_HC32A4A8_LQFP176_Global_Functions
 * @{
 */
int32_t BSP_XTAL32_Init(void);
void BSP_CLK_Init(void);

void BSP_LED_Init(void);
void BSP_LED_On(uint8_t u8Led);
void BSP_LED_Off(uint8_t u8Led);
void BSP_LED_Toggle(uint8_t u8Led);

void BSP_KEY_Init(void);
en_flag_status_t BSP_KEY_GetStatus(uint32_t u32Key);

#if (LL_PRINT_ENABLE == DDL_ON)
int32_t BSP_PRINTF_Preinit(void *vpDevice, uint32_t u32Baudrate);
#endif

/* It can't get the status of the KEYx by calling BSP_KEY_GetStatus when you re-implement BSP_KEY_KEYx_IrqHandler. */
void BSP_KEY_KEY5_IrqHandler(void);

#if (LL_I2C_ENABLE == DDL_ON)
int32_t BSP_I2C_Init(CM_I2C_TypeDef *I2Cx);
void BSP_I2C_DeInit(CM_I2C_TypeDef *I2Cx);
int32_t BSP_I2C_Write(CM_I2C_TypeDef *I2Cx, uint16_t u16DevAddr, const uint8_t *pu8Reg, uint8_t u8RegLen, const uint8_t *pu8Buf, uint32_t u32Len);
int32_t BSP_I2C_Read(CM_I2C_TypeDef *I2Cx, uint16_t u16DevAddr, const uint8_t *pu8Reg, uint8_t u8RegLen, uint8_t *pu8Buf, uint32_t u32Len);
int32_t BSP_I2C_GetDevStatus(CM_I2C_TypeDef *I2Cx, uint16_t u16DevAddr);
#endif /* LL_I2C_ENABLE */

/**
 * @}
 */

#endif /* BSP_EV_HC32A4A8_LQFP176 */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __EV_HC32A4A8_LQFP176_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
