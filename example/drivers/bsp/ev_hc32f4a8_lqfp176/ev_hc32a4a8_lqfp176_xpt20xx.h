/**
 *******************************************************************************
 * @file  ev_hc32a4a8_lqfp176_xpt20xx.h
 * @brief This file contains all the functions prototypes of the touch pad xpt20xx
 *        driver library for the board EV_HC32A4A8_LQFP176.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-09-13       CDT             First version
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
#ifndef __EV_HC32A4A8_LQFP176_XPT20XX_H__
#define __EV_HC32A4A8_LQFP176_XPT20XX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "xpt20xx.h"
#include "ev_hc32a4a8_lqfp176.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176_XPT20XX
 * @{
 */
#if ((BSP_XPT20XX_ENABLE == DDL_ON) && (BSP_EV_HC32A4A8_LQFP176 == BSP_EV_HC32A4XX))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_XPT20XX_Global_Macros EV_HC32A4A8_LQFP176 XPT20XX Global Macros
 * @{
 */

/**
 * @defgroup XPT20XX_SPI_Interface XPT20XX SPI Interface
 * @{
 */

/* SPI unit */
#define BSP_XPT20XX_SPI_UNIT             (CM_SPI6)
#define BSP_XPT20XX_SPI_FCG              (FCG1_PERIPH_SPI6)

/* SPI pin define */
#define BSP_XPT20XX_SPI_SCK_PORT         (GPIO_PORT_E)
#define BSP_XPT20XX_SPI_SCK_PIN          (GPIO_PIN_03)
#define BSP_XPT20XX_SPI_SCK_FUNC         (GPIO_FUNC_46)

#define BSP_XPT20XX_SPI_MOSI_PORT        (GPIO_PORT_E)
#define BSP_XPT20XX_SPI_MOSI_PIN         (GPIO_PIN_04)
#define BSP_XPT20XX_SPI_MOSI_FUNC        (GPIO_FUNC_47)

#define BSP_XPT20XX_SPI_MISO_PORT        (GPIO_PORT_E)
#define BSP_XPT20XX_SPI_MISO_PIN         (GPIO_PIN_05)
#define BSP_XPT20XX_SPI_MISO_FUNC        (GPIO_FUNC_48)

/**
 * @}
 */

/* SPI communication timeout */
#define SPI_COMM_TIMEOUT_VAL             (0x20000000UL)

/* LCD size */
#define LCD_WIDTH                        (320U)
#define LCD_HEIGHT                       (480U)

/* LCD dir */
typedef enum {
    LCD_DISP_DIR_0 = 0x00,
    LCD_DISP_DIR_90,
    LCD_DISP_DIR_180,
    LCD_DISP_DIR_270,
} lcd_disp_dir_t;

#define LCD_DISP_DIR                     (LCD_DISP_DIR_0)

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
 * @addtogroup EV_HC32A4A8_LQFP176_XPT20XX_Global_Functions
 * @{
 */
void BSP_XPT20XX_Init(void);
uint8_t BSP_XPT20XX_TouchDetect(void);
void BSP_XPT20XX_GET_XY(uint16_t *pu16x, uint16_t *pu16y);
/**
 * @}
 */

#endif /* BSP_XPT20XX_ENABLE && BSP_EV_HC32A4A8_LQFP176 */

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

#endif /* __EV_HC32A4A8_LQFP176_XPT20XX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
