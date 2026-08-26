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
#include "Rte.h"
/*-------------define---------------------------------*/
#define GPIO_TASK_TIME_BASE (2u)
#define PORT_DUG            1
typedef struct
{
    E_GPIO_OUTPUT_INDEX index;
    uint8_t             port;
    uint16_t            pin;
    IO_Type             valid;
    IO_Type             LowPowerState;
} S_Gpio_Output_Cfg;

typedef struct
{
    E_GPIO_INPUT_INDEX index;
    uint8_t            Port;
    uint16_t           pin;
    IO_Type            PreIOState;
    uint16_t           FilterCounter;
    const uint16_t     FilterTime;
    IO_Type            FiltedState;
    en_pin_state_t     valid;
    uint8_t            RteInputId;
} S_Gpio_Input_Cfg;

#endif

