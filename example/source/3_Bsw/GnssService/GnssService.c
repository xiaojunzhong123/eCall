/*******************************************************************************
 * @file  GnssService.c
 * @brief GNSS snapshot service P1 mock implementation.
 ******************************************************************************/
#include "GnssService.h"

static EcallMsd_t m_stGnssMsd;

void GnssService_Init(void)
{
    m_stGnssMsd.trigger_mask = 0U;
    m_stGnssMsd.gnss_valid = 1U;
    m_stGnssMsd.latitude_e7 = 311230000;
    m_stGnssMsd.longitude_e7 = 1214737000;
    m_stGnssMsd.utc_seconds = 0U;
}

void GnssService_Process(void)
{
    m_stGnssMsd.utc_seconds++;
}

uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd)
{
    if (pstMsd == 0)
    {
        return 0U;
    }

    *pstMsd = m_stGnssMsd;
    return m_stGnssMsd.gnss_valid;
}
