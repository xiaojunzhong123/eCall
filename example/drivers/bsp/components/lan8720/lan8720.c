/**
 *******************************************************************************
 * @file  lan8720.c
 * @brief This midware file provides firmware functions to manage the phy
 *        component library for lan8720.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2025-11-03       CDT             First version
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
#include "lan8720.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup LAN8720 PHY LAN8720
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
 * @defgroup LAN8720_Local_Functions LAN8720 Local Functions
 * @{
 */
/**
 * @brief  Switch link status value to macro define.
 * @param  [in] u16Value             The PHY specific status register value only bit3 & bit4
 * @retval uint16_t:
 *           - LAN8720_LINK_100M_FULL_DUPLEX
 *           - LAN8720_LINK_100M_HALF_DUPLEX
 *           - LAN8720_LINK_10M_FULL_DUPLEX
 *           - LAN8720_LINK_10M_HALF_DUPLEX
 */
static uint16_t LAN8720_LinkStatus(uint16_t u16Value)
{
    uint16_t u16LinkStatus = 0U;

    switch (u16Value) {
        case LAN8720_SSR_FULL_DUPLEX_100M:
            u16LinkStatus = LAN8720_LINK_100M_FULL_DUPLEX;
            break;
        case LAN8720_SSR_HALF_DUPLEX_100M:
            u16LinkStatus = LAN8720_LINK_100M_HALF_DUPLEX;
            break;
        case LAN8720_SSR_FULL_DUPLEX_10M:
            u16LinkStatus = LAN8720_LINK_10M_FULL_DUPLEX;
            break;
        case LAN8720_SSR_HALF_DUPLEX_10M:
            u16LinkStatus = LAN8720_LINK_10M_HALF_DUPLEX;
            break;
        default:
            break;
    }

    return u16LinkStatus;
}

/**
 * @}
 */

/**
 * @defgroup LAN8720_Global_Functions LAN8720 Global Functions
 * @{
 */
/**
 * @brief  Init LAN8720.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @retval int32_t:
 *           - LL_OK: Init success
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is NULL
 *           - LL_ERR: Init error
 */
int32_t LAN8720_Init(const stc_lan8720_ll_t *pstcLan8720LL)
{
    int32_t i32Ret;

    if (NULL == pstcLan8720LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->Init();
    }

    return i32Ret;
}

/**
 * @brief  SOftware reset LAN8720.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @retval int32_t:
 *           - LL_OK: Reset success
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is NULL
 *           - LL_ERR: Reset error
 */
int32_t LAN8720_SW_Reset(const stc_lan8720_ll_t *pstcLan8720LL)
{
    int32_t i32Ret;
    uint16_t u16RegValue;
    uint32_t u32TickStart;

    if (NULL == pstcLan8720LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->WriteReg(LAN8720_R0_BCR, LAN8720_SOFT_RESET);
        if (LL_OK == i32Ret) {
            /* To ensure reset complete */
            u32TickStart = pstcLan8720LL->GetTick();
            do {
                /* Read R0 */
                (void)pstcLan8720LL->ReadReg(LAN8720_R0_BCR, &u16RegValue);
                if ((pstcLan8720LL->GetTick() - u32TickStart) > LAN8720_SW_RST_DELAY) {
                    i32Ret = LL_ERR;
                    break;
                }
            } while (0U != (u16RegValue & LAN8720_SOFT_RESET));
        }
    }

    return i32Ret;
}

