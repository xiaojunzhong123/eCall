/*******************************************************
  * Name    :RteSys_Diag.h
  * Author  :xiaojunzhong
  * Function:共享信号缓存和值复制契约；生产者更新，使用者获取独立副本。
  * Version :V1.0.0
  * Data    :2026.11.01
*******************************************************/
#ifndef RTESYS_DIAG_H
#define RTESYS_DIAG_H
#include <stdint.h>
#define RTESYS_DIAG_COUNT 10U
#define RTESYS_DIAG_ALL_MASK 0x03FFU
typedef enum
{
    RTE_DIAG_ECU = 0U, /* 控制器完整自检。 */
    RTE_DIAG_CELLULAR_ANTENNA, /* 移动通信外部天线。 */
    RTE_DIAG_MODEM, /* 移动通信设备自身健康。 */
    RTE_DIAG_GNSS_ANTENNA, /* 定位天线连接。 */
    RTE_DIAG_GNSS_RECEIVER, /* 定位接收机健康。 */
    RTE_DIAG_CRASH_ECU, /* 碰撞控制器完整健康。 */
    RTE_DIAG_COMPONENT_LINKS, /* 必需组件间通信链路。 */
    RTE_DIAG_REMOVABLE_SIM, /* 可拆卸 SIM 存在及状态。 */
    RTE_DIAG_POWER_CONNECTION, /* 主备用电源连接。 */
    RTE_DIAG_POWER_ENERGY /* 可用电源电量和供电能力。 */
} RteSysDiagItem_t;
typedef enum
{
    RTE_DIAG_NOT_RUN = 0U, /* 尚未开始检查。 */
    RTE_DIAG_RUNNING, /* 正在检查或等待准备。 */
    RTE_DIAG_PASS, /* 来源完整检查通过。 */
    RTE_DIAG_FAIL, /* 来源报告失败，APP 仍需确认。 */
    RTE_DIAG_INDETERMINATE, /* 证据不足、过期或暂时无法判定。 */
    RTE_DIAG_UNSUPPORTED, /* 当前没有完整检测能力。 */
    RTE_DIAG_NOT_APPLICABLE /* 经批准才可判为不适用。 */
} RteSysDiagState_t;
typedef enum
{
    RTE_DIAG_SOURCE_SYSTEM = 1U,
    RTE_DIAG_SOURCE_MODEM,
    RTE_DIAG_SOURCE_GNSS,
    RTE_DIAG_SOURCE_CAN,
    RTE_DIAG_SOURCE_POWER
} RteSysDiagSource_t;
typedef enum
{
    RTE_DIAG_REASON_NONE = 0U,
    RTE_DIAG_REASON_CAPABILITY,
    RTE_DIAG_REASON_PARTIAL_COVERAGE,
    RTE_DIAG_REASON_MODEM_UNAVAILABLE,
    RTE_DIAG_REASON_STALE,
    RTE_DIAG_REASON_PREPARATION_TIMEOUT,
    RTE_DIAG_REASON_POLICY_UNCONFIRMED,
    RTE_DIAG_REASON_CONFIRMING,
    RTE_DIAG_REASON_RECOVERING,
    RTE_DIAG_REASON_SOURCE_RESET,
    RTE_DIAG_REASON_INVALID_APPLICABILITY
} RteSysDiagReason_t;
typedef struct
{
    uint32_t u32Generation; /* 本次系统上电诊断代次。 */
    uint32_t u32SourceEpoch; /* 来源重启代次，变化时原令牌失效。 */
} RteSysDiagToken_t;
typedef struct
{
    RteSysDiagToken_t stToken; /* 诊断代次和来源重启代次令牌。 */
    uint32_t u32Sequence; /* 来源新样本的单调发布序号。 */
    uint32_t u32ObservedMs; /* 来源真实观测时刻，毫秒。 */
    RteSysDiagState_t enState; /* 当前稳定业务状态。 */
    RteSysDiagSource_t enSource; /* 固定责任映射中的诊断来源。 */
    uint16_t u16Reason; /* 当前判定的原因编号。 */
    uint16_t u16CommonCause; /* 共同来源故障标识，避免推断多个硬件同时损坏。 */
    uint16_t u16ApplicabilityBasis; /* 不适用结论的批准依据编号。 */
    uint8_t u8CoverageComplete; /* 是否覆盖该项完整检查，局部响应不能置一。 */
} RteSysDiagObservation_t;
/* 仅在任务创建前初始化；新启动不保留 RAM 历史结论。 */
void RteSys_InitDiagnostics(void);
/* AppTask 在真正开始新上电诊断时创建代次，不能用于普通清故障。 */
uint32_t RteSys_DiagBeginCycle(void);
/* 服务发起异步检查前捕获令牌，返回结果时不得重新捕获以冒充新结果。 */
uint8_t RteSys_DiagReadToken(RteSysDiagItem_t enItem, RteSysDiagToken_t *pstToken);
/* 每项由所属服务单写，任务上下文发布；旧令牌、非单调样本被拒绝。 */
uint8_t RteSys_DiagPublish(RteSysDiagItem_t enItem, const RteSysDiagObservation_t *pstObservation);
/* 来源局部重启时使相关令牌失效，不直接清除 APP 活动故障。 */
void RteSys_DiagInvalidateSource(RteSysDiagSource_t enSource);
/* 按值读一个小型记录，读操作不会刷新采样年龄或消费事件。 */
uint8_t RteSys_DiagReadItem(RteSysDiagItem_t enItem, RteSysDiagObservation_t *pstObservation);
/* 查询固定责任映射，不能以其他来源覆盖同一诊断项。 */
RteSysDiagSource_t RteSys_DiagOwner(RteSysDiagItem_t enItem);
#endif
