/*******************************************************
  * Name    :TimeService.c
  * Author  :xiaojunzhong
  * Function:基于真实定位来源的 UTC 缓存，无源或过期保持无效。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
#include "TimeService.h"
#include <string.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Ecall.h"
#include "GnssService.h"

#define TIME_SERVICE_GNSS_FRESHNESS_MS 5000UL

typedef struct
{
    TimeServiceTime_t stTime; /* 来源 UTC 日历副本。 */
    uint32_t u32SampleMs; /* 来源真实采样时刻，毫秒。 */
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */
} TimeServiceCache_t;

typedef struct
{
    TimeServiceCache_t stGnssCache; /* 基于定位的时间缓存及原始年龄。 */
} TimeServiceRuntime_t;

static TimeServiceRuntime_t s_mTimeSvcRun; /* GnssTask 独占的时间来源缓存。 */

/*******************************************************
  * Name    :TimeService_IsLeapYear
  * Author  :xiaojunzhong
  * Function:判断公历闰年，供 UTC 转换及日历校验使用。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t TimeService_IsLeapYear(uint16_t u16Year)
{
    return (((u16Year % 4U) == 0U) && (((u16Year % 100U) != 0U) || ((u16Year % 400U) == 0U))) ? 1U : 0U;
}

/*******************************************************
  * Name    :TimeService_GetMonthDays
  * Author  :xiaojunzhong
  * Function:取得指定年月天数，非法月份返回零。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static uint8_t TimeService_GetMonthDays(uint16_t u16Year, uint8_t u8Month)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U,
                                              31U, 31U, 30U, 31U, 30U, 31U};

    if ((u8Month == 0U) || (u8Month > 12U))
    {
        return 0U;
    }
    return (uint8_t)(au8MonthDays[u8Month - 1U]
                     + (((u8Month == 2U) && (TimeService_IsLeapYear(u16Year) != 0U)) ? 1U : 0U));
}

/*******************************************************
  * Name    :TimeService_UtcSecondsToCalendar
  * Author  :xiaojunzhong
  * Function:将来源 UTC 秒转为二位年份日历，遵守现有时间范围。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void TimeService_UtcSecondsToCalendar(uint32_t u32UtcSeconds, TimeServiceTime_t *pstTime)
{
    uint16_t u16Year = 1970U; /* 完整公历年份。 */
    uint16_t u16DaysInYear; /* 当前公历年实际天数。 */
    uint32_t u32Days = u32UtcSeconds / 86400UL; /* 从纪元累计或剩余的整天数。 */
    uint32_t u32SecondsOfDay = u32UtcSeconds % 86400UL; /* 当天内经过的秒数。 */
    uint8_t u8Month = 1U; /* 月份，一至十二。 */
    uint8_t u8DaysInMonth; /* 当前月份实际天数。 */

    while (u16Year < 2100U)
    {
        u16DaysInYear = (TimeService_IsLeapYear(u16Year) != 0U) ? 366U : 365U;
        if (u32Days < u16DaysInYear)
        {
            break;
        }
        u32Days -= u16DaysInYear;
        u16Year++;
    }
    while (u8Month <= 12U)
    {
        u8DaysInMonth = TimeService_GetMonthDays(u16Year, u8Month);
        if (u32Days < u8DaysInMonth)
        {
            break;
        }
        u32Days -= u8DaysInMonth;
        u8Month++;
    }
    pstTime->u8Year = (uint8_t)(u16Year - 2000U);
    pstTime->u8Month = u8Month;
    pstTime->u8Day = (uint8_t)(u32Days + 1U);
    pstTime->u8Hour = (uint8_t)(u32SecondsOfDay / 3600UL);
    pstTime->u8Minute = (uint8_t)((u32SecondsOfDay % 3600UL) / 60UL);
    pstTime->u8Second = (uint8_t)(u32SecondsOfDay % 60UL);
}

/*******************************************************
  * Name    :TimeService_AddSeconds
  * Author  :xiaojunzhong
  * Function:按秒推进有效缓存日历，处理跨分钟、跨日及闰年。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
static void TimeService_AddSeconds(TimeServiceTime_t *pstTime, uint32_t u32Seconds)
{
    uint16_t u16Year; /* 完整公历年份。 */

    while (u32Seconds > 0U)
    {
        pstTime->u8Second++;
        if (pstTime->u8Second >= 60U)
        {
            pstTime->u8Second = 0U;
            pstTime->u8Minute++;
            if (pstTime->u8Minute >= 60U)
            {
                pstTime->u8Minute = 0U;
                pstTime->u8Hour++;
                if (pstTime->u8Hour >= 24U)
                {
                    pstTime->u8Hour = 0U;
                    pstTime->u8Day++;
                    u16Year = (uint16_t)(2000U + pstTime->u8Year);
                    if (pstTime->u8Day > TimeService_GetMonthDays(u16Year, pstTime->u8Month))
                    {
                        pstTime->u8Day = 1U;
                        pstTime->u8Month++;
                        if (pstTime->u8Month > 12U)
                        {
                            pstTime->u8Month = 1U;
                            pstTime->u8Year++;
                        }
                    }
                }
            }
        }
        u32Seconds--;
    }
}

