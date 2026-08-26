/**
 *******************************************************************************
 * @file  ev_hc32a4a8_lqfp176_wm8988.c
 * @brief This file provides configure functions for wm8988 of the board
 *        EV_HC32A4A8_LQFP176.
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
#include "ev_hc32a4a8_lqfp176_wm8988.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup EV_HC32A4A8_LQFP176
 * @{
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_WM8988 EV_HC32A4A8_LQFP176 WM8988
 * @{
 */

#if ((BSP_WM8988_ENABLE == DDL_ON) && (BSP_EV_HC32A4A8_LQFP176 == BSP_EV_HC32A4XX))

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
/**
 * @defgroup EV_HC32A4A8_LQFP176_WM8988_Local_Variables EV_HC32A4A8_LQFP176 WM8988 Local Variables
 * @{
 */
static stc_wm8988_ll_t m_stcWm8988Config = {0};
static uint32_t m_u32AudioDataBuf[2U][BSP_WM8988_DMA_BLK_LEN];
static stc_dma_llp_descriptor_t m_stcTxLlpDesc[2U] = {0};
static stc_dma_llp_descriptor_t m_stcRxLlpDesc[2U] = {0};
static __IO uint8_t m_u8Wm8988StopFlag = 1U;
/**
 * @}
 */

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup EV_HC32A4A8_LQFP176_WM8988_Local_Functions EV_HC32A4A8_LQFP176 WM8988 Local Functions
 * @{
 */

/**
 * @brief  Initializes I2C for WM8988.
 * @param  None
 * @retval None
 */
static void BSP_WM8988_I2C_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    /* Configuration I2C GPIO */
    (void)GPIO_StructInit(&stcGpioInit);
    (void)GPIO_Init(BSP_WM8988_I2C_SCL_PORT, BSP_WM8988_I2C_SCL_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_WM8988_I2C_SDA_PORT, BSP_WM8988_I2C_SDA_PIN, &stcGpioInit);
    GPIO_SetFunc(BSP_WM8988_I2C_SCL_PORT, BSP_WM8988_I2C_SCL_PIN, BSP_WM8988_I2C_SCL_FUNC);
    GPIO_SetFunc(BSP_WM8988_I2C_SDA_PORT, BSP_WM8988_I2C_SDA_PIN, BSP_WM8988_I2C_SDA_FUNC);
    /* Enable I2C Peripheral*/
    FCG_Fcg1PeriphClockCmd(BSP_WM8988_I2C_FCG, ENABLE);
    (void)BSP_I2C_Init(BSP_WM8988_I2C_UNIT);
}

/**
 * @brief  BSP WM8988 write data.
 * @param  [in] pu8Reg:                 The register value to be write.
 * @param  [in] pu8Buf:                 The pointer to the buffer contains the data to be write.
 * @param  [in] u32Len:                 Buffer size in byte.
 * @retval None
 */
static void BSP_WM8988_I2C_Write(const uint8_t *pu8Reg, const uint8_t *pu8Buf, uint32_t u32Len)
{
    (void)BSP_I2C_Write(BSP_WM8988_I2C_UNIT, BSP_WM8988_DEV_ADDR, pu8Reg, BSP_WM8988_REG_ADDR_LEN, pu8Buf, u32Len);
}
/**
 * @}
 */

/**
 * @defgroup EV_HC32A4A8_LQFP176_WM8988_Global_Functions EV_HC32A4A8_LQFP176 WM8988 Global Functions
 * @{
 */
/**
 * @brief  De-initialize the WM8988.
 * @retval None
 */
void BSP_WM8988_DeInit(void)
{
    (void)WM8988_Reset(&m_stcWm8988Config);
    BSP_I2S_DeInit();
    /* Reset the low layer of WM8988 */
    m_stcWm8988Config.Delay         = NULL;
    m_stcWm8988Config.Init          = NULL;
    m_stcWm8988Config.Write         = NULL;
    m_stcWm8988Config.Read          = NULL;
}

