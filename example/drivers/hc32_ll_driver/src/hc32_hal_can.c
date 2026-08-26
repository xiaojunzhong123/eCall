/**
 *******************************************************************************
 * @file  hc32_ll_can.c
 * @brief This file provides firmware functions to manage the CAN.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2025-08-05       CDT             First version
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
#include "hc32_ll_clk.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_utility.h"
#include "hc32_ll_can.h"
#include "hc32_ll_mcan.h"
#include "hc32_hal_can.h"

/**
 * @addtogroup HAL_Driver
 * @{
 */

/**
 * @defgroup HAL_CAN CAN
 * @brief CAN Driver Library
 * @{
 */

#if ((defined LL_CAN_ENABLE) && (LL_CAN_ENABLE == DDL_ON)) || ((defined LL_MCAN_ENABLE) && (LL_MCAN_ENABLE == DDL_ON))
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief HAL CAN IRQ Callback Index
 */
typedef enum {
    HAL_CAN_IRQ_RX_CB_IDX = 0U,     /*!< Rx callback index */
    HAL_CAN_IRQ_RX_FULL_CB_IDX,     /*!< Rx full callback index */
    HAL_CAN_IRQ_TX_CB_IDX,          /*!< Tx callback index */
    HAL_CAN_IRQ_ERR_CB_IDX,         /*!< Error callback index */
    HAL_CAN_IRQ_BUS_OFF_CB_IDX,     /*!< Bus Off callback index */
    HAL_CAN_IRQ_CB_NUM,             /*!< Callback index max */
} en_hal_can_irq_cb_idx;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup HAL_CAN_Local_Macros HAL CAN Local Macros
 * @{
 */

/**
 * @defgroup HAL_CAN_Cfg_Val_Max HAL CAN Configuration Value Maximum
 * @{
 */
#define HAL_CAN_MODULE_CNT              (4U)

#define HAL_CAN_CCAN_FILTER_CNT         (16U)

#define HAL_CAN_MCAN_START              (HAL_CAN3)

#define HAL_CAN_MODULE_MAX              (HAL_CAN_MODULE_CNT)
#define HAL_CAN_WORK_MD_MAX             (HAL_CAN_MD_EXTERN_LOOPBACK)
#define HAL_CAN_FRAME_FORMAT_MAX        (HAL_CAN_FRAME_NON_ISO_FD_BRS)
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_MCAN_Msg_Ram_Cfg HAL CAN MCAN Message RAM Configuration
 * @{
 */
/* Classical CAN message RAM configuration */
#define HAL_CAN_MCAN_CC_STD_FILTER_NUM              (40U)   /* Each standard filter element size is 4 bytes */
#define HAL_CAN_MCAN_CC_EXT_FILTER_NUM              (40U)   /* Each extended filter element size is 8 bytes */
#define HAL_CAN_MCAN_CC_RX_FIFO0_NUM                (64U)   /* Each Rx FIFO0 element size is 8+8 bytes */
#define HAL_CAN_MCAN_CC_RX_FIFO0_DATA_FIELD_SIZE    MCAN_DATA_SIZE_8BYTE
#define HAL_CAN_MCAN_CC_TX_FIFO_NUM                 (32U)   /* Each Tx FIFO element size is 8+8 bytes */
#define HAL_CAN_MCAN_CC_TX_NOTIFICATION_BUF         (0xFFFFFFFFUL)
/* CAN-FD message RAM configuration */
#define HAL_CAN_MCAN_FD_STD_FILTER_NUM              (26U)   /* Each standard filter element size is 4 bytes */
#define HAL_CAN_MCAN_FD_EXT_FILTER_NUM              (26U)   /* Each extended filter element size is 8 bytes */
#define HAL_CAN_MCAN_FD_RX_FIFO0_NUM                (12U)   /* Each Rx FIFO0 element size is 64+8 bytes */
#define HAL_CAN_MCAN_FD_RX_FIFO0_DATA_FIELD_SIZE    MCAN_DATA_SIZE_64BYTE
#define HAL_CAN_MCAN_FD_TX_FIFO_NUM                 (12U)   /* Each Tx FIFO element size is 64+8 bytes */
#define HAL_CAN_MCAN_FD_TX_NOTIFICATION_BUF         ((1UL << HAL_CAN_MCAN_FD_TX_FIFO_NUM) - 1U)

#define HAL_CAN_MCAN_MODULE_CNT                     (2U)    /*!< Number of MCAN modules */
#define HAL_CAN_MCAN_MSG_RAM_PER_MODULE             (MCAN_MSG_RAM_SIZE / HAL_CAN_MCAN_MODULE_CNT)   /*!< Message RAM size per MCAN module */
#define HAL_CAN_MCAN_MSG_RAM_OFFSET(module)         (HAL_CAN_MCAN_MSG_RAM_PER_MODULE * (module))    /*!< Message RAM offset for each MCAN module */
/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Int_Sel HAL CAN Interrupt Selection
 * @{
 */
#define HAL_CAN_CCAN_INT_SEL                (CAN_INT_PTB_TX | CAN_INT_RX | CAN_INT_RX_BUF_FULL | CAN_INT_ERR_INT)
#define HAL_CAN_MCAN_INT_SEL                (MCAN_INT_RX_FIFO0_NEW_MSG | MCAN_INT_RX_FIFO0_FULL | MCAN_INT_TX_CPLT | MCAN_INT_BUS_OFF | \
                                            MCAN_INT_ARB_PHASE_ERROR | MCAN_INT_DATA_PHASE_ERROR)
#define HAL_CAN_MCAN_INT_ERR                (MCAN_INT_ARB_PHASE_ERROR | MCAN_INT_DATA_PHASE_ERROR)
/**
 * @}
 */

/**
 * @defgroup CAN_Check_Parameters_Validity CAN Check Parameters Validity
 * @{
 */
#define IS_CAN_BIT_MASK(x, mask)        (((x) != 0U) && (((x) | (mask)) == (mask)))

#define IS_HAL_CAN_MODULE(x)            ((x) <= HAL_CAN_MODULE_MAX)

#define IS_HAL_CAN_WORK_MD(x)           ((x) <= HAL_CAN_WORK_MD_MAX)

#define IS_HAL_CAN_FRAME_FORMAT(x)      ((x) <= HAL_CAN_FRAME_FORMAT_MAX)

#define IS_HAL_CAN_RX_OVF_MD(x)                                                 \
(   ((x) == HAL_CAN_RX_OVF_BLOCKING)                                ||          \
    ((x) == HAL_CAN_RX_OVF_OVERWRITE))

#define IS_HAL_CAN_CCAN_FILTER_NUM(x)   ((x) <= HAL_CAN_CCAN_FILTER_CNT)
#define IS_HAL_CAN_CCAN_FILTER_TYPE(x)  ((x) == HAL_CAN_FILTER_TYPE_MASK)

#define IS_HAL_CAN_MCAN_SF_NUM(fd, x)                                           \
(   (((fd) == 0U) && ((x) <= HAL_CAN_MCAN_CC_STD_FILTER_NUM))       ||          \
    (((fd) == 1U) && ((x) <= HAL_CAN_MCAN_FD_STD_FILTER_NUM)))

#define IS_HAL_CAN_MCAN_EF_NUM(fd, x)                                           \
(   (((fd) == 0U) && ((x) <= HAL_CAN_MCAN_CC_EXT_FILTER_NUM))       ||          \
    (((fd) == 1U) && ((x) <= HAL_CAN_MCAN_FD_EXT_FILTER_NUM)))

#define IS_HAL_CAN_TX_FIFO_MD(x)        (((x) == HAL_CAN_TX_FIFO_MD_FIFO) || ((x) == HAL_CAN_TX_FIFO_MD_PRIO))

#define IS_HAL_CAN_INT(x)               IS_CAN_BIT_MASK(x, HAL_CAN_INT_ALL)

/**
 * @}
 */

/**
 * @defgroup HAL_CAN_Op_Macros HAL CAN Operation Macros
 * @{
 */
#define HAL_CAN_PERIPH_CMD              FCG_Fcg1PeriphClockCmd
#define HAL_CAN_PERIPH_ENABLE(x)        HAL_CAN_PERIPH_CMD(m_au32HalCanModulePeriphClock[(x)], ENABLE)
#define HAL_CAN_PERIPH_DISABLE(x)       HAL_CAN_PERIPH_CMD(m_au32HalCanModulePeriphClock[(x)], DISABLE)

#define HAL_CAN_CCANx                   ((CM_CAN_TypeDef *)m_au32HalCanModuleBase[u8CanModule])
#define HAL_CAN_MCANx                   ((CM_MCAN_TypeDef *)m_au32HalCanModuleBase[u8CanModule])

#define HAL_CAN_CB_PROC(cb)                                                     \
do {                                                                            \
    if ((cb) != NULL) { (cb)(); };                                              \
} while (0)

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint8_t m_au8HalCanDLC2Size[16U] = {
    0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U
};

static func_ptr_t m_apfnHalCanIrqCallback[HAL_CAN_MODULE_CNT][HAL_CAN_IRQ_CB_NUM] = {NULL};

static const uint32_t m_au32HalCanModuleBase[] = {
    CM_CAN1_BASE, CM_CAN2_BASE, CM_MCAN1_BASE, CM_MCAN2_BASE,
};

static const uint32_t m_au32HalCanModulePeriphClock[] = {
    FCG1_PERIPH_CAN1, FCG1_PERIPH_CAN2, FCG1_PERIPH_MCAN1, FCG1_PERIPH_MCAN2,
};

static const en_int_src_t m_aenHalCanIntSrc[] = {
    INT_SRC_CAN1_HOST, INT_SRC_CAN2_HOST, INT_SRC_MCAN1_INT0, INT_SRC_MCAN2_INT0,
};

#if (LL_MCAN_ENABLE == DDL_ON)
static const uint8_t m_au8HalMcanMsgRamCfg[2U][5U] = {
    {
        HAL_CAN_MCAN_CC_STD_FILTER_NUM,
        HAL_CAN_MCAN_CC_EXT_FILTER_NUM,
        HAL_CAN_MCAN_CC_RX_FIFO0_NUM,
        HAL_CAN_MCAN_CC_RX_FIFO0_DATA_FIELD_SIZE,
        HAL_CAN_MCAN_CC_TX_FIFO_NUM,
    }, /* Classical CAN */
    {
        HAL_CAN_MCAN_FD_STD_FILTER_NUM,
        HAL_CAN_MCAN_FD_EXT_FILTER_NUM,
        HAL_CAN_MCAN_FD_RX_FIFO0_NUM,
        HAL_CAN_MCAN_FD_RX_FIFO0_DATA_FIELD_SIZE,
        HAL_CAN_MCAN_FD_TX_FIFO_NUM,
    }, /* CAN-FD */
};

static const uint32_t m_au32HalMcanTxNoteBuf[2U] = {
    HAL_CAN_MCAN_CC_TX_NOTIFICATION_BUF, HAL_CAN_MCAN_FD_TX_NOTIFICATION_BUF,
};
#endif

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup CAN_Local_Functions CAN Local Functions
 * @{
 */
#if (LL_CAN_ENABLE == DDL_ON)
/**
 * @brief HAL CM_CAN IRQ routine.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be the following value of @ref HAL_CAN_Module
 * @retval None
 */
static void HAL_CAN_CCAN_IrqRoutine(uint8_t u8CanModule)
{
    uint32_t u32Status;

    u32Status = CAN_GetStatusValue(HAL_CAN_CCANx);
    if (u32Status != 0U) {
        CAN_ClearStatus(HAL_CAN_CCANx, u32Status);
    }
    /* Received a new frame */
    if ((u32Status & CAN_FLAG_RX) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_RX_CB_IDX]);
    }
    /* Rx FIFO full*/
    if ((u32Status & CAN_FLAG_RX_BUF_FULL) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_RX_FULL_CB_IDX]);
    }
    /* Transmitted a frame */
    if ((u32Status & CAN_FLAG_PTB_TX) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_TX_CB_IDX]);
    }
    /* Bus error occurred */
    if ((u32Status & CAN_FLAG_BUS_ERR) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_ERR_CB_IDX]);
    }
    /* Bus-off */
    if ((u32Status & CAN_FLAG_ERR_INT) != 0U) {
        if ((u32Status & CAN_FLAG_BUS_OFF) == 0U) {
            /* Bus-off entered */
            HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_BUS_OFF_CB_IDX]);
        }
    }
}
#endif

