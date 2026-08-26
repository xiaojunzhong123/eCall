/*******************************************************
  * Name    :Task_Cfg.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _TASK_CFG_H_
#define _TASK_CFG_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "RteSys.h"
/*-------------RteSys port----------------------------*/
#define Task_SetSysTimeMs(time) RteSys_SetSysTimeMs(time)
/*-------------function port--------------------------*/
extern void Task_InitSchduleTable(void);
extern void Task_1msSchduleTable(void);
extern void Task_2msSchduleTable(void);
extern void Task_5msSchduleTable(void);
extern void Task_10msSchduleTable(void);
extern void Task_20msSchduleTable(void);
extern void Task_IdleSchduleTable(void);
#endif

