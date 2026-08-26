/*******************************************************
  * Name    :Gpt.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Gpt_Cfg.h"
/*-------------variable statement---------------------*/
/*-------------Function-------------------------------*/
extern void Task_1msSchdule(void);
void        Gpt_DelayUs(uint32_t us);
/*******************************************************
  * Name    :INT_SRC_TMRA_1_OVF_IrqCallback
  * Author  :Robsen
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void INT_SRC_TMRA_1_OVF_IrqCallback(void)
{
    Task_1msSchdule();
    //    GPIO_TogglePins(GPIO_PORT_B, GPIO_PIN_04);
    TMRA_ClearStatus(CM_TMRA_1, TMRA_FLAG_OVF);
}
/*******************************************************
  * Name    :Gpt_Init
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_Init(void)
{
    stc_tmra_init_t         stcTmraInit;
    stc_irq_signin_config_t stcIrq;

    /* Enable TMRA_1 peripheral clock */
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_1, ENABLE);

    /************************* Configure TMRA_1 counter *************************/
    (void)TMRA_StructInit(&stcTmraInit);
    /* Config software count */
    stcTmraInit.sw_count.u8ClockDiv  = TMRA_CLK_DIV8;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD_SAWTOOTH;
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR_UP;
    stcTmraInit.u32PeriodValue       = 12500;
    (void)TMRA_Init(CM_TMRA_1, &stcTmraInit);

    stcIrq.enIntSrc    = INT_SRC_TMRA_1_OVF;
    stcIrq.enIRQn      = INT080_IRQn;
    stcIrq.pfnCallback = &INT_SRC_TMRA_1_OVF_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    NVIC_ClearPendingIRQ(INT080_IRQn);
    NVIC_SetPriority(INT080_IRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(INT080_IRQn);

    /* Enable overflow interrupt */
    TMRA_IntCmd(CM_TMRA_1, TMRA_INT_OVF, ENABLE);

    /* Start timerA */
    TMRA_Start(CM_TMRA_1);
}

/*******************************************************
  * Name    :Gpt_DeInit
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2021.12.5
*******************************************************/
void Gpt_DeInit(void)
{
    TMRA_DeInit(CM_TMRA_1);
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_1, DISABLE);
}

/*******************************************************
  * Name    :Gpt_DelayMs
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_DelayMs(uint32_t time)
{
    //uint32_t waitTime = 0;

    Gpt_DelayUs(time * 1000);
    // waitTime = Gpt_GetSysTimeMs();
    // while((Gpt_GetSysTimeMs() - waitTime) < time)
    // {
    //     ;//Gpt_WdgFeed();
    // }
}

/*******************************************************
  * Name    :Gpt_DelayUs
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_DelayUs(uint32_t us)
{
    volatile uint32_t DelayTime = us * 100;

    while (DelayTime--)
    {
        ;
    }
}
void INT_SRC_TMRA_2_OVF_IrqCallback(void)
{
    TMRA_ClearStatus(CM_TMRA_2, TMRA_FLAG_OVF);
}
/*******************************************************
  * Name    :Gpt_WakeUpTimerInit
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_WakeUpTimerInit(void)
{
    stc_tmra_init_t         stcTmraInit;
    stc_irq_signin_config_t stcIrq;

    /* Enable TMRA_1 peripheral clock */
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_2, ENABLE);

    /************************* Configure TMRA_2 counter *************************/
    (void)TMRA_StructInit(&stcTmraInit);
    /* Config software count */
    stcTmraInit.sw_count.u8ClockDiv  = TMRA_CLK_DIV512;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD_SAWTOOTH;
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR_UP;
    stcTmraInit.u32PeriodValue       = 195;
    (void)TMRA_Init(CM_TMRA_2, &stcTmraInit);

    stcIrq.enIntSrc    = INT_SRC_TMRA_2_OVF;
    stcIrq.enIRQn      = INT081_IRQn;
    stcIrq.pfnCallback = &INT_SRC_TMRA_2_OVF_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    NVIC_ClearPendingIRQ(INT081_IRQn);
    NVIC_SetPriority(INT081_IRQn, DDL_IRQ_PRIO_15);
    NVIC_EnableIRQ(INT081_IRQn);

    /* Enable overflow interrupt */
    TMRA_IntCmd(CM_TMRA_2, TMRA_INT_OVF, ENABLE);
}

/*******************************************************
  * Name    :Gpt_WakeUpTimerCycle
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_WakeUpTimerCycle(uint16_t time)
{
    TMRA_SetCountValue(CM_TMRA_2, 0);
    TMRA_SetPeriodValue(CM_TMRA_2, time * 195);
    TMRA_Start(CM_TMRA_2);
}

/*******************************************************
  * Name    :Gpt_WakeUpTimerDisable
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Gpt_WakeUpTimerDisable(void)
{
    TMRA_Stop(CM_TMRA_2);
}
