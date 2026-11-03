/*******************************************************
  * Name    :OutputService.c
  * Author  :xiaojunzhong
  * Function:逻辑输出到物理引脚的映射，及可确认的乘员提示接口。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#include "Gpio.h"
#include "OutputService.h"
#include "RteSys.h"

typedef struct
{
    E_GPIO_OUTPUT_INDEX enGpioOutput;
    RteSysOutputId_t enRteOutput; /* 该引脚对应的逻辑输出索引。 */
    IO_Type enActiveLevel; /* 该通道有效时应有的物理电平。 */
    IO_Type enInitialState; /* 初始化逻辑请求，应用时再转换为有效电平。 */
} OutputServiceChannelConfig_t;

static const OutputServiceChannelConfig_t s_astOutputServiceChannelConfig[OUTPUT_SERVICE_SIG_NUM] =
{
    {E_OUTPUT_INDICATOR_1, RTESYS_OUTPUT_INDICATOR_1, HIGH, LOW},
    {E_OUTPUT_INDICATOR_2, RTESYS_OUTPUT_INDICATOR_2, HIGH, LOW},
    {E_OUTPUT_IG_KEEPON, RTESYS_OUTPUT_IG_KEEPON, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_CAN, RTESYS_OUTPUT_PWR_EN_CAN, HIGH, HIGH},
    {E_OUTPUT_CAT4_EN, RTESYS_OUTPUT_CAT4_EN, HIGH, HIGH},
    {E_OUTPUT_CAT4_PWR_KEY, RTESYS_OUTPUT_CAT4_PWR_KEY, LOW, LOW},
    {E_OUTPUT_CAT4_RST, RTESYS_OUTPUT_CAT4_RST, LOW, LOW},
    {E_OUTPUT_PWR_EN_GNSS, RTESYS_OUTPUT_PWR_EN_GNSS, HIGH, HIGH},
    {E_OUTPUT_PWR_EN_3V3, RTESYS_OUTPUT_PWR_EN_3V3, HIGH, HIGH},
    /* 以太网已移除，PHY 不再由业务服务上电。 */
    {E_OUTPUT_PWR_EN_PHY, RTESYS_OUTPUT_PWR_EN_PHY, HIGH, LOW},
    {E_OUTPUT_PWR_EN_AUDIO, RTESYS_OUTPUT_PWR_EN_AUDIO, HIGH, HIGH}
};

/*******************************************************
  * Name    :OutputService_ApplyRteRequests
  * Author  :xiaojunzhong
  * Function:将 RTE 逻辑请求映射为配置的有效电平并写入 GPIO。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void OutputService_ApplyRteRequests(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    IO_Type enOutputLevel; /* 最终写往 GPIO 的高低电平。 */

    for (u8Index = 0U; u8Index < OUTPUT_SERVICE_SIG_NUM; u8Index++)
    {
        if (RteSys_GetOutputState(s_astOutputServiceChannelConfig[u8Index].enRteOutput)
            == RTE_SWITCH_ACTIVE)
        {
            enOutputLevel = s_astOutputServiceChannelConfig[u8Index].enActiveLevel;
        }
        else if (s_astOutputServiceChannelConfig[u8Index].enActiveLevel == HIGH)
        {
            enOutputLevel = LOW;
        }
        else
        {
            enOutputLevel = HIGH;
        }
        GPIO_SetOutputLevel(s_astOutputServiceChannelConfig[u8Index].enGpioOutput, enOutputLevel);
    }
}

/*******************************************************
  * Name    :OutputService_Init
  * Author  :xiaojunzhong
  * Function:初始化电源和指示输出；以太网删除后 PHY 保持关闭请求。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_Init(void)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */

    for (u8Index = 0U; u8Index < OUTPUT_SERVICE_SIG_NUM; u8Index++)
    {
        RteSys_SetOutputState(s_astOutputServiceChannelConfig[u8Index].enRteOutput,
                              (s_astOutputServiceChannelConfig[u8Index].enInitialState == HIGH)
                              ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE);
    }
    OutputService_ApplyRteRequests();
}

/*******************************************************
  * Name    :OutputService_Process10ms
  * Author  :xiaojunzhong
  * Function:由 AppTask 每十毫秒应用一次输出请求。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_Process10ms(void)
{
    OutputService_ApplyRteRequests();
}

/*******************************************************
  * Name    :OutputService_SetState
  * Author  :xiaojunzhong
  * Function:写入指定输出请求，物理执行留给周期处理。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_SetState(OutputServiceSignal_t enSignal, IO_Type enState)
{
    if (enSignal < OUTPUT_SERVICE_SIG_NUM)
    {
        RteSys_SetOutputState(s_astOutputServiceChannelConfig[enSignal].enRteOutput,
                              (enState == HIGH) ? RTE_SWITCH_ACTIVE : RTE_SWITCH_INACTIVE);
    }
}

/*******************************************************
  * Name    :OutputService_SetGnssReady
  * Author  :xiaojunzhong
  * Function:保留原定位指示接口，只表示调用者提供的就绪结论。
  * Version :V1.0.0
  * Data    :2026.09.11
*******************************************************/
void OutputService_SetGnssReady(uint8_t u8Ready)
{
    OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_1, (u8Ready != 0U) ? HIGH : LOW);
}


/*******************************************************
  * Name    :OutputService_SetEcallCancelWindow
  * Author  :xiaojunzhong
  * Function:保留取消窗口提示入口，当前未绑定实际显示硬件。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void OutputService_SetEcallCancelWindow(uint8_t u8Active)
{
    (void)u8Active;
}
