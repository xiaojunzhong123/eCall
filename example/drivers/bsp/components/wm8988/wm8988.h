/**
 *******************************************************************************
 * @file  wm8988.h
 * @brief This file contains all the functions prototypes of the codec component
 *        library for wm8988.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-06-30       CDT             First version
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
#ifndef __WM8988_H__
#define __WM8988_H__

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
 * @addtogroup WM8988
 * @{
 */

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup WM8988_Global_Types WM8988 Global Types
 * @{
 */

/**
 * @brief WM8988 Init structure definition
 */
typedef struct {
    uint8_t  u8InputDevice;              /*!< Specifies the intput device.
                                             This parameter can be a value of @ref WM8988_Input_Device */
    uint8_t  u8InputCh;                  /*!< Specifies the intput channel.
                                             This parameter can be a value of @ref WM8988_Input_Channel */
    uint8_t  u8DifferentialInputCh;      /*!< Specifies the differential intput channel.
                                             This parameter can be a value of @ref WM8988_Differential_Input_Channel */
    uint8_t  u8OutputCh;                 /*!< Specifies the output channel.
                                             This parameter can be a value of @ref WM8988_Output_Channel */
    uint16_t u16OutputSrc;               /*!< Specifies the data source for the output.
                                             This parameter can be a value of @ref WM8988_Output_Source */
    uint32_t u32AudioFreq;               /*!< Specifies the audio frequency of the communication.
                                             This parameter can be a value of @ref WM8988_Audio_Frequency */
    uint8_t  u8Volume;                   /*!< Specifies the volume of input and output.
                                             This parameter can be a value between Min_Data = 0 and Max_Data = 100 */
    uint8_t  u8DataForamt;               /*!< Specifies the data format of the communication.
                                             This parameter can be a value of @ref WM8988_Data_Format */
    uint8_t  u8DataWidth;                /*!< Specifies the data width of the communication.
                                             This parameter can be a value of @ref WM8988_Data_Width */
} stc_wm8988_init_t;

/**
 * @brief WM8988 low layer structure definition
 */
typedef struct {
    /* Methods */
    void (*Delay)(uint32_t);
    void (*Init)(void);
    void (*Write)(const uint8_t *, const uint8_t *, uint32_t);
    void (*Read)(const uint8_t *, uint8_t *, uint32_t);
} stc_wm8988_ll_t;

/**
 * @}
 */
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup WM8988_Global_Macros WM8988 Global Macros
 * @{
 */

/**
 * @defgroup WM8988_Register WM8988 Register
 * @{
 */
#define WM8988_R0_LINV                          (0x00U) /* Left Input volume         */
#define WM8988_R1_RINV                          (0x01U) /* Right Input volume        */
#define WM8988_R2_LOUT1V                        (0x02U) /* LOUT1 volume              */
#define WM8988_R3_ROUT1V                        (0x03U) /* ROUT1 volume              */
#define WM8988_R5_ADCDAC                        (0x05U) /* ADC & DAC Control         */
#define WM8988_R7_IFACE                         (0x07U) /* Audio Interface           */
#define WM8988_R8_SRATE                         (0x08U) /* Sample rate               */
#define WM8988_R10_LDACV                        (0x0AU) /* Left DAC volume           */
#define WM8988_R11_RDACV                        (0x0BU) /* Right DAC volume          */
#define WM8988_R12_BASS                         (0x0CU) /* Bass control              */
#define WM8988_R13_TREBLE                       (0x0DU) /* Treble control            */
#define WM8988_R15_RESET                        (0x0FU) /* Reset                     */
#define WM8988_R16_3D                           (0x10U) /* 3D control                */
#define WM8988_R17_ALC1                         (0x11U) /* ALC1                      */
#define WM8988_R18_ALC2                         (0x12U) /* ALC2                      */
#define WM8988_R19_ALC3                         (0x13U) /* ALC3                      */
#define WM8988_R20_NGATE                        (0x14U) /* Noise Gate                */
#define WM8988_R21_LADCV                        (0x15U) /* Left ADC volume           */
#define WM8988_R22_RADCV                        (0x16U) /* Right ADC volume          */
#define WM8988_R23_ADCTL1                       (0x17U) /* Additional control(1)     */
#define WM8988_R24_ADCTL2                       (0x18U) /* Additional control(2)     */
#define WM8988_R25_PWR1                         (0x19U) /* Pwr Mgmt (1)              */
#define WM8988_R26_PWR2                         (0x1AU) /* Pwr Mgmt (2)              */
#define WM8988_R27_ADCTL3                       (0x1BU) /* Additional Control (3)    */
#define WM8988_R31_ADCMODE                      (0x1FU) /* ADC input mode            */
#define WM8988_R32_LADCIN                       (0x20U) /* ADCL signal path          */
#define WM8988_R33_RADCIN                       (0x21U) /* ADCR signal path          */
#define WM8988_R34_LOUTM1                       (0x22U) /* Left out Mix (1)          */
#define WM8988_R35_LOUTM2                       (0x23U) /* Left out Mix (2)          */
#define WM8988_R36_ROUTM1                       (0x24U) /* Right out Mix (1)         */
#define WM8988_R37_ROUTM2                       (0x25U) /* Right out Mix (2)         */
#define WM8988_R40_LOUT2V                       (0x28U) /* LOUT2 volume              */
#define WM8988_R41_ROUT2V                       (0x29U) /* ROUT2 volume              */
#define WM8988_R67_LPPB                         (0x43U) /* Low Power Playback        */
/**
 * @}
 */

/**
 * @defgroup WM8988_Input_Device WM8988 Input Device
 * @{
 */
#define WM8988_INPUT_DEVICE_NONE                (0x00U)
#define WM8988_INPUT_DEVICE_MICROPHONE          (0x01U)
#define WM8988_INPUT_DEVICE_LINE                (0x02U)
/**
 * @}
 */

/**
 * @defgroup WM8988_Input_Channel WM8988 Input Channel
 * @{
 */
#define WM8988_INPUT_CH_1                       (0x00U)
#define WM8988_INPUT_CH_2                       (0x01U)
#define WM8988_INPUT_CH_DIFFERENTIAL            (0x03U)
/**
 * @}
 */

/**
 * @defgroup WM8988_Differential_Input_Channel WM8988 Differential Input Channel
 * @{
 */
#define WM8988_DIFFERENTIAL_INPUT_1             (0x00U)
#define WM8988_DIFFERENTIAL_INPUT_2             (0x01U)
/**
 * @}
 */

/**
 * @defgroup WM8988_Output_Channel WM8988 Output Channel
 * @{
 */
#define WM8988_OUTPUT_CH_NONE                   (0x00U)
#define WM8988_OUTPUT_CH_1                      (0x01U)
#define WM8988_OUTPUT_CH_2                      (0x02U)
#define WM8988_OUTPUT_CH_BOTH                   (0x03U)
/**
 * @}
 */

/**
 * @defgroup WM8988_Output_Source WM8988 Output Source
 * @{
 */
#define WM8988_OUTPUT_SRC_NONE                  (0x50U)
#define WM8988_OUTPUT_SRC_DAC                   (0x100U) /* From DAC input          */
#define WM8988_OUTPUT_SRC_INPUT_CH_1            (0x80U)  /* From INPUT1             */
#define WM8988_OUTPUT_SRC_INPUT_CH_2            (0x81U)  /* From INPUT2             */
#define WM8988_OUTPUT_SRC_ADCINPUT              (0x83U)  /* From ADCINPUT           */
#define WM8988_OUTPUT_SRC_DIFFINPUT             (0x84U)  /* From differential INPUT */
/**
 * @}
 */

/**
 * @defgroup WM8988_Audio_Frequency WM8988 Audio Frequency
 * @{
 */
#define WM8988_AUDIO_FREQ_8K                    (8000UL)
#define WM8988_AUDIO_FREQ_32K                   (32000UL)
#define WM8988_AUDIO_FREQ_48K                   (48000UL)
#define WM8988_AUDIO_FREQ_96K                   (96000UL)
/**
 * @}
 */

/**
 * @defgroup WM8988_Data_Format WM8988 Data Format
 * @{
 */
#define WM8988_DATA_FORMAT_MSB                  (0x01U) /* Left Justified Mode  */
#define WM8988_DATA_FORMAT_I2S                  (0x02U) /* I2S Mode             */
#define WM8988_DATA_FORMAT_DSP                  (0x03U) /* DSP/PCM Mode         */
/**
 * @}
 */

/**
 * @defgroup WM8988_Data_Width WM8988 Data Width
 * @{
 */
#define WM8988_DATA_WIDTH_16BIT                 (0x00U)
#define WM8988_DATA_WIDTH_20BIT                 (0x04U)
#define WM8988_DATA_WIDTH_24BIT                 (0x08U)
#define WM8988_DATA_WIDTH_32BIT                 (0x0CU)
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
 * @addtogroup WM8988_Global_Functions WM8988 Global Functions
 * @{
 */
int32_t WM8988_WriteReg(const stc_wm8988_ll_t *pstcWm8988LL, uint8_t u8Reg, uint16_t u16Data);
int32_t WM8988_Reset(const stc_wm8988_ll_t *pstcWm8988LL);
int32_t WM8988_Init(const stc_wm8988_ll_t *pstcWm8988LL, const stc_wm8988_init_t *pstcWm8988Init);
int32_t WM8988_Play(const stc_wm8988_ll_t *pstcWm8988LL);
int32_t WM8988_Stop(const stc_wm8988_ll_t *pstcWm8988LL);
int32_t WM8988_SetAudioFreq(const stc_wm8988_ll_t *pstcWm8988LL, uint32_t u32Freq);
int32_t WM8988_SetVolume(const stc_wm8988_ll_t *pstcWm8988LL, uint8_t u8Volume);

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

#endif /* __WM8988_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