#if (LL_MCAN_ENABLE == DDL_ON)
/**
 * @brief HAL CM_CAN IRQ routine.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be the following value of @ref HAL_CAN_Module
 * @retval None
 */
static void HAL_CAN_MCAN_IrqRoutine(uint8_t u8CanModule)
{
    uint32_t u32Status;

    u32Status = READ_REG32(HAL_CAN_MCANx->IR);
    if (u32Status != 0U) {
        MCAN_ClearStatus(HAL_CAN_MCANx, u32Status);
    }
    /* Received a new frame */
    if ((u32Status & MCAN_INT_RX_FIFO0_NEW_MSG) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_RX_CB_IDX]);
    }
    /* Transmitted a frame */
    if ((u32Status & MCAN_INT_TX_CPLT) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_TX_CB_IDX]);
    }
    /* Bus error occurred */
    if ((u32Status & HAL_CAN_MCAN_INT_ERR) != 0U) {
        HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_ERR_CB_IDX]);
    }
    /* Bus-off status changed: bus-off enter or bus-off recovered */
    if ((u32Status & MCAN_INT_BUS_OFF) != 0U) {
        if (MCAN_GetProtocolFlagStatus(HAL_CAN_MCANx, MCAN_PROTOCOL_FLAG_BUS_OFF) == SET) {
            /* Bus-off entered */
            /* Call HAL_CAN_Start(u8CanModule) in bus-off callback or any other function. */
            HAL_CAN_CB_PROC(m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_BUS_OFF_CB_IDX]);
        }
    }
}
#endif

