/*******************************************************
  * Name    :OutputService.h
  * Author  :xiaojunzhong
  * Function:逻辑输出到物理引脚的映射，及可确认的乘员提示接口。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
#ifndef __OUTPUT_SERVICE_H__
#define __OUTPUT_SERVICE_H__

#include "Types.h"

typedef enum
{
    OUTPUT_SERVICE_SIG_INDICATOR_1 = 0U,
    OUTPUT_SERVICE_SIG_INDICATOR_2,
    OUTPUT_SERVICE_SIG_IG_KEEPON,
    OUTPUT_SERVICE_SIG_PWR_EN_CAN,
    OUTPUT_SERVICE_SIG_CAT4_EN,
    OUTPUT_SERVICE_SIG_CAT4_PWR_KEY,
    OUTPUT_SERVICE_SIG_CAT4_RST,
    OUTPUT_SERVICE_SIG_PWR_EN_GNSS,
    OUTPUT_SERVICE_SIG_PWR_EN_3V3,
    OUTPUT_SERVICE_SIG_PWR_EN_PHY,
    OUTPUT_SERVICE_SIG_PWR_EN_AUDIO,
    OUTPUT_SERVICE_SIG_NUM
} OutputServiceSignal_t;

/* 初始化电源和指示输出；以太网删除后 PHY 保持关闭请求。 */
void OutputService_Init(void);
/* 由 AppTask 每十毫秒应用一次输出请求。 */
void OutputService_Process10ms(void);
/* 写入指定输出请求，物理执行留给周期处理。 */
void OutputService_SetState(OutputServiceSignal_t enSignal, IO_Type enState);
/* 保留原定位指示接口，只表示调用者提供的就绪结论。 */
/* 保留原定位指示接口，只表示调用者提供的就绪结论。 */
void OutputService_SetGnssReady(uint8_t u8Ready);
/* 保留取消窗口提示入口，当前未绑定实际显示硬件。 */
void OutputService_SetEcallCancelWindow(uint8_t u8Active);

#endif
