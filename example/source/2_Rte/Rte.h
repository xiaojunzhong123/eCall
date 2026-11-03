/*******************************************************
  * Name    :Rte.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _RTE_H_
#define _RTE_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Gpio.h"
#include "Gpio_Cfg.h"
//#include "Pwm.h"
//#include "NCV7240.h"
//#include "Adc_Cfg.h"
//#include "CDD_BTS.h"
//#include "CDD_BTSCfg.h"
/*-------------function port--------------------------*/
typedef enum
{
    DTC_ID_HIGH_VOLTAGE = 0x00,
    DTC_ID_LOW_VOLTAGE,
    DTC_ID_BUS_OFF,
    DTC_ID_BCM_LOST,
    DTC_ID_MAX_NUM
} Rte_Dtc_Type;

/* Input port.*/
#define PORT_INPUT_AccSw_In               E_INPUT_AccSw_In
#define PORT_INPUT_IgnSw_In               E_INPUT_IgnSw_In
#define PORT_INPUT_RlsSensorSw_In         E_INPUT_RlsSensorSw_In
#define PORT_INPUT_FrontFogSw_In          E_INPUT_FrontFogSw_In
#define PORT_INPUT_RearFogSw_In           E_INPUT_RearFogSw_In
#define PORT_INPUT_LowLightSw_In          E_INPUT_LowLightSw_In
#define PORT_INPUT_AutoLightSw_In         E_INPUT_AutoLightSw_In
#define PORT_INPUT_LeTurnLightSw_In       E_INPUT_LeTurnLightSw_In
#define PORT_INPUT_RiTurnLightSw_In       E_INPUT_RiTurnLightSw_In
#define PORT_INPUT_FrontWiperHiSpeedSw_In E_INPUT_FrontWiperHiSpeedSw_In
#define PORT_INPUT_FrontWiperLoSpeedSw_In E_INPUT_FrontWiperLoSpeedSw_In
#define PORT_INPUT_FrontWiperIntSw_In     E_INPUT_FrontWiperIntSw_In
#define PORT_INPUT_FrontWiperWashSw_In    E_INPUT_FrontWiperWashSw_In
#define PORT_INPUT_WiperParkSw_In         E_INPUT_WiperParkSw_In
#define PORT_INPUT_CentrolLockSw_In       E_INPUT_CentrolLockSw_In
#define PORT_INPUT_HornSw_In              E_INPUT_HornSw_In
#define PORT_INPUT_HazardSw_In            E_INPUT_HazardSw_In
#define PORT_INPUT_LockStatusSw_In        E_INPUT_LockStatusSw_In
#define PORT_INPUT_HighLightSw_In         E_INPUT_HighLightSw_In
#define PORT_INPUT_PosLightSw_In          E_INPUT_PosLightSw_In
#define PORT_INPUT_FlDoorAjarSw_In        E_INPUT_FlDoorAjarSw_In
#define PORT_INPUT_FrDoorAjarSw_In        E_INPUT_FrDoorAjarSw_In
#define PORT_INPUT_RlDoorAjarSw_In        E_INPUT_RlDoorAjarSw_In
#define PORT_INPUT_RrDoorAjarSw_In        E_INPUT_RrDoorAjarSw_In