/**
 * @brief HAL CAN1 IRQ handler.
 * @param  None
 * @retval None
 */
static void HAL_CAN1_IrqHandler(void)
{
#if (LL_CAN_ENABLE == DDL_ON)
    HAL_CAN_CCAN_IrqRoutine(HAL_CAN1);
#endif
}

/**
 * @brief HAL CAN2 IRQ handler.
 * @param  None
 * @retval None
 */
static void HAL_CAN2_IrqHandler(void)
{
#if (LL_CAN_ENABLE == DDL_ON)
    HAL_CAN_CCAN_IrqRoutine(HAL_CAN2);
#endif
}

/**
 * @brief HAL CAN3 IRQ handler.
 * @param  None
 * @retval None
 */
static void HAL_CAN3_IrqHandler(void)
{
#if (LL_MCAN_ENABLE == DDL_ON)
    HAL_CAN_MCAN_IrqRoutine(HAL_CAN3);
#endif
}

/**
 * @brief HAL CAN4 IRQ handler.
 * @param  None
 * @retval None
 */
static void HAL_CAN4_IrqHandler(void)
{
#if (LL_MCAN_ENABLE == DDL_ON)
    HAL_CAN_MCAN_IrqRoutine(HAL_CAN4);
#endif
}

#if (LL_CAN_ENABLE == DDL_ON)
/**
 * @brief Get module CM_CANx initialization configuration.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module that corresponds to CM_CANx
 * @param  [in]  pstcHalCanInit         Pointer to a @ref stc_hal_can_init_t structure value that
 *                                      contains the configuration information for CM_CANx.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Provided parameter is not valid.
 */
