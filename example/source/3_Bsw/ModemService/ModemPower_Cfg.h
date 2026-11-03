/*******************************************************
  * Name    :ModemPower_Cfg.h
  * Author  :xiaojunzhong
  * Function:MCU 模块电源键时序，不包含通信或 AT 协议。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#ifndef MODEM_POWER_CFG_H
#define MODEM_POWER_CFG_H
/* 原板参数保留以免清理代码改变 PWRKEY 波形；实板接线及时序仍待确认。 */
#define MODEM_POWER_RAIL_SETTLE_MS 100U     /* 供电建立等待，毫秒。 */
#define MODEM_POWER_KEY_ASSERT_MS 1000U    /* 低有效电源键持续时间，毫秒。 */
#define MODEM_POWER_BOOT_WAIT_MS 25000UL   /* 启动保护等待，不表示模组业务就绪。 */
#endif
