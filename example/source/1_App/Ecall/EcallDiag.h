/*******************************************************
  * Name    :EcallDiag.h
  * Author  :xiaojunzhong
  * Function:上电诊断和持续监测，来源观测、故障确认及恢复分别处理。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef ECALL_DIAG_H
#define ECALL_DIAG_H
#include "RteSys_Diag.h"
typedef enum
{
    ECALL_DIAG_CHECKING = 0U, /* 上电检查仍在进行。 */
    ECALL_DIAG_FAULT, /* 至少一项已确认活动故障。 */
    ECALL_DIAG_INCOMPLETE, /* 检查存在能力或证据缺口。 */
    ECALL_DIAG_HEALTHY /* 所有必需项具有完整且新鲜的合格结论。 */
} EcallDiagConclusion_t;
typedef struct
{
    RteSysDiagState_t enState; /* 当前稳定业务状态。 */
    RteSysDiagState_t enObservedState; /* 来源直接报告的状态，与确认后结论分开。 */
    uint32_t u32ObservedMs; /* 来源真实观测时刻，毫秒。 */
    uint32_t u32SourceEpoch; /* 来源重启代次，变化时原令牌失效。 */
    uint16_t u16Reason; /* 当前判定的原因编号。 */
    uint16_t u16ActiveFaultReason; /* 未恢复活动故障的首次或当前原因。 */
    uint16_t u16CommonCause; /* 共同来源故障标识，避免推断多个硬件同时损坏。 */
    uint8_t u8ActiveFault; /* 已经确认且尚未恢复的活动故障标志。 */
} EcallDiagItemStatus_t;
typedef struct
{
    uint32_t u32Generation; /* 本次系统上电诊断代次。 */
    uint32_t u32Revision; /* 发布版本；只计发布，不是时间或 MSD 编号。 */
    uint32_t u32ReminderRevision; /* 再次点火提示版本，用于重申现有警告。 */
    EcallDiagConclusion_t enConclusion; /* 故障、检查中、未完成或健康的聚合结论。 */
    uint16_t u16ActiveFaultMask; /* 十项中已确认故障的位集合。 */
    uint16_t u16IncompleteMask; /* 无法完成或证据不足的检查位集合。 */
    uint16_t u16CheckingMask; /* 仍在准备或检测中的检查位集合。 */
    uint16_t u16UnsupportedMask; /* 当前没有检测能力的检查位集合。 */
    uint16_t u16NotApplicableMask; /* 已按政策批准为不适用的检查位集合。 */
    uint8_t u8StartupComplete; /* 本次上电检查是否已离开准备阶段，不等于健康。 */
    uint8_t u8PolicyConfirmed; /* 诊断阈值和适用政策是否已经批准。 */
    uint8_t u8EmergencyPriority; /* 有 MCU 紧急事件时让出冲突检查资源。 */
    EcallDiagItemStatus_t astItems[RTESYS_DIAG_COUNT]; /* 按十类诊断索引保存的单项记录。 */
} EcallDiagStatus_t;
/* 仅系统启动前初始化；服务复位不能调用本接口清除活动故障。 */
void Ecall_DiagInit(void);
/* AppTask 有界周期执行，不发 AT、不控制硬件、不阻止紧急会话。 */
void Ecall_DiagProcess(void);
/* 其它任务可按值查询全部原因；读操作不会清除故障或刷新样本。 */
uint8_t Ecall_DiagReadStatus(EcallDiagStatus_t *pstStatus);
#endif
