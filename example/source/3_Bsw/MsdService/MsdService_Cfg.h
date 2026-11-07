/*******************************************************
  * Name    :MsdService_Cfg.h
  * Author  :xiaojunzhong
  * Function:BSW层MSD串口配置；用户确认的占位值，不代表真实车辆观测。
  * Version :V1.0.0
  * Data    :2026.11.06
*******************************************************/
#ifndef MSD_SERVICE_CFG_H
#define MSD_SERVICE_CFG_H

/* MSD版本为CS的二；串口封装版本仍为一。按用户确认的Excel数值发送。 */
#define MSD_SERVICE_MSD_VERSION             2U
#define MSD_SERVICE_VALID_MASK              0xFFFFU
#define MSD_SERVICE_TEST_CALL               0U
/* 位置由模组提供；四字节字段发送FF FF 00 00，而不是全FF。 */
#define MSD_SERVICE_POSITION_PLACEHOLDER    0x0000FFFFUL
#define MSD_SERVICE_DIRECTION_PLACEHOLDER   0xFFU
#define MSD_SERVICE_DELTA_PLACEHOLDER       0xFFFFU
/* 以下是协议占位值；翻滚十五超出原头文件零/一范围，按用户明确确认保留。 */
#define MSD_SERVICE_IMPACT_PLACEHOLDER      6U
#define MSD_SERVICE_ROLLOVER_PLACEHOLDER    0x0FU
#define MSD_SERVICE_RANGE_PLACEHOLDER       0xFFU
#define MSD_SERVICE_DELTA_V_PLACEHOLDER     0x00FFU

#endif