/**
 * @brief  Get LAN8720 link mode.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [out] pu16LinkStatus         The link status
 *   @arg  LAN8720_LINK_DOWN
 *   @arg  LAN8720_LINK_AUTO_NEGO_NOT_CPLT
 *   @arg  LAN8720_LINK_100M_FULL_DUPLEX
 *   @arg  LAN8720_LINK_100M_HALF_DUPLEX
 *   @arg  LAN8720_LINK_10M_FULL_DUPLEX
 *   @arg  LAN8720_LINK_10M_HALF_DUPLEX
 * @retval uint32_t:
 *           - LL_OK: Get link mode success
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 */
int32_t LAN8720_GetLinkStatus(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t *pu16LinkStatus)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if ((NULL == pstcLan8720LL) || (NULL == pu16LinkStatus)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        *pu16LinkStatus = 0U;
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R1_BSR, &u16Value);
        if (LL_OK == i32Ret) {
            /* To judge whether establish valid link */
            if (LAN8720_LINK_STAT != (u16Value & LAN8720_LINK_STAT)) {
                /* No valid link */
                *pu16LinkStatus = LAN8720_LINK_DOWN;
            } else {
                /* Valid link */
                i32Ret = pstcLan8720LL->ReadReg(LAN8720_R0_BCR, &u16Value);
                if (LL_OK == i32Ret) {
                    /* To judge whether enable auto-negotiation */
                    if (LAN8720_AUTO_NEGO == (u16Value & LAN8720_AUTO_NEGO)) {
                        /* Auto negotiation */
                        if (LL_OK == pstcLan8720LL->ReadReg(LAN8720_R1_BSR, &u16Value)) {
                            /* To judge whether auto-negotiation complete */
                            if (LAN8720_AUTO_NEGO_CPLT != (u16Value & LAN8720_AUTO_NEGO_CPLT)) {
                                /* auto-negotiation not complete*/
                                *pu16LinkStatus = LAN8720_LINK_AUTO_NEGO_NOT_CPLT;
                            } else {
                                /* auto-negotiation complete*/
                                goto get_sr;
                            }
                        } else {
                            i32Ret = LL_ERR;
                        }
                    } else {
get_sr:
                        if (LL_OK == pstcLan8720LL->ReadReg(LAN8720_R31_PSCSR, &u16Value)) {
                            *pu16LinkStatus = LAN8720_LinkStatus(u16Value & LAN8720_SSR_FULL_DUPLEX_100M);
                        } else {
                            i32Ret = LL_ERR;
                        }
                    }
                }
                if (0U == *pu16LinkStatus) {
                    i32Ret = LL_ERR;
                }
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Set LAN8720 link mode.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [in] u16Mode                 specifies the link mode
 *   @arg  LAN8720_LINK_100M_FULL_DUPLEX
 *   @arg  LAN8720_LINK_100M_HALF_DUPLEX
 *   @arg  LAN8720_LINK_10M_FULL_DUPLEX
 *   @arg  LAN8720_LINK_10M_HALF_DUPLEX
 * @retval int32_t:
 *           - LL_OK: Set link mode success
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 */
int32_t LAN8720_SetLinkMode(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16Mode)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if (NULL == pstcLan8720LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            /* Set the register value */
            switch (u16Mode) {
                case LAN8720_LINK_100M_FULL_DUPLEX:
                    MODIFY_REG16(u16Value, LAN8720_SPEED_BIT | LAN8720_DUPLEX_BIT, LAN8720_FULL_DUPLEX_100M);
                    break;
                case LAN8720_LINK_100M_HALF_DUPLEX:
                    MODIFY_REG16(u16Value, LAN8720_SPEED_BIT | LAN8720_DUPLEX_BIT, LAN8720_HALF_DUPLEX_100M);
                    break;
                case LAN8720_LINK_10M_FULL_DUPLEX:
                    MODIFY_REG16(u16Value, LAN8720_SPEED_BIT | LAN8720_DUPLEX_BIT, LAN8720_FULL_DUPLEX_10M);
                    break;
                case LAN8720_LINK_10M_HALF_DUPLEX:
                    MODIFY_REG16(u16Value, LAN8720_SPEED_BIT | LAN8720_DUPLEX_BIT, LAN8720_HALF_DUPLEX_10M);
                    break;
                default:
                    i32Ret = LL_ERR_INVD_PARAM;
                    break;
            }
            if (LL_OK == i32Ret) {
                /* set link mode */
                if (LL_OK != pstcLan8720LL->WriteReg(LAN8720_R0_BCR, u16Value)) {
                    i32Ret = LL_ERR;
                }
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable LAN8720 auto negotiation.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *           - LL_OK: Auto negotiation success
 *           - LL_ERR_TIMEOUT: auto negotiation timeout
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 */
int32_t LAN8720_AutoNegoCmd(const stc_lan8720_ll_t *pstcLan8720LL, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;
    uint32_t u32TickStart;

    if (NULL == pstcLan8720LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, LAN8720_AUTO_NEGO);
            } else {
                CLR_REG16_BIT(u16Value, LAN8720_AUTO_NEGO);
            }
            if (LL_OK != pstcLan8720LL->WriteReg(LAN8720_R0_BCR, u16Value)) {
                i32Ret = LL_ERR;
            } else {
                if (ENABLE == enNewState) {
                    /* Wait until the auto-negotiation will be completed */
                    u32TickStart = pstcLan8720LL->GetTick();
                    u16Value = 0U;
                    while (LAN8720_AUTO_NEGO_CPLT != (u16Value & LAN8720_AUTO_NEGO_CPLT)) {
                        (void)pstcLan8720LL->ReadReg(LAN8720_R1_BSR, &u16Value);
                        /* Check for the Timeout (3s) */
                        if ((pstcLan8720LL->GetTick() - u32TickStart) > 3000U) {
                            i32Ret = LL_ERR_TIMEOUT;
                            break;
                        }
                    }
                }
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable LAN8720 loopback.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *           - LL_OK: Enable or disable loopback success
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 *           - LL_ERR: Communication error
 */
int32_t LAN8720_LoopBackCmd(const stc_lan8720_ll_t *pstcLan8720LL, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if (NULL == pstcLan8720LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, LAN8720_LOOPBACK);
            } else {
                CLR_REG16_BIT(u16Value, LAN8720_LOOPBACK);
            }
            if (LL_OK != pstcLan8720LL->WriteReg(LAN8720_R0_BCR, u16Value)) {
                i32Ret = LL_ERR;
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable the specifies interrupt.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [in] u16IntType              A @ref LAN8720_Interrupt value
 *   @arg  LAN8720_INT_AUT_NEGO_CPLT
 *   @arg  LAN8720_INT_LINK_DOWN
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value
 * @retval int32_t:
 *           - LL_OK: Enable or disable the interrupt success
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 *           - LL_ERR: Communication error
 */
int32_t LAN8720_IntCmd(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16IntType, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if ((NULL == pstcLan8720LL) || (0U != u16IntType)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R30_IMR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, u16IntType);
            } else {
                CLR_REG16_BIT(u16Value, u16IntType);
            }
            if (LL_OK != pstcLan8720LL->WriteReg(LAN8720_R30_IMR, u16Value)) {
                i32Ret = LL_ERR;
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Get the specifies interrupt status.
 * @param  [in] pstcLan8720LL           Pointer to a @ref stc_lan8720_ll_t structure
 * @param  [in] u16IntType              A @ref LAN8720_Interrupt value
 *   @arg  LAN8720_INT_AUT_NEGO_CPLT
 *   @arg  LAN8720_INT_LINK_DOWN
 * @param  [out] penStatus              An @ref en_flag_status_t enumeration value
 * @retval int32_t:
 *           - LL_OK: Get the interrupt status success
 *           - LL_ERR_INVD_PARAM: pstcLan8720LL is null
 *           - LL_ERR: Communication error
 */
int32_t LAN8720_GetIntStatus(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16IntType, en_flag_status_t *penStatus)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if ((NULL == pstcLan8720LL) || (NULL == penStatus)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcLan8720LL->ReadReg(LAN8720_R29_ISR, &u16Value);
        if (LL_OK == i32Ret) {
            *penStatus = ((READ_REG16_BIT(u16Value, u16IntType) != 0U) ? SET : RESET);
        }
    }

    return i32Ret;
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
