/*******************************************************
  * Name    :RteSys.h
  * Author  :xiaojunzhong
  * Function:共享信号缓存和值复制契约；生产者更新，使用者获取独立副本。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _RTESYS_H_
#define _RTESYS_H_
/*-------------include file---------------------------*/
#include "Types.h"
#include <stdint.h>
typedef enum
{
    RTE_FUNIND_LearnRkeReq = 0x00,
    RTE_FUNIND_RkeCMD,
    RTE_FUNIND_RkeNumber,
    RTE_FUNIND_CANWakeup,
    RTE_FUNIND_LtchOverheatProtd,
    RTE_FUNIND_SearchCarSig,
    RTE_FUNIND_IGNoffAllowSwWork,
    RTE_FUNIND_RfIntFlag,
    RTE_FUNIND_RkeLearnSuccess,
    RTE_FUNIND_Quit_KeyLearn,
    RTE_FUNIND_SysVoltMode,
    RTE_FUNIND_CanVoltMode,
    RTE_FUNIND_LockActiveReson,
    RTE_FUNIND_AntifTriggerFlag,
    RTE_FUNIND_BefoeSleepDoorState,
    RTE_FUNIND_NetSleepRequest,
    RTE_FUNIND_NetIsActive,
    RTE_FUNIND_IntoTestMode,
    RTE_FUNIND_RkeLongPress,
    RTE_FUNIND_RfSleepmMode,
    RTE_Fun_ID_MAX_NUM
} Rte_Fun_Type;
/* 读取原通信框架毫秒计数；该计数与内核时钟不同，生产时基仍须核对。 */
/* 读取原通信框架毫秒计数；该计数与内核时钟不同，生产时基仍须核对。 */
extern uint32_t RteSys_GetSysTimeMs(void);
/* 为原通信计数累加传入步长；仅由确定的系统计时入口调用。 */
extern void     RteSys_SetSysTimeMs(uint8_t time);

typedef enum
{
    RTE_SWITCH_INACTIVE = 0U,
    RTE_SWITCH_ACTIVE
} RteSwitchState_t;

typedef enum
{
    RTESYS_SWITCH_SOS = 0U,
    RTESYS_SWITCH_SRS,
    RTESYS_SWITCH_PBATTERY_SRS,
    RTESYS_SWITCH_KL15,
    RTESYS_SWITCH_NUM
} RteSysSwitchId_t;

typedef enum
{
    RTESYS_OUTPUT_INDICATOR_1 = 0U,
    RTESYS_OUTPUT_INDICATOR_2,
    RTESYS_OUTPUT_IG_KEEPON,
    RTESYS_OUTPUT_PWR_EN_CAN,
    RTESYS_OUTPUT_CAT4_EN,
    RTESYS_OUTPUT_CAT4_PWR_KEY,
    RTESYS_OUTPUT_CAT4_RST,
    RTESYS_OUTPUT_PWR_EN_GNSS,
    RTESYS_OUTPUT_PWR_EN_3V3,
    RTESYS_OUTPUT_PWR_EN_PHY,
    RTESYS_OUTPUT_PWR_EN_AUDIO,
    RTESYS_OUTPUT_NUM
} RteSysOutputId_t;
/* 读取原通信框架毫秒计数；该计数与内核时钟不同，生产时基仍须核对。 */
/* 读取原通信框架毫秒计数；该计数与内核时钟不同，生产时基仍须核对。 */
extern uint32_t RteSys_GetSysTimeMs(void);
/* 为原通信计数累加传入步长；仅由确定的系统计时入口调用。 */
extern void     RteSys_SetSysTimeMs(uint8_t time);

/* 读取 CAN 管理层报告的休眠标志，不等于整机已经睡眠。 */
extern Bool RteSys_GetCanSleepFlag(void);
/* 保存 CAN 管理层休眠结论，不直接控制硬件。 */
extern void RteSys_SetCanSleepFlag(Bool flag);

/* 读取本地休眠请求，供 CAN 网络管理查询。 */
extern Bool RteSys_GetLocalSleepFlag(void);
/* 保存本地休眠请求；请求不等于模组或硬件允许睡眠。 */
extern void RteSys_SetLocalSleepFlag(Bool flag);