/**
 * @brief  Initialize the WM8988.
 * @param  u8InputDevice:               Specifies the input device.
 *         This parameter can be one of the following values:
 *           @arg WM8988_INPUT_DEVICE_NONE:         No device input
 *           @arg WM8988_INPUT_DEVICE_MICROPHONE:   Microphone input
 *           @arg WM8988_INPUT_DEVICE_LINE:         Line in intput
 * @param  u8OutputCh:              Specifies the output device.
 *         This parameter can be one of the following values:
 *           @arg WM8988_OUTPUT_CH_NONE:        No channel output
 *           @arg WM8988_OUTPUT_CH_1:           output channel 1
 *           @arg WM8988_OUTPUT_CH_2:           output channel 2
 *           @arg WM8988_OUTPUT_CH_BOTH:        output channel 1 and 2
 * @param  u8Volume:                    Specifies the volume of input and output, Range is 0 to 100.
 * @param  u32AudioFreq:                Specifies the audio frequency of the communication.
 *         This parameter can be one of the following values:
 *           @arg WM8988_AUDIO_FREQ_8K:     8K
 *           @arg WM8988_AUDIO_FREQ_32K:    32K
 *           @arg WM8988_AUDIO_FREQ_48K:    48K
 *           @arg WM8988_AUDIO_FREQ_96K:    96K
 * @param  u8DataWidth:                 Specifies the data width.
 *         This parameter can be one of the following values:
 *           @arg WM8988_DATA_WIDTH_32BIT:  16Bits
 *           @arg WM8988_DATA_WIDTH_20BIT:  20Bits
 *           @arg WM8988_DATA_WIDTH_24BIT:  24Bits
 *           @arg WM8988_DATA_WIDTH_32BIT:  32Bits
 * @retval None
 */
int32_t BSP_WM8988_Init(uint8_t u8InputDevice, uint8_t u8OutputCh,
                        uint8_t u8Volume, uint32_t u32AudioFreq, uint8_t u8DataWidth)
{
    stc_wm8988_init_t stcWm8988Init;

    /* Configuration the low layer of WM8988 */
    m_stcWm8988Config.Delay         = DDL_DelayMS;
    m_stcWm8988Config.Init          = BSP_WM8988_I2C_Init;
    m_stcWm8988Config.Write         = BSP_WM8988_I2C_Write;
    m_stcWm8988Config.Read          = NULL;
    /* Configuration the WM8988 */
    stcWm8988Init.u8InputDevice             = u8InputDevice;
    stcWm8988Init.u8InputCh                 = WM8988_INPUT_CH_DIFFERENTIAL;
    stcWm8988Init.u8DifferentialInputCh     = WM8988_DIFFERENTIAL_INPUT_1;
    stcWm8988Init.u8OutputCh                = u8OutputCh;
    stcWm8988Init.u16OutputSrc              = WM8988_OUTPUT_SRC_DAC;
    stcWm8988Init.u32AudioFreq              = u32AudioFreq;
    stcWm8988Init.u8Volume                  = u8Volume;
    stcWm8988Init.u8DataForamt              = WM8988_DATA_FORMAT_I2S;
    stcWm8988Init.u8DataWidth               = u8DataWidth;
    (void)WM8988_Init(&m_stcWm8988Config, &stcWm8988Init);
    /* Play audio */
    (void)WM8988_Play(&m_stcWm8988Config);
    /* Init I2S */
    return BSP_I2S_Init(u32AudioFreq);
}

/**
 * @brief  Starts recording and playing audio.
 * @param  None
 * @retval None
 */
void BSP_WM8988_Record_And_Play(void)
{
    if (0U == m_u8Wm8988StopFlag) {
        return;
    }

    (void)DMA_SetDestAddr(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, (uint32_t)(&m_u32AudioDataBuf[1U][0U]));
    (void)DMA_SetTransCount(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, BSP_WM8988_DMA_BLK_LEN);
    (void)DMA_SetBlockSize(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, BSP_WM8988_DMA_BLK_SIZE);
    DMA_SetLlpAddr(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, (uint32_t)&m_stcRxLlpDesc[0U]);
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, ENABLE);

    (void)DMA_SetSrcAddr(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, (uint32_t)(&m_u32AudioDataBuf[0U][0U]));
    (void)DMA_SetTransCount(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, BSP_WM8988_DMA_BLK_LEN);
    (void)DMA_SetBlockSize(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, BSP_WM8988_DMA_BLK_SIZE);
    DMA_SetLlpAddr(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, (uint32_t)&m_stcTxLlpDesc[0U]);
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, ENABLE);

    if (RESET != I2S_GetStatus(BSP_WM8988_I2S_UNIT, I2S_FLAG_RX_ERR | I2S_FLAG_TX_ERR)) {
        I2S_ClearStatus(BSP_WM8988_I2S_UNIT, I2S_FLAG_RX_ERR | I2S_FLAG_TX_ERR);
    }
    I2S_SetReceiveFIFOLevel(BSP_WM8988_I2S_UNIT, I2S_RECEIVE_LVL3);
    I2S_SetTransFIFOLevel(BSP_WM8988_I2S_UNIT, I2S_TRANS_LVL3);
    I2S_FuncCmd(BSP_WM8988_I2S_UNIT, (I2S_FUNC_RX | I2S_FUNC_TX), ENABLE);

    m_u8Wm8988StopFlag = 0U;
}

