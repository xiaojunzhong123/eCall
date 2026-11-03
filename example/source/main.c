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
#include "hc32a4a8.h"
#include "hc32_ll.h"
#include "5_Mcal/Mcu/Mcu.h"
#include "Uart.h"
//#include "ev_hc32a4a8_lqfp176_bsp.h"
//#include <string.h>
//#include <stdlib.h>
#include "Gpio.h"
#include "cmsis_os.h"
#include "TaskManager.h"
// #include "Mcu.h"

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
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/*******************************************************
  * Name    :SysTick_Handler
  * Author  :xiaojunzhong
  * Function:一毫秒中断只推进内核和厂商 tick，不执行自检、触发或通信业务。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void SysTick_Handler(void)
{
    osSystickHandler();
    SysTick_IncTick();

    __DSB();  /* Arm Errata 838869 */
}
/*******************************************************
  * Name    :main
  * Author  :xiaojunzhong
  * Function:先初始化 MCU/GPIO/UART/时基，再建立服务及任务，最后启动 RTOS。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/

int32_t main(void)
{
    LL_PERIPH_WE(0xff);
    Mcu_Init();
    Gpio_Init();
    LL_PERIPH_WE(0xff);
    Uart_Init();
    (void)SysTick_Init(1000U);

    TaskManager_Start();

    osKernelStart();
    while (1)
    {

    }
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