/* 读取原射频检测状态缓存。 */
extern Bool RteSys_GetRfDet(void);
/* 更新原射频检测状态缓存。 */
extern void RteSys_SetRfDet(Bool flag);

/* 读取诊断故障码控制标志，沿用原通信框架约定。 */
extern Bool RteSys_GetDtcDeactiveFlag(void);
/* 保存诊断故障码控制标志，不执行实际故障检测。 */
extern void RteSys_SetDtcDeactiveFlag(Bool flag);

/* 读取原框架点火缓存，与归一化 KL15 输入分别维护。 */
extern Bool RteSys_GetIgnStatusFlag(void);
/* 更新原框架点火缓存，不直接采集 GPIO。 */
extern void RteSys_SetIgnStatusFlag(Bool flag);

/* 读取原有电压缓存；接口本身没有有效性，不可据此证明电源健康。 */
extern uint16_t RteSys_GetVoltageValue(void);
/* 保存来源提供的电压值；物理单位及采样有效性须由生产者确认。 */
extern void     RteSys_SetVoltageValue(uint16_t value);

/* 读取原遥控命令缓存，保留原接口编码。 */
extern uint8_t RteSys_GetRkeValue(void);   //0.IDLE 1.LOCK 2.Unlock 3.Windows close 4.Windows open 5.search car
/* 更新原遥控命令缓存，不直接驱动执行器。 */
extern void    RteSys_SetRkeValue(uint8_t value);

/* 读取原顶灯占空比缓存。 */
extern uint8_t RteSys_GetTopLampPwmPercent(void);
/* 更新原顶灯占空比缓存，不直接操作 PWM。 */
extern void    RteSys_SetTopLampPwmPercent(uint8_t value);

/* 读取原通信车速缓存，单位沿用已有矩阵接口。 */
extern float RteSys_GetVehiclespeed(void);
/* 更新原通信车速缓存，不改变信号有效性规则。 */
extern void  RteSys_SetVehiclespeed(float value);

/* 读取原通信发动机转速缓存。 */
extern float RteSys_GetEnginSpeedspeed(void);
/* 更新原通信发动机转速缓存。 */
extern void  RteSys_SetEnginSpeedspeed(float value);

/* 按功能索引写入原有状态表；索引必须在枚举范围内。 */
extern void    RTE_write_FunInd(Rte_Fun_Type fun, uint8_t value);
/* 按功能索引读取原有状态表；不执行硬件采样。 */
extern uint8_t RTE_read_FunInd(Rte_Fun_Type fun);

#define RteSys_SetlearnKeyFlag(cmd) RTE_write_FunInd(RTE_FUNIND_LearnRkeReq, cmd)
#define RteSys_GetlearnKeyFlag()    RTE_read_FunInd(RTE_FUNIND_LearnRkeReq)

#define RteSys_SetlearnKeyCnt(cmd) RTE_write_FunInd(RTE_FUNIND_RkeNumber, cmd)
#define RteSys_GetlearnKeyCnt()    RTE_read_FunInd(RTE_FUNIND_RkeNumber)


/* 保存输入服务消抖后的逻辑开关值，屏蔽高低有效电平差异。 */
extern void RteSys_SetSwitchState(RteSysSwitchId_t enSwitch, RteSwitchState_t enState);
/* 读取归一化开关值；非法索引返回未激活。 */
extern RteSwitchState_t RteSys_GetSwitchState(RteSysSwitchId_t enSwitch);
/* 保存业务输出请求；实际 GPIO 由输出服务周期应用。 */
extern void RteSys_SetOutputState(RteSysOutputId_t enOutput, RteSwitchState_t enState);
/* 读取业务输出请求，不代表物理引脚已达到该电平。 */
extern RteSwitchState_t RteSys_GetOutputState(RteSysOutputId_t enOutput);


#define RTESYS_MSD_SRS_MAX_AGE_MS         1000U
#define RTESYS_MSD_GEAR_MAX_AGE_MS        200U
#define RTESYS_MSD_PUBLISH_MAX_AGE_MS     5000U

