/*******************************************************
  * Name    :Gpt_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _GPT_CFG_H_
#define _GPT_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Wdg.h"
#include "RteSys.h"
#include "hc32_ll.h"
/*-------------function port--------------------------*/

/*-------------Wdg port-------------------------------*/
#define Gpt_WdgFeed()      Wdg_Feed()
/*-------------RteSys port----------------------------*/
#define Gpt_GetSysTimeMs() RteSys_GetSysTimeMs()
/*-------------Task port------------------------------*/
#define Gpt_1msSchdule()   Task_1msSchdule()
#endif

