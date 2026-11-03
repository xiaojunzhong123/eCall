/*******************************************************
  * Name    :Gpio.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_
/*-------------include file---------------------------*/
#include "Types.h"

typedef enum
{
    E_OUTPUT_INDICATOR_1 = 0U,
    E_OUTPUT_INDICATOR_2,
    E_OUTPUT_IG_KEEPON,
    E_OUTPUT_PWR_EN_CAN,
    E_OUTPUT_CAN1_STB,
    E_OUTPUT_CAT4_EN,
    E_OUTPUT_CAT4_PWR_KEY,
    E_OUTPUT_CAT4_RST,
    E_OUTPUT_PWR_EN_GNSS,
    E_OUTPUT_PWR_EN_3V3,
    E_OUTPUT_PWR_EN_PHY,
    E_OUTPUT_PHY_RESET,
	E_OUTPUT_PWR_EN_AUDIO,
    E_OUTPUT_GPIO_NUM
} E_GPIO_OUTPUT_INDEX;

typedef enum
{
    E_INPUT_SOS_SWITCH = 0U,
    E_INPUT_SRS_SWITCH,
    E_INPUT_PBATTERY_SRS_SWITCH,
    E_INPUT_KL15_SWITCH,
    E_INPUT_GPIO_NUM
} E_GPIO_INPUT_INDEX;

/*-------------function port--------------------------*/
extern void Gpio_Init(void);
extern void Mcal_Gpio_DeInit(void);
extern void GpioScanTask(void);
extern IO_Type GPIO_GetInputLevel(E_GPIO_INPUT_INDEX index);
extern void GPIO_SetOutputLevel(E_GPIO_OUTPUT_INDEX index, IO_Type state);
extern IO_Type GPIO_GetOutputLevel(E_GPIO_OUTPUT_INDEX index);

#endif

