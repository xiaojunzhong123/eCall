/*******************************************************
  * Name    :MsdService.h
  * Author  :xiaojunzhong
  * Function:BSW层车辆数据串口封装；APP只使用业务接口，不直接访问UART。
  * Version :V1.0.0
  * Data    :2026.11.06
*******************************************************/
#ifndef MSD_SERVICE_H
#define MSD_SERVICE_H
#include <stdint.h>
#include "RteSys.h"

/* 后续ACK适配输出的业务结果；由EcallTask读取值副本，无共享可写实例。
 * 解析方须先校验原始ACK及序号，再映射完整事件号；忙或待重试不生成终态结果。 */
typedef struct
{
    uint32_t u32SessionId; /* 非零MCU事件号，不是截断后的串口序号。 */
    uint8_t u8Success;     /* 一为确认交付，零为最终拒绝；非终态不返回记录。 */
} MsdServiceAck_t;

/* 启动阶段清空静态运行态并将下一个帧序号置一，不重新初始化UART。 */
void MsdService_Init(void);
/* EcallTask发送冻结数据；来源位沿用手动一、自动二、CAN四。返回一仅表示驱动返回成功。 */
uint8_t MsdService_Send(const RteSysVehicleSnapshot_t *pstVehicle, uint8_t u8TriggerMask, uint32_t u32SessionId);
/* 非阻塞ACK预留口，当前始终返回零，不消费UART字节，也不修改输出。 */
uint8_t MsdService_ReadAck(MsdServiceAck_t *pstAck);

#endif
