/*******************************************************
  * Name    :Gpio.c
  * Author  :WangHu
  * Function:Actual-board GPIO driver and RMII pin setup.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
/*-------------include file---------------------------*/
#include "Gpio_Cfg.h"
#include "hc32_ll_interrupts.h"

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :GPIO_SetOutputLevel
  * Author  :LoH
  * Function:Write one physical GPIO output level.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
void GPIO_SetOutputLevel(E_GPIO_OUTPUT_INDEX index, IO_Type state)
{
    if (index >= E_OUTPUT_GPIO_NUM)
    {
        return;
    }

    if (state == HIGH)
    {
        GPIO_SetPins(g_astGpioOutputConfig[index].u8Port, g_astGpioOutputConfig[index].u16Pin);
    }
    else
    {
        GPIO_ResetPins(g_astGpioOutputConfig[index].u8Port, g_astGpioOutputConfig[index].u16Pin);
    }
}

/*******************************************************
  * Name    :Gpio_Init
  * Author  :LoH
  * Function:Initialize actual-board GPIO and RMII pins.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
void Gpio_Init(void)
{
    uint8_t u8Index = 0U;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    for (u8Index = 0U; u8Index < E_INPUT_GPIO_NUM; u8Index++)
    {
        (void)GPIO_Init(g_astGpioInputConfig[u8Index].u8Port,
                        g_astGpioInputConfig[u8Index].u16Pin,
                        &stcGpioInit);
    }

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
    for (u8Index = 0U; u8Index < E_OUTPUT_GPIO_NUM; u8Index++)
    {
        (void)GPIO_Init(g_astGpioOutputConfig[u8Index].u8Port,
                        g_astGpioOutputConfig[u8Index].u16Pin,
                        &stcGpioInit);
        GPIO_SetOutputLevel((E_GPIO_OUTPUT_INDEX)u8Index, LOW);
    }

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
    (void)GPIO_Init(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), &stcGpioInit);
    GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_02 | GPIO_PIN_07), GPIO_FUNC_11);
    GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_01 | GPIO_PIN_04 | GPIO_PIN_05), GPIO_FUNC_11);
    GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14), GPIO_FUNC_11);
}

/*******************************************************
  * Name    :Mcal_Gpio_DeInit
  * Author  :LoH
  * Function:Reserve the GPIO deinitialization interface.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
void Mcal_Gpio_DeInit(void)
{
}

/*******************************************************
  * Name    :GPIO_GetInputLevel
  * Author  :LoH
  * Function:Read one physical GPIO input level.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
IO_Type GPIO_GetInputLevel(E_GPIO_INPUT_INDEX index)
{
    if (index >= E_INPUT_GPIO_NUM)
    {
        return LOW;
    }

    return (IO_Type)GPIO_ReadInputPins(g_astGpioInputConfig[index].u8Port,
                                        g_astGpioInputConfig[index].u16Pin);
}

/*******************************************************
  * Name    :GpioScanTask
  * Author  :LoH
  * Function:Retain the legacy scheduler hook; filtering runs in InputService.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
void GpioScanTask(void)
{
}

/*******************************************************
  * Name    :GPIO_GetOutputLevel
  * Author  :LoH
  * Function:Read one physical GPIO output level.
  * Version :V1.0.0
  * Data    :2026.09.08
*******************************************************/
IO_Type GPIO_GetOutputLevel(E_GPIO_OUTPUT_INDEX index)
{
    if (index >= E_OUTPUT_GPIO_NUM)
    {
        return LOW;
    }

    return (IO_Type)GPIO_ReadOutputPins(g_astGpioOutputConfig[index].u8Port,
                                         g_astGpioOutputConfig[index].u16Pin);
}