/* 保留的通用CAN信号字典供现有矩阵读取；定位/日历无MCU采样生产者。
 * 不把这些兼容字段加入对模组的车辆交付数据。
 * Value-copy contracts only. RteSys.c owns all instances.
 * Received data is not CRC/counter verified unless explicitly marked.
 */
typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint8_t u8Received; /* 是否有新鲜接收观测，不表示 CRC 已通过。 */
    uint8_t u8IntegrityVerified; /* CRC/计数器完整性已验证标志，当前不能擅自置一。 */
    uint8_t u8Crash; /* 碰撞观测位，置位本身不表示传感器故障。 */
    uint8_t u8AirbagWarning; /* 气囊系统报告的警告观测。 */
    uint8_t u8Counter; /* 原报文计数器观测，尚不代表校验成功。 */
    uint8_t u8Checksum; /* 原报文校验字节观测。 */
} RteSysMsdSrs_t;

typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint8_t u8Received; /* 是否有新鲜接收观测，不表示 CRC 已通过。 */
    uint8_t u8IntegrityVerified; /* CRC/计数器完整性已验证标志，当前不能擅自置一。 */
    uint8_t u8GearValid; /* 挡位来源及编码是否有效。 */
    uint8_t u8Gear; /* 挡位原代码，按当前矩阵解释。 */
    uint8_t u8Counter; /* 原报文计数器观测，尚不代表校验成功。 */
    uint8_t u8Checksum; /* 原报文校验字节观测。 */
} RteSysMsdGear_t;

typedef struct
{
    int32_t i32LongitudeE7; /* 经度，度乘一千万，有效性另存。 */
    int32_t i32LatitudeE7; /* 纬度，度乘一千万，有效性另存。 */
    uint32_t u32UtcSeconds; /* 来源 UTC 秒，不能用查询时刻替代。 */
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint16_t u16HeadingCdeg; /* 真北航向，百分之一度，正常范围零至三万五千九百九十九。 */
    uint16_t u16SpeedCentiKmh; /* 速度，百分之一公里每小时。 */
    uint8_t u8PositionValid; /* 当前位置是否有效且新鲜。 */
    uint8_t u8HeadingValid; /* 独立航向是否有效且新鲜。 */
    uint8_t u8SpeedValid; /* 速度来源是否有效。 */
} RteSysMsdNavigation_t;

typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint16_t u16Year; /* 完整公历年份。 */
    uint8_t u8Month; /* 月份，一至十二。 */
    uint8_t u8Day; /* 日，一至当月天数。 */
    uint8_t u8Hour; /* 时，零至二十三。 */
    uint8_t u8Minute; /* 分，零至五十九。 */
    uint8_t u8Second; /* 秒，零至五十九。 */
    uint8_t u8Source; /* 来源枚举，零表示无有效来源。 */
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */
    /* Source acquisition tick; u32UpdateMs is the publication/watchdog tick. */
    uint32_t u32SourceSampleMs; /* 来源真实采样时刻，毫秒，周期发布不刷新它。 */
} RteSysMsdUtc_t;


#define RTESYS_MSD_VIN_LENGTH              17U
#define RTESYS_MSD_LOAD_MAX_AGE_MS         5000U
#define RTESYS_MSD_OCCUPANT_UNKNOWN        0xFFU
/* User-confirmed: load is kg, excluding cargo; average 75 kg/person.
 * 0=floor, 1=nearest (half up), 2=ceiling. No minimum occupant is invented.
 */
#define RTESYS_MSD_OCCUPANT_MASS_KG        75U
#define RTESYS_MSD_OCCUPANT_ROUNDING       1U
#define RTESYS_MSD_OCCUPANT_MAX            5U
#define RTESYS_MSD_VEHICLE_TYPE_VALID      0x01U
#define RTESYS_MSD_ENERGY_TYPE_VALID       0x02U

#if ((RTESYS_MSD_OCCUPANT_MASS_KG == 0U) || (RTESYS_MSD_OCCUPANT_MASS_KG > 65535U))
#error Invalid_MSD_occupant_mass
#endif
#if ((RTESYS_MSD_OCCUPANT_MAX == 0U) || (RTESYS_MSD_OCCUPANT_MAX >= RTESYS_MSD_OCCUPANT_UNKNOWN))
#error Invalid_MSD_occupant_limit
#endif
#if (RTESYS_MSD_OCCUPANT_ROUNDING > 2U)
#error Invalid_MSD_occupant_rounding
#endif

