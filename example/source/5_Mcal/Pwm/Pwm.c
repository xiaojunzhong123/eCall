/*******************************************************
  * Name    :Pwm.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Pwm_Cfg.h"
/*-------------variable statement---------------------*/

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Pwm_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Pwm_Init(void)
{
    stc_tmra_init_t     stcTmraInit;
    stc_tmra_pwm_init_t stcPwmInit;

    /* 1. Enable TimerA peripheral clock. */
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_6, ENABLE);

    /* 2. Set a default initialization value for stcTmraInit. */
    (void)TMRA_StructInit(&stcTmraInit);

    /* 3. Modifies the initialization values depends on the application. */
    stcTmraInit.sw_count.u8ClockDiv  = TMRA_CLK_DIV8;
    stcTmraInit.sw_count.u8CountMode = TMRA_MD_SAWTOOTH;
    stcTmraInit.sw_count.u8CountDir  = TMRA_DIR_UP;
    stcTmraInit.u32PeriodValue       = (12500 - 1);
    (void)TMRA_Init(CM_TMRA_6, &stcTmraInit);

    (void)TMRA_PWM_StructInit(&stcPwmInit);
    stcPwmInit.u32CompareValue         = (0);
    stcPwmInit.u16StartPolarity        = TMRA_PWM_HIGH;
    stcPwmInit.u16StopPolarity         = TMRA_PWM_LOW;
    stcPwmInit.u16CompareMatchPolarity = TMRA_PWM_LOW;
    stcPwmInit.u16PeriodMatchPolarity  = TMRA_PWM_INVT;
    GPIO_SetFunc(GPIO_PORT_D, GPIO_PIN_10, GPIO_FUNC_5);
    (void)TMRA_PWM_Init(CM_TMRA_6, TMRA_CH3, &stcPwmInit);
    TMRA_PWM_SetForcePolarity(CM_TMRA_6, TMRA_CH3, TMRA_PWM_FORCE_LOW);
    TMRA_PWM_OutputCmd(CM_TMRA_6, TMRA_CH3, ENABLE);
    TMRA_Start(CM_TMRA_6);
}

/*******************************************************
  * Name    :Pwm_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Pwm_DeInit(void)
{
    TMRA_DeInit(CM_TMRA_6);
    FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMRA_6, DISABLE);
}

/*******************************************************
  * Name    :Pwm_UpdataCompValue
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Pwm_UpdataCompValue(uint8_t PwmPercent)
{
    PwmPercent = 100 - PwmPercent;

    if (PwmPercent == 0)
    {
        TMRA_PWM_SetForcePolarity(CM_TMRA_6, TMRA_CH3, TMRA_PWM_FORCE_LOW);
    }
    else if (PwmPercent == 100)
    {
        TMRA_PWM_SetForcePolarity(CM_TMRA_6, TMRA_CH3, TMRA_PWM_FORCE_HIGH);
    }
    else
    {
        TMRA_PWM_SetForcePolarity(CM_TMRA_6, TMRA_CH3, TMRA_PWM_FORCE_INVD);
        TMRA_SetCompareValue(CM_TMRA_6, TMRA_CH3, PwmPercent * 125);
    }
}
