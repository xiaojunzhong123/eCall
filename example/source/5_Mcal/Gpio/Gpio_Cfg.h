/*******************************************************
  * Name    :Gpio_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _GPIO_CFG_H_
#define _GPIO_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Gpio.h"
#include "hc32_ll.h"
typedef struct
{
    E_GPIO_OUTPUT_INDEX enOutput;
    uint8_t u8Port;
    uint16_t u16Pin;
} GpioOutputConfig_t;

typedef struct
{
    E_GPIO_INPUT_INDEX enInput;
    uint8_t u8Port;
    uint16_t u16Pin;
} GpioInputConfig_t;

extern const GpioOutputConfig_t g_astGpioOutputConfig[E_OUTPUT_GPIO_NUM];
extern const GpioInputConfig_t g_astGpioInputConfig[E_INPUT_GPIO_NUM];

#endif

