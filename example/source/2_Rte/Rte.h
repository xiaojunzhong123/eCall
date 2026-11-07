/*******************************************************
  * Name    :Rte.h
  * Author  :xiaojunzhong
  * Function:RTE层传统DTC编号及当前故障状态接口声明。
  * Version :V1.1.0
  * Data    :2026.11.04
*******************************************************/
#ifndef _RTE_H_
#define _RTE_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include "Gpio.h"
#include "Gpio_Cfg.h"
/*-------------function port--------------------------*/
/* 保留既有包含关系，避免改变调用方对GPIO类型和配置的可见性。
 * 旧车身端口、驱动和ADC宏无现有调用，已移除；车辆数据使用RteSys接口。
 */
/* 传统诊断索引；数值与DTC配置表及Rte.c状态数组保持一致，不重新编号。 */
typedef enum
{
    DTC_ID_HIGH_VOLTAGE = 0x00, /* 蓄电池过压故障索引。 */
    DTC_ID_LOW_VOLTAGE,        /* 蓄电池欠压故障索引。 */
    DTC_ID_BUS_OFF,            /* CAN总线关闭故障索引。 */
    DTC_ID_BCM_LOST,           /* 保留既有车身控制器失联索引。 */
    DTC_ID_MAX_NUM             /* 索引数量及状态数组边界，不是具体故障。 */
} Rte_Dtc_Type;

/* 读取有效索引的当前故障标志；由诊断模块调用，不提供越界保护。 */
extern Bool Rte_GetDtcCurErrFlag(Rte_Dtc_Type id);
/* 发布有效索引的当前故障标志；沿用既有所有权，不新增ISR或并发保证。 */
extern void Rte_SetDtcCurErrFlag(Rte_Dtc_Type id, Bool flag);

#endif
