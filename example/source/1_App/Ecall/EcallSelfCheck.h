/*******************************************************
  * Name    :EcallSelfCheck.h
  * Author  :xiaojunzhong
  * Function:APP 层精简自检状态与结果上报接口，不包含硬件访问。
  * Version :V1.0.0
  * Data    :2026.11.05
*******************************************************/
#ifndef ECALL_SELF_CHECK_H
#define ECALL_SELF_CHECK_H

#include <stdint.h>

#define ECALL_SELF_CHECK_ALL_MASK 0x03FFU

/* 十项检查顺序同时定义待接入位和故障位，低十位有效。 */
typedef enum
{
    ECALL_SELF_CHECK_ECU = 0U,       /* ECU、处理器、存储及逻辑初始化。 */
    ECALL_SELF_CHECK_MOBILE_ANTENNA, /* 移动网络外部天线连接。 */
    ECALL_SELF_CHECK_MODEM,          /* 模组健康与响应，不由 MCU 发起 AT。 */
    ECALL_SELF_CHECK_GNSS_ANTENNA,   /* 模组提供的定位天线诊断。 */
    ECALL_SELF_CHECK_GNSS_RECEIVER,  /* 模组提供的定位接收机诊断。 */
    ECALL_SELF_CHECK_CRASH_ECU,      /* 碰撞控制单元连接，碰撞事件不是故障。 */
    ECALL_SELF_CHECK_COMMUNICATION,  /* 已确认范围内的组件及总线连接。 */
    ECALL_SELF_CHECK_SIM,            /* 可拆卸 SIM 是否存在。 */
    ECALL_SELF_CHECK_POWER_CONNECTION, /* 产品定义的电源路径连接。 */
    ECALL_SELF_CHECK_POWER_ENERGY,   /* 按批准阈值判定的电量充足状态。 */
    ECALL_SELF_CHECK_COUNT
} EcallSelfCheckItem_t;

/* 本次上电稳定结果；布尔字段只取 TRUE/FALSE，默认 TRUE 不等于真实检查通过。
 * AppTask 单写，其他任务经 ReadStatus 在短临界区复制，禁止直接写实例。
 */
typedef struct
{
    uint8_t u8EcuNormal;             /* ECU 正常；完整检查来源待接入。 */
    uint8_t u8MobileAntennaNormal;   /* 移动天线连接正常；检测来源待接入。 */
    uint8_t u8ModemNormal;           /* 模组正常；真实状态协议待接入。 */
    uint8_t u8GnssAntennaNormal;     /* 定位天线正常；模组结果待接入。 */
    uint8_t u8GnssReceiverNormal;    /* 定位接收机正常；模组结果待接入。 */
    uint8_t u8CrashEcuNormal;        /* 碰撞控制单元连接正常；完整规则待接入。 */
    uint8_t u8CommunicationNormal;   /* 组件通信正常；必需链路清单待接入。 */
    uint8_t u8SimPresent;            /* SIM 存在；模组卡检测结果待接入。 */
    uint8_t u8PowerConnectionNormal; /* 电源连接正常；真实路径检测待接入。 */
    uint8_t u8PowerEnergySufficient; /* 电量充足；采样、阈值及回差待接入。 */
    uint16_t u16PendingCheckMask;    /* 低十位置一表示尚未收到该项真实检查结果。 */
    uint16_t u16FaultMask;           /* 低十位置一表示该项为 FALSE，任一位控制故障灯。 */
} EcallSelfCheckStatus_t;

/* Ecall_Init 在输出服务初始化后、任务创建前调用一次；十项显式置 TRUE。 */
void Ecall_SelfCheckInit(void);
/* AppTask 周期调用，先消费结果再汇总 D29 请求，不阻塞、不直接操作 GPIO。 */
void Ecall_SelfCheckProcess(void);
/* 任务上下文上报已确认结果。u8Normal 只接受 TRUE/FALSE；无结果时不要调用。
 * 返回一仅表示进入邮箱；同项同批次故障优先，恢复需在后续批次单独报告 TRUE。
 * 检测未接入时保留默认 TRUE 和待接入位，不用该接口伪造健康。
 */
uint8_t Ecall_SelfCheckReportResult(EcallSelfCheckItem_t enItem, uint8_t u8Normal);
/* 任务上下文按值复制状态；未初始化或空指针返回零，查询不清故障。 */
uint8_t Ecall_SelfCheckReadStatus(EcallSelfCheckStatus_t *pstStatus);

#endif
