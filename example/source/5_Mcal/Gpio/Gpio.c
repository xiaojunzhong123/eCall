/*******************************************************
  * Name    :Gpio.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Gpio_Cfg.h"
#include "RteSys.h"
#include "hc32_ll_interrupts.h"
/*-------------variable statement---------------------*/
const S_Gpio_Output_Cfg output_cfg_tbl[E_OUTPUT_GPIO_NUM] = {
    {E_OUTPUT_LED0, GPIO_PORT_I, GPIO_PIN_08, HIGH,  LOW},
    {E_OUTPUT_LED1, GPIO_PORT_H, GPIO_PIN_05, HIGH,  LOW},
    {E_OUTPUT_LED2, GPIO_PORT_A, GPIO_PIN_06, HIGH,  LOW},

};

S_Gpio_Input_Cfg input_cfg_tbl[E_INPUT_GPIO_NUM] = {
    /* PA0 Wakeup switch: external pull-up, low level is the eCall SOS active state. */
    {E_INPUT_ECALL_SOS_SWITCH, GPIO_PORT_A, GPIO_PIN_00, LOW, 0u, 140u, LOW, PIN_RESET, 0},
};
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :GPIO_SetOutputState
  * Author  :LoHs
  * Function:****
  * Version :V1.0.0
  * Data    :2023.4.26
*******************************************************/
void GPIO_SetOutputState(E_GPIO_OUTPUT_INDEX index, IO_Type state)
{
    if (index >= E_OUTPUT_GPIO_NUM)
    {
        return;
    }

    if (state == HIGH)
    {
        if (output_cfg_tbl[index].valid == HIGH)
        {
            GPIO_SetPins(output_cfg_tbl[index].port, output_cfg_tbl[index].pin);
        }
        else
        {
            GPIO_ResetPins(output_cfg_tbl[index].port, 1 << output_cfg_tbl[index].pin);
        }
    }
    else
    {
        if (output_cfg_tbl[index].valid == HIGH)
        {
            GPIO_ResetPins(output_cfg_tbl[index].port, output_cfg_tbl[index].pin);
        }
        else
        {
            GPIO_SetPins(output_cfg_tbl[index].port, 1 << output_cfg_tbl[index].pin);
        }
    }
}
/*******************************************************
  * Name    :Gpio_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpio_Init(void)
{
    uint8_t         index = 0;
    stc_gpio_init_t stcGpioInit;

//    GPIO_SetDebugPort(GPIO_PIN_TDI, DISABLE);    //PA15
//    GPIO_SetDebugPort(GPIO_PIN_TDO, DISABLE);    //PB3
//    GPIO_SetDebugPort(GPIO_PIN_TRST, DISABLE);   //PB4

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    for (index = 0; index < E_INPUT_GPIO_NUM; index++)
    {
        (void)GPIO_Init(input_cfg_tbl[index].Port, input_cfg_tbl[index].pin, &stcGpioInit);
    }

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir  = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
    stcGpioInit.u16PinDrv  = PIN_HIGH_DRV;
    for (index = 0; index < E_OUTPUT_GPIO_NUM; index++)
    {
        (void)GPIO_Init(output_cfg_tbl[index].port, output_cfg_tbl[index].pin, &stcGpioInit);
    }
}


/*******************************************************
  * Name    :Gpio_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Mcal_Gpio_DeInit(void)
{

}


/*******************************************************
  * Name    :GPIO_GetInputState
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
IO_Type GPIO_GetInputState(E_GPIO_INPUT_INDEX index)
{
    IO_Type        return_value = LOW;
    en_pin_state_t ReadVal;

    if (index >= E_INPUT_GPIO_NUM)
    {
        return LOW;
    }

    ReadVal = GPIO_ReadInputPins(input_cfg_tbl[index].Port, input_cfg_tbl[index].pin);

    if (ReadVal == input_cfg_tbl[index].valid)   //Low is valid.
    {
        return_value = HIGH;
    }
    else
    {
        return_value = LOW;
    }

    return return_value;
}

/*******************************************************
  * Name    :GpioScanTask
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void GpioScanTask(void)
{
    uint8_t ScanIndex = 0u;
    IO_Type state     = LOW;

    for (ScanIndex = 0u; ScanIndex < E_INPUT_GPIO_NUM; ScanIndex++)
    {
        state = GPIO_GetInputState((E_GPIO_INPUT_INDEX)ScanIndex);
        if (state == input_cfg_tbl[ScanIndex].PreIOState)
        {
            input_cfg_tbl[ScanIndex].FilterCounter += GPIO_TASK_TIME_BASE;
            if (input_cfg_tbl[ScanIndex].FilterCounter >= input_cfg_tbl[ScanIndex].FilterTime)
            {
                input_cfg_tbl[ScanIndex].FilterCounter = input_cfg_tbl[ScanIndex].FilterTime;
                input_cfg_tbl[ScanIndex].FiltedState   = state;
            }
        }
        else
        {
            input_cfg_tbl[ScanIndex].FilterCounter = 0u;
        }

        input_cfg_tbl[ScanIndex].PreIOState = state;
    }
}

/*******************************************************
  * Name    :GPIO_GetFiltInputState
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
IO_Type GPIO_GetFiltInputState(E_GPIO_INPUT_INDEX index)
{
    if (index < E_INPUT_GPIO_NUM)
    {
        return input_cfg_tbl[index].FiltedState;
    }
    return LOW;
}

/*******************************************************
  * Name    :GPIO_GetOutputState
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
IO_Type GPIO_GetOutputState(E_GPIO_OUTPUT_INDEX index)
{
    return (IO_Type)GPIO_ReadOutputPins(output_cfg_tbl[index].port, output_cfg_tbl[index].pin);
}
/*******************************************************
  * Name    :GPIO_SetOutPutTogglePin
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void    GPIO_SetOutPutTogglePin(E_GPIO_OUTPUT_INDEX index)
{
    if (index >= E_OUTPUT_GPIO_NUM)
    {
        return;
    }
    GPIO_TogglePins(output_cfg_tbl[index].port, output_cfg_tbl[index].pin);
}
