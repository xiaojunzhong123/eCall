/*******************************************************************************
 * @file  GnssService.h
 * @brief GNSS snapshot service interface.
 ******************************************************************************/
#ifndef __GNSS_SERVICE_H__
#define __GNSS_SERVICE_H__

#include "Ecall.h"

void GnssService_Init(void);
void GnssService_Process(void);
uint8_t GnssService_GetLatest(EcallMsd_t *pstMsd);

#endif
