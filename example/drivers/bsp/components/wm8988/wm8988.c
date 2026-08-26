/**
 *******************************************************************************
 * @file  wm8988.c
 * @brief This midware file provides firmware functions to manage the codec
 *        component library for wm8988.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-06-30       CDT             First version
   2024-08-31       CDT             Add prefix to static global variable
   2024-09-13       CDT             Fixed MISRA-2012 warnings
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
#include "wm8988.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup Components
 * @{
 */

/**
 * @defgroup WM8988 Codec WM8988
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup WM8988_Local_Macros WM8988 Local Macros
 * @{
 */

/* WM8988 volume convert */
#define WM8988_OUTPUTVOL_CONV(__VOL__) (((__VOL__) >= 100U) ? 80U : ((uint8_t)((((uint32_t)__VOL__) * 80U) / 100U)))

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
 * @defgroup WM8988_Local_Variables WM8988 Local Variables
 * @{
 */

static uint8_t m_u8InputDevice  = 0U;
static uint8_t m_u8OutputCh = 0U;
static uint16_t m_u16OutputSrc  = 0U;

/**
 * @}
 */
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup WM8988_Global_Functions WM8988 Global Functions
 * @{
 */

int32_t WM8988_WriteReg(const stc_wm8988_ll_t *pstcWm8988LL, uint8_t u8Reg, uint16_t u16Data)
{
    int32_t i32Ret = LL_OK;
    uint8_t u8WriteData[2];

    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        u8WriteData[0] = (u8Reg << 1U) | (uint8_t)(u16Data >> 8U);
        u8WriteData[1] = (uint8_t)(u16Data & 0xFFU);
        pstcWm8988LL->Write(&u8WriteData[0], &u8WriteData[1], 1U);
    }

    return i32Ret;
}

/**
 * @brief  Reset WM8988.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure
 * @retval int32_t:
 *           - LL_OK: Reset success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_Reset(const stc_wm8988_ll_t *pstcWm8988LL)
{
    int32_t i32Ret = LL_OK;

    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Writing 00000000 to register resets device */
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R15_RESET, 0U);
        m_u8InputDevice  = 0U;
        m_u8OutputCh = 0U;
        m_u16OutputSrc = 0x50U;
    }

    return i32Ret;
}

/**
 * @brief  Initialize WM8988.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure.
 * @param  [in] pstcWm8988Init          Pointer to a @ref stc_wm8988_init_t structure
 * @retval int32_t:
 *           - LL_OK: Initialize success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_Init(const stc_wm8988_ll_t *pstcWm8988LL, const stc_wm8988_init_t *pstcWm8988Init)
{
    int32_t i32Ret = LL_OK;
    uint16_t u16PWR1;
    uint16_t u16PWR2;
    uint16_t u16ADCIN;
    uint16_t u16ADCMODE;
    uint16_t u16INV;

    if ((pstcWm8988LL == NULL) || (NULL == pstcWm8988Init) || (pstcWm8988LL->Init == NULL)
        || (pstcWm8988LL->Write == NULL) || (pstcWm8988LL->Delay == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Initialize the control interface of the codec */
        pstcWm8988LL->Init();
        /* Writing 00000000 to register resets device */
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R15_RESET, 0U);
        pstcWm8988LL->Delay(50U);

        m_u8InputDevice  = pstcWm8988Init->u8InputDevice;
        m_u8OutputCh = pstcWm8988Init->u8OutputCh;
        m_u16OutputSrc = pstcWm8988Init->u16OutputSrc;
        /* Input Configurations*/
        if (WM8988_INPUT_DEVICE_NONE != m_u8InputDevice) {
            u16INV = 0x117;
            switch (m_u8InputDevice) {
                case WM8988_INPUT_DEVICE_MICROPHONE:
                    u16PWR1 = 0xFCU;
                    u16ADCIN = 0x10U;
                    break;
                case WM8988_INPUT_DEVICE_LINE:
                    u16PWR1 = 0xFCU;
                    u16ADCIN = 0x00U;
                    break;
                default:
                    /* Invalid input device */
                    u16PWR1 = 0xC0;
                    u16ADCIN = 0x00U;
                    break;
            }
        } else {
            u16PWR1 = 0xC0U;
            u16ADCIN = 0x00U;
            u16INV = 0x197;
        }

        u16ADCIN |= (uint16_t)pstcWm8988Init->u8InputCh << 6U;
        u16ADCMODE = ((uint16_t)pstcWm8988Init->u8DifferentialInputCh << 8U) & 0x100U;
        /* Output Configurations */
        if (WM8988_OUTPUT_CH_NONE != m_u8OutputCh) {
            switch (m_u8OutputCh) {
                case WM8988_OUTPUT_CH_1:
                    u16PWR2 = 0x1E0U;
                    break;
                case WM8988_OUTPUT_CH_2:
                    u16PWR2 = 0x198U;
                    break;
                case WM8988_OUTPUT_CH_BOTH:
                    u16PWR2 = 0x1F8U;
                    break;
                default:
                    u16PWR2 = 0x00U;
                    break;
            }
        } else {
            u16PWR2 = 0x00U;
        }
        /* Power control */
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R25_PWR1, u16PWR1);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R26_PWR2, u16PWR2);
        /* Analogue audio path control */
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R0_LINV, u16INV);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R1_RINV, u16INV);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R31_ADCMODE, u16ADCMODE);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R32_LADCIN, u16ADCIN);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R33_RADCIN, u16ADCIN);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R34_LOUTM1, m_u16OutputSrc);
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R36_ROUTM1, m_u16OutputSrc);
        /* Set audio frequency */
        (void)WM8988_SetAudioFreq(pstcWm8988LL, pstcWm8988Init->u32AudioFreq);
        /* Configure digital audio interface format, Slave mode */
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R7_IFACE, (uint16_t)pstcWm8988Init->u8DataForamt | (uint16_t)pstcWm8988Init->u8DataWidth);
        /* Set audio volume */
        (void)WM8988_SetVolume(pstcWm8988LL, pstcWm8988Init->u8Volume);
    }

    return i32Ret;
}

