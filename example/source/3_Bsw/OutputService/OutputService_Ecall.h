/*******************************************************
  * Name    :OutputService_Ecall.h
  * Author  :xiaojunzhong
  * Function:逻辑输出到物理引脚的映射，及可确认的乘员提示接口。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef OUTPUT_SERVICE_ECALL_H
#define OUTPUT_SERVICE_ECALL_H
#include <stdint.h>
typedef enum
{
    OUTPUT_ECALL_MESSAGE_NONE = 0U, /* 不需要诊断文字提示。 */
    OUTPUT_ECALL_MESSAGE_CHECKING, /* 正在上电检查。 */
    OUTPUT_ECALL_MESSAGE_INCOMPLETE, /* 自检尚未完整完成。 */
    OUTPUT_ECALL_MESSAGE_FAULT /* 已确认故障提示。 */
} OutputServiceEcallMessage_t;
typedef enum
{
    OUTPUT_ECALL_UNAVAILABLE = 0U, /* 实际输出通道不可用。 */
    OUTPUT_ECALL_PENDING, /* 请求已保存，等待实际输出确认。 */
    OUTPUT_ECALL_DELIVERED, /* 实际显示服务已确认完成。 */
    OUTPUT_ECALL_FAILED /* 实际显示服务报告失败。 */
} OutputServiceEcallDelivery_t;
typedef struct
{
    uint32_t u32RequestId; /* 非零请求号，用于过滤迟到输出确认。 */
    uint32_t u32Generation; /* 本次系统上电诊断代次。 */
    uint32_t u32ReminderRevision; /* 再次点火提示版本，用于重申现有警告。 */
    uint16_t u16ActiveFaultMask; /* 十项中已确认故障的位集合。 */
    uint16_t u16IncompleteMask; /* 无法完成或证据不足的检查位集合。 */
    OutputServiceEcallMessage_t enDiagnosticMessage; /* 检查中、未完成或故障提示类别。 */
    uint8_t u8EcallState; /* 仅表示 MCU 事件阶段，不表示模组正在通话。 */
    uint8_t u8PersistentWarning; /* 是否需要持续保持乘员警告。 */
    uint8_t u8EnglishAvailable; /* 英语文案是否存在。 */
    uint8_t u8ArabicAvailable; /* 已评审的阿拉伯语文案是否存在。 */
} OutputServiceEcallRequest_t;
typedef struct
{
    uint32_t u32RequestId; /* 非零请求号，用于过滤迟到输出确认。 */
    OutputServiceEcallDelivery_t enDelivery; /* 不可用、等待、已交付或失败的输出结果。 */
    uint8_t u8ChannelAvailable; /* 真实乘员输出通道是否可用。 */
} OutputServiceEcallStatus_t;
/* 任务创建前初始化；不操作两路现有灯，默认没有实际乘员显示通道。 */
void OutputService_InitEcallIndication(void);
/* AppTask 单写提示意图；返回仅表示已保存，不能当作乘员已看到。 */
uint8_t OutputService_PublishEcallIndication(const OutputServiceEcallRequest_t *pstRequest, uint32_t *pu32RequestId);
/* 未来实际显示服务按值读最新请求，不消费、不获取 APP 内部指针。 */
uint8_t OutputService_ReadEcallIndication(OutputServiceEcallRequest_t *pstRequest);
/* 查询实际通道与交付状态；不能由日志成功反推交付成功。 */
uint8_t OutputService_ReadEcallIndicationStatus(OutputServiceEcallStatus_t *pstStatus);
/* 仅由未来已确认的显示服务报告通道能力，不能由 APP 自动置为可用。 */
void OutputService_SetEcallIndicationAvailable(uint8_t u8Available);
/* 实际输出完成后按请求号确认；迟到、旧请求和不可用通道确认被拒绝。 */
uint8_t OutputService_ReportEcallIndication(uint32_t u32RequestId, uint8_t u8Success);
#endif
