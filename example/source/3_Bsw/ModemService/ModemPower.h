/*******************************************************
  * Name    :ModemPower.h
  * Author  :xiaojunzhong
  * Function:MCU 模块电源键时序，不包含通信或 AT 协议。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#ifndef __MODEM_POWER_H__
#define __MODEM_POWER_H__
#include <stdint.h>
/* 建立模块供电及电源键/复位初值；在输出服务初始化之后调用。 */
/* 建立模块供电及电源键/复位初值；在输出服务初始化之后调用。 */
void ModemPower_Init(void);
/* 撤销尚未处理的睡眠意图；保持供电，不表示业务在线。 */
/* 撤销尚未处理的睡眠意图；保持供电，不表示业务在线。 */
void ModemPower_RequestOnline(void);
/* 只登记睡眠意图；自定义握手未接入前不切电。 */
/* 只登记睡眠意图；自定义握手未接入前不切电。 */
void ModemPower_RequestSleep(void);
/* PowTask 周期调度物理时序；休眠缺少握手时仅记录一次待接入状态。 */
/* PowTask 周期调度物理时序；休眠缺少握手时仅记录一次待接入状态。 */
void ModemPower_Process(void);
/* 仅返回启动保护是否结束，不能用于判断模组自检或通话能力。 */
/* 仅返回启动保护是否结束，不能用于判断模组自检或通话能力。 */
uint8_t ModemPower_IsOnlineReady(void);
#endif
