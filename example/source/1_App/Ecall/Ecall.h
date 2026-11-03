/*******************************************************
  * Name    :Ecall.h
  * Author  :xiaojunzhong
  * Function:MCU 紧急事件仲裁和数据交付接口，不管理模组呼叫。
  * Version :V1.2.0
  * Data    :2026.11.03
*******************************************************/
#ifndef __ECALL_H__
#define __ECALL_H__
#include <stdint.h>
#include "RteSys.h"

/* 仅描述 MCU 的处理阶段；模组通话、重拨及回呼状态不在此维护。 */
typedef enum
{
    ECALL_STATE_STANDBY = 0U,          /* 待机：允许建立新的事件。 */
    ECALL_STATE_MANUAL_CANCEL_WINDOW, /* 手动触发后的十秒取消窗口。 */
    ECALL_STATE_DATA_PENDING,         /* 等待自定义协议取数和报告交付结果。 */
    ECALL_STATE_COMPLETE,             /* 仅表示数据已交付，下一周期回待机。 */
    ECALL_STATE_FAILED                /* 数据被明确拒绝，下一周期回待机。 */
} EcallState_t;
typedef enum
{
    ECALL_TRIGGER_MANUAL = 0U, /* SOS 按键；由 AppTask 长按判定。 */
    ECALL_TRIGGER_AUTO,        /* 本机碰撞算法；当前尚无真实生产者。 */
    ECALL_TRIGGER_CAN          /* CAN 服务确认的碰撞上升沿。 */
} EcallTrigger_t;
#define ECALL_TRIGGER_MASK_MANUAL     0x01U
#define ECALL_TRIGGER_MASK_AUTO       0x02U
#define ECALL_TRIGGER_MASK_CAN        0x04U
#define ECALL_TRIGGER_MASK_ALL        0x07U
#define ECALL_MANUAL_CANCEL_WINDOW_MS 10000U
#define ECALL_GNSS_HISTORY_COUNT       9U

/* 定位服务现有值复制容器，不是 MSD 编码报文。无源时各有效标志为零。 */
typedef struct
{
    uint8_t trigger_mask;       /* 原接口兼容字段；触发由应用独立仲裁。 */
    uint8_t gnss_valid;         /* 当前定位是否有效。 */
    int32_t latitude_e7;        /* 纬度，度乘一千万。 */
    int32_t longitude_e7;       /* 经度，度乘一千万。 */
    uint32_t utc_seconds;      /* 定位样本 UTC 秒。 */
    int32_t history_latitude_e7[ECALL_GNSS_HISTORY_COUNT];  /* 真实历史纬度。 */
    int32_t history_longitude_e7[ECALL_GNSS_HISTORY_COUNT]; /* 真实历史经度。 */
    uint32_t history_utc_seconds[ECALL_GNSS_HISTORY_COUNT]; /* 历史采样时刻。 */
    uint8_t history_count;      /* 实际有效点数，不用零值补足。 */
    uint8_t delta_v_longitudinal_raw; /* 历史容器字段，无实际来源时无效。 */
    uint8_t delta_v_lateral_raw;      /* 横向冲击原始值，不表示物理单位。 */
    uint8_t occupants;          /* 历史容器字段；当前人数由 RTE 载重估算。 */
    uint8_t rollover;           /* 翻滚原始值，不代替来源有效性。 */
} EcallMsd_t;
typedef struct
{
    uint32_t u32SessionId;      /* 非零事件号，用于拒绝迟到交付结果。 */
    EcallState_t enState;       /* 当前 MCU 业务阶段。 */
    uint16_t u16FailureReason;  /* 零无错误，一为协议明确拒绝。 */
    uint8_t u8SessionActive;    /* 有待处理事件时保持电源锁。 */
    uint8_t u8TriggerMask;      /* 当前事件合并的来源位。 */
} EcallStatus_t;
typedef struct
{
    uint32_t u32SessionId;      /* 由消费者原样回传的事件号。 */
    uint8_t u8TriggerMask;      /* 自动/CAN 优先于手动取消。 */
    RteSysMsdSnapshot_t stSnapshot; /* 首次触发冻结的数据及逐项有效性。 */
} EcallRequest_t;

/* 任务启动前初始化；其余业务修改只由 EcallTask 执行。 */
void Ecall_Init(void);
void Ecall_Process(void);
/* 提交单一路来源或合并位；仅 EcallTask 调用，不允许 ISR 调用。 */
void Ecall_OnTrigger(EcallTrigger_t enTrigger);
void Ecall_OnTriggerMask(uint8_t u8TriggerMask);
/* 仅手动取消窗口接受取消；交付开始后取消由未来协议另行定义。 */
void Ecall_OnManualCancel(void);
/* 未来协议服务按值读待交付事件。返回一只表示取得副本，不表示模组收到。 */
uint8_t Ecall_ReadPendingRequest(EcallRequest_t *pstRequest);
/* 未来协议服务按事件号回报实际交付结果；成功不表示呼叫或 MSD 发送成功。
 * 尚未接入 CAN/UART 协议，因此当前没有生产调用方；默认保持待交付。 */
uint8_t Ecall_ReportDelivery(uint32_t u32SessionId, uint8_t u8Success);
/* 跨任务只读接口；不返回内部指针，不清除事件。 */
uint8_t Ecall_ReadStatus(EcallStatus_t *pstStatus);
EcallState_t Ecall_GetState(void);
uint8_t Ecall_IsSessionActive(void);
#endif
