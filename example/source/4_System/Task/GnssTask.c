/*******************************************************
  * Name    :GnssTask.c
  * Author  :xiaojunzhong
  * Function:一百毫秒发布导航和时间；没有数据时发布无效标志。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
/* 流程阅读说明：
 * 定位和时间仍保留独立有效性与原始年龄；未来数据协议接入后才能产生真实来源。
 * 本任务只发布物理数据，不发 AT、不等待呼叫，也不推断航向和位置可信度。
 */

#include "cmsis_os.h"
#include "Ecall.h"
#include "GnssService.h"
#include "Hmi.h"
#include "TaskManager.h"
#include "TimeService.h"
#include "RteSys.h"
#include <string.h>

/*******************************************************
  * Name    :GnssTask_PublishMsdSignals
  * Author  :xiaojunzhong
  * Function:发布物理导航与 UTC；保存真实采样年龄，不覆盖独立业务航向。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
static void GnssTask_PublishMsdSignals(void)
{
    EcallMsd_t stMsd; /* 定位服务的物理数据容器，不是标准编码 MSD。 */
    TimeServiceSnapshot_t stTime; /* 来源 UTC 日历副本。 */
    uint32_t u32GnssSampleMs; /* 定位来源真实采样时刻，毫秒。 */
    RteSysMsdNavigation_t stNavigation; /* 物理位置、航向和速度组。 */
    RteSysMsdUtc_t stUtc; /* UTC 日历和来源组。 */
    RteSysMsdHistory_t stHistory; /* 与当前点配套的真实历史组。 */

    (void)memset(&stNavigation, 0, sizeof(stNavigation));
    (void)memset(&stUtc, 0, sizeof(stUtc));
    stNavigation.u8PositionValid = GnssService_GetSnapshot(&stMsd, &u32GnssSampleMs);
    stNavigation.i32LongitudeE7 = stMsd.longitude_e7;
    stNavigation.i32LatitudeE7 = stMsd.latitude_e7;
    stNavigation.u32UtcSeconds = stMsd.utc_seconds;
    stNavigation.u32UpdateMs = u32GnssSampleMs;
    /* Current sample has no speed. Position publication preserves business heading. */
    (void)memset(&stHistory, 0, sizeof(stHistory));
    stHistory.u32UpdateMs = stNavigation.u32UpdateMs;
    stHistory.u32PositionUtcSeconds = stMsd.utc_seconds;
    stHistory.u8Count = stMsd.history_count;
    (void)memcpy(stHistory.ai32LatitudeE7, stMsd.history_latitude_e7, sizeof(stHistory.ai32LatitudeE7));
    (void)memcpy(stHistory.ai32LongitudeE7, stMsd.history_longitude_e7, sizeof(stHistory.ai32LongitudeE7));
    (void)memcpy(stHistory.au32UtcSeconds, stMsd.history_utc_seconds, sizeof(stHistory.au32UtcSeconds));
    RteSys_PublishMsdPositionHistory(&stNavigation, &stHistory);

    stUtc.u8Source = TimeService_GetUtcSnapshot(&stTime);
    stUtc.u8Valid = (stUtc.u8Source != TIME_SERVICE_SOURCE_INVALID) ? 1U : 0U;
    if (stUtc.u8Valid != 0U)
    {
        stUtc.u16Year = (uint16_t)(2000U + stTime.stUtc.u8Year);
        stUtc.u8Month = stTime.stUtc.u8Month;
        stUtc.u8Day = stTime.stUtc.u8Day;
        stUtc.u8Hour = stTime.stUtc.u8Hour;
        stUtc.u8Minute = stTime.stUtc.u8Minute;
        stUtc.u8Second = stTime.stUtc.u8Second;
    }
    stUtc.u32UpdateMs = stTime.u32ReadMs;
    stUtc.u32SourceSampleMs = stTime.u32SourceSampleMs;
    RteSys_PublishMsdUtc(&stUtc);
}

/*******************************************************
  * Name    :GnssTask_Entry
  * Author  :xiaojunzhong
  * Function:每一百毫秒维护导航/时间并发布到 RTE，不参与通话流程。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
static void GnssTask_Entry(void const *argument)
{

    (void)argument;
    Hmi_ShowTaskStarted("GnssTask");

    for (;;)
    {
        GnssService_Process();
        TimeService_Process();
        GnssTask_PublishMsdSignals();

        /* 只发布真实物理数据，不向 eCall 发送旧定位等待通知。 */
        osDelay(100U);
    }
}

/*******************************************************
  * Name    :GnssTask_Create
  * Author  :xiaojunzhong
  * Function:创建低于普通级的定位任务，沿用原栈配置。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void GnssTask_Create(void)
{
    osThreadDef(GnssTask, GnssTask_Entry, osPriorityBelowNormal, 0U, 384U);
    (void)osThreadCreate(osThread(GnssTask), NULL);
}
