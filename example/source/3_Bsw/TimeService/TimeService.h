/*******************************************************
  * Name    :TimeService.h
  * Author  :xiaojunzhong
  * Function:基于真实定位来源的 UTC 缓存，无源或过期保持无效。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#ifndef __TIME_SERVICE_H__
#define __TIME_SERVICE_H__

#include <stdint.h>

typedef struct
{
    uint8_t u8Year; /* 年份减二千后的两位年。 */
    uint8_t u8Month; /* 月份，一至十二。 */
    uint8_t u8Day; /* 日，一至当月天数。 */
    uint8_t u8Hour; /* 时，零至二十三。 */
    uint8_t u8Minute; /* 分，零至五十九。 */
    uint8_t u8Second; /* 秒，零至五十九。 */
} TimeServiceTime_t;

typedef enum
{
    TIME_SERVICE_SOURCE_INVALID = 0U,
    TIME_SERVICE_SOURCE_GNSS,
    TIME_SERVICE_SOURCE_CELLULAR
} TimeServiceSource_t;

typedef struct
{
    TimeServiceTime_t stUtc; /* UTC 日历和来源组。 */
    uint32_t u32SourceSampleMs; /* 来源真实采样时刻，毫秒，周期发布不刷新它。 */
    uint32_t u32ReadMs; /* 读取时刻，毫秒，不代替原采样时刻。 */
    uint8_t u8Source; /* 来源枚举，零表示无有效来源。 */
} TimeServiceSnapshot_t;

/* 复制 UTC 及原采样时刻，过期或无源时明确返回无效。 */
/* 复制 UTC 及原采样时刻，过期或无源时明确返回无效。 */
uint8_t TimeService_GetUtcSnapshot(TimeServiceSnapshot_t *pstSnapshot);
/* 清零时间缓存，默认没有有效来源。 */
void TimeService_Init(void);
/* 读取真实定位来源并更新时间缓存；无源时不产生新样本。 */
void TimeService_Process(void);
/* 从带来源时间的快照取得当前 UTC，不刷新来源年龄。 */
uint8_t TimeService_GetUtc(TimeServiceTime_t *pstTime);

#endif