static int32_t HAL_CAN_CcanInitCfg(uint8_t u8CanModule, const stc_hal_can_init_t *pstcHalCanInit)
{
    uint8_t i;
    uint8_t j = 0U;
    int32_t i32Ret;
    uint32_t u32IntSel = 0UL;
    stc_can_init_t stcCanInit;
    stc_can_filter_config_t astcCcanFilterList[HAL_CAN_CCAN_FILTER_CNT];
    stc_canfd_config_t stcCcanFdCfg;
    const uint8_t au8RxOvfMode[] = {CAN_RX_OVF_DISCARD_NEW, CAN_RX_OVF_SAVE_NEW};
    const uint32_t au32CcanIntSel[] = {CAN_INT_RX, CAN_INT_RX_BUF_FULL, CAN_INT_PTB_TX, CAN_INT_BUS_ERR, CAN_INT_ERR_INT};

    /* Total filter number */
    i = pstcHalCanInit->stcFilterCfg.u8StdFilterNum + pstcHalCanInit->stcFilterCfg.u8ExtFilterNum;
    DDL_ASSERT(IS_HAL_CAN_RX_OVF_MD(pstcHalCanInit->u8RxOvfMode));
    DDL_ASSERT(IS_HAL_CAN_CCAN_FILTER_NUM(i));

    stcCanInit.u8WorkMode             = pstcHalCanInit->u8Mode;
    stcCanInit.u8PTBSingleShotTx      = (uint8_t)pstcHalCanInit->enSingleShotTxState << CAN_CFG_STAT_TPSS_POS;
    stcCanInit.u8STBSingleShotTx      = CAN_STB_SINGLESHOT_TX_DISABLE;
    stcCanInit.u8STBPrioMode          = CAN_STB_PRIO_MD_DISABLE;
    stcCanInit.u8RxWarnLimit          = 7U;
    stcCanInit.u8ErrorWarnLimit       = 0U;
    stcCanInit.u8RxAllFrame           = CAN_RX_ALL_FRAME_DISABLE;
    stcCanInit.u8RxOvfMode            = au8RxOvfMode[pstcHalCanInit->u8RxOvfMode];
    stcCanInit.u8SelfAck              = CAN_SELF_ACK_DISABLE;
    stcCanInit.pstcCanTtc             = NULL;

    /* Nominal bit time */
    stcCanInit.stcBitCfg.u32Prescaler = pstcHalCanInit->stcBitTimeCfg.u32NominalPrescaler;
    stcCanInit.stcBitCfg.u32TimeSeg1  = pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg1;
    stcCanInit.stcBitCfg.u32TimeSeg2  = pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg2;
    stcCanInit.stcBitCfg.u32SJW       = pstcHalCanInit->stcBitTimeCfg.u32NominalSyncJumpWidth;

    /* Filter */
    stcCanInit.pstcFilter             = astcCcanFilterList;
    stcCanInit.u16FilterSelect        = (uint16_t)(1UL << i) - 1U;
    /* Standard ID filter */
    for (i = 0U; i < pstcHalCanInit->stcFilterCfg.u8StdFilterNum; i++) {
        astcCcanFilterList[j].u32ID     = pstcHalCanInit->stcFilterCfg.pstcStdFilterList[i].u32FilterId1;
        astcCcanFilterList[j].u32IDMask = (~pstcHalCanInit->stcFilterCfg.pstcStdFilterList[i].u32FilterId2) & HAL_CAN_STD_ID_MASK;
        astcCcanFilterList[j].u32IDType = CAN_ID_STD;
        j++;
    }
    /* Extended ID filter */
    for (i = 0U; i < pstcHalCanInit->stcFilterCfg.u8ExtFilterNum; i++) {
        astcCcanFilterList[j].u32ID     = pstcHalCanInit->stcFilterCfg.pstcExtFilterList[i].u32FilterId1;
        astcCcanFilterList[j].u32IDMask = (~pstcHalCanInit->stcFilterCfg.pstcExtFilterList[i].u32FilterId2) & HAL_CAN_EXT_ID_MASK;
        astcCcanFilterList[j].u32IDType = CAN_ID_EXT;
        j++;
    }

    DDL_ASSERT(IS_HAL_CAN_FRAME_FORMAT(pstcHalCanInit->u8FrameForamt));
    /* CAN-FD */
    stcCanInit.pstcCanFd = NULL;
    if (pstcHalCanInit->u8FrameForamt != HAL_CAN_FRAME_CLASSIC) {
        stcCcanFdCfg.stcBitCfg.u32Prescaler = pstcHalCanInit->stcBitTimeCfg.u32DataPrescaler;
        stcCcanFdCfg.stcBitCfg.u32TimeSeg1  = pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg1;
        stcCcanFdCfg.stcBitCfg.u32TimeSeg2  = pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg2;
        stcCcanFdCfg.stcBitCfg.u32SJW       = pstcHalCanInit->stcBitTimeCfg.u32DataSyncJumpWidth;
        stcCcanFdCfg.u8TDC                  = (uint8_t)pstcHalCanInit->stcBitTimeCfg.enTdcState << CAN_TDC_TDCEN_POS;
        stcCcanFdCfg.u8SSPOffset            = (uint8_t)pstcHalCanInit->stcBitTimeCfg.u32SspOffset;
        stcCcanFdCfg.u8Mode = CAN_FD_MD_ISO;
        if ((pstcHalCanInit->u8FrameForamt == HAL_CAN_FRAME_NON_ISO_FD_NO_BRS) || \
            (pstcHalCanInit->u8FrameForamt == HAL_CAN_FRAME_NON_ISO_FD_BRS)) {
            stcCcanFdCfg.u8Mode = CAN_FD_MD_BOSCH;
        }
        stcCanInit.pstcCanFd = &stcCcanFdCfg;
        /* Enable CAN-FD */
        CAN_FD_Cmd(HAL_CAN_CCANx, ENABLE);
    }
    else {
        /* Disable CAN-FD */
        CAN_FD_Cmd(HAL_CAN_CCANx, DISABLE);
    }

    /* Interrupts selection */
    for (i = 0U; i < (uint8_t)HAL_CAN_IRQ_CB_NUM; i++) {
        if ((pstcHalCanInit->stcIrqCfg.u32IntSel & (1UL << i)) != 0U) {
            u32IntSel |= au32CcanIntSel[i];
        }
    }

    i32Ret = CAN_Init(HAL_CAN_CCANx, &stcCanInit);
    if (i32Ret == LL_OK) {
        /* Enable the interrupts, the status flags can be read. */
        CAN_IntCmd(HAL_CAN_CCANx, CAN_INT_ALL, DISABLE);
        /* Enable the interrupts that needed. */
        CAN_IntCmd(HAL_CAN_CCANx, u32IntSel, ENABLE);
    }

    return i32Ret;
}
#endif

#if (LL_MCAN_ENABLE == DDL_ON)
/**
 * @brief Get module CM_MCANx initialization configuration.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module that corresponds to CM_MCANx
 * @param  [in]  pstcHalCanInit         Pointer to a @ref stc_hal_can_init_t structure value that
 *                                      contains the configuration information for CM_MCANx.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Provided parameter is not valid.
 */
