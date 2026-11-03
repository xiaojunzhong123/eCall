/*******************************************************
  * Name    :InputService_Cfg.c
  * Author  :xiaojunzhong
  * Function:四路数字输入的有效电平转换及五十毫秒消抖。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "InputService_Cfg.h"

const InputServiceChannelConfig_t g_astInputServiceChannelConfig[INPUT_SERVICE_SIG_NUM] =
{
    {E_INPUT_SOS_SWITCH, LOW, RTESYS_SWITCH_SOS, 50U, "SOS_ACTIVE\r\n"},
    {E_INPUT_SRS_SWITCH, LOW, RTESYS_SWITCH_SRS, 50U, "SRS_ACTIVE\r\n"},
    {E_INPUT_PBATTERY_SRS_SWITCH, LOW, RTESYS_SWITCH_PBATTERY_SRS, 50U, "PBATTERY_SRS_ACTIVE\r\n"},
    {E_INPUT_KL15_SWITCH, HIGH, RTESYS_SWITCH_KL15, 50U, "KL15_ACTIVE\r\n"}
};
