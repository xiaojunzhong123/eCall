/*******************************************************
  * Name    :CanManage.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _CANMANAGE_H_
#define _CANMANAGE_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

/*-------------function port--------------------------*/
extern void CanManage_Main(void);
extern void CanManage_TimerCtrl(void);
extern void CanManageTask_Create(void);
#endif