static int32_t HAL_CAN_McanInitCfg(uint8_t u8CanModule, const stc_hal_can_init_t *pstcHalCanInit)
{
    uint8_t i;
    uint8_t u8FD = 0U;
    int32_t i32Ret;
    uint32_t u32IntSel = 0UL;
    stc_mcan_init_t stcMcanInit;
    stc_mcan_filter_t stcMcanFilter;

    const uint8_t au8McanMode[] = {MCAN_MD_NORMAL, MCAN_MD_BUS_MON, MCAN_MD_INTERN_LOOPBACK, MCAN_MD_EXTERN_LOOPBACK};
    const uint32_t au32McanFrameFormat[] = {MCAN_FRAME_CLASSIC, MCAN_FRAME_ISO_FD_NO_BRS, MCAN_FRAME_ISO_FD_BRS,
                                            MCAN_FRAME_NON_ISO_FD_NO_BRS, MCAN_FRAME_NON_ISO_FD_BRS};
    const uint32_t au32McanAutoRetx[] = {MCAN_AUTO_RETX_ENABLE, MCAN_AUTO_RETX_DISABLE};
    const uint32_t au32McanRxOvfMode[] = {MCAN_RX_FIFO_BLOCKING, MCAN_RX_FIFO_OVERWRITE};
    const uint32_t au32McanIntSel[] = {MCAN_INT_RX_FIFO0_NEW_MSG, MCAN_INT_RX_FIFO0_FULL, MCAN_INT_TX_CPLT,
                                       HAL_CAN_MCAN_INT_ERR, MCAN_INT_BUS_OFF};

    DDL_ASSERT(IS_HAL_CAN_FRAME_FORMAT(pstcHalCanInit->u8FrameForamt));
    if (pstcHalCanInit->u8FrameForamt != HAL_CAN_FRAME_CLASSIC) {
        u8FD = 1U;
    }

    DDL_ASSERT(IS_HAL_CAN_WORK_MD(pstcHalCanInit->u8Mode));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHalCanInit->enSingleShotTxState));
    DDL_ASSERT(IS_HAL_CAN_RX_OVF_MD(pstcHalCanInit->u8RxOvfMode));
    DDL_ASSERT(IS_HAL_CAN_TX_FIFO_MD(pstcHalCanInit->u8TxFifoMode));
    DDL_ASSERT(IS_HAL_CAN_MCAN_SF_NUM(u8FD, pstcHalCanInit->stcFilterCfg.u8StdFilterNum));
    DDL_ASSERT(IS_HAL_CAN_MCAN_EF_NUM(u8FD, pstcHalCanInit->stcFilterCfg.u8ExtFilterNum));

    MCAN_StructInit(&stcMcanInit);

    stcMcanInit.u32Mode              = au8McanMode[pstcHalCanInit->u8Mode];
    stcMcanInit.u32FrameFormat       = au32McanFrameFormat[pstcHalCanInit->u8FrameForamt];
    stcMcanInit.u32AutoRetx          = au32McanAutoRetx[(uint8_t)pstcHalCanInit->enSingleShotTxState];

    stcMcanInit.stcBitTime.u32NominalPrescaler     = pstcHalCanInit->stcBitTimeCfg.u32NominalPrescaler;
    stcMcanInit.stcBitTime.u32NominalTimeSeg1      = pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg1;
    stcMcanInit.stcBitTime.u32NominalTimeSeg2      = pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg2;
    stcMcanInit.stcBitTime.u32NominalSyncJumpWidth = pstcHalCanInit->stcBitTimeCfg.u32NominalSyncJumpWidth;
    stcMcanInit.stcBitTime.u32DataPrescaler        = pstcHalCanInit->stcBitTimeCfg.u32DataPrescaler;
    stcMcanInit.stcBitTime.u32DataTimeSeg1         = pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg1;
    stcMcanInit.stcBitTime.u32DataTimeSeg2         = pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg2;
    stcMcanInit.stcBitTime.u32DataSyncJumpWidth    = pstcHalCanInit->stcBitTimeCfg.u32DataSyncJumpWidth;
    stcMcanInit.stcBitTime.u32TDC                  = (uint32_t)pstcHalCanInit->stcBitTimeCfg.enTdcState << MCAN_DBTP_TDC_POS;
    stcMcanInit.stcBitTime.u32SspOffset            = pstcHalCanInit->stcBitTimeCfg.u32SspOffset * pstcHalCanInit->stcBitTimeCfg.u32DataPrescaler;
    stcMcanInit.stcBitTime.u32TdcFilter            = 0U;

    stcMcanInit.stcMsgRam.u32AddrOffset         = HAL_CAN_MCAN_MSG_RAM_OFFSET((uint32_t)u8CanModule - HAL_CAN_MCAN_START);
    stcMcanInit.stcMsgRam.u32StdFilterNum       = m_au8HalMcanMsgRamCfg[u8FD][0U];
    stcMcanInit.stcMsgRam.u32ExtFilterNum       = m_au8HalMcanMsgRamCfg[u8FD][1U];
    stcMcanInit.stcMsgRam.u32RxFifo0Num         = m_au8HalMcanMsgRamCfg[u8FD][2U];
    stcMcanInit.stcMsgRam.u32RxFifo0DataSize    = m_au8HalMcanMsgRamCfg[u8FD][3U];
    stcMcanInit.stcMsgRam.u32TxFifoQueueNum     = m_au8HalMcanMsgRamCfg[u8FD][4U];
    stcMcanInit.stcMsgRam.u32TxFifoQueueMode    = (uint32_t)pstcHalCanInit->u8TxFifoMode << MCAN_TXBC_TFQM_POS;
    stcMcanInit.stcMsgRam.u32TxDataSize         = stcMcanInit.stcMsgRam.u32RxFifo0DataSize;

    i32Ret = MCAN_Init(HAL_CAN_MCANx, &stcMcanInit);
    if (i32Ret == LL_OK) {
        for (i = 0U; i < pstcHalCanInit->stcFilterCfg.u8StdFilterNum; i++) {
            stcMcanFilter.u32IdType        = MCAN_STD_ID;
            stcMcanFilter.u32FilterIndex   = i;
            stcMcanFilter.u32FilterType    = pstcHalCanInit->stcFilterCfg.pstcStdFilterList[i].u8FilterType;
            stcMcanFilter.u32FilterConfig  = MCAN_FILTER_TO_RX_FIFO0;
            stcMcanFilter.u32FilterId1     = pstcHalCanInit->stcFilterCfg.pstcStdFilterList[i].u32FilterId1;
            stcMcanFilter.u32FilterId2     = pstcHalCanInit->stcFilterCfg.pstcStdFilterList[i].u32FilterId2;
            stcMcanFilter.u32RxBufferIndex = 0U; /* Dedicated Rx buffer not used */
            (void)MCAN_FilterConfig(HAL_CAN_MCANx, &stcMcanFilter);
        }

        for (i = 0U; i < pstcHalCanInit->stcFilterCfg.u8ExtFilterNum; i++) {
            stcMcanFilter.u32IdType        = MCAN_EXT_ID;
            stcMcanFilter.u32FilterIndex   = i;
            stcMcanFilter.u32FilterType    = pstcHalCanInit->stcFilterCfg.pstcExtFilterList[i].u8FilterType;
            stcMcanFilter.u32FilterConfig  = MCAN_FILTER_TO_RX_FIFO0;
            stcMcanFilter.u32FilterId1     = pstcHalCanInit->stcFilterCfg.pstcExtFilterList[i].u32FilterId1;
            stcMcanFilter.u32FilterId2     = pstcHalCanInit->stcFilterCfg.pstcExtFilterList[i].u32FilterId2;
            stcMcanFilter.u32RxBufferIndex = 0U; /* Dedicated Rx buffer not used */
            (void)MCAN_FilterConfig(HAL_CAN_MCANx, &stcMcanFilter);
        }

        /* Interrupts selection */
        for (i = 0U; i < (uint8_t)HAL_CAN_IRQ_CB_NUM; i++) {
            if ((pstcHalCanInit->stcIrqCfg.u32IntSel & (1UL << i)) != 0U) {
                u32IntSel |= au32McanIntSel[i];
            }
        }

        MCAN_RxFifoOperationModeConfig(HAL_CAN_MCANx, MCAN_RX_FIFO0, au32McanRxOvfMode[pstcHalCanInit->u8RxOvfMode]);
        /* The Tx buffer can cause transmission completed interrupt only when its own transmission completed interrupt is enabled. */
        MCAN_TxBufferNotificationCmd(HAL_CAN_MCANx, m_au32HalMcanTxNoteBuf[u8FD], MCAN_INT_TX_CPLT, ENABLE);
        MCAN_IntCmd(HAL_CAN_MCANx, u32IntSel, MCAN_INT_LINE0, ENABLE);
    }

    return i32Ret;
}
#endif