/**
 * @brief  Stops audio playing and recording.
 * @param  None
 * @retval None
 */
void BSP_WM8988_Stop(void)
{
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, DISABLE);
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, DISABLE);

    I2S_FuncCmd(BSP_WM8988_I2S_UNIT, (I2S_FUNC_RX | I2S_FUNC_TX), DISABLE);
    I2S_SWReset(BSP_WM8988_I2S_UNIT, I2S_RST_TYPE_FIFO);
    I2S_SWReset(BSP_WM8988_I2S_UNIT, I2S_RST_TYPE_SW);
    I2S_SetReceiveFIFOLevel(BSP_WM8988_I2S_UNIT, I2S_RECEIVE_LVL4);
    I2S_SetTransFIFOLevel(BSP_WM8988_I2S_UNIT, I2S_TRANS_LVL4);

    m_u8Wm8988StopFlag = 1U;
}

/**
 * @brief  Update the audio frequency.
 * @param  u32AudioFreq:               Audio frequency used to play the audio.
 * @retval None
 */
void BSP_WM8988_SetFreq(uint32_t u32AudioFreq)
{
    (void)WM8988_SetAudioFreq(&m_stcWm8988Config, u32AudioFreq);
    (void)I2S_SetAudioFreq(BSP_WM8988_I2S_UNIT, u32AudioFreq);
}

/**
 * @brief  Set the audio volume level.
 * @param  u8Volume:                   Volume level, Range is 0 to 100.
 * @retval None
 */
void BSP_WM8988_SetVolume(uint8_t u8Volume)
{
    (void)WM8988_SetVolume(&m_stcWm8988Config, u8Volume);
}

/**
 * @brief  De-Initializes the I2S and DMA for the board.
 * @param  None
 * @retval None
 */
__WEAKDEF void BSP_I2S_DeInit(void)
{
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, DISABLE);
    (void)DMA_ChCmd(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, DISABLE);
    I2S_FuncCmd(BSP_WM8988_I2S_UNIT, I2S_FUNC_RX | I2S_FUNC_TX, DISABLE);
    I2S_SWReset(BSP_WM8988_I2S_UNIT, I2S_RST_TYPE_ALL);
}

/**
 * @brief  Initializes the I2S and DMA for the board.
 * @param  [in] u32AudioFreq            The audio frequency
 * @retval int32_t:
 *           - LL_OK: Initializes success
 *           - LL_ERR_UNINIT: Initializes DMA failed
 *           - LL_ERR_INVD_PARAM: I2S Invalid parameter
 *           - LL_ERR: I2S Set frequency failed
 */
