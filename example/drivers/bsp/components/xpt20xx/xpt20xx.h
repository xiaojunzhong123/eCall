/**
 *******************************************************************************
 * @file  xpt20xx.h
 * @brief This file contains all the functions prototypes of the touch pad XPT20XX
 *        driver library.
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
#ifndef __XPT20XX_H__
#define __XPT20XX_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_def.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @addtogroup XPT20XX
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup XPT20XX_Global_Types XPT20XX Global Types
 * @{
 */

/**
 * @brief XPT20XX low layer structure definition
 */
typedef struct {
    /* Methods */
    void (*Init)(void);
    uint16_t (*Read)(uint8_t cmd);
    uint8_t (*TouchStatus)(void);
} stc_xpt20xx_ll_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup XPT20XX_Local_Macros XPT20XX Local Macros
 * @{
 */

#define XPT20XX_FILTER_CNT               (5U)
#define XPT20XX_DELTA_THRESHOLD          (50U)

/**
 * @}
 */

/**
 * @defgroup XPT20XX_Global_Macros XPT20XX Global Macros
 * @{
 */

/* cmd */
#define XPT20XX_TOUCH_CMD_X              (0xD0)
#define XPT20XX_TOUCH_CMD_Y              (0x90)

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
 * @addtogroup XPT20XX_Global_Functions
 * @{
 */
uint16_t XPT20XX_GetXY_Adc(const stc_xpt20xx_ll_t *pstcXpt20xxLL, uint8_t cmd);
uint8_t XPT20XX_GetTouchStatus(const stc_xpt20xx_ll_t *pstcXpt20xxLL);
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

#ifdef __cplusplus
}
#endif

#endif /* __XPT20XX_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