/**
 * @}
 */

/**
 * @defgroup CAN_Global_Functions CAN Global Functions
 * @{
 */
/**
 * @brief  Initializes the specified CAN module according to the specified parameters
 *         in the structure stc_hal_can_init_t.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @param  [in]  pstcHalCanInit         Pointer to a @ref stc_hal_can_init_t structure value that
 *                                      contains the configuration information for the CAN module.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Provided parameter is not valid.
 */
int32_t HAL_CAN_Init(uint8_t u8CanModule, const stc_hal_can_init_t *pstcHalCanInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    func_ptr_t apfnCanIrqCallback[HAL_CAN_MODULE_CNT] = {
        HAL_CAN1_IrqHandler,
        HAL_CAN2_IrqHandler,
        HAL_CAN3_IrqHandler,
        HAL_CAN4_IrqHandler,
    };

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));
    DDL_ASSERT(IS_HAL_CAN_INT(pstcHalCanInit->stcIrqCfg.u32IntSel));

    if (pstcHalCanInit != NULL) {
        CLK_SetCANClockSrc(1U << u8CanModule, pstcHalCanInit->u8Clock);
        HAL_CAN_PERIPH_DISABLE(u8CanModule);
        HAL_CAN_PERIPH_ENABLE(u8CanModule);
        i32Ret = HAL_CAN_DeInit(u8CanModule);
        if (i32Ret == LL_OK) {
        switch (u8CanModule) {
        #if (LL_CAN_ENABLE == DDL_ON)
            case HAL_CAN1:
            case HAL_CAN2:
                i32Ret = HAL_CAN_CcanInitCfg(u8CanModule, pstcHalCanInit);
                break;
        #endif
        #if (LL_MCAN_ENABLE == DDL_ON)
            case HAL_CAN3:
            case HAL_CAN4:
                i32Ret = HAL_CAN_McanInitCfg(u8CanModule, pstcHalCanInit);
                break;
        #endif
            default:
                break;
        }
        }
    }

    if (i32Ret == LL_OK) {
        m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_RX_CB_IDX]      = pstcHalCanInit->stcIrqCfg.pfnRxCallback;
        m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_RX_FULL_CB_IDX] = pstcHalCanInit->stcIrqCfg.pfnRxFullCallback;
        m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_TX_CB_IDX]      = pstcHalCanInit->stcIrqCfg.pfnTxCallback;
        m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_ERR_CB_IDX]     = pstcHalCanInit->stcIrqCfg.pfnErrorCallback;
        m_apfnHalCanIrqCallback[u8CanModule][HAL_CAN_IRQ_BUS_OFF_CB_IDX] = pstcHalCanInit->stcIrqCfg.pfnBusOffCallback;
        (void)INTC_IrqInstallHandle(pstcHalCanInit->stcIrqCfg.enIRQn, m_aenHalCanIntSrc[u8CanModule],
                                    pstcHalCanInit->stcIrqCfg.u8IrqPriority, apfnCanIrqCallback[u8CanModule]);
    } else {
        HAL_CAN_PERIPH_DISABLE(u8CanModule);
    }

    return i32Ret;
}

/**
 * @brief  Set each @ref stc_hal_can_init_t field to a default value.
 * @param  [in]  pstcHalCanInit         Pointer to a @ref stc_hal_can_init_t structure
 *                                      whose fields will be set to default values.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       pstcHalCanInit == NULL.
 */
