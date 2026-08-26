/**
 *******************************************************************************
 * @file  ev_hc32f4a8_lqfp176_lan8720.h
 * @brief This file contains all the functions prototypes for lan8720 of the
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
#ifndef __EV_HC32F4A8_LQFP176_LAN8720_H__
#define __EV_HC32F4A8_LQFP176_LAN8720_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lan8720.h"
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
 * @addtogroup EV_HC32F4A8_LQFP176_LAN8720
 * @{
 */

#if ((BSP_LAN8720_ENABLE == DDL_ON) && (BSP_EV_HC32F4A8_LQFP176 == BSP_EV_HC32F4XX))

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32F4A8_LQFP176_LAN8720_Global_Macros  EV_HC32F4A8_LQFP176 LAN8720 Global Macros
 * @{
 */
/* LAN8720 device address */
#define BSP_LAN8720_DEV_ADDR             (0x01U)

/**
 * @defgroup BSP_LAN8720_SMI_Port_configure BSP LAN8720 ETH SMI Port Configure
 * @{
 */
#define BSP_LAN8720_SMI_MDIO_PORT        (GPIO_PORT_A)
#define BSP_LAN8720_SMI_MDIO_PIN         (GPIO_PIN_02)
#define BSP_LAN8720_SMI_MDC_PORT         (GPIO_PORT_C)
#define BSP_LAN8720_SMI_MDC_PIN          (GPIO_PIN_01)
#define BSP_LAN8720_SMI_FUNC             (GPIO_FUNC_11)
/**
 * @}
 */

/**
 * @defgroup BSP_LAN8720_Reset_Mode BSP LAN8720 Reset Mode
 * @{
 */
#define BSP_LAN8720_RST_MD_SW            (0x01U)
#define BSP_LAN8720_RST_MD_HW            (0x02U)
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
 * @addtogroup EV_HC32F4A8_LQFP176_LAN8720_Global_Functions
 * @{
 */

int32_t BSP_LAN8720_Init(void);
int32_t BSP_LAN8720_Reset(uint16_t u16Mode);
int32_t BSP_LAN8720_AutoNegoCmd(en_functional_state_t enNewState);
int32_t BSP_LAN8720_SetLinkMode(uint32_t u32Mode);
int32_t BSP_LAN8720_SetLedMode(uint32_t u32Mode);
int32_t BSP_LAN8720_IntCmd(uint16_t u16IntType, en_functional_state_t enNewState);
int32_t BSP_LAN8720_LoopBackCmd(en_functional_state_t enNewState);
int32_t BSP_LAN8720_GetLinkStatus(uint16_t *pu16LinkStatus);
int32_t BSP_LAN8720_GetIntStatus(uint16_t u16IntType, en_flag_status_t *penStatus);

/**
 * @}
 */

#endif /* (BSP_LAN8720_ENABLE && BSP_EV_HC32F4A8_LQFP176) */

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

#endif /* __EV_HC32F4A8_LQFP176_LAN8720_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
