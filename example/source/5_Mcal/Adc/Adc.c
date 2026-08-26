/*******************************************************
  * Name    :Adc.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Adc_Cfg.h"
/*-------------variable statement---------------------*/
uint16_t           adcbuff[12] = { 0 };
S_AD_CHANNEL_VALUE AdcChannelValue;
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Adc_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Adc_Init(void)
{
    stc_adc_init_t  stcAdcInit;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(GPIO_PORT_C, GPIO_PIN_00, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_C, GPIO_PIN_01, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_C, GPIO_PIN_03, &stcGpioInit);   //FrWinSw_Mcu_In
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_00, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_01, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_02, &stcGpioInit);
    //    (void)GPIO_Init(GPIO_PORT_A, GPIO_PIN_03, &stcGpioInit);XV5PAÎªÆÕÍ¨GPIO
    (void)GPIO_Init(GPIO_PORT_C, GPIO_PIN_04, &stcGpioInit);   //DrvFlWinSw_Mcu_In
    (void)GPIO_Init(GPIO_PORT_C, GPIO_PIN_05, &stcGpioInit);   //DrvFrWinSw_Mcu_In
    (void)GPIO_Init(GPIO_PORT_B, GPIO_PIN_00, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_B, GPIO_PIN_01, &stcGpioInit);

    CLK_SetClockDiv((CLK_BUS_PCLK2 | CLK_BUS_PCLK4), (CLK_PCLK2_DIV8 | CLK_PCLK4_DIV2));
    CLK_SetPeriClockSrc(CLK_PERIPHCLK_PCLK);
    /* 1. Enable ADC peripheral clock. */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_ADC1, ENABLE);

    /* 2. Modify the default value depends on the application. Not needed here. */
    (void)ADC_StructInit(&stcAdcInit);

    /* 3. Initializes ADC. */
    (void)ADC_Init(CM_ADC1, &stcAdcInit);

    /* 4.2 Enable ADC channels. Call ADC_ChCmd() again to enable more channels if needed. */
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH10, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH11, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH13, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH0, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH1, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH2, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH3, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH14, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH15, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH8, ENABLE);
    ADC_ChCmd(CM_ADC1, ADC_SEQ_A, ADC_CH9, ENABLE);

    /* 5. Set sampling time if needed. */
    ADC_SetSampleTime(CM_ADC1, ADC_CH10, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH11, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH13, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH0, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH1, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH2, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH3, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH14, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH15, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH8, 0x40U);
    ADC_SetSampleTime(CM_ADC1, ADC_CH9, 0x40U);

    /* 6. Conversion data average calculation function, if needed.
          Call ADC_ConvDataAverageChCmd() again to enable more average channels if needed. */
    ADC_ConvDataAverageConfig(CM_ADC1, ADC_AVG_CNT8);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH10, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH11, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH13, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH0, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH1, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH2, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH3, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH14, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH15, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH8, ENABLE);
    ADC_ConvDataAverageChCmd(CM_ADC1, ADC_CH9, ENABLE);
}

