/**
 *******************************************************************************
 * @file  xpt20xx.c
 * @brief This file provides firmware functions for Touch Pad XPT20XX.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-08-31       CDT             First version
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
#include "xpt20xx.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup XPT20XX Touch Pad XPT20XX
 * @{
 */

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup XPT20XX_Local_Functions XPT20XX Local Functions
 * @{
 */

/**
 * @brief  XPT20XX Get the filtered value of the adc
 * @note   Sample XPT20XX_FILTER_CNT times in a row, remove the maximum and minimum values to find the average value
 * @param  [in]  pstcXpt20xxLL          Pointer to a @ref stc_xpt20xx_ll_t structure
 * @param  [in]  cmd:                   need to send cmd
 * @retval uint16_t the filtered value
 */
static uint16_t XPT20XX_GET_AdcFilter(const stc_xpt20xx_ll_t *pstcXpt20xxLL, uint8_t cmd)
{
    uint16_t i, j;
    uint16_t u16Temp;
    uint32_t u32AdcValueSum = 0U;
    uint16_t au16AdcValue[XPT20XX_FILTER_CNT] = {0};

    if (pstcXpt20xxLL != NULL && pstcXpt20xxLL->Read != NULL) {
        /* get data */
        for (i = 0; i < XPT20XX_FILTER_CNT; i++) {
            au16AdcValue[i] = pstcXpt20xxLL->Read(cmd);
        }
        /* bubble sort */
        for (i = 0U; i < XPT20XX_FILTER_CNT; i++) {
            for (j = 0U; j < XPT20XX_FILTER_CNT - 1U - i; j++) {
                if (au16AdcValue[j] > au16AdcValue[j + 1U]) {
                    u16Temp = au16AdcValue[j];
                    au16AdcValue[j] = au16AdcValue[j + 1U];
                    au16AdcValue[j + 1U] = u16Temp;
                }
            }
        }
        /* get average */
        for (i = 1U; i < XPT20XX_FILTER_CNT - 1U; i++) {
            u32AdcValueSum += au16AdcValue[i];
        }
    }

    return (uint16_t)(u32AdcValueSum / (XPT20XX_FILTER_CNT - 2U));
}

/**
 * @}
 */

/**
 * @defgroup XPT20XX_Global_Functions XPT20XX Global Functions
 * @{
 */

/**
 * @brief  XPT20XX Gets the X/Y position adc value
 * @param  [in]  pstcXpt20xxLL          Pointer to a @ref stc_xpt20xx_ll_t structure
 * @param  [in]  cmd:                   need to send cmd
 * @retval uint16_t the filtered value
 */
uint16_t XPT20XX_GetXY_Adc(const stc_xpt20xx_ll_t *pstcXpt20xxLL, uint8_t cmd)
{
    uint16_t u16delta;
    uint16_t au16dat[2];

    do {
        au16dat[0] = XPT20XX_GET_AdcFilter(pstcXpt20xxLL, cmd);
        au16dat[1] = XPT20XX_GET_AdcFilter(pstcXpt20xxLL, cmd);
        if (au16dat[0] > au16dat[1]) {
            u16delta = au16dat[0] - au16dat[1];
        } else {
            u16delta = au16dat[1] - au16dat[0];
        }
    } while (u16delta > XPT20XX_DELTA_THRESHOLD);

    return ((au16dat[0] + au16dat[1]) >> 1U);
}

/**
 * @brief  Get XPT20XX touch status
 * @param  [in] pstcXpt20xxLL           Pointer to a @ref stc_xpt20xx_ll_t structure
 * @retval Touch status
 */
uint8_t XPT20XX_GetTouchStatus(const stc_xpt20xx_ll_t *pstcXpt20xxLL)
{
    uint8_t u8Status = 0U;
    if (pstcXpt20xxLL != NULL && pstcXpt20xxLL->TouchStatus != NULL) {
        if (0U == pstcXpt20xxLL->TouchStatus()) {
            u8Status = 1U;
        };
    }

    return u8Status;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
