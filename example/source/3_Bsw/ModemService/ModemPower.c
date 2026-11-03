/*******************************************************
  * Name    :ModemPower.c
  * Author  :xiaojunzhong
  * Function:MCU 模块电源键时序，不包含通信或 AT 协议。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
/* 流程阅读说明：
 * 仅 PowTask 调用本模块过程；物理引脚由 OutputService 在 AppTask 中统一应用。
 * 保留原板候选电源键时序；模块呼叫、通信、重拨和回呼由模块自行负责。
 * 真实睡眠握手未接入，所以休眠请求不直接切断模块电源。
 */

#include "ModemPower.h"
#include <string.h>
#include "Hmi.h"
#include "ModemPower_Cfg.h"
#include "OutputService.h"

#define MODEM_POWER_PROCESS_PERIOD_MS        100U

typedef enum
{
    MODEM_POWER_RAIL_SETTLE = 0U, /* 等待模块电源建立。 */
    MODEM_POWER_KEY_ASSERT, /* 保持低有效电源键脉冲。 */
    MODEM_POWER_BOOT_WAIT, /* 等待模块启动保护结束。 */
    MODEM_POWER_ONLINE /* 仅物理保护时间已到，不表示通信在线。 */
} ModemPowerState_t;

typedef struct
{
    uint8_t u8SleepRequested; /* 未处理的休眠意图，不是休眠完成标志。 */
    uint32_t u32ElapsedMs; /* 当前电源阶段累计调度毫秒数。 */
} ModemPowerRuntime_t;

typedef struct
{
    ModemPowerState_t enPowerState; /* 物理电源键时序阶段。 */
} ModemPowerStatus_t;

static ModemPowerRuntime_t s_mPowerRun; /* PowTask 独占的电源时序过程。 */
static ModemPowerStatus_t s_mPowerSts; /* 物理电源保护阶段结论。 */

/*******************************************************
  * Name    :ModemPower_EnterState
  * Author  :xiaojunzhong
  * Function:集中切换物理电源阶段并清零该阶段累计时间。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void ModemPower_EnterState(ModemPowerState_t enPowerState)
{
    s_mPowerSts.enPowerState = enPowerState;
    s_mPowerRun.u32ElapsedMs = 0U;
}

/*******************************************************
  * Name    :ModemPower_ProcessStartup
  * Author  :xiaojunzhong
  * Function:依次执行供电建立、电源键脉冲和启动保护，候选时序仍须实板验证。
  * Version :V1.0.0
  * Data    :2026.11.02
*******************************************************/
static void ModemPower_ProcessStartup(void)
{
    if (s_mPowerRun.u32ElapsedMs <= (0xFFFFFFFFUL - MODEM_POWER_PROCESS_PERIOD_MS))
    {
        s_mPowerRun.u32ElapsedMs += MODEM_POWER_PROCESS_PERIOD_MS;
    }

    switch (s_mPowerSts.enPowerState)
    {
        case MODEM_POWER_RAIL_SETTLE:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_RAIL_SETTLE_MS)
            {
                OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, HIGH);
                HMI_LOG_MODEM("PWRKEY_ASSERT\r\n");
                ModemPower_EnterState(MODEM_POWER_KEY_ASSERT);
            }
            break;

        case MODEM_POWER_KEY_ASSERT:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_KEY_ASSERT_MS)
            {
                OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, LOW);
                HMI_LOG_MODEM("PWRKEY_RELEASE\r\n");
                ModemPower_EnterState(MODEM_POWER_BOOT_WAIT);
            }
            break;

        case MODEM_POWER_BOOT_WAIT:
            if (s_mPowerRun.u32ElapsedMs >= MODEM_POWER_BOOT_WAIT_MS)
            {
                HMI_LOG_MODEM("MODEM_POWER_GUARD_EXPIRED\r\n");
                ModemPower_EnterState(MODEM_POWER_ONLINE);
            }
            break;

        default:
            break;
    }
}


/*******************************************************
  * Name    :ModemPower_Init
  * Author  :xiaojunzhong
  * Function:建立模块供电及电源键/复位初值；在输出服务初始化之后调用。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void ModemPower_Init(void)
{
    (void)memset(&s_mPowerRun, 0, sizeof(s_mPowerRun));
    (void)memset(&s_mPowerSts, 0, sizeof(s_mPowerSts));
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_EN, HIGH);
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_PWR_KEY, LOW);
    OutputService_SetState(OUTPUT_SERVICE_SIG_CAT4_RST, LOW);
    ModemPower_EnterState(MODEM_POWER_RAIL_SETTLE);
}

/*******************************************************
  * Name    :ModemPower_RequestOnline
  * Author  :xiaojunzhong
  * Function:撤销尚未处理的睡眠意图；保持供电，不表示业务在线。
  * Version :V1.0.0
  * Data    :2026.11.02
*******************************************************/
void ModemPower_RequestOnline(void)
{
    s_mPowerRun.u8SleepRequested = 0U;
}

/*******************************************************
  * Name    :ModemPower_RequestSleep
  * Author  :xiaojunzhong
  * Function:只登记睡眠意图；自定义握手未接入前不切电。
  * Version :V1.0.0
  * Data    :2026.11.02
*******************************************************/
void ModemPower_RequestSleep(void)
{
    s_mPowerRun.u8SleepRequested = 1U;
}

/*******************************************************
  * Name    :ModemPower_Process
  * Author  :xiaojunzhong
  * Function:PowTask 周期调度物理时序；休眠缺少握手时仅记录一次待接入状态。
  * Version :V1.1.0
  * Data    :2026.11.02
*******************************************************/
void ModemPower_Process(void)
{
    switch (s_mPowerSts.enPowerState)
    {
        case MODEM_POWER_RAIL_SETTLE:
        case MODEM_POWER_KEY_ASSERT:
        case MODEM_POWER_BOOT_WAIT:
            ModemPower_ProcessStartup();
            break;
        case MODEM_POWER_ONLINE:
            /* 休眠握手协议未接入：只记录一次意图，保持供电。
             * 不能在模组通话期间仅凭 KL15 断开就切断电源。 */
            if (s_mPowerRun.u8SleepRequested != 0U)
            {
                s_mPowerRun.u8SleepRequested = 0U;
                HMI_LOG_MODEM("MODEM_SLEEP_PROTOCOL_PENDING\r\n");
            }
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :ModemPower_IsOnlineReady
  * Author  :xiaojunzhong
  * Function:仅返回启动保护是否结束，不能用于判断模组自检或通话能力。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint8_t ModemPower_IsOnlineReady(void)
{
    return (s_mPowerSts.enPowerState == MODEM_POWER_ONLINE) ? 1U : 0U;
}