int32_t HAL_CAN_StructInit(stc_hal_can_init_t *pstcHalCanInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if (pstcHalCanInit != NULL) {
        pstcHalCanInit->u8Mode              = HAL_CAN_MD_NORMAL;
        pstcHalCanInit->u8RxOvfMode         = HAL_CAN_RX_OVF_BLOCKING;
        pstcHalCanInit->u8TxFifoMode        = HAL_CAN_TX_FIFO_MD_FIFO;
        pstcHalCanInit->enSingleShotTxState = DISABLE;
        /* Based on: CAN clock 40MHz. Nominal bit rate: 1Mbps; data bit rate: 4Mbps */
        pstcHalCanInit->u8Clock = HAL_CAN_CLK_SYSCLK_DIV6;
        pstcHalCanInit->u8FrameForamt = HAL_CAN_FRAME_ISO_FD_BRS;
        pstcHalCanInit->stcBitTimeCfg.u32NominalPrescaler     = 1U;
        pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg1      = 32U;
        pstcHalCanInit->stcBitTimeCfg.u32NominalTimeSeg2      = 8U;
        pstcHalCanInit->stcBitTimeCfg.u32NominalSyncJumpWidth = 8U;
        pstcHalCanInit->stcBitTimeCfg.u32DataPrescaler        = 1U;
        pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg1         = 8U;
        pstcHalCanInit->stcBitTimeCfg.u32DataTimeSeg2         = 2U;
        pstcHalCanInit->stcBitTimeCfg.u32DataSyncJumpWidth    = 2U;
        pstcHalCanInit->stcBitTimeCfg.enTdcState              = ENABLE;
        pstcHalCanInit->stcBitTimeCfg.u32SspOffset            = 8U;
        pstcHalCanInit->stcFilterCfg.pstcStdFilterList = NULL;
        pstcHalCanInit->stcFilterCfg.pstcExtFilterList = NULL;
        pstcHalCanInit->stcFilterCfg.u8StdFilterNum    = 0U;
        pstcHalCanInit->stcFilterCfg.u8ExtFilterNum    = 0U;
        pstcHalCanInit->stcIrqCfg.enIRQn            = (IRQn_Type)0xFFU; /* Invalid value */
        pstcHalCanInit->stcIrqCfg.u8IrqPriority     = DDL_IRQ_PRIO_DEFAULT;
        pstcHalCanInit->stcIrqCfg.pfnRxCallback     = NULL;
        pstcHalCanInit->stcIrqCfg.pfnRxFullCallback = NULL;
        pstcHalCanInit->stcIrqCfg.pfnTxCallback     = NULL;
        pstcHalCanInit->stcIrqCfg.pfnErrorCallback  = NULL;
        pstcHalCanInit->stcIrqCfg.pfnBusOffCallback = NULL;

        i32Ret = LL_OK;
    }

    return i32Ret;
}

/**
 * @brief  Deinitialize the specified CAN peripheral registers to their default reset values.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @retval int32_t:
 *           - LL_OK:                   De-Initialize success.
 *           - LL_ERR_TIMEOUT:          Timeout.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 */
int32_t HAL_CAN_DeInit(uint8_t u8CanModule)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    switch (u8CanModule) {
    #if (LL_CAN_ENABLE == DDL_ON)
        case HAL_CAN1:
        case HAL_CAN2:
            i32Ret = CAN_DeInit(HAL_CAN_CCANx);
            CAN_FD_Cmd(HAL_CAN_CCANx, DISABLE);
            break;
    #endif
    #if (LL_MCAN_ENABLE == DDL_ON)
        case HAL_CAN3:
        case HAL_CAN4:
            i32Ret = MCAN_DeInit(HAL_CAN_MCANx);
            break;
    #endif
        default:
            break;
    }
    return i32Ret;
}

/**
 * @brief Leave the initialization mode and start the CAN module.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 */
int32_t HAL_CAN_Start(uint8_t u8CanModule)
{
    int32_t i32Ret = LL_OK;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    switch (u8CanModule) {
    #if (LL_CAN_ENABLE == DDL_ON)
        case HAL_CAN1:
        case HAL_CAN2:
            CAN_ExitLocalReset(HAL_CAN_CCANx);
            break;
    #endif
    #if (LL_MCAN_ENABLE == DDL_ON)
        case HAL_CAN3:
        case HAL_CAN4:
            MCAN_Start(HAL_CAN_MCANx);
            break;
    #endif
        default:
            i32Ret = LL_ERR_INVD_PARAM;
            break;
    }
    return i32Ret;
}

/**
 * @brief Stop the CAN module and enable access to configuration registers.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 *           - LL_ERR_TIMEOUT:          Operation timeout.
 */
int32_t HAL_CAN_Stop(uint8_t u8CanModule)
{
    int32_t i32Ret = LL_OK;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    switch (u8CanModule) {
    #if (LL_CAN_ENABLE == DDL_ON)
        case HAL_CAN1:
        case HAL_CAN2:
            CAN_EnterLocalReset(HAL_CAN_CCANx);
            break;
    #endif
    #if (LL_MCAN_ENABLE == DDL_ON)
        case HAL_CAN3:
        case HAL_CAN4:
            i32Ret = MCAN_Stop(HAL_CAN_MCANx);
            break;
    #endif
        default:
            i32Ret = LL_ERR_INVD_PARAM;
            break;
    }
    return i32Ret;
}

/**
 * @brief  Transmit a frame via the specified CAN module.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @param  [in]  pstcTxFrame            Pointer to a @ref stc_hal_can_frame_t structure that
 *                                      contains the frame information to be transmitted.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 *           - LL_ERR_BUF_FULL:         The transmit FIFO is full.
 *           - LL_ERR_BUSY:             The transmit FIFO is being transmitted.
 */
int32_t HAL_CAN_TxFrame(uint8_t u8CanModule, const stc_hal_can_frame_t *pstcTxFrame)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    #if (LL_CAN_ENABLE == DDL_ON)
    stc_can_tx_frame_t stcCanTxFrame;
    #endif
    #if (LL_MCAN_ENABLE == DDL_ON)
    uint8_t i;
    stc_mcan_tx_msg_t stcMcanTxMsg;
    #endif

    if (pstcTxFrame != NULL) {
        switch (u8CanModule) {
        #if (LL_CAN_ENABLE == DDL_ON)
            case HAL_CAN1:
            case HAL_CAN2:
                stcCanTxFrame = *((stc_can_tx_frame_t *)pstcTxFrame);
                i32Ret = CAN_FillTxFrame(HAL_CAN_CCANx, CAN_TX_BUF_PTB, &stcCanTxFrame);
                if (i32Ret == LL_OK) {
                    CAN_StartTx(HAL_CAN_CCANx, CAN_TX_REQ_PTB);
                }
                break;
        #endif
        #if (LL_MCAN_ENABLE == DDL_ON)
            case HAL_CAN3:
            case HAL_CAN4:
                stcMcanTxMsg.ID  = pstcTxFrame->u32ID;
                stcMcanTxMsg.IDE = pstcTxFrame->IDE;
                stcMcanTxMsg.RTR = pstcTxFrame->RTR;
                stcMcanTxMsg.DLC = pstcTxFrame->DLC;
                stcMcanTxMsg.BRS = pstcTxFrame->BRS;
                stcMcanTxMsg.FDF = pstcTxFrame->FDF;
                stcMcanTxMsg.ESI = 0U;
                stcMcanTxMsg.u32StoreTxEvent = 0U;
                stcMcanTxMsg.u32MsgMarker    = 0U;
                for (i = 0U; i < m_au8HalCanDLC2Size[pstcTxFrame->DLC]; i++) {
                    stcMcanTxMsg.au8Data[i] = pstcTxFrame->au8Data[i];
                }
                i32Ret = MCAN_AddMsgToTxFifoQueue(HAL_CAN_MCANx, &stcMcanTxMsg);
                break;
        #endif
            default:
                break;
        }
    }

    return i32Ret;
}

