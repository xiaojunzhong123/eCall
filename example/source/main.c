/**
 *******************************************************************************
 * @file  main.c
 * @brief Main program template.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-09-13       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2025, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32a4a8ritj.h"
//#include "hc32_ll.h"
//#include "ev_hc32a4a8_lqfp176_bsp.h"
//#include <string.h>
//#include <stdlib.h>
#include "Gpio.h"
#include "cmsis_os.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint32_t callcnt=0;
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
 void SysTick_Handler(void)
{
    osSystickHandler();
    SysTick_IncTick();

    __DSB();  /* Arm Errata 838869 */
}
void Thread3Handle(void const *argument)
{
  uint8_t temp[10] = "send ok\r\n";
    for (;;) {
      callcnt++;
      GPIO_SetOutPutTogglePin(E_OUTPUT_LED0);
      osDelay(1000);
      Uart_TxData(&temp, 9);
    }
}
void Thread1Handle(void const *argument)
{
    for (;;) {
      GPIO_SetOutPutTogglePin(E_OUTPUT_LED1);
        osDelay(500);
    }
}
void Thread2Handle(void const *argument)
{
    for (;;) {
      GPIO_SetOutPutTogglePin(E_OUTPUT_LED2);
        osDelay(200);
    }
}
/**
 * @brief  Main function of template project
 * @param  None
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    LL_PERIPH_WE(0xff);
    Mcu_Init();
    Gpio_Init();
    LL_PERIPH_WE(0xff);
    Uart_Init();
    (void)SysTick_Init(1000U);
    osThreadDef(Thread1, Thread1Handle, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 5);
    osThreadCreate(osThread(Thread1), NULL);
    osThreadDef(Thread2, Thread2Handle, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 5);
    osThreadCreate(osThread(Thread2), NULL);
    osThreadDef(Thread3, Thread3Handle, osPriorityLow, 0, configMINIMAL_STACK_SIZE * 5);
    osThreadCreate(osThread(Thread3), NULL);
    osKernelStart();
    while (1) {
    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
