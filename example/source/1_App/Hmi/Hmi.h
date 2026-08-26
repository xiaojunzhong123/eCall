/*******************************************************************************
 * @file  Hmi.h
 * @brief Sample display interface for the TBOX application.
 ******************************************************************************/
#ifndef __HMI_H__
#define __HMI_H__

#include "Ecall.h"

void Hmi_Init(void);
void Hmi_Log(const char *pcText);
void Hmi_Process(void);
void Hmi_ShowTaskStarted(const char *pcTaskName);
void Hmi_ShowEcallState(EcallState_t enState);
void Hmi_ShowText(const char *pcText);

#endif