/**
 * @brief  Start the audio codec play.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure.
 * @retval int32_t:
 *           - LL_OK: Operation success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_Play(const stc_wm8988_ll_t *pstcWm8988LL)
{
    int32_t i32Ret = LL_OK;

    /* Unmute the output */
    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        if ((WM8988_OUTPUT_CH_NONE != m_u8OutputCh) && (WM8988_OUTPUT_SRC_NONE != m_u16OutputSrc)) {
            if (WM8988_OUTPUT_SRC_DAC == m_u16OutputSrc) {
                /* Disable DAC soft mute */
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R5_ADCDAC, 0x00U);
            } else {
                /* Unmute MIXSEL Signal to Mixer */
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R34_LOUTM1, m_u16OutputSrc);
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R36_ROUTM1, m_u16OutputSrc);
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Stop the audio codec play.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure.
 * @retval int32_t:
 *           - LL_OK: Operation success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_Stop(const stc_wm8988_ll_t *pstcWm8988LL)
{
    int32_t i32Ret = LL_OK;

    /* Mute the output */
    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        if ((WM8988_OUTPUT_CH_NONE != m_u8OutputCh) && (WM8988_OUTPUT_SRC_NONE != m_u16OutputSrc)) {
            if (WM8988_OUTPUT_SRC_DAC == m_u16OutputSrc) {
                /* Enable DAC soft mute */
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R5_ADCDAC, 0x08U);
            } else {
                /* Mute MIXSEL Signal to Mixer */
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R34_LOUTM1, WM8988_OUTPUT_SRC_NONE);
                (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R36_ROUTM1, WM8988_OUTPUT_SRC_NONE);
            }
        }
    }

    return i32Ret;
}

/**
 * @brief  Set the new audio frequency.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure.
 * @param  [in] u32Freq                 The audio frequency
 *         This parameter can be one of the following values:
 *           @arg WM8988_AUDIO_FREQ_8K
 *           @arg WM8988_AUDIO_FREQ_32K
 *           @arg WM8988_AUDIO_FREQ_48K
 *           @arg WM8988_AUDIO_FREQ_96K
 * @retval int32_t:
 *           - LL_OK: Operation success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_SetAudioFreq(const stc_wm8988_ll_t *pstcWm8988LL, uint32_t u32Freq)
{
    int32_t i32Ret = LL_OK;
    uint16_t u16WriteData;

    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Base Over-Sampling Rate = 256fs */
        switch (u32Freq) {
            case WM8988_AUDIO_FREQ_8K:
                u16WriteData = 0x0CU;
                break;
            case WM8988_AUDIO_FREQ_32K:
                u16WriteData = 0x18U;
                break;
            case WM8988_AUDIO_FREQ_48K:
                u16WriteData = 0x00U;
                break;
            case WM8988_AUDIO_FREQ_96K:
                u16WriteData = 0x1CU;
                break;
            default:
                /* Sample Rate = 48 (KHz) */
                u16WriteData = 0x00U;
                break;
        }
        (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R8_SRATE, u16WriteData);
    }

    return i32Ret;
}

/**
 * @brief  Set the codec volume level.
 * @param  [in] pstcWm8988LL            Pointer to a @ref stc_wm8988_ll_t structure.
 * @param  [in] u8Volume                The codec volume level
 *         This parameter can be a value between Min_Data = 0 and Max_Data = 100
 * @retval int32_t:
 *           - LL_OK: Operation success
 *           - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t WM8988_SetVolume(const stc_wm8988_ll_t *pstcWm8988LL, uint8_t u8Volume)
{
    int32_t i32Ret = LL_OK;
    uint8_t u8ConvVolume;
    uint16_t u16WriteData;

    if ((pstcWm8988LL == NULL) || (pstcWm8988LL->Write == NULL)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        if (WM8988_OUTPUT_CH_NONE != m_u8OutputCh) {
            /* Output volume */
            u8ConvVolume = WM8988_OUTPUTVOL_CONV(u8Volume) + 0x2FU;
            if (u8ConvVolume > 0x2FU) {
                u16WriteData = (1U << 8U) | (uint16_t)u8ConvVolume;
            } else {
                /* Mute output  */
                u16WriteData = 0x100U;
            }
            switch (m_u8OutputCh) {
                case WM8988_OUTPUT_CH_1:
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R2_LOUT1V, u16WriteData);
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R3_ROUT1V, u16WriteData);
                    break;
                case WM8988_OUTPUT_CH_2:
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R40_LOUT2V, u16WriteData);
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R41_ROUT2V, u16WriteData);
                    break;
                case WM8988_OUTPUT_CH_BOTH:
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R2_LOUT1V, u16WriteData);
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R3_ROUT1V, u16WriteData);
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R40_LOUT2V, u16WriteData);
                    (void)WM8988_WriteReg(pstcWm8988LL, WM8988_R41_ROUT2V, u16WriteData);
                    break;
                default:
                    break;
            }
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