/*******************************************************
  * Name    :TimeService_IsCacheFresh
  * Author  :xiaojunzhong
  * Function:根据来源原采样时间检查缓存是否仍新鲜，查询不会刷新来源年龄。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
static uint8_t TimeService_IsCacheFresh(const TimeServiceCache_t *pstCache,
                                        uint32_t u32NowMs,
                                        uint32_t u32FreshnessMs)
{
    if (pstCache->u8Valid == 0U)
    {
        return 0U;
    }

    if ((u32FreshnessMs != 0U)
        && ((u32NowMs - pstCache->u32SampleMs) >= u32FreshnessMs))
    {
        return 0U;
    }

    return 1U;
}

/*******************************************************
  * Name    :TimeService_Init
  * Author  :xiaojunzhong
  * Function:清零时间缓存，默认没有有效来源。
  * Version :V1.0.0
  * Data    :2026.09.13
*******************************************************/
void TimeService_Init(void)
{
    (void)memset(&s_mTimeSvcRun, 0, sizeof(s_mTimeSvcRun));
}

/*******************************************************
  * Name    :TimeService_Process
  * Author  :xiaojunzhong
  * Function:读取真实定位来源并更新时间缓存；无源时不产生新样本。
  * Version :V1.0.0
  * Data    :2026.11.02
*******************************************************/
void TimeService_Process(void)
{
    EcallMsd_t stMsd; /* 定位服务的物理数据容器，不是标准编码 MSD。 */
    uint32_t u32GnssSampleMs; /* 定位来源真实采样时刻，毫秒。 */

    if (GnssService_GetSnapshot(&stMsd, &u32GnssSampleMs) != 0U)
    {
        portENTER_CRITICAL();
        TimeService_UtcSecondsToCalendar(stMsd.utc_seconds, &s_mTimeSvcRun.stGnssCache.stTime);
        s_mTimeSvcRun.stGnssCache.u32SampleMs = u32GnssSampleMs;
        s_mTimeSvcRun.stGnssCache.u8Valid = 1U;
        portEXIT_CRITICAL();
    }


}

/*******************************************************
  * Name    :TimeService_GetUtcSnapshot
  * Author  :xiaojunzhong
  * Function:复制 UTC 及原采样时刻，过期或无源时明确返回无效。
  * Version :V1.1.0
  * Data    :2026.11.02
*******************************************************/
uint8_t TimeService_GetUtcSnapshot(TimeServiceSnapshot_t *pstSnapshot)
{
    TimeServiceTime_t stTime; /* 来源 UTC 日历副本。 */
    TimeServiceCache_t stGnssCache; /* 基于定位的时间缓存及原始年龄。 */
    TimeServiceSource_t enSource = TIME_SERVICE_SOURCE_INVALID; /* 固定责任映射中的诊断来源。 */
    uint32_t u32SampleMs; /* 来源真实采样时刻，毫秒。 */
    uint32_t u32NowMs; /* 当前单调毫秒计数，用无符号差值处理回绕。 */

    if (pstSnapshot == NULL)
    {
        return TIME_SERVICE_SOURCE_INVALID;
    }
    (void)memset(pstSnapshot, 0, sizeof(*pstSnapshot));
    portENTER_CRITICAL();
    stGnssCache = s_mTimeSvcRun.stGnssCache;
    portEXIT_CRITICAL();

    u32NowMs = osKernelSysTick();
    if (TimeService_IsCacheFresh(&stGnssCache, u32NowMs, TIME_SERVICE_GNSS_FRESHNESS_MS) != 0U)
    {
        stTime = stGnssCache.stTime;
        u32SampleMs = stGnssCache.u32SampleMs;
        enSource = TIME_SERVICE_SOURCE_GNSS;
    }
    else
    {
        pstSnapshot->u32ReadMs = u32NowMs;
        return TIME_SERVICE_SOURCE_INVALID;
    }
    TimeService_AddSeconds(&stTime, (u32NowMs - u32SampleMs) / 1000UL);
    pstSnapshot->stUtc = stTime;
    pstSnapshot->u32SourceSampleMs = u32SampleMs;
    pstSnapshot->u32ReadMs = u32NowMs;
    pstSnapshot->u8Source = (uint8_t)enSource;
    return (uint8_t)enSource;
}

/*******************************************************
  * Name    :TimeService_GetUtc
  * Author  :xiaojunzhong
  * Function:从带来源时间的快照取得当前 UTC，不刷新来源年龄。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
uint8_t TimeService_GetUtc(TimeServiceTime_t *pstTime)
{
    TimeServiceSnapshot_t stSnapshot; /* 按值取得的 RTE 事故数据视图。 */
    uint8_t u8Source; /* 来源枚举，零表示无有效来源。 */

    if (pstTime == NULL)
    {
        return TIME_SERVICE_SOURCE_INVALID;
    }
    u8Source = TimeService_GetUtcSnapshot(&stSnapshot);
    *pstTime = stSnapshot.stUtc;
    return u8Source;
}