typedef struct
{
    uint8_t au8Vin[RTESYS_MSD_VIN_LENGTH]; /* 17字节 VIN*/
    uint8_t u8VinValid; /* VIN 长度和字符资格有效标志。 */
    uint8_t u8VehicleType; /* 车型代码，映射由项目配置确认。 */
    uint8_t u8VehicleTypeValid; /* 车型代码是否已确认可用。 */
    uint8_t u8EnergyType; /* 旧模式能源代码，与组合能源位分别保存。 */
    uint8_t u8EnergyTypeValid; /* 旧能源代码是否有效。 */
} RteSysMsdVehicle_t;

typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint16_t u16LoadKg; /* 人员载重公斤值，不包含货物。 */
    uint8_t u8Received; /* 是否有新鲜接收观测，不表示 CRC 已通过。 */
    uint8_t u8LoadValid; /* 载重有效位和来源年龄是否合格。 */
    uint8_t u8Occupants; /* 估算人数，无效值为二百五十五。 */
    uint8_t u8OccupantsValid; /* 人数估算是否具备有效载重来源。 */
    uint8_t u8OccupantsEstimated; /* 一表示人数来自载重估算而非座位检测。 */
    uint8_t u8OccupantsLimited; /* 估算人数是否经过五人上限限幅。 */
} RteSysMsdLoad_t;

/* Raw Delta-V encoding is source-specific, not a physical unit conversion.
 * Future producers must bind that encoding to the selected MSD schema.
 */
typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint8_t u8DeltaVLongitudinalRaw; /* 纵向速度变化原始编码，物理换算未冻结。 */
    uint8_t u8DeltaVLateralRaw; /* 横向速度变化原始编码，物理换算未冻结。 */
    uint8_t u8Rollover; /* 来源翻滚状态原值。 */
    uint8_t u8DeltaVLongitudinalValid; /* 纵向冲击来源是否有效。 */
    uint8_t u8DeltaVLateralValid; /* 横向冲击来源是否有效。 */
    uint8_t u8RolloverValid; /* 翻滚来源是否有效。 */
} RteSysMsdImpact_t;

typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint8_t u8PositionTrusted; /* 位置可信度，独立于定位有效性。 */
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */
} RteSysMsdPositionQuality_t;


#define RTESYS_MSD_HISTORY_COUNT 9U
typedef struct
{
    int32_t ai32LatitudeE7[RTESYS_MSD_HISTORY_COUNT]; /* 真实历史纬度数组，度乘一千万。 */
    int32_t ai32LongitudeE7[RTESYS_MSD_HISTORY_COUNT]; /* 真实历史经度数组，度乘一千万。 */
    uint32_t au32UtcSeconds[RTESYS_MSD_HISTORY_COUNT]; /* 每个历史点的真实 UTC 秒。 */
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint32_t u32PositionUtcSeconds; /* 当前点 UTC，用于关联历史点组。 */
    uint8_t u8Count; /* 实际有效历史点数，不以默认零值补足。 */
} RteSysMsdHistory_t;

typedef struct
{
    RteSysMsdSrs_t stSrs; /* SRS 完整观测组。 */
    RteSysMsdGear_t stGear; /* 挡位完整观测组。 */
    RteSysMsdNavigation_t stNavigation; /* 物理位置、航向和速度组。 */
    RteSysMsdUtc_t stUtc; /* UTC 日历和来源组。 */
    RteSysMsdVehicle_t stVehicle; /* 车辆低频身份配置组。 */
    RteSysMsdLoad_t stLoad; /* 载重及人数估算组。 */
    RteSysMsdImpact_t stImpact; /* 可选冲击和翻滚组。 */
    RteSysMsdPositionQuality_t stPositionQuality; /* 独立位置质量组。 */
    RteSysMsdHistory_t stHistory; /* 与当前点配套的真实历史组。 */
} RteSysMsdSignals_t;

