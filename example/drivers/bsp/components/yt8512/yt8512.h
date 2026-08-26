/**
 *******************************************************************************
 * @file  yt8512.h
 * @brief This file contains all the functions prototypes of the phy component
 *        library for yt8512.
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
#ifndef __YT8512_H__
#define __YT8512_H__

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
 * @addtogroup YT8512
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup YT8512_Global_Types YT8512 Global Types
 * @{
 */

/**
 * @brief YT8512 low layer structure definition
 */
typedef struct {
    /* Methods */
    int32_t (*Init)(void);
    int32_t (*ReadReg)(uint16_t u16Reg, uint16_t *u16Value);
    int32_t (*WriteReg)(uint16_t u16Reg, uint16_t u16Value);
    uint32_t (*GetTick)(void);
} stc_yt8512_ll_t;

/**
 * @}
 */


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup YT8512_Global_Macros YT8512 Global Macros
 * @{
 */

/**
 * @defgroup YT8512_Delay YT8512 Delay
 * @{
 */
#define YT8512_HW_RST_DELAY                 (20U) /*!< For a complete reset, Set RESET_N pin to low at least 10ms */
#define YT8512_SW_RST_DELAY                 (40U) /*!< For a software reset, at least 20ms */
#define YT8512_RD_TIMEOUT                   (10U)
/**
 * @}
 */

/**
 * @defgroup YT8512_Register YT8512 Register
 * @{
 */
#define YT8512_R0_BCR                       (0x00U) /*!< Basic Control Register                             */
#define YT8512_R1_BSR                       (0x01U) /*!< Basic Status Register                              */
#define YT8512_R2_PIDT1                     (0x02U) /*!< PHY Identifier Register 1                          */
#define YT8512_R3_PIDT2                     (0x03U) /*!< PHY Identifier Register 2                          */
#define YT8512_R4_ANAR                      (0x04U) /*!< Auto-Negotiation Advertisement Register            */
#define YT8512_R5_ANLPAR                    (0x05U) /*!< Auto-Negotiation Link Partner Ability Register     */
#define YT8512_R6_ANER                      (0x06U) /*!< Auto-Negotiation Expansion Register                */
#define YT8512_R7_ANNPR                     (0x07U) /*!< Auto-Negotiation Next Page Register                */
#define YT8512_R8_ANLPR_NPR                 (0x08U) /*!< Auto-Negotiation Link Partner Received Next Page Register  */
#define YT8512_R10_MSSR                     (0x0AU) /*!< MASTER-SLAVE Status Register                       */
#define YT8512_R13_MACR                     (0x0DU) /*!< MMD Access Control Register                        */
#define YT8512_R14_MADR                     (0x0EU) /*!< MMD Access Data Register                           */
#define YT8512_R15_ESR                      (0x0FU) /*!< Extended Status Register                           */
#define YT8512_R16_PSFCR                    (0x10U) /*!< PHY specific function control register             */
#define YT8512_R17_PSSR                     (0x11U) /*!< PHY specific status register                       */
#define YT8512_R18_IMR                      (0x12U) /*!< Interrupt Mask Register                            */
#define YT8512_R19_ISR                      (0x13U) /*!< Interrupt Status Register                          */
#define YT8512_R20_SADCR                    (0x14U) /*!< Speed Auto Downgrade Control Register              */
#define YT8512_R21_RECR                     (0x15U) /*!< Rx Error Counter Register                          */
#define YT8512_R30_DRAOR                    (0x1EU) /*!< Debug Register’s Address Offset Register           */
#define YT8512_R31_DRDR                     (0x1FU) /*!< Debug Register’s Data Register                     */
/**
 * @}
 */

/**
 * @defgroup YT8512_Basic_Control_Register YT8512 Basic Control Register
 * @{
 */
#define YT8512_SOFT_RESET                   (0x8000U)   /*!< PHY Soft Reset                         */
#define YT8512_LOOPBACK                     (0x4000U)   /*!< Select loop-back mode                  */
#define YT8512_SPEED_BIT                    (0x2040U)   /*!< The speed control bit                  */
#define YT8512_AUTO_NEGO                    (0x1000U)   /*!< Enable Auto-Negotiation function       */
#define YT8512_POWER_DOWN                   (0x0800U)   /*!< Select the power down mode             */
#define YT8512_ISOLATE                      (0x0400U)   /*!< Isolate PHY from MII                   */
#define YT8512_RESTART_AUTO_NEGO            (0x0200U)   /*!< Restart Auto-Negotiation function      */
#define YT8512_DUPLEX_BIT                   (0x0100U)   /*!< The duplex control bit                 */
#define YT8512_COLLISION_TEST               (0x0080U)   /*!< Collision test function                */

#define YT8512_FULL_DUPLEX_100M             (0x2100U)   /*!< Set the full-duplex mode at 100 Mb/s   */
#define YT8512_HALF_DUPLEX_100M             (0x2000U)   /*!< Set the half-duplex mode at 100 Mb/s   */
#define YT8512_FULL_DUPLEX_10M              (0x0100U)   /*!< Set the full-duplex mode at 10 Mb/s    */
#define YT8512_HALF_DUPLEX_10M              (0x0000U)   /*!< Set the half-duplex mode at 10 Mb/s    */
/**
 * @}
 */

/**
 * @defgroup YT8512_Basic_Status_Register YT8512 Basic Status Register
 * @{
 */
#define YT8512_100BASE_T4                   (0x8000U)   /*!< 100Base-T4 support                             */
#define YT8512_100BASE_TX_FD                (0x4000U)   /*!< 100Base-TX full duplex support                 */
#define YT8512_100BASE_TX_HD                (0x2000U)   /*!< 100Base-TX half duplex support                 */
#define YT8512_10BASE_T_FD                  (0x1000U)   /*!< 10Base-T full duplex support                   */
#define YT8512_10BASE_T_HD                  (0x0800U)   /*!< 10Base-T half duplex support                   */
#define YT8512_AUTO_NEGO_CPLT               (0x0020U)   /*!< Auto-Negotiation process completed             */
#define YT8512_LINK_STAT                    (0x0004U)   /*!< Valid link established                         */
#define YT8512_JABBER_DETECT                (0x0002U)   /*!< Jabber condition detected                      */
/**
 * @}
 */

/**
 * @defgroup YT8512_Specific_Status_Register YT8512 Specific Status Register
 * @{
 */
#define YT8512_SSR_SPEED_MD                 (0xC000U)   /*!< The speed control bit          */
#define YT8512_SSR_DUPLEX_MD                (0x2000U)   /*!< The duplex control bit         */
#define YT8512_SSR_LINK_STAT_REALTIME       (0x0400U)   /*!< Link status real-time          */

#define YT8512_SSR_FULL_DUPLEX_100M         (0x6000U)   /*!< The full-duplex mode at 100 Mb/s   */
#define YT8512_SSR_HALF_DUPLEX_100M         (0x4000U)   /*!< The half-duplex mode at 100 Mb/s   */
#define YT8512_SSR_FULL_DUPLEX_10M          (0x2000U)   /*!< The full-duplex mode at 10 Mb/s    */
#define YT8512_SSR_HALF_DUPLEX_10M          (0x0000U)   /*!< The half-duplex mode at 10 Mb/s    */
/**
 * @}
 */

/**
 * @defgroup YT8512_Interrupt YT8512 Interrupt
 * @{
 */
#define YT8512_INT_AUT_NEGO_ERR             (0x8000U)   /*!< Auto-Negotiation Error Interrupt   */
#define YT8512_INT_SPEED_MD_CHANGE          (0x4000U)   /*!< Speed Mode Change Interrupt        */
#define YT8512_INT_DUPLEX_MD_CHANGE         (0x2000U)   /*!< Duplex Mode Change Interrupt       */
#define YT8512_INT_LINK_FAILED              (0x0800U)   /*!< Link Failed Interrupt              */
#define YT8512_INT_LINK_SUCCEED             (0x0400U)   /*!< Link Succeed Interrupt             */
/**
 * @}
 */

/**
 * @defgroup YT8512_Interrupt_Status YT8512 Interrupt Status
 * @{
 */
#define YT8512_FLAG_AUT_NEGO_ERR            (0x8000U)   /*!< Auto-Negotiation Error             */
#define YT8512_FLAG_SPEED_MD_CHANGE         (0x4000U)   /*!< Speed Mode Change                  */
#define YT8512_FLAG_DUPLEX_MD_CHANGE        (0x2000U)   /*!< Duplex Mode Change                 */
#define YT8512_FLAG_LINK_FAILED             (0x0800U)   /*!< Link Failed                        */
#define YT8512_FLAG_LINK_SUCCEED            (0x0400U)   /*!< Link Succeed                       */
/**
 * @}
 */

/**
 * @defgroup YT8512_Link_Statue YT8512 Link State
 * @{
 */
#define YT8512_LINK_DOWN                    (0x01U) /*!< No valid link established */
#define YT8512_LINK_AUTO_NEGO_NOT_CPLT      (0x02U) /*!< Auto-negotiation process not completed */
#define YT8512_LINK_100M_FULL_DUPLEX        (0x03U)
#define YT8512_LINK_100M_HALF_DUPLEX        (0x04U)
#define YT8512_LINK_10M_FULL_DUPLEX         (0x05U)
#define YT8512_LINK_10M_HALF_DUPLEX         (0x06U)
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
 * @addtogroup YT8512_Global_Functions YT8512 Global Functions
 * @{
 */
int32_t YT8512_Init(const stc_yt8512_ll_t *pstcYt8512LL);
int32_t YT8512_SW_Reset(const stc_yt8512_ll_t *pstcYt8512LL);
int32_t YT8512_GetLinkStatus(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t *pu16LinkStatus);
int32_t YT8512_SetLinkMode(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16Mode);

int32_t YT8512_AutoNegoCmd(const stc_yt8512_ll_t *pstcYt8512LL, en_functional_state_t enNewState);
int32_t YT8512_LoopBackCmd(const stc_yt8512_ll_t *pstcYt8512LL, en_functional_state_t enNewState);
int32_t YT8512_IntCmd(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16IntType, en_functional_state_t enNewState);
int32_t YT8512_GetIntStatus(const stc_yt8512_ll_t *pstcYt8512LL, uint16_t u16IntType, en_flag_status_t *penStatus);
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

#endif /* __YT8512_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
