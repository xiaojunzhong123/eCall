/*******************************************************
  * Name    :GnssService.h
  * Author  :xiaojunzhong
  * Function:定位查询和过期语义；真实采样接口当前待接入。
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __GNSS_SERVICE_H__
#define __GNSS_SERVICE_H__

#include "Ecall.h"

/* 清空当前定位、原采样时间和历史点；默认全部不可用。 */
/* 清空当前定位、原采样时间和历史点；默认全部不可用。 */
void GnssService_Init(void);
/* 定位采样协议尚未接入，此处保留空入口，不伪造有效位置。 */
/* 定位采样协议尚未接入，此处保留空入口，不伪造有效位置。 */
void GnssService_Process(void);
/* 通过带时间接口读取定位副本，保留原查询契约。 */
/* 通过带时间接口读取定位副本，保留原查询契约。 */
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd);

/* 原子复制定位及来源时间；过期则清除副本有效性，查询不刷新年龄。 */
/* 原子复制定位及来源时间；过期则清除副本有效性，查询不刷新年龄。 */
uint8_t GnssService_GetSnapshot(EcallMsd_t *pstMsd, uint32_t *pu32SampleMs);

#endif
