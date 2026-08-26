/*******************************************************
  * Name    :Task_Cfg.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
#include "Mcu.h"
#include "Wdg.h"
#include "Gpt.h"
#include "Adc.h"
#include "Nvm.h"
#include "Pwm.h"
#include "Uds.h"
#include "Gpio.h"
#include "Uart.h"
#include "BatteryCtr.h"
#include "Spi.h"
#include "CanTp.h"
#include "CanManage.h"
#include "CanMatrix.h"
#include "CanTransciever.h"
#include "L9826tr.h"
#include "appInterface.h"
#include "LampControl.h"
#include "LockControl.h"
#include "Turn_light.h"
#include "Wiper.h"
#include "antiTheft.h"
#include "App_Memory.h"
#include "test.h"
#include "rke_rcv.h"
#include "rke_top.h"
#include "windows.h"
#include "sleep_manage.h"
#include "radio.h"
#include "rke_interactive.h"
#include "InputOuput.h"
#include "Power_management.h"
#include "RteSys.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Task_InitSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_InitSchduleTable(void)
{
    //driver init
    LL_PERIPH_WRITE_ENABLE();
    Mcu_DisableIrq();
    Mcu_Init();
    Wdg_Init();
    Gpio_Init();
    Gpt_Init();
    Adc_Init();
    Nvm_Init();
    // RF_Uart_Init();
    Spi_Init();
    Pwm_Init();

    //ram init
    Uds_Init();
    Eeprom_Powerup_Read_Parameter();
    CanMatrix_Init();
    RFInit();     //射频模块初始化
    RFRxMode();   //进入接收模式
    Mcu_EnableIrq();
    LL_PERIPH_WRITE_DISABLE();
}

/*******************************************************
  * Name    :Task_1msSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_1msSchduleTable(void)
{
    Uds_TimerCtrl();
    CanTp_TimerCtrl();
    CanManage_TimerCtrl();
    GpioScanTask();
    RkeLearnKeyTimeRun();
    InputOuputTask();
}

/*******************************************************
  * Name    :Task_2msSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_2msSchduleTable(void)
{
    rf_buff_task();
    Adc_Scan();
    CanManage_Main();
    NCV7240_Main(0);
    //BSW_L9826tr_task();
}

/*******************************************************
  * Name    :Task_5msSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_5msSchduleTable(void)
{
    Wdg_Feed();
}

/*******************************************************
  * Name    :Task_10msSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_10msSchduleTable(void)
{
    /* App Task.*/
    UpdataAppInput();
    UpdateLampControl_Main();
    UpdateLockControl_Main();
    turn_light_task();
    wiper_task();
    UpdateAntiTheftState();
    BatteryCtr_Main();
    CanMatrix_RxMsgMain();
    Memory_Main();
    test_task();
    windows_task();
    BTS_WorkMain();
    BTS_DiagMain();
    if (RTE_read_FunInd(RTE_FUNIND_IntoTestMode) == FALSE)
    {
        sleep_manage_task();
    }

    rke_interactive_task(10);
    power_manage_task(10);
}

/*******************************************************
  * Name    :Task_20msSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_20msSchduleTable(void)
{
    //    Wdg_Feed();
}

/*******************************************************
  * Name    :Task_IdleSchduleTable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Task_IdleSchduleTable(void)
{
    Uds_Main();
    CanTp_Main();
}

