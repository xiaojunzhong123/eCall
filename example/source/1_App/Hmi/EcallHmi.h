/*******************************************************
  * Name    :EcallHmi.h
  * Author  :xiaojunzhong
  * Function:自检与 MCU 事件提示仲裁，不接管模组通话状态。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef ECALL_HMI_H
#define ECALL_HMI_H
#include "OutputService_Ecall.h"
typedef struct
{
    uint32_t u32RequestId; /* 非零请求号，用于过滤迟到输出确认。 */
    OutputServiceEcallDelivery_t enDelivery; /* 不可用、等待、已交付或失败的输出结果。 */
    uint8_t u8PersistentWarning; /* 是否需要持续保持乘员警告。 */
    uint8_t u8LanguageComplete; /* 英语和已评审阿拉伯语资源是否齐备。 */
    uint8_t u8DeliveryTimedOut; /* 实际输出确认是否超过候选看护期限。 */
} EcallHmiStatus_t;
/* 在任务创建前清零提示仲裁；不绑定实际显示硬件。 */
void Ecall_HmiInit(void);
/* AppTask 周期执行，唯一发布诊断与通话的组合提示意图。 */
void Ecall_HmiProcess(void);
/* 按值读取交付及双语资源缺口，不能据此清除诊断故障。 */
uint8_t Ecall_HmiReadStatus(EcallHmiStatus_t *pstStatus);
/* 语言 0 英语、1 阿拉伯语；未评审/缺资源返回 NULL，不伪称双语完成。 */
const char *Ecall_HmiGetText(OutputServiceEcallMessage_t enMessage, uint8_t u8Language);
#endif
