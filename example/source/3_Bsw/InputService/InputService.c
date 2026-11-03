/*******************************************************
  * Name    :InputService.c
  * Author  :xiaojunzhong
  * Function:四路数字输入的有效电平转换及五十毫秒消抖。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "Hmi.h"
#include "InputService.h"
#include "InputService_Cfg.h"
#include <string.h>

#define INPUT_SERVICE_PERIOD_MS             10U

typedef struct
{
    RteSwitchState_t enState; /* 当前稳定业务状态。 */
    uint16_t u16DebounceMs; /* 输入连续异态累计时间，毫秒。 */
} InputServiceChannelRuntime_t;

typedef struct
{
    InputServiceChannelRuntime_t astChannel[INPUT_SERVICE_SIG_NUM]; /* 每路输入独立的消抖过程。 */
} InputServiceRuntime_t;

static InputServiceRuntime_t s_mInputRun; /* AppTask 独占的输入消抖缓存。 */

/*******************************************************
  * Name    :InputService_Init
  * Author  :xiaojunzhong
  * Function:初始化四路消抖状态并发布未激活初值。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Init(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */

    (void)memset(&s_mInputRun, 0, sizeof(s_mInputRun));
    for (u8Index = 0U; u8Index < INPUT_SERVICE_SIG_NUM; u8Index++)
    {
        s_mInputRun.astChannel[u8Index].enState = RTE_SWITCH_INACTIVE;
        RteSys_SetSwitchState(g_astInputServiceChannelConfig[u8Index].enRteSwitch,
                               RTE_SWITCH_INACTIVE);
    }
}

/*******************************************************
  * Name    :InputService_Process10ms
  * Author  :xiaojunzhong
  * Function:按十毫秒节拍采样并消抖五十毫秒，稳定后写入 RTE。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void InputService_Process10ms(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    IO_Type enRawLevel; /* 本次读取到的物理引脚电平。 */
    RteSwitchState_t enRawState; /* 按有效电平转换后的本次逻辑输入。 */

    for (u8Index = 0U; u8Index < INPUT_SERVICE_SIG_NUM; u8Index++)
    {
        enRawLevel = GPIO_GetInputLevel(g_astInputServiceChannelConfig[u8Index].enGpioInput);
        enRawState = (enRawLevel == g_astInputServiceChannelConfig[u8Index].enActiveLevel)
                     ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE;
        if (enRawState == s_mInputRun.astChannel[u8Index].enState)
        {
            s_mInputRun.astChannel[u8Index].u16DebounceMs = 0U;
        }
        else
        {
            s_mInputRun.astChannel[u8Index].u16DebounceMs += INPUT_SERVICE_PERIOD_MS;
            if (s_mInputRun.astChannel[u8Index].u16DebounceMs
                >= g_astInputServiceChannelConfig[u8Index].u16DebounceMs)
            {
                s_mInputRun.astChannel[u8Index].enState = enRawState;
                s_mInputRun.astChannel[u8Index].u16DebounceMs = 0U;
                RteSys_SetSwitchState(g_astInputServiceChannelConfig[u8Index].enRteSwitch, enRawState);
                if ((enRawState == RTE_SWITCH_ACTIVE)
                    && (g_astInputServiceChannelConfig[u8Index].pcActiveLog != NULL))
                {
                    HMI_LOG_APP(g_astInputServiceChannelConfig[u8Index].pcActiveLog);
                }
            }
        }
    }
}

/*******************************************************
  * Name    :InputService_GetState
  * Author  :xiaojunzhong
  * Function:读取一路消抖后的逻辑输入，非法索引返回未激活。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t InputService_GetState(InputServiceSignal_t enSignal)
{
    if (enSignal < INPUT_SERVICE_SIG_NUM)
    {
        return s_mInputRun.astChannel[enSignal].enState;
    }
    return RTE_SWITCH_INACTIVE;
}
