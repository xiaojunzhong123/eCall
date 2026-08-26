/*******************************************************
  * Name    :Adc_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "HC4851.h"
#include "hc32_ll.h"
#include "Rte.h"
#include "RteSys.h"
/*-------------define---------------------------------*/
typedef struct
{
    uint16_t Adc_4851Out4;
    uint16_t Adc_BatAd;
    uint16_t Adc_FrWinSw;
    uint16_t Adc_RslSensorSw;
    uint16_t Adc_RlWinSw;
    uint16_t Adc_RrWinSw;
    uint16_t Adc_WinLockSw;
    uint16_t Adc_DrvFlWinSw;
    uint16_t Adc_DrvFrWinSw;
    uint16_t Adc_DrvRlWinSw;
    uint16_t Adc_DrvRrWinSw;
    uint16_t Adc_FlDoorAjarSw;
} S_AD_CHANNEL_VALUE;

/*-------------function port--------------------------*/
/*-------------Memory---------------------------------*/

/*-------------enum and struct------------------------*/

/*-------------variable statement---------------------*/
extern S_AD_CHANNEL_VALUE AdcChannelValue;
#endif

