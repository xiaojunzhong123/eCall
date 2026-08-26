/*******************************************************
  * Name    :GnssService.c
  * Author  :LoH
  * Function:GNSS snapshot service P1 mock implementation.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#include "GnssService.h"

static EcallMsd_t m_stGnssMsd;

/*******************************************************
  * Name    :GnssService_Init
  * Author  :LoH
  * Function:Initialize mock GNSS location and UTC data.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void GnssService_Init(void)
{
    m_stGnssMsd.trigger_mask = 0U;
    m_stGnssMsd.gnss_valid = 1U;
    m_stGnssMsd.latitude_e7 = 311230000;
    m_stGnssMsd.longitude_e7 = 1214737000;
    m_stGnssMsd.utc_seconds = 0U;
}

/*******************************************************
  * Name    :GnssService_Process
  * Author  :LoH
  * Function:Update mock UTC data for each task cycle.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
void GnssService_Process(void)
{
    m_stGnssMsd.utc_seconds++;
}

/*******************************************************
  * Name    :GnssService_GetLatest
  * Author  :LoH
  * Function:Return latest GNSS snapshot and validity flag.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd)
{
    if (pstMsd == 0)
    {
        return 0U;
    }

    *pstMsd = m_stGnssMsd;
    return m_stGnssMsd.gnss_valid;
}
