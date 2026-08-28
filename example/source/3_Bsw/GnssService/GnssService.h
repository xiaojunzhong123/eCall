/*******************************************************
  * Name    :GnssService.h
  * Author  :LoH
  * Function:GNSS snapshot service interface.
  * Version :V1.0.0
  * Data    :2026.08.26
*******************************************************/
#ifndef __GNSS_SERVICE_H__
#define __GNSS_SERVICE_H__

#include "Ecall.h"

/* Initialize GNSS snapshot service. */
void GnssService_Init(void);
/* Consume one modem GNSS sample and update current/history cache. */
void GnssService_Process(void);
/* Copy the latest GNSS snapshot for an eCall MSD. */
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd);

#endif
