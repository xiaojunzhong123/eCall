/*******************************************************
  * Name    :GnssService.c
  * Author  :xiaojunzhong
  * Function:定位查询和过期语义；真实采样接口当前待接入。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#include "GnssService.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <string.h>

#define GNSS_SERVICE_FRESHNESS_MS 5000U

typedef struct
{
    EcallMsd_t stMsd; /* 定位服务的物理数据容器，不是标准编码 MSD。 */
    uint32_t u32LastUpdateMs; /* 最后真实采样时刻，毫秒。 */
} GnssServiceRuntime_t;

static GnssServiceRuntime_t s_mGnssRun; /* 定位来源私有缓存，当前尚无真实采样生产者。 */

/*******************************************************
  * Name    :GnssService_Init
  * Author  :xiaojunzhong
  * Function:清空当前定位、原采样时间和历史点；默认全部不可用。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
void GnssService_Init(void)
{
    (void)memset(&s_mGnssRun, 0, sizeof(s_mGnssRun));
}

/*******************************************************
  * Name    :GnssService_Process
  * Author  :xiaojunzhong
  * Function:定位采样协议尚未接入，此处保留空入口，不伪造有效位置。
  * Version :V1.0.0
  * Data    :2026.11.02
*******************************************************/
void GnssService_Process(void)
{
    /* A7805E GNSS acquisition is not connected. Keep samples invalid until
     * a confirmed source is integrated; do not reuse the removed modem AT.
     */
}

/*******************************************************
  * Name    :GnssService_GetSnapshot
  * Author  :xiaojunzhong
  * Function:原子复制定位及来源时间；过期则清除副本有效性，查询不刷新年龄。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
uint8_t GnssService_GetSnapshot(EcallMsd_t *pstMsd, uint32_t *pu32SampleMs)
{
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */

    if ((pstMsd == NULL) || (pu32SampleMs == NULL))
    {
        return 0U;
    }

    portENTER_CRITICAL();
    *pstMsd = s_mGnssRun.stMsd;
    *pu32SampleMs = s_mGnssRun.u32LastUpdateMs;
    u8Valid = ((s_mGnssRun.stMsd.gnss_valid != 0U)
               && ((osKernelSysTick() - s_mGnssRun.u32LastUpdateMs) < GNSS_SERVICE_FRESHNESS_MS)) ? 1U : 0U;
    portEXIT_CRITICAL();
    pstMsd->gnss_valid = u8Valid;
    if (u8Valid == 0U)
    {
        pstMsd->latitude_e7 = 0;
        pstMsd->longitude_e7 = 0;
        pstMsd->utc_seconds = 0U;
        pstMsd->history_count = 0U;
        (void)memset(pstMsd->history_utc_seconds, 0, sizeof(pstMsd->history_utc_seconds));
        (void)memset(pstMsd->history_latitude_e7, 0, sizeof(pstMsd->history_latitude_e7));
        (void)memset(pstMsd->history_longitude_e7, 0, sizeof(pstMsd->history_longitude_e7));
    }
    return u8Valid;
}

/*******************************************************
  * Name    :GnssService_GetLatest
  * Author  :xiaojunzhong
  * Function:通过带时间接口读取定位副本，保留原查询契约。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd)
{
    uint32_t u32SampleMs; /* 来源真实采样时刻，毫秒。 */

    return GnssService_GetSnapshot(pstMsd, &u32SampleMs);
}
