/*******************************************************
  * Name    :PowTask.c
  * Author  :xiaojunzhong
  * Function:保留一百毫秒电源调度；KL15 和紧急事件决定保持供电意图。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#include "cmsis_os.h"
#include "Hmi.h"
#include "PowerService.h"
#include "ModemPower.h"
#include "RteSys.h"

/*******************************************************
  * Name    :PowTask_Entry
  * Author  :xiaojunzhong
  * Function:替代已删除的 ModemTask 电源调度；不处理数据协议和模组呼叫。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
static void PowTask_Entry(void const *argument)
{
    uint8_t u8LastKeepAwake = 2U; /* 非布尔初值，保证首周期发布实际意图。 */
    uint8_t u8KeepAwake;          /* 点火或事件未交付时保持供电。 */
    (void)argument;
    Hmi_ShowTaskStarted("PowTask");
    for (;;)
    {
        u8KeepAwake = ((RteSys_GetSwitchState(RTESYS_SWITCH_KL15) == RTE_SWITCH_ACTIVE)
            || (PowerService_IsEcallLocked() != 0U)) ? 1U : 0U;
        if (u8KeepAwake != u8LastKeepAwake)
        {
            if (u8KeepAwake != 0U)
            {
                ModemPower_RequestOnline();
            }
            else
            {
                ModemPower_RequestSleep();
            }
            u8LastKeepAwake = u8KeepAwake;
        }
        ModemPower_Process();
        PowerService_Process();
        osDelay(100U);
    }
}

/*******************************************************
  * Name    :PowTask_Create
  * Author  :xiaojunzhong
  * Function:沿用低于普通级优先级和二百五十六字节配置，不新增任务。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
void PowTask_Create(void)
{
    osThreadDef(PowTask, PowTask_Entry, osPriorityBelowNormal, 0U, 256U);
    (void)osThreadCreate(osThread(PowTask), NULL);
}
