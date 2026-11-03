/*******************************************************
  * Name    :PowerService.c
  * Author  :xiaojunzhong
  * Function:MCU 紧急事件电源锁；真正睡眠的握手和硬件接入仍待实现。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "PowerService.h"
#include "FreeRTOS.h"

/* EcallTask 写入，PowTask 读取；用短临界区建立跨任务访问边界。 */
static uint8_t s_pwrEcallLock = 0U;

/*******************************************************
  * Name    :PowerService_Init
  * Author  :xiaojunzhong
  * Function:初始化 MCU 紧急数据处理电源锁。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_Init(void)
{
    s_pwrEcallLock = 0U;
}

/*******************************************************
  * Name    :PowerService_SetEcallLock
  * Author  :xiaojunzhong
  * Function:由 EcallTask 更新事件电源锁，跨任务读写由临界区保护。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_SetEcallLock(uint8_t u8Enable)
{
    portENTER_CRITICAL();
    s_pwrEcallLock = (u8Enable != 0U) ? 1U : 0U;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :PowerService_IsEcallLocked
  * Author  :xiaojunzhong
  * Function:读取 MCU 事件电源锁，不代表模组通话或睡眠状态。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
uint8_t PowerService_IsEcallLocked(void)
{
    uint8_t u8Locked; /* 一为仍有 MCU 紧急数据处理，不表示模组通话状态。 */
    portENTER_CRITICAL();
    u8Locked = s_pwrEcallLock;
    portEXIT_CRITICAL();
    return u8Locked;
}

/*******************************************************
  * Name    :PowerService_Process
  * Author  :xiaojunzhong
  * Function:预留真正低功耗握手与硬件接入，当前不执行睡眠或关电。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void PowerService_Process(void)
{
    /* 当前没有实板低功耗执行入口。进入睡眠前必须满足：
     * KL15 关闭、紧急数据已交付、自定义协议确认模组允许睡眠、
     * CAN 和存储静止，且硬件唤醒源与 RTOS 时基恢复已经验证。
     * 本轮保留电源锁和周期入口，不能用空函数推断睡眠已实现。 */
}