__WEAKDEF int32_t BSP_I2S_Init(uint32_t u32AudioFreq)
{
    stc_dma_init_t stcDmaInit;
    stc_dma_llp_init_t stcDmaLlpInit;
    stc_i2s_init_t stcI2sInit;

    /* I2S pins configuration */
    GPIO_SetFunc(BSP_WM8988_I2S_CK_PORT,   BSP_WM8988_I2S_CK_PIN,   BSP_WM8988_I2S_CK_FUNC);
    GPIO_SetFunc(BSP_WM8988_I2S_WS_PORT,   BSP_WM8988_I2S_WS_PIN,   BSP_WM8988_I2S_WS_FUNC);
    GPIO_SetFunc(BSP_WM8988_I2S_SD_PORT,   BSP_WM8988_I2S_SD_PIN,   BSP_WM8988_I2S_SD_FUNC);
    GPIO_SetFunc(BSP_WM8988_I2S_SDIN_PORT, BSP_WM8988_I2S_SDIN_PIN, BSP_WM8988_I2S_SDIN_FUNC);
    GPIO_SetFunc(BSP_WM8988_I2S_MCK_PORT,  BSP_WM8988_I2S_MCK_PIN,  BSP_WM8988_I2S_MCK_FUNC);

    /* I2S DMA configuration */
    FCG_Fcg0PeriphClockCmd(BSP_WM8988_DMA_CLK, ENABLE);
    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn      = DMA_INT_DISABLE;
    stcDmaInit.u32BlockSize  = BSP_WM8988_DMA_BLK_SIZE;
    stcDmaInit.u32TransCount = BSP_WM8988_DMA_BLK_LEN;
    stcDmaInit.u32DataWidth  = DMA_DATAWIDTH_32BIT;
    (void)DMA_LlpStructInit(&stcDmaLlpInit);
    stcDmaLlpInit.u32State = DMA_LLP_ENABLE;
    stcDmaLlpInit.u32Mode  = DMA_LLP_WAIT;

    /* Configure I2S DMA transfer */
    stcDmaInit.u32DestAddr   = (uint32_t)(&BSP_WM8988_I2S_UNIT->TXBUF);
    stcDmaInit.u32SrcAddr    = (uint32_t)(&m_u32AudioDataBuf[0U][0U]);
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
    if (LL_OK != DMA_Init(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, &stcDmaInit)) {
        return LL_ERR_UNINIT;
    }
    m_stcTxLlpDesc[0U].SARx   = (uint32_t)(&m_u32AudioDataBuf[1U][0U]);
    m_stcTxLlpDesc[0U].DARx   = (uint32_t)(&BSP_WM8988_I2S_UNIT->TXBUF);
    m_stcTxLlpDesc[0U].DTCTLx = (BSP_WM8988_DMA_BLK_LEN << DMA_DTCTL_CNT_POS) | (BSP_WM8988_DMA_BLK_SIZE << DMA_DTCTL_BLKSIZE_POS);
    m_stcTxLlpDesc[0U].LLPx   = (uint32_t)&m_stcTxLlpDesc[1U];
    m_stcTxLlpDesc[0U].CHCTLx = DMA_SRC_ADDR_INC | DMA_DEST_ADDR_FIX | DMA_DATAWIDTH_32BIT |  \
                                DMA_LLP_ENABLE   | DMA_LLP_WAIT;
    m_stcTxLlpDesc[1U].SARx   = (uint32_t)(&m_u32AudioDataBuf[0U][0U]);
    m_stcTxLlpDesc[1U].DARx   = (uint32_t)(&BSP_WM8988_I2S_UNIT->TXBUF);
    m_stcTxLlpDesc[1U].DTCTLx = (BSP_WM8988_DMA_BLK_LEN << DMA_DTCTL_CNT_POS) | (BSP_WM8988_DMA_BLK_SIZE << DMA_DTCTL_BLKSIZE_POS);
    m_stcTxLlpDesc[1U].LLPx   = (uint32_t)&m_stcTxLlpDesc[0U];
    m_stcTxLlpDesc[1U].CHCTLx = DMA_SRC_ADDR_INC | DMA_DEST_ADDR_FIX | DMA_DATAWIDTH_32BIT |  \
                                DMA_LLP_ENABLE   | DMA_LLP_WAIT;
    stcDmaLlpInit.u32Addr  = (uint32_t)&m_stcTxLlpDesc[0U];
    if (LL_OK != DMA_LlpInit(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SD_CH, &stcDmaLlpInit)) {
        return LL_ERR_UNINIT;
    }
    AOS_SetTriggerEventSrc(BSP_WM8988_DMA_SD_TRIG_CH, BSP_WM8988_I2S_SD_EVT_SRC);

    /* Configure I2S DMA receive */
    stcDmaInit.u32DestAddr   = (uint32_t)(&m_u32AudioDataBuf[1U][0U]);
    stcDmaInit.u32SrcAddr    = (uint32_t)(&BSP_WM8988_I2S_UNIT->RXBUF);
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
    if (LL_OK != DMA_Init(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, &stcDmaInit)) {
        return LL_ERR_UNINIT;
    }
    m_stcRxLlpDesc[0U].SARx   = (uint32_t)(&BSP_WM8988_I2S_UNIT->RXBUF);
    m_stcRxLlpDesc[0U].DARx   = (uint32_t)(&m_u32AudioDataBuf[0U][0U]);
    m_stcRxLlpDesc[0U].DTCTLx = (BSP_WM8988_DMA_BLK_LEN << DMA_DTCTL_CNT_POS) | (BSP_WM8988_DMA_BLK_SIZE << DMA_DTCTL_BLKSIZE_POS);
    m_stcRxLlpDesc[0U].LLPx   = (uint32_t)&m_stcRxLlpDesc[1U];
    m_stcRxLlpDesc[0U].CHCTLx = DMA_SRC_ADDR_FIX | DMA_DEST_ADDR_INC | DMA_DATAWIDTH_32BIT |  \
                                DMA_LLP_ENABLE   | DMA_LLP_WAIT;
    m_stcRxLlpDesc[1U].SARx   = (uint32_t)(&BSP_WM8988_I2S_UNIT->RXBUF);
    m_stcRxLlpDesc[1U].DARx   = (uint32_t)(&m_u32AudioDataBuf[1U][0U]);
    m_stcRxLlpDesc[1U].DTCTLx = (BSP_WM8988_DMA_BLK_LEN << DMA_DTCTL_CNT_POS) | (BSP_WM8988_DMA_BLK_SIZE << DMA_DTCTL_BLKSIZE_POS);
    m_stcRxLlpDesc[1U].LLPx   = (uint32_t)&m_stcRxLlpDesc[0U];
    m_stcRxLlpDesc[1U].CHCTLx = DMA_SRC_ADDR_FIX | DMA_DEST_ADDR_INC | DMA_DATAWIDTH_32BIT |  \
                                DMA_LLP_ENABLE   | DMA_LLP_WAIT;
    stcDmaLlpInit.u32Addr  = (uint32_t)&m_stcRxLlpDesc[0U];
    if (LL_OK != DMA_LlpInit(BSP_WM8988_DMA_UNIT, BSP_WM8988_DMA_SDIN_CH, &stcDmaLlpInit)) {
        return LL_ERR_UNINIT;
    }
    AOS_SetTriggerEventSrc(BSP_WM8988_DMA_SDIN_TRIG_CH, BSP_WM8988_I2S_SDIN_EVT_SRC);

    /* Enable DMA unit */
    DMA_Cmd(BSP_WM8988_DMA_UNIT, ENABLE);

    /* Set stop flag */
    m_u8Wm8988StopFlag = 1U;

    /* Enable I2S clock */
    FCG_Fcg1PeriphClockCmd(BSP_WM8988_I2S_CLK, ENABLE);
    CLK_SetI2SClockSrc(BSP_WM8988_I2S_CLK_CH, BSP_WM8988_I2S_CLK_SRC);
    /* I2S configuration */
    (void)I2S_StructInit(&stcI2sInit);
    stcI2sInit.u32ClockSrc         = I2S_CLK_SRC_PLL;
    stcI2sInit.u32Mode             = I2S_MD_MASTER;
    stcI2sInit.u32Protocol         = I2S_PROTOCOL_PHILLIPS;
    stcI2sInit.u32TransMode        = I2S_TRANS_MD_FULL_DUPLEX;
    stcI2sInit.u32AudioFreq        = u32AudioFreq;
    stcI2sInit.u32ChWidth          = I2S_CH_LEN_32BIT;
    stcI2sInit.u32DataWidth        = I2S_DATA_LEN_32BIT;
    stcI2sInit.u32TransFIFOLevel   = I2S_TRANS_LVL4;
    stcI2sInit.u32ReceiveFIFOLevel = I2S_RECEIVE_LVL4;
    stcI2sInit.u32MCKOutput        = I2S_MCK_OUTPUT_ENABLE;
    return I2S_Init(BSP_WM8988_I2S_UNIT, &stcI2sInit);
}

/**
 * @}
 */

#endif /* (BSP_WM8988_ENABLE && BSP_EV_HC32A4A8_LQFP176) */

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
