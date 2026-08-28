/*******************************************************
  * Name    :ModemCall.h
  * Author  :LoH
  * Function:Private modem call state processing interface.
  * Version :V1.0.0
  * Data    :2026.08.28
*******************************************************/
#ifndef __MODEM_CALL_H__
#define __MODEM_CALL_H__

#include "ModemService.h"

void ModemCall_Init(void);
void ModemCall_Reset(void);
void ModemCall_Start(ModemCallState_t *penCallState);
void ModemCall_Process(ModemCallState_t *penCallState);

#endif
