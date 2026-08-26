/*******************************************************
  * Name    :Adc.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _ADC_H_
#define _ADC_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Adc_Cfg.h"
/*-------------function port--------------------------*/
extern void               Adc_Init(void);
extern void               Adc_Scan(void);
extern void               Adc_DeInit(void);
extern Bool               Adc_GetBatteryValue(uint16_t *p_data);
extern S_AD_CHANNEL_VALUE AdcChannelValue;
#endif

