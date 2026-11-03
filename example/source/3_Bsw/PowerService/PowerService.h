/*******************************************************
  * Name    :PowerService.h
  * Author  :xiaojunzhong
  * Function:MCU 紧急事件电源锁；真正睡眠的握手和硬件接入仍待实现。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __POWER_SERVICE_H__
#define __POWER_SERVICE_H__

#include <stdint.h>

/* 初始化 MCU 紧急数据处理电源锁。 */
/* 初始化 MCU 紧急数据处理电源锁。 */
void PowerService_Init(void);
/* 由 EcallTask 更新事件电源锁；临界区保护跨任务写入。 */
/* 由 EcallTask 更新事件电源锁；临界区保护跨任务写入。 */
void PowerService_SetEcallLock(uint8_t u8Enable);
/* 向 PowTask 返回事件电源锁，不能替代模组休眠许可。 */
/* 向 PowTask 返回事件电源锁，不能替代模组休眠许可。 */
uint8_t PowerService_IsEcallLocked(void);
/* 预留真正低功耗握手与硬件接入，当前不执行睡眠或关电。 */
/* 预留真正低功耗握手与硬件接入，当前不执行睡眠或关电。 */
void PowerService_Process(void);

#endif
