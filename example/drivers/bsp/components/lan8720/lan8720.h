/**
 *******************************************************************************
 * @file  lan8720.h
 * @brief This file contains all the functions prototypes of the phy component
 *        library for lan8720.
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
#ifndef __LAN8720_H__
#define __LAN8720_H__

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
 * @addtogroup LAN8720
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup LAN8720_Global_Types LAN8720 Global Types
 * @{
 */

/**
 * @brief LAN8720 low layer structure definition
 */
typedef struct {
    /* Methods */
    int32_t (*Init)(void);
    int32_t (*ReadReg)(uint16_t u16Reg, uint16_t *u16Value);
    int32_t (*WriteReg)(uint16_t u16Reg, uint16_t u16Value);
    uint32_t (*GetTick)(void);
} stc_lan8720_ll_t;

/**
 * @}
 */


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup LAN8720_Global_Macros LAN8720 Global Macros
 * @{
 */

/**
 * @defgroup LAN8720_Delay LAN8720 Delay
 * @{
 */
#define LAN8720_HW_RST_DELAY                (20U) /*!< For a complete reset, Set nRST pin to low at least 10ms */
#define LAN8720_SW_RST_DELAY                (40U) /*!< For a software reset, at least 20ms */
#define LAN8720_RD_TIMEOUT                  (10U)
/**
 * @}
 */

/**
 * @defgroup LAN8720_Register LAN8720 Register
 * @{
 */
#define LAN8720_R0_BCR                      (0x00U) /*!< Basic Control Register                             */
#define LAN8720_R1_BSR                      (0x01U) /*!< Basic Status Register                              */
#define LAN8720_R2_PIDT1                    (0x02U) /*!< PHY Identifier Register 1                          */
#define LAN8720_R3_PIDT2                    (0x03U) /*!< PHY Identifier Register 2                          */
#define LAN8720_R4_ANAR                     (0x04U) /*!< Auto-Negotiation Advertisement Register            */
#define LAN8720_R5_ANLPAR                   (0x05U) /*!< Auto-Negotiation Link Partner Ability Register     */
#define LAN8720_R6_ANER                     (0x06U) /*!< Auto-Negotiation Expansion Register                */
#define LAN8720_R17_MCSR                    (0x11U) /*!< Mode Control/Status Register                       */
#define LAN8720_R18_SMR                     (0x12U) /*!< Special Modes Register                             */
#define LAN8720_R26_SECR                    (0x1AU) /*!< Symbol Error Counter Register                      */
#define LAN8720_R27_CSIR                    (0x1BU) /*!< Control / Status Indication Register               */
#define LAN8720_R29_ISR                     (0x1DU) /*!< Interrupt Source Register                          */
#define LAN8720_R30_IMR                     (0x1EU) /*!< Interrupt Mask Register                            */
#define LAN8720_R31_PSCSR                   (0x1FU) /*!< PHY Special Control/Status Register                */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Basic_Control_Register LAN8720 Basic Control Register
 * @{
 */
#define LAN8720_SOFT_RESET                  (0x8000U)   /*!< PHY Soft Reset                         */
#define LAN8720_LOOPBACK                    (0x4000U)   /*!< Select loop-back mode                  */
#define LAN8720_SPEED_BIT                   (0x2000U)   /*!< The speed control bit                  */
#define LAN8720_AUTO_NEGO                   (0x1000U)   /*!< Enable Auto-Negotiation function       */
#define LAN8720_POWER_DOWN                  (0x0800U)   /*!< Select the power down mode             */
#define LAN8720_ISOLATE                     (0x0400U)   /*!< Isolate PHY from MII                   */
#define LAN8720_RESTART_AUTO_NEGO           (0x0200U)   /*!< Restart Auto-Negotiation function      */
#define LAN8720_DUPLEX_BIT                  (0x0100U)   /*!< The duplex control bit                 */

#define LAN8720_FULL_DUPLEX_100M            (0x2100U)   /*!< Set the full-duplex mode at 100 Mb/s   */
#define LAN8720_HALF_DUPLEX_100M            (0x2000U)   /*!< Set the half-duplex mode at 100 Mb/s   */
#define LAN8720_FULL_DUPLEX_10M             (0x0100U)   /*!< Set the full-duplex mode at 10 Mb/s    */
#define LAN8720_HALF_DUPLEX_10M             (0x0000U)   /*!< Set the half-duplex mode at 10 Mb/s    */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Basic_Status_Register LAN8720 Basic Status Register
 * @{
 */
#define LAN8720_100BASE_T4                  (0x8000U)   /*!< 100Base-T4 support                             */
#define LAN8720_100BASE_TX_FD               (0x4000U)   /*!< 100Base-TX full duplex support                 */
#define LAN8720_100BASE_TX_HD               (0x2000U)   /*!< 100Base-TX half duplex support                 */
#define LAN8720_10BASE_T_FD                 (0x1000U)   /*!< 10Base-T full duplex support                   */
#define LAN8720_10BASE_T_HD                 (0x0800U)   /*!< 10Base-T half duplex support                   */
#define LAN8720_AUTO_NEGO_CPLT              (0x0020U)   /*!< Auto-Negotiation process completed             */
#define LAN8720_LINK_STAT                   (0x0004U)   /*!< Valid link established                         */
#define LAN8720_JABBER_DETECT               (0x0002U)   /*!< Jabber condition detected                      */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Specific_Control_Status_Register LAN8720 Specific Control/Status Register
 * @{
 */
#define LAN8720_SSR_AUT_NEGO_CPLT           (0x1000U)   /*!< Auto-negotiation done indication   */
#define LAN8720_SSR_DUPLEX_MD               (0x0010U)   /*!< The duplex control bit             */
#define LAN8720_SSR_SPEED_MD                (0x000CU)   /*!< The speed control bit              */

#define LAN8720_SSR_FULL_DUPLEX_100M        (0x0018U)   /*!< The full-duplex mode at 100 Mb/s   */
#define LAN8720_SSR_HALF_DUPLEX_100M        (0x0008U)   /*!< The half-duplex mode at 100 Mb/s   */
#define LAN8720_SSR_FULL_DUPLEX_10M         (0x0014U)   /*!< The full-duplex mode at 10 Mb/s    */
#define LAN8720_SSR_HALF_DUPLEX_10M         (0x0004U)   /*!< The half-duplex mode at 10 Mb/s    */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Interrupt LAN8720 Interrupt
 * @{
 */
#define LAN8720_INT_AUT_NEGO_CPLT           (0x0040U)   /*!< Auto-Negotiation complete Interrupt    */
#define LAN8720_INT_LINK_DOWN               (0x0010U)   /*!< Link Down Interrupt                    */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Interrupt_Status LAN8720 Interrupt Status
 * @{
 */
#define LAN8720_FLAG_AUT_NEGO_CPLT          (0x0040U)   /*!< Auto-Negotiation complete          */
#define LAN8720_FLAG_LINK_DOWN              (0x0010U)   /*!< Link Down                          */
/**
 * @}
 */

/**
 * @defgroup LAN8720_Link_Statue LAN8720 Link State
 * @{
 */
#define LAN8720_LINK_DOWN                   (0x01U) /*!< No valid link established */
#define LAN8720_LINK_AUTO_NEGO_NOT_CPLT     (0x02U) /*!< Auto-negotiation process not completed */
#define LAN8720_LINK_100M_FULL_DUPLEX       (0x03U)
#define LAN8720_LINK_100M_HALF_DUPLEX       (0x04U)
#define LAN8720_LINK_10M_FULL_DUPLEX        (0x05U)
#define LAN8720_LINK_10M_HALF_DUPLEX        (0x06U)
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
 * @addtogroup LAN8720_Global_Functions LAN8720 Global Functions
 * @{
 */
int32_t LAN8720_Init(const stc_lan8720_ll_t *pstcLan8720LL);
int32_t LAN8720_SW_Reset(const stc_lan8720_ll_t *pstcLan8720LL);
int32_t LAN8720_GetLinkStatus(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t *pu16LinkStatus);
int32_t LAN8720_SetLinkMode(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16Mode);

int32_t LAN8720_AutoNegoCmd(const stc_lan8720_ll_t *pstcLan8720LL, en_functional_state_t enNewState);
int32_t LAN8720_LoopBackCmd(const stc_lan8720_ll_t *pstcLan8720LL, en_functional_state_t enNewState);
int32_t LAN8720_IntCmd(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16IntType, en_functional_state_t enNewState);
int32_t LAN8720_GetIntStatus(const stc_lan8720_ll_t *pstcLan8720LL, uint16_t u16IntType, en_flag_status_t *penStatus);
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

#endif /* __LAN8720_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
