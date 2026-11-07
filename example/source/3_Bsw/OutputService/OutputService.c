/*******************************************************
  * Name    :OutputService.c
  * Author  :xiaojunzhong
  * Function:BSW 通用输出服务，映射逻辑电源和指示请求到物理引脚。
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

/*******************************************************
  * Name    :OutputService_SetEcallFault
  * Author  :xiaojunzhong
  * Function:将精简自检故障映射到 D29，PD9 高电平点亮。
  * Version :V1.0.0
  * Data    :2026.11.05
  * Input   :零无故障，非零有故障；来源由自检模块统一汇总。
  * Output  :无返回值；只更新 INDICATOR_1 逻辑请求。
  * Caller  :启动阶段或 AppTask，不允许 ISR 调用。
  * Precond :OutputService 已初始化，D29 所需外部三点三伏供电有效。
  * Failure :没有光学或电流反馈，不宣称灯实物已经点亮。
  * Effect  :不操作第二盏灯、电源锁或取消窗口；GPIO 在输出周期应用。
*******************************************************/
void OutputService_SetEcallFault(uint8_t u8HasFault)
{
    /* 最新原理图第4页：PD9 -> Q19 -> D29，沿用现有高有效映射。 */
    OutputService_SetState(OUTPUT_SERVICE_SIG_INDICATOR_1, (u8HasFault != 0U) ? HIGH : LOW);
}
