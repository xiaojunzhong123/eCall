/**
 *******************************************************************************
 * @file  ev_hc32a4a8_lqfp176_xpt20xx.c
 * @brief This file provides firmware functions of the touch pad xpt20xx driver
 *        library for the board EV_HC32A4A8_LQFP176.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ev_hc32a4a8_lqfp176_bsp.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176
 * @{
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_XPT20XX EV_HC32A4A8_LQFP176 XPT20XX
 * @{
 */

#if ((BSP_XPT20XX_ENABLE == DDL_ON) && (BSP_EV_HC32A4A8_LQFP176 == BSP_EV_HC32A4XX))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
 * @addtogroup EV_HC32A4A8_LQFP176_XPT20XX_Local_Functions
 * @{
 */
static void BSP_XPT20XX_IO_Init(void);
static uint16_t BSP_XPT20XX_SPI_WriteRead(uint8_t cmd);
static uint8_t BSP_XPT20XX_TouchPinDetect(void);
/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_XPT20XX_Local_Variables EV_HC32A4A8_LQFP176 XPT20XX Local Variables
 * @{
 */
const static stc_xpt20xx_ll_t m_stcXpt20xxLL = {
    .Init  = BSP_XPT20XX_IO_Init,
    .Read  = BSP_XPT20XX_SPI_WriteRead,
    .TouchStatus = BSP_XPT20XX_TouchPinDetect
};

static struct {
    struct {
        float32_t x;
        float32_t y;
    } factor;
    struct {
        uint16_t x;
        uint16_t y;
    } center;
} g_xpt20xx_touch_sta;
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_XPT20XX_Local_Functions EV_HC32A4A8_LQFP176 XPT20XX Local Functions
 * @{
 */

/**
 * @brief  BSP XPT20XX SPI write cmd and read adc raw data
 * @param  [in]  cmd:       need to send cmd
 * @retval uint16_t the adc 12bit raw data
 */
static uint16_t BSP_XPT20XX_SPI_WriteRead(uint8_t cmd)
{
    uint8_t u8Data[3U] = {cmd, 0U, 0U};
    uint16_t u16AdcValue = 0U;

    BSP_IO_WritePortPin(LCD_RTCS_PORT, LCD_RTCS_PIN, EIO_PIN_RESET);
    if (LL_OK == SPI_TransReceive(BSP_XPT20XX_SPI_UNIT, u8Data, u8Data, 3U, SPI_COMM_TIMEOUT_VAL)) {
        u16AdcValue = (uint16_t)(((uint16_t)u8Data[1] << 8U) | u8Data[2]) >> 3U;
    }
    BSP_IO_WritePortPin(LCD_RTCS_PORT, LCD_RTCS_PIN, EIO_PIN_SET);

    return u16AdcValue;
}

/**
 * @brief  Displays a calibration point at the specified position
 * @param  [in]  u16x:       x coordinate
 * @param  [in]  u16y:       y coordinate
 * @param  [in]  color:   display color
 *                        This parameter can be a value of @ref LCD_Color
 * @retval None
 */
static void lcd_draw_touch_point(uint16_t u16x, uint16_t u16y, uint16_t color)
{
    BSP_NT35510_DrawLine(u16x - 12U, u16y, u16x + 13U, u16y, color);
    BSP_NT35510_DrawLine(u16x, u16y - 12U, u16x, u16y + 13U, color);
    BSP_NT35510_WritePixel(u16x + 1U, u16y + 1U, color);
    BSP_NT35510_WritePixel(u16x - 1U, u16y + 1U, color);
    BSP_NT35510_WritePixel(u16x + 1U, u16y - 1U, color);
    BSP_NT35510_WritePixel(u16x - 1U, u16y - 1U, color);
    BSP_NT35510_DrawCircle(u16x, u16y, 6U, color);
}

/**
 * @brief  XPT20XX touch calibration
 * @param  None
 * @retval None
 */
static void BSP_XPT20XX_Touch_Calibration(void)
{
    struct {
        uint16_t x;
        uint16_t y;
    } point[5U];
    uint8_t point_index;
    int32_t d1, d2, d3, d4;
    double x_fac, y_fac;

    BSP_NT35510_Clear(LCD_COLOR_WHITE);

    for (;;) {
        for (point_index = 0U; point_index < 5U; point_index++) {
            switch (point_index) {
                case 0: {
                    lcd_draw_touch_point(LCD_WIDTH >> 1U, LCD_HEIGHT >> 1U, LCD_COLOR_WHITE);
                    lcd_draw_touch_point(20U, 20U, LCD_COLOR_RED);
                    break;
                }
                case 1: {
                    lcd_draw_touch_point(20U, 20U, LCD_COLOR_WHITE);
                    lcd_draw_touch_point(LCD_WIDTH - 20U, 20U, LCD_COLOR_RED);
                    break;
                }
                case 2: {
                    lcd_draw_touch_point(LCD_WIDTH - 20U, 20U, LCD_COLOR_WHITE);
                    lcd_draw_touch_point(20, LCD_HEIGHT - 20U, LCD_COLOR_RED);
                    break;
                }
                case 3: {
                    lcd_draw_touch_point(20, LCD_HEIGHT - 20U, LCD_COLOR_WHITE);
                    lcd_draw_touch_point(LCD_WIDTH - 20U, LCD_HEIGHT - 20U, LCD_COLOR_RED);
                    break;
                }
                case 4: {
                    lcd_draw_touch_point(LCD_WIDTH - 20U, LCD_HEIGHT - 20U, LCD_COLOR_WHITE);
                    lcd_draw_touch_point(LCD_WIDTH >> 1U, LCD_HEIGHT >> 1U, LCD_COLOR_RED);
                    break;
                }
                default: {
                    break;
                }
            }
            DDL_DelayMS(200U);
            /* wait press */
            while (1U != BSP_XPT20XX_TouchDetect()) {
                ;
            }
            point[point_index].x = XPT20XX_GetXY_Adc(&m_stcXpt20xxLL, XPT20XX_TOUCH_CMD_X);
            point[point_index].y = XPT20XX_GetXY_Adc(&m_stcXpt20xxLL, XPT20XX_TOUCH_CMD_Y);
            /* wait release */
            while (1U == BSP_XPT20XX_TouchDetect()) {
                ;
            }
        }

        d1 = ((int32_t)point[1U].x - (int32_t)point[0U].x);
        d3 = ((int32_t)point[3U].x - (int32_t)point[2U].x);
        d2 = ((int32_t)point[3U].y - (int32_t)point[1U].y);
        d4 = ((int32_t)point[2U].y - (int32_t)point[0U].y);

        if (0 != d3 && 0 != d4) {
            x_fac = (double)d1 / (double)d3;
            y_fac = (double)d2 / (double)d4;

            if (x_fac < 0.0f) {
                x_fac = -x_fac;
            }
            if (y_fac < 0.0f) {
                y_fac = -y_fac;
            }
            /* Determine whether the calibration has passed or not */
            if ((fabs(x_fac - 1.0f)) <= 0.05f && (fabs(y_fac - 1.0f)) <= 0.05f && \
                abs(d1) < 4095 && abs(d2) < 4095 && abs(d3) < 4095 && abs(d4) < 4095 && \
                abs(d1) != 0 && abs(d2) != 0 && abs(d3) != 0 && abs(d4) != 0) {
                /* get scale factor */
                g_xpt20xx_touch_sta.factor.x = (float32_t)((double)d1 + (double)d3) / (float32_t)((LCD_WIDTH - 40U) << 1U);
                g_xpt20xx_touch_sta.factor.y = (float32_t)((double)d2 + (double)d4) / (float32_t)((LCD_HEIGHT - 40U) << 1U);
                /* get center points adc values */
                g_xpt20xx_touch_sta.center.x = point[4U].x;
                g_xpt20xx_touch_sta.center.y = point[4U].y;

                BSP_NT35510_Clear(LCD_COLOR_WHITE);

                return;
            }
        }
    }
}

/**
 * @brief  Initializes IO for XPT20XX
 * @param  None
 * @retval None
 */
static void BSP_XPT20XX_IO_Init(void)
{
    stc_spi_init_t stcSpiInit;
    stc_gpio_init_t stcGpioInit;

    /* Configure SPI Port */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv       = PIN_HIGH_DRV;
    stcGpioInit.u16PinInputType = PIN_IN_TYPE_CMOS;
    (void)GPIO_Init(BSP_XPT20XX_SPI_SCK_PORT,  BSP_XPT20XX_SPI_SCK_PIN,  &stcGpioInit);
    (void)GPIO_Init(BSP_XPT20XX_SPI_MOSI_PORT, BSP_XPT20XX_SPI_MOSI_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_XPT20XX_SPI_MISO_PORT, BSP_XPT20XX_SPI_MISO_PIN, &stcGpioInit);
    /* Configure IO Func */
    GPIO_SetFunc(BSP_XPT20XX_SPI_SCK_PORT,  BSP_XPT20XX_SPI_SCK_PIN,  BSP_XPT20XX_SPI_SCK_FUNC);
    GPIO_SetFunc(BSP_XPT20XX_SPI_MOSI_PORT, BSP_XPT20XX_SPI_MOSI_PIN, BSP_XPT20XX_SPI_MOSI_FUNC);
    GPIO_SetFunc(BSP_XPT20XX_SPI_MISO_PORT, BSP_XPT20XX_SPI_MISO_PIN, BSP_XPT20XX_SPI_MISO_FUNC);
    /* Configuration SPI */
    FCG_Fcg1PeriphClockCmd(BSP_XPT20XX_SPI_FCG, ENABLE);
    (void)SPI_StructInit(&stcSpiInit);
    stcSpiInit.u32WireMode          = SPI_3_WIRE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32Parity            = SPI_PARITY_INVD;
    stcSpiInit.u32SpiMode           = SPI_MD_0;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV64;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    stcSpiInit.u32FrameLevel        = SPI_1_FRAME;
    (void)SPI_Init(BSP_XPT20XX_SPI_UNIT, &stcSpiInit);
    SPI_Cmd(BSP_XPT20XX_SPI_UNIT, ENABLE);
    /* CS */
    BSP_IO_ConfigPortPin(LCD_RTCS_PORT, LCD_RTCS_PIN, EIO_DIR_OUT);
    /* TOUCH_INT */
    BSP_IO_ConfigPortPin(LCD_CTINT_PORT, LCD_CTINT_PIN, EIO_DIR_IN);
}

/**
 * @brief  XPT20XX touch pin detection
 * @param  None
 * @retval uint8_t pin status
 */
static uint8_t BSP_XPT20XX_TouchPinDetect(void)
{
    return BSP_IO_ReadPortPin(LCD_CTINT_PORT, LCD_CTINT_PIN);
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_XPT20XX_Global_Functions EV_HC32A4A8_LQFP176 XPT20XX Global Functions
 * @{
 */

/**
 * @brief  XPT20XX touch detection
 * @param  None
 * @retval uint8_t touch status
 */
uint8_t BSP_XPT20XX_TouchDetect(void)
{
    return XPT20XX_GetTouchStatus(&m_stcXpt20xxLL);
}

/**
 * @brief  BSP XPT20XX get x/y coordinate
 * @param  [out]  pu16x:         x coordinate
 * @param  [out]  pu16y:         y coordinate
 * @retval None
 */
void BSP_XPT20XX_GET_XY(uint16_t *pu16x, uint16_t *pu16y)
{
    uint16_t u16x_adc;
    uint16_t u16y_adc;
    uint16_t u16coord_x;
    uint16_t u16coord_y;
    float32_t fOffset;

    if (1U == BSP_XPT20XX_TouchDetect()) {
        u16x_adc = XPT20XX_GetXY_Adc(&m_stcXpt20xxLL, XPT20XX_TOUCH_CMD_X);
        u16y_adc = XPT20XX_GetXY_Adc(&m_stcXpt20xxLL, XPT20XX_TOUCH_CMD_Y);
        /* X-coordinate transformation */
        fOffset = ((float32_t)u16x_adc - (float32_t)g_xpt20xx_touch_sta.center.x) / g_xpt20xx_touch_sta.factor.x;
        fOffset += (float32_t)(LCD_WIDTH >> 1U);
        u16coord_x = (uint16_t)fOffset;
        /* Y-coordinate transformation */
        fOffset = ((float32_t)u16y_adc - (float32_t)g_xpt20xx_touch_sta.center.y) / g_xpt20xx_touch_sta.factor.y;
        fOffset += (float32_t)(LCD_HEIGHT >> 1U);
        u16coord_y = (uint16_t)fOffset;

        if ((u16coord_x >= LCD_WIDTH) || (u16coord_y >= LCD_HEIGHT)) {
            return;
        }

#if (LCD_DISP_DIR == LCD_DISP_DIR_0)
        *pu16x = u16coord_x;
        *pu16y = u16coord_y;
#elif (LCD_DISP_DIR == LCD_DISP_DIR_90)
        *pu16x = u16coord_y;
        *pu16y = LCD_HEIGHT - u16coord_x;
#elif (LCD_DISP_DIR == LCD_DISP_DIR_180)
        *pu16x = LCD_WIDTH - u16coord_x;
        *pu16y = LCD_HEIGHT - u16coord_y;
#else
        *pu16x = LCD_WIDTH - u16coord_y;
        *pu16y = u16coord_x;
#endif
    }
}

/**
 * @brief  XPT20XX device initialize.
 * @param  None
 * @retval None
 */
void BSP_XPT20XX_Init(void)
{
    /* Initialize HW peripheral */
    BSP_XPT20XX_IO_Init();
    /* Touch Calibration */
    BSP_XPT20XX_Touch_Calibration();
}

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

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
