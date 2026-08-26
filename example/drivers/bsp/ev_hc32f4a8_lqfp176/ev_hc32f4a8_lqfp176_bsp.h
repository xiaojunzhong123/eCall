/**
 *******************************************************************************
 * @file  ev_hc32f4a8_lqfp176_bsp.h
 * @brief This file contains all the header file of the EV_HC32F4A8_LQFP176
 *        BSP driver library.
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

#ifndef __EV_HC32F4A8_LQFP176_BSP__
#define __EV_HC32F4A8_LQFP176_BSP__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
/**
 * @brief Include BSP board's header file
 */
#if (BSP_EV_HC32F4A8_LQFP176 == BSP_EV_HC32F4XX)
#include "ev_hc32a4a8_lqfp176.h"
#endif /* BSP_EV_HC32F4A8_LQFP176 */

/**
 * @brief Include BSP device component's header file
 */
#if (BSP_24CXX_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_24cxx.h"
#endif /* BSP_24CXX_ENABLE */

#if (BSP_IS62WV51216_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_is62wv51216.h"
#endif /* BSP_IS62WV51216_ENABLE */

#if (BSP_LAN8720_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_lan8720.h"
#endif /* BSP_LAN8720_ENABLE */

#if (BSP_MT29F2G08AB_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_mt29f2g08ab.h"
#endif /* BSP_MT29F2G08AB_ENABLE */

#if (BSP_NT35510_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_nt35510.h"
#endif /* BSP_NT35510_ENABLE */

#if (BSP_OV5640_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_ov5640.h"
#endif /* BSP_OV5640_ENABLE */

#if (BSP_RTL8201_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_rtl8201.h"
#endif /* BSP_RTL8201_ENABLE */

#if (BSP_TCA9539_ENABLE == DDL_ON)
#include "ev_hc32a4a8_lqfp176_tca9539.h"
#endif /* BSP_TCA9539_ENABLE */

#if (BSP_W25QXX_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_w25qxx.h"
#endif /* BSP_W25QXX_ENABLE */

#if (BSP_W9825G6KH_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_w9825g6kh.h"
#endif /* BSP_W9825G6KH_ENABLE */

#if (BSP_WM8988_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_wm8988.h"
#endif /* BSP_WM8988_ENABLE */

#if (BSP_XPT20XX_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_xpt20xx.h"
#endif /* BSP_XPT20XX_ENABLE */

#if (BSP_YT8512_ENABLE == DDL_ON)
#include "ev_hc32f4a8_lqfp176_yt8512.h"
#endif /* BSP_YT8512_ENABLE */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __EV_HC32F4A8_LQFP176_BSP__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
