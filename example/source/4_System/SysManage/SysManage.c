/*******************************************************
  * Name    :SysManage.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "SysManage_Cfg.h"
#include "Gpio.h"
/*-------------variable statement---------------------*/
static SysManage_POLLING_Type sleepstate = SYSMANAGE_MODE_SLEEP;
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :SysManage_CheckSleepCondition
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool SysManage_CheckSleepCondition(void)
{
    return SysManage_GetCanSleepFlag();
}

/*******************************************************
  * Name    :SysManage_SysPowerOff
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool SysManage_SysPowerOff(void)
{
    Mcu_PerSleep();
    return TRUE;
}
/*******************************************************
  * Name    :SysManage_SysPowerOff
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static Bool SysManage_WakeUpTask(void)
{
    Bool           ret              = FALSE;
    static uint8_t quickScanCounter = 0;
    static uint8_t RfDectLongTime   = 0;
    uint8_t        rke = 0, can_wakeup = 0;
    Bool           wakestate = FALSE;

    switch (sleepstate)
    {
        case SYSMANAGE_MODE_SLEEP:
            GPIO_SetOutputState(E_OUTPUT_WK12V_Mcu_Out, LOW);
            GPIO_SetOutputState(E_OUTPUT_CanEnable, HIGH);
            /* Wake up by WKTM */
            //PWC_PD_WakeupCmd(PWC_PD_WKUP_WKTM, ENABLE);
            LL_PERIPH_WRITE_ENABLE();
            /* Disable WKTM in advance */
            PWC_WKT_Cmd(DISABLE);
            /* LRC for WKTM */
            CLK_LrcCmd(ENABLE);
            /* WKTM init */
            PWC_WKT_Config(PWC_WKT_CLK_SRC_LRC, 0xFFFU);

            /* Wakeup timer NVIC config, not mandatory for this sample */
            (void)INTC_ShareIrqCmd(INT_SRC_WKTM_PRD, ENABLE);
            NVIC_ClearPendingIRQ(INT130_IRQn);
            NVIC_SetPriority(INT130_IRQn, DDL_IRQ_PRIO_DEFAULT);
            NVIC_EnableIRQ(INT130_IRQn);
            DDL_DelayUS(1000);
            if ((LL_OK == STOP_IsReady())
                && (GPIO_GetInputState(E_INPUT_RF_GPIO1) == LOW))
            {
                PWC_WKT_Cmd(ENABLE);
                LL_PERIPH_WRITE_ENABLE();
                PWC_STOP_Enter(PWC_STOP_WFE_INT);
            }
            RteSys_SetRfDet(FALSE);
            GPIO_SetOutputState(E_OUTPUT_WK12V_Mcu_Out, HIGH);
            GPIO_SetOutputState(E_OUTPUT_CanEnable, LOW);
            quickScanCounter = 0;
            RfDectLongTime   = 1;
            sleepstate       = SYSMANAGE_MODE_SCAN;
            DDL_DelayUS(400);
            break;
        case SYSMANAGE_MODE_SCAN:
            Wdg_Feed();
            if ((RTE_read_FunInd(RTE_FUNIND_RfIntFlag) == 1)
                || (GPIO_GetInputState(E_INPUT_RF_GPIO1) == HIGH))
            {
                RTE_write_FunInd(RTE_FUNIND_RfIntFlag, 0);
                LL_PERIPH_WRITE_ENABLE();
                CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
                LL_PERIPH_WRITE_DISABLE();
                rf_buff_task();
                //GPIO_SetOutputState(E_OUTPUT_CanEnable, LOW);
                rf_buff_task();
                //GPIO_SetOutputState(E_OUTPUT_CanEnable, HIGH);
                RfDectLongTime = 100;
                sleepstate     = SYSMANAGE_MODE_SLEEP;
            }
            if (RteSys_GetRkeValue() == 0)
            {
                LL_PERIPH_WRITE_ENABLE();
                CLK_SetSysClockSrc(CLK_SYSCLK_SRC_HRC);
                LL_PERIPH_WRITE_DISABLE();
            }
            //wakeup scan IO rke
            wakestate = Gpio_GetWakeUpState();
            rke       = RteSys_GetRkeValue();
            //            can_wakeup = RTE_read_FunInd(RTE_FUNIND_CANWakeup);
            if ((wakestate == TRUE)
                || (rke != 0)
                || (can_wakeup != 0))   //RKE flag
            {
                RTE_write_FunInd(RTE_FUNIND_CANWakeup, 0);
                Sleep_wakeup_set_wait_time();
                ret = TRUE;
                RteSys_SetRfDet(FALSE);
                sleepstate = SYSMANAGE_MODE_SLEEP;
                if(RTE_read_FunInd(RTE_FUNIND_RfSleepmMode) == 1)
                {
                    RTE_write_FunInd(RTE_FUNIND_RfSleepmMode, 0);
                    RF_Init();
                }
            }
            else if (++quickScanCounter < RfDectLongTime)
            {
                // Gpt_WakeUpTimerCycle(10);
                // PWC_SLEEP_Enter();
                // Gpt_WakeUpTimerDisable();
                // sleepstate = SYSMANAGE_MODE_QUICK;
            }
            else
            {
                RteSys_SetRfDet(FALSE);
                sleepstate = SYSMANAGE_MODE_SLEEP;
            }
            break;
        case SYSMANAGE_MODE_QUICK:

            sleepstate = SYSMANAGE_MODE_SCAN;
            break;
        default:
            break;
    }

    return ret;
}
/*******************************************************
  * Name    :SysManage_Main
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void SysManage_Main(void)
{
    static SysManage_Mode_Type sysManageMode = SYSMANAGE_MODE_INIT;

    switch (sysManageMode)
    {
        case SYSMANAGE_MODE_INIT:
            SysManage_TaskInitSchdule();
            sysManageMode = SYSMANAGE_MODE_NORMAL;
            break;
        case SYSMANAGE_MODE_NORMAL:
            SysManage_TaskRunSchdule();
            if (SysManage_CheckSleepCondition() == TRUE)
            {
                //RteSys_SetLocalSleepFlag(FALSE);
                sysManageMode = SYSMANAGE_MODE_TWBS;
            }
            break;
        case SYSMANAGE_MODE_TWBS:
            if (SysManage_SysPowerOff() == TRUE)
            {
                RteSys_SetIoWakeState((uint8_t)GPIO_GetInputState(E_INPUT_LockStatusSw_In));
                RTE_write_FunInd(RTE_FUNIND_BefoeSleepDoorState, (uint8_t)GPIO_GetInputState(E_INPUT_FlDoorAjarSw_In));
                if(RTE_read_FunInd(RTE_FUNIND_RfSleepmMode) == 0)
                {
                    RTE_write_FunInd(RTE_FUNIND_RfSleepmMode, 1);
                    RF_Init();
                }
                LL_PERIPH_WRITE_ENABLE();
                //CLK_HrcCmd(DISABLE);
                CLK_MrcCmd(DISABLE);
                //    CLK_LrcCmd(DISABLE);
                CLK_XtalCmd(DISABLE);
                CLK_Xtal32Cmd(DISABLE);
                CLK_SetSysClockSrc(CLK_SYSCLK_SRC_HRC);
                DDL_DelayUS(30);
                // RF_Uart_Init();
                //                PWC_LDO_Cmd(PWC_LDO_PLL|PWC_LDO_HRC, DISABLE);
                //   PWC_HighPerformanceToLowSpeed();
                // PWC_HighSpeedToLowSpeed();
                //                Set_Can_WakeUp();

                Set_rf_detect_wakeup();
                LL_PERIPH_WRITE_DISABLE();
                Set_Sleep_Enter_Flag();
                // Gpt_WakeUpTimerInit();
                sysManageMode = SYSMANAGE_MODE_POLLING;
            }
            break;
        case SYSMANAGE_MODE_POLLING:
            //wait
            Wdg_Feed();
            if (SysManage_WakeUpTask() == TRUE)
            {
                RteSys_SetCanSleepFlag(FALSE);
                LL_PERIPH_WRITE_ENABLE();
                CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
                DDL_DelayUS(30);
                //disable can wake
                Disable_Can_Wakeup();
                //init Mcal
                Mcu_Init();
                Gpio_Init();
                Gpt_Init();
                Adc_Init();
                // RF_Uart_Init();
                Spi_Init();
                Pwm_Init();
                Can_Init();
                NCV7240_wakeup();
                BTS_Init();
                LL_PERIPH_WRITE_DISABLE();
                sysManageMode = SYSMANAGE_MODE_NORMAL;
            }
            else
            {
                //sysManageMode = SYSMANAGE_MODE_TWBS;
            }
            break;
        default:
            sysManageMode = SYSMANAGE_MODE_INIT;
            break;
    }
}

