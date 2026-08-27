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
    E_OUTPUT_LED0 = 0U,
    E_OUTPUT_LED1,
    E_OUTPUT_LED2,
    E_OUTPUT_GPIO_NUM
} E_GPIO_OUTPUT_INDEX;

typedef enum
{
    E_INPUT_ECALL_SOS_SWITCH = 0U,
    E_INPUT_KL15_SWITCH,
    E_INPUT_GPIO_NUM
} E_GPIO_INPUT_INDEX;

extern IO_Type g_outCtrl[E_OUTPUT_GPIO_NUM];
/*-------------function port--------------------------*/
extern void    Gpio_Init(void);
extern void    Mcal_Gpio_DeInit(void);
extern void    GpioScanTask(void);
extern IO_Type GPIO_GetInputState(E_GPIO_INPUT_INDEX index);
extern void    GPIO_SetOutputState(E_GPIO_OUTPUT_INDEX index, IO_Type state);
extern IO_Type GPIO_GetFiltInputState(E_GPIO_INPUT_INDEX index);
extern IO_Type GPIO_GetOutputState(E_GPIO_OUTPUT_INDEX index);
extern void    GPIO_SetOutPutTogglePin(E_GPIO_OUTPUT_INDEX index);

#endif