/* 任务创建前将车辆、定位、时间等数据组初始化为不可用。 */
/* 任务创建前将车辆、定位、时间等数据组初始化为不可用。 */
void RteSys_InitMsdSignals(void);
/* 发布完整 SRS 接收观测或使其失效；接收不等于校验完整性通过。 */
/* 发布完整 SRS 接收观测或使其失效；接收不等于校验完整性通过。 */
void RteSys_UpdateMsdSrs(const RteSysMsdSrs_t *pstSrs);
/* 发布挡位及有效性；超时检查由快照资格判定完成。 */
void RteSys_UpdateMsdGear(const RteSysMsdGear_t *pstGear);
/* 发布导航数据组及来源时间，不能用查询时刻冒充采样时刻。 */
/* 发布导航数据组及来源时间，不能用查询时刻冒充采样时刻。 */
void RteSys_PublishMsdNavigation(const RteSysMsdNavigation_t *pstNavigation);
/* 发布 UTC 日历及来源采样时间，读取时独立检查新鲜度。 */
void RteSys_PublishMsdUtc(const RteSysMsdUtc_t *pstUtc);
/* 保留原值复制接口；成功只表示取得副本，不保证所有字段有效。 */
/* 保留原值复制接口；成功只表示取得副本，不保证所有字段有效。 */
uint8_t RteSys_ReadMsdSignals(RteSysMsdSignals_t *pstSignals);

/* 检查 VIN 必须恰好十七字节且字符合法，不越界读输入。 */
/* 检查 VIN 必须恰好十七字节且字符合法，不越界读输入。 */
uint8_t RteSys_IsMsdVinValid(const uint8_t *pu8Vin, uint8_t u8Length);
/* 仅发布成功获得且语法合法的 VIN，非法输入保留上次有效值。 */
/* 仅发布成功获得且语法合法的 VIN，非法输入保留上次有效值。 */
uint8_t RteSys_UpdateMsdVin(const uint8_t *pu8Vin, uint8_t u8Length);
/* 发布经确认的车型及旧能源编码，不覆盖 VIN。 */
/* 发布经确认的车型及旧能源编码，不覆盖 VIN。 */
void RteSys_PublishMsdVehicleProperties(uint8_t u8VehicleType, uint8_t u8EnergyType, uint8_t u8ValidMask);
/* 保存公斤载重，按七十五公斤每人四舍五入估算，最多五人；估算不是实测人数。 */
/* 保存公斤载重，按七十五公斤每人四舍五入估算，最多五人；估算不是实测人数。 */
void RteSys_UpdateMsdLoad(const RteSysMsdLoad_t *pstLoad);
/* 更新位置，同时保留独立航向及其原始年龄。 */
/* 更新位置，同时保留独立航向及其原始年龄。 */
void RteSys_PublishMsdPosition(const RteSysMsdNavigation_t *pstNavigation);
/* 在同一临界区发布当前位置及其所属的真实历史点。 */
/* 在同一临界区发布当前位置及其所属的真实历史点。 */
void RteSys_PublishMsdPositionHistory(const RteSysMsdNavigation_t *pstNavigation, const RteSysMsdHistory_t *pstHistory);
/* 发布业务提供的航向，单位为百分之一度，单独记录采样年龄。 */
/* 发布业务提供的航向，单位为百分之一度，单独记录采样年龄。 */
void RteSys_PublishMsdHeading(uint16_t u16HeadingCdeg, uint8_t u8Valid);
/* 发布冲击和翻滚观测；空指针明确使该组失效，不伪造默认成功。 */
/* 发布冲击和翻滚观测；空指针明确使该组失效，不伪造默认成功。 */
void RteSys_PublishMsdImpact(const RteSysMsdImpact_t *pstImpact);
/* 发布位置可信度及有效性，定位有效不等于位置可信。 */
void RteSys_PublishMsdPositionQuality(const RteSysMsdPositionQuality_t *pstQuality);


/* 车辆交付与模组定位/时间分离；七种组合能源由确认配置提供。 */
#define RTESYS_MSD_PROPULSION_GASOLINE     0x01U
#define RTESYS_MSD_PROPULSION_DIESEL       0x02U
#define RTESYS_MSD_PROPULSION_CNG          0x04U
#define RTESYS_MSD_PROPULSION_LPG          0x08U
#define RTESYS_MSD_PROPULSION_ELECTRIC      0x10U
#define RTESYS_MSD_PROPULSION_HYDROGEN      0x20U
#define RTESYS_MSD_PROPULSION_OTHER        0x40U
#define RTESYS_MSD_PROPULSION_MASK         0x7FU