/* Output*/
#define PORT_OUTPUT_HSD_DEN                  E_OUTPUT_HSD_DEN
#define PORT_OUTPUT_HSD_DSEL                 E_OUTPUT_HSD_DSEL
#define PORT_OUTPUT_RearFogLight_Mcu_Out     E_OUTPUT_RearFogLight_Mcu_Out
#define PORT_OUTPUT_FlowWaterLampPwr_Mcu_Out E_OUTPUT_FlowWaterLampPwr_Mcu_Out
#define PORT_OUTPUT_LeTurnLight_Mcu_Out      E_OUTPUT_LeTurnLight_Mcu_Out
#define PORT_OUTPUT_RiTurnLight_Mcu_Out      E_OUTPUT_RiTurnLight_Mcu_Out
#define PORT_OUTPUT_FrontFogLight_Mcu_Out    E_OUTPUT_FrontFogLight_Mcu_Out
#define PORT_OUTPUT_DayLight_Mcu_Out         E_OUTPUT_DayLight_Mcu_Out
#define PORT_OUTPUT_LockSwBkgLight_Mcu_Out   E_OUTPUT_LockSwBkgLight_Mcu_Out
#define PORT_OUTPUT_HazardSwBkgLight_Mcu_Out E_OUTPUT_HazardSwBkgLight_Mcu_Out
#define PORT_OUTPUT_CentrolLock_Mcu_Out      E_OUTPUT_CentrolLock_Mcu_Out
#define PORT_OUTPUT_CentrolUnlock_Mcu_Out    E_OUTPUT_CentrolUnlock_Mcu_Out
#define PORT_OUTPUT_FlWinUp_Mcu_Out          E_OUTPUT_FlWinUp_Mcu_Out
#define PORT_OUTPUT_FlWinDown_Mcu_Out        E_OUTPUT_FlWinDown_Mcu_Out
#define PORT_OUTPUT_FrWinUp_Mcu_Out          E_OUTPUT_FrWinUp_Mcu_Out
#define PORT_OUTPUT_FrWinDown_Mcu_Out        E_OUTPUT_FrWinDown_Mcu_Out
#define PORT_OUTPUT_RlWinUp_Mcu_Out          E_OUTPUT_RlWinUp_Mcu_Out
#define PORT_OUTPUT_RlWinDown_Mcu_Out        E_OUTPUT_RlWinDown_Mcu_Out
#define PORT_OUTPUT_RrWinUp_Mcu_Out          E_OUTPUT_RrWinUp_Mcu_Out
#define PORT_OUTPUT_RrWinDown_Mcu_Out        E_OUTPUT_RrWinDown_Mcu_Out
#define PORT_OUTPUT_UWK12V_Mcu_Out           E_OUTPUT_UWK12V_Mcu_Out
#define PORT_OUTPUT_WK12V_Mcu_Out            E_OUTPUT_WK12V_Mcu_Out
#define PORT_OUTPUT_UWK3_3_Mcu_Out           E_OUTPUT_UWK3_3_Mcu_Out
#define PORT_OUTPUT_LinEnable                E_OUTPUT_LinEnable
#define PORT_OUTPUT_CanEnable                E_OUTPUT_CanEnable
#define PORT_OUTPUT_LSD_L9826_EN             E_OUTPUT_LSD_L9826_EN
#define PORT_OUTPUT_LSD_L9826_NCS            E_OUTPUT_LSD_L9826_NCS
#define PORT_OUTPUT_EE_SPI_CS                E_OUTPUT_EE_SPI_CS
#define PORT_OUTPUT_WiperWash_Mcu_Out        E_OUTPUT_WiperWash_Mcu_Out
#define PORT_OUTPUT_Buzzer_Mcu_Out           E_OUTPUT_Buzzer_Mcu_Out

extern Bool Rte_GetDtcCurErrFlag(Rte_Dtc_Type id);
extern void Rte_SetDtcCurErrFlag(Rte_Dtc_Type id, Bool flag);

#define Rte_Port_R_InputState(id)         (Bool) GPIO_GetFiltInputState(id)
#define Rte_Port_W_OutputState(id, state) GPIO_SetOutputState((E_GPIO_OUTPUT_INDEX)(id), (IO_Type)state)
#if PORT_DUG
    #define Rte_Port_R_OutputState(id) g_outCtrl[id]   //(Bool)GPIO_GetOutputState(id)
#else
    #define Rte_Port_R_OutputState(id) (Bool) GPIO_GetOutputState(id)
#endif

#define Rte_Port_W_WipreWash_Out(cmd)         NCV7240Chip1_WriteOutput(0, cmd)
#define Rte_Port_W_PosLight_Out(cmd)          NCV7240Chip1_WriteOutput(1, cmd)
#define Rte_Port_W_FrontWiperHiSpeed_Out(cmd) NCV7240Chip1_WriteOutput(2, cmd)
#define Rte_Port_W_FrontWiperLoSpeed_Out(cmd) NCV7240Chip1_WriteOutput(3, cmd)
#define Rte_Port_W_Horn_Out(cmd)              NCV7240Chip1_WriteOutput(4, cmd)
#define Rte_Port_W_HighLight_Out(cmd)         NCV7240Chip1_WriteOutput(5, cmd)
#define Rte_Port_W_LowLight_Out(cmd)          NCV7240Chip1_WriteOutput(6, cmd)

