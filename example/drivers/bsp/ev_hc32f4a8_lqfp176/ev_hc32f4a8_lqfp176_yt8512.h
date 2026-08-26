/**
 *******************************************************************************
 * @file  ev_hc32f4a8_lqfp176_yt8512.h
 * @brief This file contains all the functions prototypes for yt8512 of the
 *        board EV_HC32F4A8_LQFP176.
 @verbatim
   Change Logs:
   Date             Author          Notes
   xxxx-xx-xx       CDT             First version
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
#ifndef __EV_HC32F4A8_LQFP176_YT8512_H__
#define __EV_HC32F4A8_LQFP176_YT8512_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "yt8512.h"
#include "hc32_ll_eth.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32F4A8_LQFP176
 * @{
 */

/**
 * @addtogroup EV_HC32F4A8_LQFP176_YT8512
 * @{
 */

#if ((BSP_YT8512_ENABLE == DDL_ON) && (BSP_EV_HC32F4A8_LQFP176 == BSP_EV_HC32F4XX))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A8_LQFP176_YT8512_Global_Macros  EV_HC32F4A8_LQFP176 YT8512 Global Macros
 * @{
 */
/* YT8512 device address */
#define BSP_YT8512_DEV_ADDR             (0x01U)

/**
 * @defgroup BSP_YT8512_SMI_Port_configure BSP YT8512 ETH SMI Port Configure
 * @{
 */
#define BSP_YT8512_SMI_MDIO_PORT        (GPIO_PORT_A)
#define BSP_YT8512_SMI_MDIO_PIN         (GPIO_PIN_02)
#define BSP_YT8512_SMI_MDC_PORT         (GPIO_PORT_C)
#define BSP_YT8512_SMI_MDC_PIN          (GPIO_PIN_01)
#define BSP_YT8512_SMI_FUNC             (GPIO_FUNC_11)
/**
 * @}
 */

/**
 * @defgroup BSP_YT8512_Reset_Mode BSP YT8512 Reset Mode
 * @{
 */
#define BSP_YT8512_RST_MD_SW            (0x01U)
#define BSP_YT8512_RST_MD_HW            (0x02U)
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
 * @addtogroup EV_HC32F4A8_LQFP176_YT8512_Global_Functions
 * @{
 */

int32_t BSP_YT8512_Init(void);
int32_t BSP_YT8512_Reset(uint16_t u16Mode);
int32_t BSP_YT8512_AutoNegoCmd(en_functional_state_t enNewState);
int32_t BSP_YT8512_SetLinkMode(uint32_t u32Mode);
int32_t BSP_YT8512_SetLedMode(uint32_t u32Mode);
int32_t BSP_YT8512_IntCmd(uint16_t u16IntType, en_functional_state_t enNewState);
int32_t BSP_YT8512_LoopBackCmd(en_functional_state_t enNewState);
int32_t BSP_YT8512_GetLinkStatus(uint16_t *pu16LinkStatus);
int32_t BSP_YT8512_GetIntStatus(uint16_t u16IntType, en_flag_status_t *penStatus);

/**
 * @}
 */

#endif /* (BSP_YT8512_ENABLE && BSP_EV_HC32F4A8_LQFP176) */

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

#endif /* __EV_HC32F4A8_LQFP176_YT8512_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
