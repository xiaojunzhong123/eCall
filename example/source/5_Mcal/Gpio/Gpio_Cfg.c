/*******************************************************
  * Name    :Gpio_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Gpio_Cfg.h"
/*-------------variable statement---------------------*/
const GpioOutputConfig_t g_astGpioOutputConfig[E_OUTPUT_GPIO_NUM] =
{
    {E_OUTPUT_INDICATOR_1, GPIO_PORT_D, GPIO_PIN_09},
    {E_OUTPUT_INDICATOR_2, GPIO_PORT_D, GPIO_PIN_10},
    {E_OUTPUT_IG_KEEPON, GPIO_PORT_B, GPIO_PIN_08},
    {E_OUTPUT_PWR_EN_CAN, GPIO_PORT_B, GPIO_PIN_07},
    {E_OUTPUT_CAN1_STB, GPIO_PORT_E, GPIO_PIN_06},
    {E_OUTPUT_CAT4_EN, GPIO_PORT_G, GPIO_PIN_05},
    {E_OUTPUT_CAT4_PWR_KEY, GPIO_PORT_C, GPIO_PIN_06},
    {E_OUTPUT_CAT4_RST, GPIO_PORT_C, GPIO_PIN_07},
    {E_OUTPUT_PWR_EN_GNSS, GPIO_PORT_F, GPIO_PIN_10},
    {E_OUTPUT_PWR_EN_3V3, GPIO_PORT_G, GPIO_PIN_04},
    {E_OUTPUT_PWR_EN_PHY, GPIO_PORT_G, GPIO_PIN_02},
    {E_OUTPUT_PHY_RESET, GPIO_PORT_C, GPIO_PIN_03},
	{E_OUTPUT_PWR_EN_AUDIO, GPIO_PORT_I, GPIO_PIN_13},
};

const GpioInputConfig_t g_astGpioInputConfig[E_INPUT_GPIO_NUM] =
{
    {E_INPUT_SOS_SWITCH, GPIO_PORT_B, GPIO_PIN_13},
    {E_INPUT_SRS_SWITCH, GPIO_PORT_B, GPIO_PIN_14},
    {E_INPUT_PBATTERY_SRS_SWITCH, GPIO_PORT_B, GPIO_PIN_15},
    {E_INPUT_KL15_SWITCH, GPIO_PORT_B, GPIO_PIN_09}
};

/*-------------Function-------------------------------*/
/*******************************************************
  * Name    :****
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/

