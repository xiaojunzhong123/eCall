/*******************************************************
  * Name    :CanService.h
  * Author  :xiaojunzhong
  * Function:CAN碰撞接收、车辆数据发布及总线错误恢复。
  * Version :V1.0.0
  * Data    :2026.08.29
*******************************************************/
#ifndef __CAN_SERVICE_H__
#define __CAN_SERVICE_H__

#include <stdint.h>

typedef union
{
    uint8_t U[8]; /* 报文原始字节视图。 */
    struct
    {
        uint8_t u8ReservedByte0; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8ReservedByte1; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8CrashOutputStatus : 1; /* 报文碰撞输出位，只有上升沿触发。 */
        uint8_t u8ReservedByte2 : 7; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8ReservedByte3; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8ReservedByte4; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8ReservedByte5; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8RollingCounter : 4; /* 原帧四位循环计数，尚未进行完整性验证。 */
        uint8_t u8ReservedByte6 : 4; /* 矩阵保留位，沿用原编码初值，不解释为业务数据。 */
        uint8_t u8Checksum; /* 原报文校验字节观测。 */
    } B;
} CanServiceSrsInfoFrame_t;



/* 初始化 CAN 服务私有缓存和碰撞重新使能标志，启动已有 CAN2 配置。 */
/* 初始化 CAN 服务私有缓存和碰撞重新使能标志，启动已有 CAN2 配置。 */
void CanService_Init(void);
/* 按接收和错误恢复顺序处理，不采集或发送模组定位时间。 */
/* 按接收和错误恢复顺序处理，不采集或发送模组定位时间。 */
void CanService_Process(void);
/* 原子取走一次碰撞锁存位；由 CanTask 转交 EcallTask。 */
/* 原子取走一次碰撞锁存位；由 CanTask 转交 EcallTask。 */
uint8_t CanService_TakeEcallTrigger(void);

#endif
