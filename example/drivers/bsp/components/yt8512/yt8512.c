/**
 *******************************************************************************
 * @file  yt8512.c
 * @brief This midware file provides firmware functions to manage the phy
 *        component library for yt8512.
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
#include "yt8512.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup YT8512 PHY YT8512
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
 * @defgroup YT8512_Local_Functions YT8512 Local Functions
 * @{
 */
/**
 * @brief  Switch link status value to macro define.
 * @param  [in] u16Value                The PHY specific status register value only bit13 & bit14
 * @retval uint16_t:
 *           - YT8512_LINK_100M_FULL_DUPLEX
 *           - YT8512_LINK_100M_HALF_DUPLEX
 *           - YT8512_LINK_10M_FULL_DUPLEX
 *           - YT8512_LINK_10M_HALF_DUPLEX
 */
static uint16_t YT8512_LinkStatus(uint16_t u16Value)
{
    uint16_t u16LinkStatus = 0U;

    switch (u16Value) {
        case YT8512_SSR_FULL_DUPLEX_100M:
            u16LinkStatus = YT8512_LINK_100M_FULL_DUPLEX;
            break;
        case YT8512_SSR_HALF_DUPLEX_100M:
            u16LinkStatus = YT8512_LINK_100M_HALF_DUPLEX;
            break;
        case YT8512_SSR_FULL_DUPLEX_10M:
            u16LinkStatus = YT8512_LINK_10M_FULL_DUPLEX;
            break;
        case YT8512_SSR_HALF_DUPLEX_10M:
            u16LinkStatus = YT8512_LINK_10M_HALF_DUPLEX;
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
 * @defgroup YT8512_Global_Functions YT8512 Global Functions
 * @{
 */
/**
 * @brief  Init YT8512.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @retval int32_t:
 *           - LL_OK: Init success
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is NULL
 *           - LL_ERR: Init error
 */
int32_t YT8512_Init(const stc_yt8512_ll_t *pstcYt8512LL)
{
    int32_t i32Ret;

    if (NULL == pstcYt8512LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->Init();
    }

    return i32Ret;
}

/**
 * @brief  SOftware reset YT8512.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @retval int32_t:
 *           - LL_OK: Reset success
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is NULL
 *           - LL_ERR: Reset error
 */
int32_t YT8512_SW_Reset(const stc_yt8512_ll_t *pstcYt8512LL)
{
    int32_t i32Ret;
    uint16_t u16RegValue;
    uint32_t u32TickStart;

    if (NULL == pstcYt8512LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->WriteReg(YT8512_R0_BCR, YT8512_SOFT_RESET);
        if (LL_OK == i32Ret) {
            /* To ensure reset complete */
            u32TickStart = pstcYt8512LL->GetTick();
            do {
                /* Read R0 */
                (void)pstcYt8512LL->ReadReg(YT8512_R0_BCR, &u16RegValue);
                if ((pstcYt8512LL->GetTick() - u32TickStart) > YT8512_SW_RST_DELAY) {
                    i32Ret = LL_ERR;
                    break;
                }
            } while (0U != (u16RegValue & YT8512_SOFT_RESET));
        }
    }

    return i32Ret;
}

/**
 * @brief  Get YT8512 link mode.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [out] pu16LinkStatus         The link status
 *   @arg  YT8512_LINK_DOWN
 *   @arg  YT8512_LINK_AUTO_NEGO_NOT_CPLT
 *   @arg  YT8512_LINK_100M_FULL_DUPLEX
 *   @arg  YT8512_LINK_100M_HALF_DUPLEX
 *   @arg  YT8512_LINK_10M_FULL_DUPLEX
 *   @arg  YT8512_LINK_10M_HALF_DUPLEX
 * @retval uint32_t:
 *           - LL_OK: Get link mode success
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 */
int32_t YT8512_GetLinkStatus(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t *pu16LinkStatus)
{
    int32_t i32Ret;
    uint16_t u16Value;
    uint16_t u16SpecValue;

    if ((NULL == pstcYt8512LL) || (NULL == pu16LinkStatus)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        *pu16LinkStatus = 0U;
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R17_PSSR, &u16SpecValue);
        if (LL_OK == i32Ret) {
            /* To judge whether establish valid link */
            if (YT8512_SSR_LINK_STAT_REALTIME != (u16SpecValue & YT8512_SSR_LINK_STAT_REALTIME)) {
                /* No valid link */
                *pu16LinkStatus = YT8512_LINK_DOWN;
            } else {
                /* Valid link */
                i32Ret = pstcYt8512LL->ReadReg(YT8512_R0_BCR, &u16Value);
                if (LL_OK == i32Ret) {
                    /* To judge whether enable auto-negotiation */
                    if (YT8512_AUTO_NEGO == (u16Value & YT8512_AUTO_NEGO)) {
                        /* Auto negotiation */
                        if (LL_OK == pstcYt8512LL->ReadReg(YT8512_R1_BSR, &u16Value)) {
                            /* To judge whether auto-negotiation complete */
                            if (YT8512_AUTO_NEGO_CPLT != (u16Value & YT8512_AUTO_NEGO_CPLT)) {
                                /* auto-negotiation not complete*/
                                *pu16LinkStatus = YT8512_LINK_AUTO_NEGO_NOT_CPLT;
                            } else {
                                /* auto-negotiation complete*/
                                if (LL_OK == pstcYt8512LL->ReadReg(YT8512_R17_PSSR, &u16SpecValue)) {
                                    *pu16LinkStatus = YT8512_LinkStatus(u16SpecValue & YT8512_SSR_FULL_DUPLEX_100M);
                                } else {
                                    i32Ret = LL_ERR;
                                }
                            }
                        } else {
                            i32Ret = LL_ERR;
                        }
                    } else {
                        /* Not auto negotiation */
                        *pu16LinkStatus = YT8512_LinkStatus(u16SpecValue & YT8512_SSR_FULL_DUPLEX_100M);
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
 * @brief  Set YT8512 link mode.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [in] u16Mode                 specifies the link mode
 *   @arg  YT8512_LINK_100M_FULL_DUPLEX
 *   @arg  YT8512_LINK_100M_HALF_DUPLEX
 *   @arg  YT8512_LINK_10M_FULL_DUPLEX
 *   @arg  YT8512_LINK_10M_HALF_DUPLEX
 * @retval int32_t:
 *           - LL_OK: Set link mode success
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 */
int32_t YT8512_SetLinkMode(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16Mode)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if (NULL == pstcYt8512LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            /* Set the register value */
            switch (u16Mode) {
                case YT8512_LINK_100M_FULL_DUPLEX:
                    MODIFY_REG16(u16Value, YT8512_SPEED_BIT | YT8512_DUPLEX_BIT, YT8512_FULL_DUPLEX_100M);
                    break;
                case YT8512_LINK_100M_HALF_DUPLEX:
                    MODIFY_REG16(u16Value, YT8512_SPEED_BIT | YT8512_DUPLEX_BIT, YT8512_HALF_DUPLEX_100M);
                    break;
                case YT8512_LINK_10M_FULL_DUPLEX:
                    MODIFY_REG16(u16Value, YT8512_SPEED_BIT | YT8512_DUPLEX_BIT, YT8512_FULL_DUPLEX_10M);
                    break;
                case YT8512_LINK_10M_HALF_DUPLEX:
                    MODIFY_REG16(u16Value, YT8512_SPEED_BIT | YT8512_DUPLEX_BIT, YT8512_HALF_DUPLEX_10M);
                    break;
                default:
                    i32Ret = LL_ERR_INVD_PARAM;
                    break;
            }
            if (LL_OK == i32Ret) {
                /* set link mode */
                if (LL_OK != pstcYt8512LL->WriteReg(YT8512_R0_BCR, u16Value)) {
                    i32Ret = LL_ERR;
                }
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable YT8512 auto negotiation.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *           - LL_OK: Auto negotiation success
 *           - LL_ERR_TIMEOUT: auto negotiation timeout
 *           - LL_ERR: Communication error
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 */
int32_t YT8512_AutoNegoCmd(const stc_yt8512_ll_t *pstcYt8512LL, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;
    uint32_t u32TickStart;

    if (NULL == pstcYt8512LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, YT8512_AUTO_NEGO);
            } else {
                CLR_REG16_BIT(u16Value, YT8512_AUTO_NEGO);
            }
            if (LL_OK != pstcYt8512LL->WriteReg(YT8512_R0_BCR, u16Value)) {
                i32Ret = LL_ERR;
            } else {
                if (ENABLE == enNewState) {
                    /* Wait until the auto-negotiation will be completed */
                    u32TickStart = pstcYt8512LL->GetTick();
                    u16Value = 0U;
                    while (YT8512_AUTO_NEGO_CPLT != (u16Value & YT8512_AUTO_NEGO_CPLT)) {
                        (void)pstcYt8512LL->ReadReg(YT8512_R1_BSR, &u16Value);
                        /* Check for the Timeout (3s) */
                        if ((pstcYt8512LL->GetTick() - u32TickStart) > 3000U) {
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
 * @brief  Enable or disable YT8512 loopback.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *           - LL_OK: Enable or disable loopback success
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 *           - LL_ERR: Communication error
 */
int32_t YT8512_LoopBackCmd(const stc_yt8512_ll_t *pstcYt8512LL, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if (NULL == pstcYt8512LL) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R0_BCR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, YT8512_LOOPBACK);
            } else {
                CLR_REG16_BIT(u16Value, YT8512_LOOPBACK);
            }
            if (LL_OK != pstcYt8512LL->WriteReg(YT8512_R0_BCR, u16Value)) {
                i32Ret = LL_ERR;
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable the specifies interrupt.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [in] u16IntType              A @ref YT8512_Interrupt value
 *   @arg  YT8512_INT_AUT_NEGO_ERR
 *   @arg  YT8512_INT_SPEED_MD_CHANGE
 *   @arg  YT8512_INT_DUPLEX_MD_CHANGE
 *   @arg  YT8512_INT_LINK_FAILED
 *   @arg  YT8512_INT_LINK_SUCCEED
 * @param  [in] enNewState              An @ref en_functional_state_t enumeration value
 * @retval int32_t:
 *           - LL_OK: Enable or disable the interrupt success
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 *           - LL_ERR: Communication error
 */
int32_t YT8512_IntCmd(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16IntType, en_functional_state_t enNewState)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if ((NULL == pstcYt8512LL) || (0U != u16IntType)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R18_IMR, &u16Value);
        if (LL_OK == i32Ret) {
            if (ENABLE == enNewState) {
                SET_REG16_BIT(u16Value, u16IntType);
            } else {
                CLR_REG16_BIT(u16Value, u16IntType);
            }
            if (LL_OK != pstcYt8512LL->WriteReg(YT8512_R18_IMR, u16Value)) {
                i32Ret = LL_ERR;
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Get the specifies interrupt status.
 * @param  [in] pstcYt8512LL            Pointer to a @ref stc_yt8512_ll_t structure
 * @param  [in] u16IntType              A @ref YT8512_Interrupt value
 *   @arg  YT8512_INT_AUT_NEGO_ERR
 *   @arg  YT8512_INT_SPEED_MD_CHANGE
 *   @arg  YT8512_INT_DUPLEX_MD_CHANGE
 *   @arg  YT8512_INT_LINK_FAILED
 *   @arg  YT8512_INT_LINK_SUCCEED
 * @param  [out] penStatus              An @ref en_flag_status_t enumeration value
 * @retval int32_t:
 *           - LL_OK: Get the interrupt status success
 *           - LL_ERR_INVD_PARAM: pstcYt8512LL is null
 *           - LL_ERR: Communication error
 */
int32_t YT8512_GetIntStatus(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16IntType, en_flag_status_t *penStatus)
{
    int32_t i32Ret;
    uint16_t u16Value;

    if ((NULL == pstcYt8512LL) || (NULL == penStatus)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        i32Ret = pstcYt8512LL->ReadReg(YT8512_R19_ISR, &u16Value);
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
