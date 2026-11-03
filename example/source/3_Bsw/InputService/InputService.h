/*******************************************************
  * Name    :InputService.h
  * Author  :xiaojunzhong
  * Function:四路数字输入的有效电平转换及五十毫秒消抖。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __INPUT_SERVICE_H__
#define __INPUT_SERVICE_H__

#include "RteSys.h"

typedef enum
{
    INPUT_SERVICE_SIG_SOS = 0U,
    INPUT_SERVICE_SIG_SRS,
    INPUT_SERVICE_SIG_PBATTERY_SRS,
    INPUT_SERVICE_SIG_KL15,
    INPUT_SERVICE_SIG_NUM
} InputServiceSignal_t;

/* 初始化四路消抖状态并发布未激活初值。 */
/* 初始化四路消抖状态并发布未激活初值。 */
void InputService_Init(void);
/* 按十毫秒节拍采样并消抖五十毫秒，稳定后写入 RTE。 */
/* 按十毫秒节拍采样并消抖五十毫秒，稳定后写入 RTE。 */
void InputService_Process10ms(void);
/* 读取一路消抖后的逻辑输入，非法索引返回未激活。 */
/* 读取一路消抖后的逻辑输入，非法索引返回未激活。 */
RteSwitchState_t InputService_GetState(InputServiceSignal_t enSignal);

#endif