/*******************************************************
  * Name    :Adc_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Adc_DeInit(void)
{
    ADC_DeInit(CM_ADC1);
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_ADC1, DISABLE);
}

/*******************************************************
  * Name    :Adc_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void AdcPolling(void)
{
    int32_t       iRet         = LL_ERR;
    __IO uint32_t u32TimeCount = 0UL;

    /* Can ONLY start sequence A conversion.
       Sequence B needs hardware trigger to start conversion. */
    ADC_Start(CM_ADC1);
    do
    {
        if (ADC_GetStatus(CM_ADC1, ADC_FLAG_EOCA) == SET)
        {
            ADC_ClearStatus(CM_ADC1, ADC_FLAG_EOCA);
            iRet = LL_OK;
            break;
        }
    } while (u32TimeCount++ < 1000U);

    if (iRet == LL_OK)
    {
        adcbuff[0]  = ADC_GetValue(CM_ADC1, ADC_CH0);
        adcbuff[1]  = ADC_GetValue(CM_ADC1, ADC_CH1);
        adcbuff[2]  = ADC_GetValue(CM_ADC1, ADC_CH2);
        adcbuff[3]  = ADC_GetValue(CM_ADC1, ADC_CH3);
        adcbuff[4]  = ADC_GetValue(CM_ADC1, ADC_CH8);
        adcbuff[5]  = ADC_GetValue(CM_ADC1, ADC_CH9);
        adcbuff[6]  = ADC_GetValue(CM_ADC1, ADC_CH10);
        adcbuff[7]  = ADC_GetValue(CM_ADC1, ADC_CH11);
        adcbuff[8]  = ADC_GetValue(CM_ADC1, ADC_CH13);
        adcbuff[9]  = ADC_GetValue(CM_ADC1, ADC_CH14);
        adcbuff[10] = ADC_GetValue(CM_ADC1, ADC_CH15);
    }
    else
    {
        ADC_Stop(CM_ADC1);
    }
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterFrWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterRslSensorSwScan(uint16_t Value)
{
    static uint8_t scanTimer = 0;
    static uint8_t reTemp    = 7;

    if (Value < 230)
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = 1;
        }
    }
    else
    {
        if (scanTimer != 0)
        {
            scanTimer--;
        }
        else
        {
            reTemp = 0;
        }
    }

    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterRlWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterRrWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t FilterWinLockSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterDrvFlWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterDrvFrWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;   //下降
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;   //上升
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterDrvRlWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static uint8_t FilterDrvRrWinSwScan(uint16_t Value)
{
    static uint8_t scanTimer  = 0;
    static uint8_t reTemp     = 0;
    static uint8_t PerSwState = 0;
    uint8_t        SwState    = 0;

    if (Value < 230)
    {
        SwState = 2;
    }
    else if ((Value >= 230) && (Value < 2400))
    {
        SwState = 1;
    }
    else
    {
        SwState = 0;
    }

    if (PerSwState != SwState)
    {
        scanTimer = 0;
    }
    else
    {
        scanTimer++;
        if (scanTimer >= 6)
        {
            scanTimer = 6;
            reTemp    = SwState;
        }
    }
    PerSwState = SwState;
    return (reTemp);
}
/*******************************************************
  * Name    :Adc_Scan
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Adc_Scan(void)
{
    AdcPolling();
    AdcChannelValue.Adc_4851Out4 = adcbuff[6];
    AdcChannelValue.Adc_BatAd    = adcbuff[7];
    //RteSys_SetVoltageValue(AdcChannelValue.Adc_BatAd);
    AdcChannelValue.Adc_FrWinSw  = adcbuff[8];
    Rte_W_Input(Rte_FrWinSw_In, FilterFrWinSwScan(AdcChannelValue.Adc_FrWinSw));
    AdcChannelValue.Adc_RslSensorSw = adcbuff[0];
    Rte_W_Input(Rte_RlsSensorSw_In, FilterRslSensorSwScan(AdcChannelValue.Adc_RslSensorSw));
    AdcChannelValue.Adc_RlWinSw = adcbuff[1];
    Rte_W_Input(Rte_RlWinSw_In, FilterRlWinSwScan(AdcChannelValue.Adc_RlWinSw));   //Reserve
    AdcChannelValue.Adc_RrWinSw = adcbuff[2];
    Rte_W_Input(Rte_RrWinSw_In, FilterRrWinSwScan(AdcChannelValue.Adc_RrWinSw));   //Reserve
    AdcChannelValue.Adc_WinLockSw  = adcbuff[3];
    //    Rte_W_Input(Rte_WinLockSw_In,FilterWinLockSwScan(AdcChannelValue.Adc_WinLockSw));//
    AdcChannelValue.Adc_DrvFlWinSw = adcbuff[9];
    Rte_W_Input(Rte_DrvFlWinSw_In, FilterDrvFlWinSwScan(AdcChannelValue.Adc_DrvFlWinSw));
    AdcChannelValue.Adc_DrvFrWinSw = adcbuff[10];
    Rte_W_Input(Rte_DrvFrWinSw_In, FilterDrvFrWinSwScan(AdcChannelValue.Adc_DrvFrWinSw));
    AdcChannelValue.Adc_DrvRlWinSw = adcbuff[4];
    Rte_W_Input(Rte_DrvRlWinSw_In, FilterDrvRlWinSwScan(AdcChannelValue.Adc_DrvRlWinSw));   //Reserve
    AdcChannelValue.Adc_DrvRrWinSw = adcbuff[5];
    Rte_W_Input(Rte_DrvRrWinSw_In, FilterDrvRrWinSwScan(AdcChannelValue.Adc_DrvRrWinSw));   //Reserve

    HCF4851_main();
}

/*******************************************************
  * Name    :Adc_GetBatteryValue
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Adc_GetBatteryValue(uint16_t *p_data)
{
    *p_data = AdcChannelValue.Adc_BatAd;
    return TRUE;
}

