/*******************************************************
  * Name    :InputService_Cfg.h
  * Author  :xiaojunzhong
  * Function:四路数字输入的有效电平转换及五十毫秒消抖。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __INPUT_SERVICE_CFG_H__
#define __INPUT_SERVICE_CFG_H__

#include "Gpio.h"
#include "InputService.h"

typedef struct
{
    E_GPIO_INPUT_INDEX enGpioInput;
    IO_Type enActiveLevel; /* 该通道有效时应有的物理电平。 */
    RteSysSwitchId_t enRteSwitch; /* 该输入对应的 RTE 开关索引。 */
    uint16_t u16DebounceMs; /* 输入连续异态累计时间，毫秒。 */
    const char *pcActiveLog;
} InputServiceChannelConfig_t;

extern const InputServiceChannelConfig_t g_astInputServiceChannelConfig[INPUT_SERVICE_SIG_NUM];

#endif