/**
 * @brief Get a received frame from the specified CAN module.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @param  [out] pstcRxFrame            Pointer to a @ref stc_hal_can_frame_t structure which is used to save
 *                                      the received frame.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 *           - LL_ERR_BUF_EMPTY:        The selected Rx FIFO is empty.
 */
int32_t HAL_CAN_GetRxFrame(uint8_t u8CanModule, stc_hal_can_frame_t *pstcRxFrame)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    #if (LL_MCAN_ENABLE == DDL_ON)
    uint8_t i;
    stc_mcan_rx_msg_t stcMcanRxFrame;
    #endif

    if (pstcRxFrame != NULL) {
        pstcRxFrame->u8DataSize = 0U;
        switch (u8CanModule) {
        #if (LL_CAN_ENABLE == DDL_ON)
            case HAL_CAN1:
            case HAL_CAN2:
                i32Ret = CAN_GetRxFrame(HAL_CAN_CCANx, (stc_can_rx_frame_t *)pstcRxFrame);
                if (i32Ret == LL_OK) {
                    pstcRxFrame->u8DataSize = m_au8HalCanDLC2Size[pstcRxFrame->DLC];
                }
                break;
        #endif
        #if (LL_MCAN_ENABLE == DDL_ON)
            case HAL_CAN3:
            case HAL_CAN4:
                i32Ret = MCAN_GetRxMsg(HAL_CAN_MCANx, MCAN_RX_FIFO0, &stcMcanRxFrame);
                if (i32Ret == LL_OK) {
                    pstcRxFrame->u32ID = stcMcanRxFrame.ID;
                    pstcRxFrame->IDE   = stcMcanRxFrame.IDE;
                    pstcRxFrame->RTR   = stcMcanRxFrame.RTR;
                    pstcRxFrame->DLC   = stcMcanRxFrame.DLC;
                    pstcRxFrame->BRS   = stcMcanRxFrame.BRS;
                    pstcRxFrame->FDF   = stcMcanRxFrame.FDF;
                    for (i = 0U; i < m_au8HalCanDLC2Size[pstcRxFrame->DLC]; i++) {
                        pstcRxFrame->au8Data[i] = stcMcanRxFrame.au8Data[i];
                    }
                    pstcRxFrame->u8DataSize = m_au8HalCanDLC2Size[pstcRxFrame->DLC];
                }
                break;
        #endif

            default:
                break;
        }
    }

    return i32Ret;
}

/**
 * @brief Get a the error information of the specified CAN module.
 * @param  [in]  u8CanModule            HAL CAN module index.
 *                                      This parameter can be a value of @ref HAL_CAN_Module
 * @param  [out] pu8ErrInfo             Pointer to a @ref stc_hal_can_error_info_t structure which is used to save
 *                                      the error information.
 * @retval int32_t:
 *           - LL_OK:                   No error occurred.
 *           - LL_ERR_INVD_PARAM:       Invalid parameter.
 */
int32_t HAL_CAN_GetErrorInfo(uint8_t u8CanModule, stc_hal_can_error_info_t *pu8ErrInfo)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_HAL_CAN_MODULE(u8CanModule));

    #if (LL_CAN_ENABLE == DDL_ON)
    stc_can_error_info_t stcCanErr;
    #endif
    #if (LL_MCAN_ENABLE == DDL_ON)
    static const uint8_t au8McanErrorType[] = {
        HAL_CAN_ERR_NONE, HAL_CAN_ERR_STUFF, HAL_CAN_ERR_FORM, HAL_CAN_ERR_ACK,
        HAL_CAN_ERR_BIT, HAL_CAN_ERR_BIT, HAL_CAN_ERR_CRC, HAL_CAN_ERR_NONE
    };
    #endif

    if (pu8ErrInfo != NULL) {
        switch (u8CanModule) {
        #if (LL_CAN_ENABLE == DDL_ON)
            case HAL_CAN1:
            case HAL_CAN2:
                (void)CAN_GetErrorInfo(HAL_CAN_CCANx, &stcCanErr);
                pu8ErrInfo->u8ErrorType     = stcCanErr.u8ErrorType;
                pu8ErrInfo->u8DataErrorType = 0U;
                pu8ErrInfo->u8RxErrorCount  = stcCanErr.u8RxErrorCount;
                pu8ErrInfo->u8TxErrorCount  = stcCanErr.u8TxErrorCount;
                i32Ret = LL_OK;
                break;
        #endif
        #if (LL_MCAN_ENABLE == DDL_ON)
            case HAL_CAN3:
            case HAL_CAN4:
                pu8ErrInfo->u8ErrorType     = au8McanErrorType[READ_REG32_BIT(HAL_CAN_MCANx->PSR, MCAN_PSR_LEC)];
                pu8ErrInfo->u8DataErrorType = au8McanErrorType[READ_REG32_BIT(HAL_CAN_MCANx->PSR, MCAN_PSR_DLEC) >> MCAN_PSR_DLEC_POS];
                pu8ErrInfo->u8RxErrorCount  = (uint8_t)(READ_REG32_BIT(HAL_CAN_MCANx->ECR, MCAN_ECR_REC) >> MCAN_ECR_REC_POS);
                pu8ErrInfo->u8TxErrorCount  = (uint8_t)(READ_REG32_BIT(HAL_CAN_MCANx->ECR, MCAN_ECR_TEC));
                i32Ret = LL_OK;
                break;
        #endif
            default:
                break;
        }
    }

    return i32Ret;
}

/**
 * @}
 */

#endif /* LL_CAN_ENABLE || LL_MCAN_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