#define Rte_LSD_R_WipreWash_DiagState()         Get_NCV7240_Chip1_DiagFaultrResult(0)
#define Rte_LSD_R_PosLight_DiagState()          Get_NCV7240_Chip1_DiagFaultrResult(1)
#define Rte_LSD_R_FrontWiperHiSpeed_DiagState() Get_NCV7240_Chip1_DiagFaultrResult(2)
#define Rte_LSD_R_FrontWiperLoSpeed_DiagState() Get_NCV7240_Chip1_DiagFaultrResult(3)
#define Rte_LSD_R_Horn_DiagState()              Get_NCV7240_Chip1_DiagFaultrResult(4)
#define Rte_LSD_R_HighLight_DiagState()         Get_NCV7240_Chip1_DiagFaultrResult(5)
#define Rte_LSD_R_LowLight_DiagState()          Get_NCV7240_Chip1_DiagFaultrResult(6)

#define Rte_HSD_W_RearFogLight(cmd)     BTS_SetBTSSignal(BTS_REAR_FOG_LIGHT, cmd)
#define Rte_HSD_W_FlowWaterLampPwr(cmd) BTS_SetBTSSignal(BTS_FLOWATER_LAMP, cmd)
#define Rte_HSD_W_FrontFogLight(cmd)    BTS_SetBTSSignal(BTS_FRONTFOG_LIGHT, cmd)
#define Rte_HSD_W_DayLight(cmd)         BTS_SetBTSSignal(BTS_DAYLIGHT, cmd)
#define Rte_HSD_W_LeTurnLight(cmd)      BTS_SetBTSSignal(BTS_LETURN_LIGHT, cmd)
#define Rte_HSD_W_RiTurnLight(cmd)      BTS_SetBTSSignal(BTS_RITURN_LIGHT, cmd)

#define Rte_HSD_R_RearFogLight_DiagState()     BTS_GetChip_DiagFaultrResult(BTS_REAR_FOG_LIGHT)
#define Rte_HSD_R_FlowWaterLampPwr_DiagState() BTS_GetChip_DiagFaultrResult(BTS_FLOWATER_LAMP)
#define Rte_HSD_R_FrontFogLight_DiagState()    BTS_GetChip_DiagFaultrResult(BTS_FRONTFOG_LIGHT)
#define Rte_HSD_R_DayLight_DiagState()         BTS_GetChip_DiagFaultrResult(BTS_DAYLIGHT)
#define Rte_HSD_R_LeTurnLight_DiagState()      BTS_GetChip_DiagFaultrResult(BTS_LETURN_LIGHT)
#define Rte_HSD_R_RiTurnLight_DiagState()      BTS_GetChip_DiagFaultrResult(BTS_RITURN_LIGHT)

#define Rte_Any_Port_Is_Working()     GPIO_get_output_port_is_working()
#define Rte_PWM_W_TopBeam(PwmPercent) Pwm_UpdataCompValue(PwmPercent)

/***************************************ADC******************************/
#define RTE_Adc_R_RearFogLight_Cur()      AdcChannelValue.RearFogLight_Cur
#define RTE_Adc_R_FlowWaterLampPwr_Cur()  AdcChannelValue.FlowWaterLampPwr_Cur
#define RTE_Adc_R_LeTurnLight_Cur()       AdcChannelValue.LeTurnLight_Cur
#define RTE_Adc_R_RiTurnLight_Cur()       AdcChannelValue.RiTurnLight_Cur
#define RTE_Adc_R_FrontFogLight_Cur()     AdcChannelValue.FrontFogLight_Cur
#define RTE_Adc_R_DayLight_Cur()          AdcChannelValue.DayLight_Cur
#define RTE_Adc_R_FlWinCurrentIs_Mcu_In() AdcChannelValue.FlWinCurrentIs_Mcu_In
#define RTE_Adc_R_FrWinCurrentIs_Mcu_In() AdcChannelValue.FrWinCurrentIs_Mcu_In
#define RTE_Adc_R_RlWinCurrentIs_Mcu_In() AdcChannelValue.RlWinCurrentIs_Mcu_In
#define RTE_Adc_R_RrWinCurrentIs_Mcu_In() AdcChannelValue.RrWinCurrentIs_Mcu_In
#define RTE_Adc_R_BatAd_Mcu_In()          AdcChannelValue.BatAd_Mcu_In
#define RTE_Adc_R_FlWinSw_In()            AdcChannelValue.FlWinSw_In
#define RTE_Adc_R_FrWinSw_In()            AdcChannelValue.FrWinSw_In
#define RTE_Adc_R_RrWinSw_In()            AdcChannelValue.RrWinSw_In
#define RTE_Adc_R_RlWinSw_In()            AdcChannelValue.RlWinSw_In

#endif