typedef struct
{
    uint32_t u32SnapshotMs; /* 本机读取时刻，毫秒；不是事故UTC或平台时间。 */
    uint32_t u32Revision; /* 车辆来源发布版本，不是标准数据消息编号。 */
    RteSysMsdVehicle_t stVehicle; /* VIN和车型等低频车辆配置及有效性。 */
    RteSysMsdSrs_t stSrs; /* 碰撞和气囊观测，接收不代表完整性校验通过。 */
    RteSysMsdGear_t stGear; /* 挡位来源及年龄。 */
    RteSysMsdLoad_t stLoad; /* 公斤载重和独立人数估算。 */
    RteSysMsdImpact_t stImpact; /* 可选车辆冲击/翻滚及逐项有效性。 */
    uint8_t u8PropulsionMask; /* 七种组合能源位，不从运行状态推断。 */
    uint8_t u8PropulsionValid; /* 组合能源配置是否已经确认。 */
} RteSysVehicleSnapshot_t;

/* 发布独立的七位组合能源配置，不从车辆运行状态推断能源。 */
/* 发布独立的七位组合能源配置，不从车辆运行状态推断能源。 */
void RteSys_PublishMsdPropulsion(uint8_t u8PropulsionMask, uint8_t u8Valid);
/* 任务上下文复制车辆数据；成功只表示复制，不保证每个字段有效。 */
uint8_t RteSys_ReadVehicleSnapshot(RteSysVehicleSnapshot_t *pstSnapshot);

/* 模组通话状态直接映射矩阵TBOX_ECallSts，不从MCU事件或数据ACK推导。 */
typedef enum
{
    RTESYS_MODEM_CALL_NO_DISPLAY = 0U, /* 无显示；仅表示模组明确报告的状态。 */
    RTESYS_MODEM_CALL_OUTGOING,        /* 模组正在发起呼叫。 */
    RTESYS_MODEM_CALL_INCOMING,        /* 模组收到来电。 */
    RTESYS_MODEM_CALL_CONNECTED,       /* 模组已建立通话。 */
    RTESYS_MODEM_CALL_ENDED            /* 模组明确报告通话结束。 */
} RteSysModemCallState_t;

#define RTESYS_MODEM_CALL_DURATION_MAX_SECONDS 86399UL /* 矩阵时长上限：二十三时五十九分五十九秒。 */

typedef struct
{
    uint32_t u32UpdateMs;       /* 最后一次合法模组观测的本机毫秒时刻，不是呼叫开始时刻。 */
    uint32_t u32DurationSeconds; /* 模组提供的累计通话秒数，不由MCU累计；有效时零至八万六千三百九十九。 */
    uint8_t u8CallState;        /* 0-4，编码与上方枚举及现有矩阵一致。 */
    uint8_t u8SourceValid;      /* 一表示已获得有效来源；零表示未接入或来源已失效。 */
    uint8_t u8DurationValid;    /* 一表示模组提供有效时长；零时CAN发布时长不可用编码。 */
} RteSysModemCallStatus_t;

/* 协议任务发布已校验的模组状态；时长有效位只接受零或一，返回一表示缓存已更新。
 * 仅允许单个协议任务生产，不在ISR调用；无效输入不覆盖上次结果，不改变eCall事件或电源锁。
 */
uint8_t RteSys_PublishModemCallStatus(RteSysModemCallState_t enCallState, uint32_t u32DurationSeconds, uint8_t u8DurationValid);
/* 协议任务在断链或模组重启时使来源失效，保留最后观测，不伪造结束或恢复音量。 */
void RteSys_InvalidateModemCallStatus(void);
/* 任务上下文获取短临界区保护的值副本；返回一表示复制成功，来源有效性须另查。 */
uint8_t RteSys_ReadModemCallStatus(RteSysModemCallStatus_t *pstStatus);

#endif
