/**
 *******************************************************************************
 * @file  ev_hc32a4a8_lqfp176_w9825g6kh.c
 * @brief This file provides configure functions for w9825g6kh of the
 *        board EV_HC32A4A8_LQFP176.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_clk.h"
#include "hc32_ll_fcg.h"
#include "ev_hc32a4a8_lqfp176_w9825g6kh.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176
 * @{
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_W9825G6KH EV_HC32A4A8_LQFP176 W9825G6KH
 * @{
 */

#if ((BSP_W9825G6KH_ENABLE == DDL_ON) && (BSP_EV_HC32A4A8_LQFP176 == BSP_EV_HC32A4XX))

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_W9825G6KH_Local_Macros EV_HC32A4A8_LQFP176 W9825G6KH Local Macros
 * @{
 */

/**
 * @defgroup DMC_Max_Timeout DMC Max Timeout
 * @{
 */
#define DMC_MAX_TIMEOUT                           (0x100000UL)
/**
 * @}
 */

/**
 * @defgroup W9825G6KH_Mode_Register_Field W9825G6KH Mode Register Field
 * @{
 */
/* W9825G6KH burst length definition */
#define W9825G6KH_MR_BURST_1BEAT                  (0UL)
#define W9825G6KH_MR_BURST_2BEAT                  (1UL)
#define W9825G6KH_MR_BURST_4BEAT                  (2UL)
#define W9825G6KH_MR_BURST_8BEAT                  (3UL)
#define W9825G6KH_MR_BURST_LEN_FULLPAGE           (7UL)

/* W9825G6KH burst type definition */
#define W9825G6KH_MR_BURST_TYPE_SEQUENTIAL        (0UL)
#define W9825G6KH_MR_BURST_TYPE_INTERLEAVED       (1UL << 3)

/* W9825G6KH CAS latency definition */
#define W9825G6KH_MR_CAS_LATENCY_2                (2UL << 4)
#define W9825G6KH_MR_CAS_LATENCY_3                (3UL << 4)

/* W9825G6KH write burst mode definition */
#define W9825G6KH_MR_WRITEBURST_PROGRAMMED        (0UL)
#define W9825G6KH_MR_WRITEBURST_SINGLE            (1UL << 9)

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
/**
 * @addtogroup EV_HC32A4A8_LQFP176_W9825G6KH_Local_Functions
 * @{
 */
static void BSP_DMC_PortInit(void);
static void BSP_SDRAM_InitSequence(uint32_t u32Chip, uint32_t u32Bank, uint32_t u32MdRegValue);
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_W9825G6KH_Global_Functions EV_HC32A4A8_LQFP176 W9825G6KH Global Functions
 * @{
 */

/**
 * @brief  Initialize DMC for W9825G6KH.
 * @param  None
 * @retval int32_t:
 *           - LL_OK:                   Initialize successfully.
 *           - LL_ERR:                  Initialize unsuccessfully.
 */
int32_t BSP_W9825G6KH_Init(void)
{
    __IO uint32_t u32To = 0UL;
    uint32_t u32DmcClock;
    uint32_t u32MdRegValue;
    uint32_t u32RefreshPeriod;
    int32_t i32Ret = LL_OK;
    stc_exmc_dmc_init_t stcDmcInit;
    stc_exmc_dmc_chip_config_t stcCsConfig;

    /* Initialization DMC port.*/
    BSP_DMC_PortInit();

    /* Enable DMC clock */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_DMC, ENABLE);

    /* Enable DMC. */
    EXMC_DMC_Cmd(ENABLE);

    /* Configure DMC width && refresh period & chip & timing. */
    u32DmcClock = CLK_GetBusClockFreq(CLK_BUS_EXCLK);
    u32RefreshPeriod = (u32DmcClock / (BSP_W9825G6KH_REFRESH_CYCLES * 1000UL)) * BSP_W9825G6KH_REFRESH_TIME - 20UL;
    (void)EXMC_DMC_StructInit(&stcDmcInit);
    stcDmcInit.u32RefreshPeriod = u32RefreshPeriod;
    stcDmcInit.u32ColumnBitsNumber = EXMC_DMC_COLUMN_BITS_NUM9;
    stcDmcInit.u32RowBitsNumber = EXMC_DMC_ROW_BITS_NUM13;
    stcDmcInit.u32MemBurst = EXMC_DMC_BURST_1BEAT;
    stcDmcInit.u32AutoRefreshChips = EXMC_DMC_AUTO_REFRESH_1CHIP;

    /* EXCLK bus frequency@60MHz: 3.3V */
    stcDmcInit.stcTimingConfig.u8CASL = 2U;
    stcDmcInit.stcTimingConfig.u8DQSS = 0U;
    stcDmcInit.stcTimingConfig.u8MRD = 2U;      /* tMRD: min=2CLK */
    stcDmcInit.stcTimingConfig.u8RAS = 3U;      /* tRAS: min=42ns */
    stcDmcInit.stcTimingConfig.u8RC = 4U;       /* tRC:  min=60ns */
    stcDmcInit.stcTimingConfig.u8RCD_B = 3U;    /* tRCD: min=15ns */
    stcDmcInit.stcTimingConfig.u8RCD_P = 0U;
    stcDmcInit.stcTimingConfig.u8RFC_B = 4U;    /* tRFC: min=60ns */
    stcDmcInit.stcTimingConfig.u8RFC_P = 0U;
    stcDmcInit.stcTimingConfig.u8RP_B = 3U;     /* tRP:  min=15ns */
    stcDmcInit.stcTimingConfig.u8RP_P = 0U;
    stcDmcInit.stcTimingConfig.u8RRD = 2U;      /* tRRD: min=2CLK */
    stcDmcInit.stcTimingConfig.u8WR = 2U;       /* tWR:  min=2CLK */
    stcDmcInit.stcTimingConfig.u8WTR = 1U;
    stcDmcInit.stcTimingConfig.u8XP = 1U;
    stcDmcInit.stcTimingConfig.u8XSR = 5U;      /* tXSR: min=72ns */
    stcDmcInit.stcTimingConfig.u8ESR = 5U;
    (void)EXMC_DMC_Init(&stcDmcInit);

    /* Configure DMC address space. */
    stcCsConfig.u32AddrMask = BSP_W9825G6KH_ADDR_MASK;
    stcCsConfig.u32AddrMatch = BSP_W9825G6KH_ADDR_MATCH;
    stcCsConfig.u32AddrDecodeMode = EXMC_DMC_CS_DECODE_ROWBANKCOL;
    (void)EXMC_DMC_ChipConfig(BSP_W9825G6KH_CHIP, &stcCsConfig);

    /* SDRAM initialization sequence. */
    u32MdRegValue = (W9825G6KH_MR_BURST_TYPE_SEQUENTIAL | W9825G6KH_MR_WRITEBURST_PROGRAMMED);
    if (2U == stcDmcInit.stcTimingConfig.u8CASL) {
        u32MdRegValue |= W9825G6KH_MR_CAS_LATENCY_2;
    } else {
        u32MdRegValue |= W9825G6KH_MR_CAS_LATENCY_3;
    }

    if (EXMC_DMC_BURST_1BEAT == stcDmcInit.u32MemBurst) {
        u32MdRegValue |= W9825G6KH_MR_BURST_1BEAT;
    } else if (EXMC_DMC_BURST_2BEAT == stcDmcInit.u32MemBurst) {
        u32MdRegValue |= W9825G6KH_MR_BURST_2BEAT;
    } else if (EXMC_DMC_BURST_4BEAT == stcDmcInit.u32MemBurst) {
        u32MdRegValue |= W9825G6KH_MR_BURST_4BEAT;
    } else {
        u32MdRegValue |= W9825G6KH_MR_BURST_8BEAT;
    }

    BSP_SDRAM_InitSequence(BSP_W9825G6KH_CHIP, 0UL, u32MdRegValue);

    /* Switch state from configure to ready */
    EXMC_DMC_SetState(EXMC_DMC_CTRL_STATE_GO);
    EXMC_DMC_SetState(EXMC_DMC_CTRL_STATE_WAKEUP);
    EXMC_DMC_SetState(EXMC_DMC_CTRL_STATE_GO);

    /* Check status */
    while (EXMC_DMC_CURR_STATUS_RDY != EXMC_DMC_GetStatus()) {
        if (u32To > DMC_MAX_TIMEOUT) {
            i32Ret = LL_ERR;
            break;
        }
        u32To++;
    }

    return i32Ret;
}

/**
 * @brief  Get memory information.
 * @param  [out] pu32MemoryStartAddr    The pointer for memory start address
 * @param  [out] pu32MemoryByteSize     The pointer for memory size(unit: Byte)
 * @retval None
 */
void BSP_W9825G6KH_GetMemInfo(uint32_t *pu32MemoryStartAddr, uint32_t *pu32MemoryByteSize)
{
    if (NULL != pu32MemoryStartAddr) {
        *pu32MemoryStartAddr = BSP_W9825G6KH_START_ADDR;
    }

    if (NULL != pu32MemoryByteSize) {
        *pu32MemoryByteSize = BSP_W9825G6KH_SIZE;
    }
}

/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_W9825G6KH_Local_Functions EV_HC32A4A8_LQFP176 W9825G6KH Local Functions
 * @{
 */

/**
 * @brief  Initialize DMC port.
 * @param  None
 * @retval None
 */
static void BSP_DMC_PortInit(void)
{
    stc_gpio_init_t stcGpioInit;

    /************************* Set pin drive capacity *************************/
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;

    /* DMC_CKE */
    (void)GPIO_Init(BSP_W9825G6KH_CKE_PORT, BSP_W9825G6KH_CKE_PIN, &stcGpioInit);

    /* DMC_CLK */
    (void)GPIO_Init(BSP_W9825G6KH_CLK_PORT, BSP_W9825G6KH_CLK_PIN, &stcGpioInit);

    /* DMC_LDQM && DMC_UDQM */
    (void)GPIO_Init(BSP_W9825G6KH_DQM0_PORT, BSP_W9825G6KH_DQM0_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DQM1_PORT, BSP_W9825G6KH_DQM1_PIN, &stcGpioInit);

    /* DMC_BA[0:1] */
    (void)GPIO_Init(BSP_W9825G6KH_BA0_PORT, BSP_W9825G6KH_BA0_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_BA1_PORT, BSP_W9825G6KH_BA1_PIN, &stcGpioInit);

    /* DMC_CAS && DMC_RAS */
    (void)GPIO_Init(BSP_W9825G6KH_CAS_PORT, BSP_W9825G6KH_CAS_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_RAS_PORT, BSP_W9825G6KH_RAS_PIN, &stcGpioInit);

    /* DMC_WE */
    (void)GPIO_Init(BSP_W9825G6KH_WE_PORT, BSP_W9825G6KH_WE_PIN, &stcGpioInit);

    /* DMC_DATA[0:15] */
    (void)GPIO_Init(BSP_W9825G6KH_DATA0_PORT, BSP_W9825G6KH_DATA0_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA1_PORT, BSP_W9825G6KH_DATA1_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA2_PORT, BSP_W9825G6KH_DATA2_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA3_PORT, BSP_W9825G6KH_DATA3_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA4_PORT, BSP_W9825G6KH_DATA4_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA5_PORT, BSP_W9825G6KH_DATA5_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA6_PORT, BSP_W9825G6KH_DATA6_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA7_PORT, BSP_W9825G6KH_DATA7_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA8_PORT, BSP_W9825G6KH_DATA8_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA9_PORT, BSP_W9825G6KH_DATA9_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA10_PORT, BSP_W9825G6KH_DATA10_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA11_PORT, BSP_W9825G6KH_DATA11_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA12_PORT, BSP_W9825G6KH_DATA12_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA13_PORT, BSP_W9825G6KH_DATA13_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA14_PORT, BSP_W9825G6KH_DATA14_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_DATA15_PORT, BSP_W9825G6KH_DATA15_PIN, &stcGpioInit);

    /* DMC_ADD[0:11]*/
    (void)GPIO_Init(BSP_W9825G6KH_ADD0_PORT, BSP_W9825G6KH_ADD0_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD1_PORT, BSP_W9825G6KH_ADD1_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD2_PORT, BSP_W9825G6KH_ADD2_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD3_PORT, BSP_W9825G6KH_ADD3_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD4_PORT, BSP_W9825G6KH_ADD4_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD5_PORT, BSP_W9825G6KH_ADD5_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD6_PORT, BSP_W9825G6KH_ADD6_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD7_PORT, BSP_W9825G6KH_ADD7_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD8_PORT, BSP_W9825G6KH_ADD8_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD9_PORT, BSP_W9825G6KH_ADD9_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD10_PORT, BSP_W9825G6KH_ADD10_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD11_PORT, BSP_W9825G6KH_ADD11_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_W9825G6KH_ADD12_PORT, BSP_W9825G6KH_ADD12_PIN, &stcGpioInit);

    /************************** Set EXMC pin function *************************/
    /* DMC_CKE */
    GPIO_SetFunc(BSP_W9825G6KH_CKE_PORT, BSP_W9825G6KH_CKE_PIN, BSP_W9825G6KH_CKE_FUNC);

    /* DMC_CLK */
    GPIO_SetFunc(BSP_W9825G6KH_CLK_PORT, BSP_W9825G6KH_CLK_PIN, BSP_W9825G6KH_CLK_FUNC);

    /* DMC_LDQM && DMC_UDQM */
    GPIO_SetFunc(BSP_W9825G6KH_DQM0_PORT, BSP_W9825G6KH_DQM0_PIN, BSP_W9825G6KH_DQM0_FUNC);
    GPIO_SetFunc(BSP_W9825G6KH_DQM1_PORT, BSP_W9825G6KH_DQM1_PIN, BSP_W9825G6KH_DQM1_FUNC);

    /* DMC_BA[0:1] */
    GPIO_SetFunc(BSP_W9825G6KH_BA0_PORT, BSP_W9825G6KH_BA0_PIN, BSP_W9825G6KH_BA0_FUNC);
    GPIO_SetFunc(BSP_W9825G6KH_BA1_PORT, BSP_W9825G6KH_BA1_PIN, BSP_W9825G6KH_BA1_FUNC);

    /* DMC_CS */
    GPIO_SetFunc(BSP_W9825G6KH_CS0_PORT, BSP_W9825G6KH_CS0_PIN, BSP_W9825G6KH_CS0_FUNC);

    /* DMC_CAS && DMC_RAS */
    GPIO_SetFunc(BSP_W9825G6KH_CAS_PORT, BSP_W9825G6KH_CAS_PIN, BSP_W9825G6KH_CAS_FUNC);
    GPIO_SetFunc(BSP_W9825G6KH_RAS_PORT, BSP_W9825G6KH_RAS_PIN, BSP_W9825G6KH_RAS_FUNC);

    /* DMC_WE */
    GPIO_SetFunc(BSP_W9825G6KH_WE_PORT, BSP_W9825G6KH_WE_PIN, BSP_W9825G6KH_WE_FUNC);

    /* DMC_DATA[0:15] */
    GPIO_SetFunc(BSP_W9825G6KH_DATA0_PORT, BSP_W9825G6KH_DATA0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA1_PORT, BSP_W9825G6KH_DATA1_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA2_PORT, BSP_W9825G6KH_DATA2_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA3_PORT, BSP_W9825G6KH_DATA3_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA4_PORT, BSP_W9825G6KH_DATA4_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA5_PORT, BSP_W9825G6KH_DATA5_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA6_PORT, BSP_W9825G6KH_DATA6_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA7_PORT, BSP_W9825G6KH_DATA7_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA8_PORT, BSP_W9825G6KH_DATA8_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA9_PORT, BSP_W9825G6KH_DATA9_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA10_PORT, BSP_W9825G6KH_DATA10_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA11_PORT, BSP_W9825G6KH_DATA11_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA12_PORT, BSP_W9825G6KH_DATA12_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA13_PORT, BSP_W9825G6KH_DATA13_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA14_PORT, BSP_W9825G6KH_DATA14_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_DATA15_PORT, BSP_W9825G6KH_DATA15_PIN, GPIO_FUNC_12);

    /* DMC_ADD[0:11]*/
    GPIO_SetFunc(BSP_W9825G6KH_ADD0_PORT, BSP_W9825G6KH_ADD0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD1_PORT, BSP_W9825G6KH_ADD1_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD2_PORT, BSP_W9825G6KH_ADD2_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD3_PORT, BSP_W9825G6KH_ADD3_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD4_PORT, BSP_W9825G6KH_ADD4_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD5_PORT, BSP_W9825G6KH_ADD5_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD6_PORT, BSP_W9825G6KH_ADD6_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD7_PORT, BSP_W9825G6KH_ADD7_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD8_PORT, BSP_W9825G6KH_ADD8_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD9_PORT, BSP_W9825G6KH_ADD9_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD10_PORT, BSP_W9825G6KH_ADD10_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD11_PORT, BSP_W9825G6KH_ADD11_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_W9825G6KH_ADD12_PORT, BSP_W9825G6KH_ADD12_PIN, GPIO_FUNC_12);
}

/**
 * @brief  SDRAM W9825G6KH initialization sequence.
 * @param  [in] u32Chip                     The command chip number.
 *         This parameter can be one of the following values:
 *           @arg EXMC_DMC_CHIP0:           Chip 0
 *           @arg EXMC_DMC_CHIP1:           Chip 1
 *           @arg EXMC_DMC_CHIP2:           Chip 2
 *           @arg EXMC_DMC_CHIP3:           Chip 3
 * @param  [in] u32Bank                     The command bank.
 *         This parameter can be one of the following values:
 *           @arg EXMC_DMC_BANK0:           Bank 0
 *           @arg EXMC_DMC_BANK1:           Bank 1
 *           @arg EXMC_DMC_BANK2:           Bank 2
 *           @arg EXMC_DMC_BANK3:           Bank 3
 * @param  [in] u32MdRegValue               The SDRAM mode register value
 * @retval None
 */
static void BSP_SDRAM_InitSequence(uint32_t u32Chip, uint32_t u32Bank, uint32_t u32MdRegValue)
{
    /* SDRAM initialization sequence:
       CMD NOP->PrechargeAll->AutoRefresh->AutoRefresh->MdRegConfig->NOP */
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_NOP, 0UL);
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_PRECHARGE_ALL, 0UL);
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_AUTO_REFRESH, 0UL);
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_AUTO_REFRESH, 0UL);
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_MDREG_CONFIG, u32MdRegValue);
    (void)EXMC_DMC_SetCommand(u32Chip, u32Bank, EXMC_DMC_CMD_NOP, 0UL);
}

/**
 * @}
 */

#endif /* BSP_W9825G6KH_ENABLE && BSP_EV_HC32A4A8_LQFP176 */

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
