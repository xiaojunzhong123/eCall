/*******************************************************
  * Name    :RteSys.c
  * Author  :xiaojunzhong
  * Function:共享信号缓存和值复制契约；生产者更新，使用者获取独立副本。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/* 流程阅读说明：
 * 数据路径：CanTask/矩阵回调、GnssTask、DID 等生产者 -> 发布接口 -> 私有缓存 -> ReadMsdSnapshot。
 * 每组独立有效，读取时检查原采样年龄；事故快照由 Ecall 首次触发后自行持有。
 * 原有历史 RTE 缓存与新增事故数据契约并存，不能把原通信时基当成内核时基。
 */

/*-------------include file---------------------------*/
#include "Types.h"
#include "RteSys.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <string.h>
/*-------------variable statement---------------------*/
static RteSwitchState_t s_aeRteSysSwitchState[RTESYS_SWITCH_NUM]; /* 输入服务写入的归一化开关缓存。 */
static RteSwitchState_t s_aeRteSysOutputState[RTESYS_OUTPUT_NUM]; /* 输出服务周期应用的逻辑输出请求缓存。 */

static Bool              g_RteSysCanSleepFlag    = FALSE; //本模块已执行休眠路径
static Bool              g_RteSysLocalSleepFlag  = FALSE; //本地是否要求保持休眠
static Bool              g_RteSysDtcDeactiveFlag = FALSE; /* 原诊断控制缓存，保留历史接口。 */
static Bool              g_RteSysIgnStatusFlag   = FALSE; /* 原通信框架点火缓存。 */
static volatile uint32_t g_RteSysTimeMs          = 0;
static uint16_t          g_RteVoltageValue       = 0xFFFF; /* 原电压数值缓存，单独缺少来源有效性。 */
static uint8_t           g_RteRkeValue           = 0; /* 原遥控命令缓存。 */
static uint8_t           g_TopLampPwmPercent     = 0; /* 原顶灯占空比百分数缓存。 */
static float             g_Vehiclespeed          = 0.0; /* 原车速缓存，单位遵循原接口。 */
static float             g_EnginSpeed            = 0.0; /* 原发动机转速缓存。 */
static Bool              g_RfDect                = FALSE; /* 原射频检测状态缓存。 */
static uint8_t           g_FunBuf[RTE_Fun_ID_MAX_NUM]; /* 按历史功能枚举索引的状态缓存。 */
/*-------------function-------------------------------*/
/*******************************************************
  * Name    :RteSys_GetSysTimeMs
  * Author  :xiaojunzhong
  * Function:读取原通信框架毫秒计数；该计数与内核时钟不同，生产时基仍须核对。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint32_t RteSys_GetSysTimeMs(void)
{
    return g_RteSysTimeMs;
}

/*******************************************************
  * Name    :RteSys_SetSysTimeMs
  * Author  :xiaojunzhong
  * Function:为原通信计数累加传入步长；仅由确定的系统计时入口调用。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetSysTimeMs(uint8_t time)
{
    g_RteSysTimeMs += time;
}

/*******************************************************
  * Name    :RteSys_GetCanSleepFlag
  * Author  :xiaojunzhong
  * Function:读取 CAN 管理层报告的休眠标志，不等于整机已经睡眠。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool RteSys_GetCanSleepFlag(void)
{
    return g_RteSysCanSleepFlag;
}

/*******************************************************
  * Name    :RteSys_SetCanSleepFlag
  * Author  :xiaojunzhong
  * Function:保存 CAN 管理层休眠结论，不直接控制硬件。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetCanSleepFlag(Bool flag)
{
    g_RteSysCanSleepFlag = flag;
}

/*******************************************************
  * Name    :RteSys_GetLocalSleepFlag
  * Author  :xiaojunzhong
  * Function:读取本地休眠请求，供 CAN 网络管理查询。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool RteSys_GetLocalSleepFlag(void)
{
    return g_RteSysLocalSleepFlag;
}

/*******************************************************
  * Name    :RteSys_SetLocalSleepFlag
  * Author  :xiaojunzhong
  * Function:保存本地休眠请求；请求不等于模组或硬件允许睡眠。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetLocalSleepFlag(Bool flag)
{
    g_RteSysLocalSleepFlag = flag;
}

/*******************************************************
  * Name    :RteSys_GetDtcDeactiveFlag
  * Author  :xiaojunzhong
  * Function:读取诊断故障码控制标志，沿用原通信框架约定。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool RteSys_GetDtcDeactiveFlag(void)
{
    return g_RteSysDtcDeactiveFlag;
}

/*******************************************************
  * Name    :RteSys_SetDtcDeactiveFlag
  * Author  :xiaojunzhong
  * Function:保存诊断故障码控制标志，不执行实际故障检测。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetDtcDeactiveFlag(Bool flag)
{
    g_RteSysDtcDeactiveFlag = flag;
}

/*******************************************************
  * Name    :RteSys_GetIgnStatusFlag
  * Author  :xiaojunzhong
  * Function:读取原框架点火缓存，与归一化 KL15 输入分别维护。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool RteSys_GetIgnStatusFlag(void)
{
    return g_RteSysIgnStatusFlag;
}

/*******************************************************
  * Name    :RteSys_SetIgnStatusFlag
  * Author  :xiaojunzhong
  * Function:更新原框架点火缓存，不直接采集 GPIO。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetIgnStatusFlag(Bool flag)
{
    g_RteSysIgnStatusFlag = flag;
}

/*******************************************************
  * Name    :RteSys_GetVoltageValue
  * Author  :xiaojunzhong
  * Function:读取原有电压缓存；接口本身没有有效性，不可据此证明电源健康。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint16_t RteSys_GetVoltageValue(void)
{
    return g_RteVoltageValue;
}

/*******************************************************
  * Name    :RteSys_SetVoltageValue
  * Author  :xiaojunzhong
  * Function:保存来源提供的电压值；物理单位及采样有效性须由生产者确认。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetVoltageValue(uint16_t value)
{
    g_RteVoltageValue = value;
}

/*******************************************************
  * Name    :RteSys_GetRkeValue
  * Author  :xiaojunzhong
  * Function:读取原遥控命令缓存，保留原接口编码。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t RteSys_GetRkeValue(void)
{
    return g_RteRkeValue;
}

/*******************************************************
  * Name    :RteSys_SetRkeValue
  * Author  :xiaojunzhong
  * Function:更新原遥控命令缓存，不直接驱动执行器。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetRkeValue(uint8_t value)
{
    g_RteRkeValue = value;
}

/*******************************************************
  * Name    :RteSys_GetTopLampPwmPercent
  * Author  :xiaojunzhong
  * Function:读取原顶灯占空比缓存。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t RteSys_GetTopLampPwmPercent(void)
{
    return g_TopLampPwmPercent;
}

/*******************************************************
  * Name    :RteSys_SetTopLampPwmPercent
  * Author  :xiaojunzhong
  * Function:更新原顶灯占空比缓存，不直接操作 PWM。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetTopLampPwmPercent(uint8_t value)
{
    g_TopLampPwmPercent = value;
}

/*******************************************************
  * Name    :RteSys_GetVehiclespeed
  * Author  :xiaojunzhong
  * Function:读取原通信车速缓存，单位沿用已有矩阵接口。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
float RteSys_GetVehiclespeed(void)
{
    return g_Vehiclespeed;
}

/*******************************************************
  * Name    :RteSys_SetVehiclespeed
  * Author  :xiaojunzhong
  * Function:更新原通信车速缓存，不改变信号有效性规则。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetVehiclespeed(float value)
{
    g_Vehiclespeed = value;
}

/*******************************************************
  * Name    :RteSys_GetEnginSpeedspeed
  * Author  :xiaojunzhong
  * Function:读取原通信发动机转速缓存。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
float RteSys_GetEnginSpeedspeed(void)
{
    return g_EnginSpeed;
}

/*******************************************************
  * Name    :RteSys_SetEnginSpeedspeed
  * Author  :xiaojunzhong
  * Function:更新原通信发动机转速缓存。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetEnginSpeedspeed(float value)
{
    g_EnginSpeed = value;
}

/*******************************************************
  * Name    :RteSys_GetRfDet
  * Author  :xiaojunzhong
  * Function:读取原射频检测状态缓存。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool RteSys_GetRfDet(void)
{
    return g_RfDect;
}

/*******************************************************
  * Name    :RteSys_SetRfDet
  * Author  :xiaojunzhong
  * Function:更新原射频检测状态缓存。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RteSys_SetRfDet(Bool flag)
{
    g_RfDect = flag;
}
/*******************************************************
  * Name    :RTE_write_FunInd
  * Author  :xiaojunzhong
  * Function:按功能索引写入原有状态表；索引必须在枚举范围内。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void RTE_write_FunInd(Rte_Fun_Type fun, uint8_t value)
{
    if (fun < RTE_Fun_ID_MAX_NUM)
    {
        g_FunBuf[fun] = value;
    }
}
/*******************************************************
  * Name    :RTE_read_FunInd
  * Author  :xiaojunzhong
  * Function:按功能索引读取原有状态表；不执行硬件采样。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint8_t RTE_read_FunInd(Rte_Fun_Type fun)
{
    if (fun < RTE_Fun_ID_MAX_NUM)
    {
        return g_FunBuf[fun];
    }
    else
    {
        return 0xff;
    }
}

/*******************************************************
  * Name    :RteSys_SetSwitchState
  * Author  :xiaojunzhong
  * Function:保存输入服务消抖后的逻辑开关值，屏蔽高低有效电平差异。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void RteSys_SetSwitchState(RteSysSwitchId_t enSwitch, RteSwitchState_t enState)
{
    if (enSwitch < RTESYS_SWITCH_NUM)
    {
        s_aeRteSysSwitchState[enSwitch] = enState;
    }
}

/*******************************************************
  * Name    :RteSys_GetSwitchState
  * Author  :xiaojunzhong
  * Function:读取归一化开关值；非法索引返回未激活。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t RteSys_GetSwitchState(RteSysSwitchId_t enSwitch)
{
    if (enSwitch < RTESYS_SWITCH_NUM)
    {
        return s_aeRteSysSwitchState[enSwitch];
    }
    return RTE_SWITCH_INACTIVE;
}

/*******************************************************
  * Name    :RteSys_SetOutputState
  * Author  :xiaojunzhong
  * Function:保存业务输出请求；实际 GPIO 由输出服务周期应用。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void RteSys_SetOutputState(RteSysOutputId_t enOutput, RteSwitchState_t enState)
{
    if (enOutput < RTESYS_OUTPUT_NUM)
    {
        s_aeRteSysOutputState[enOutput] = enState;
    }
}

/*******************************************************
  * Name    :RteSys_GetOutputState
  * Author  :xiaojunzhong
  * Function:读取业务输出请求，不代表物理引脚已达到该电平。
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t RteSys_GetOutputState(RteSysOutputId_t enOutput)
{
    if (enOutput < RTESYS_OUTPUT_NUM)
    {
        return s_aeRteSysOutputState[enOutput];
    }
    return RTE_SWITCH_INACTIVE;
}

/* The single CAN task owns SRS/gear/load writes; GnssTask owns position/UTC.
 * UdsTask publishes VIN; qualified business producers own optional fields.
 * All task-context group copies use short FreeRTOS critical sections.
 * No ISR access, shared pointers, heap allocation or volatile synchronization.
 * Rx ages mirror matrix lostTime; publication age is a producer watchdog.
 */
static RteSysMsdSrs_t s_mRteMsdSrsSts; /* SRS 观测及原接收时刻。 */
static RteSysMsdGear_t s_mRteMsdGearSts; /* 挡位观测及有效性。 */
static RteSysMsdNavigation_t s_mRteMsdNavSts; /* 物理导航缓存，不保存编码字节。 */
static RteSysMsdUtc_t s_mRteMsdUtcSts; /* UTC 日历及来源年龄。 */
static RteSysMsdHistory_t s_mRteMsdHistorySts; /* 当前位置对应的真实历史点组。 */

/* Low-frequency vehicle configuration is independent of transient sensor data. */
static RteSysMsdVehicle_t s_mRteMsdVehicleCfg; /* 低频 VIN、车型及能源代码配置。 */
static RteSysMsdLoad_t s_mRteMsdLoadSts; /* 公斤载重及独立人数估算结论。 */
static RteSysMsdImpact_t s_mRteMsdImpactSts; /* 冲击和翻滚可选来源缓存。 */
static RteSysMsdPositionQuality_t s_mRteMsdQualitySts; /* 位置可信度独立来源缓存。 */
typedef struct
{
    uint32_t u32UpdateMs; /* 该组原采样或发布时间，具体语义由发布接口约定，毫秒。 */
    uint16_t u16HeadingCdeg; /* 真北航向，百分之一度，正常范围零至三万五千九百九十九。 */
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */
} RteSysMsdHeadingStatus_t;
static RteSysMsdHeadingStatus_t s_mRteMsdHeadingSts; /* 独立业务航向及来源年龄，位置更新不刷新它。 */

typedef struct
{
    uint8_t u8Mask; /* 合法配置或来源的位集合。 */
    uint8_t u8Valid; /* 此字段或来源是否有效，不以数值非零代替有效性。 */
} RteSysMsdPropulsionConfig_t;
static RteSysMsdPropulsionConfig_t s_mRteMsdPropulsionCfg; /* 低频七种组合能源位配置。 */

typedef struct
{
    uint32_t u32Revision; /* 发布版本；只计发布，不是时间或 MSD 编号。 */
} RteSysMsdSnapshotStatus_t;
static RteSysMsdSnapshotStatus_t s_mRteMsdSnapshotSts; /* 数据组发布版本计数。 */


/*******************************************************
  * Name    :RteSys_InitMsdSignals
  * Author  :xiaojunzhong
  * Function:任务创建前将车辆、定位、时间等数据组初始化为不可用。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_InitMsdSignals(void)
{
    (void)memset(&s_mRteMsdSrsSts, 0, sizeof(s_mRteMsdSrsSts));
    (void)memset(&s_mRteMsdGearSts, 0, sizeof(s_mRteMsdGearSts));
    (void)memset(&s_mRteMsdNavSts, 0, sizeof(s_mRteMsdNavSts));
    (void)memset(&s_mRteMsdUtcSts, 0, sizeof(s_mRteMsdUtcSts));
    s_mRteMsdGearSts.u8Gear = 0xFFU;
    (void)memset(&s_mRteMsdHistorySts, 0, sizeof(s_mRteMsdHistorySts));
    (void)memset(&s_mRteMsdVehicleCfg, 0, sizeof(s_mRteMsdVehicleCfg));
    (void)memset(&s_mRteMsdLoadSts, 0, sizeof(s_mRteMsdLoadSts));
    (void)memset(&s_mRteMsdImpactSts, 0, sizeof(s_mRteMsdImpactSts));
    (void)memset(&s_mRteMsdQualitySts, 0, sizeof(s_mRteMsdQualitySts));
    (void)memset(&s_mRteMsdHeadingSts, 0, sizeof(s_mRteMsdHeadingSts));
    s_mRteMsdVehicleCfg.u8VehicleType = 0xFFU;
    s_mRteMsdVehicleCfg.u8EnergyType = 0xFFU;
    s_mRteMsdLoadSts.u8Occupants = RTESYS_MSD_OCCUPANT_UNKNOWN;
    (void)memset(&s_mRteMsdPropulsionCfg, 0, sizeof(s_mRteMsdPropulsionCfg));
    (void)memset(&s_mRteMsdSnapshotSts, 0, sizeof(s_mRteMsdSnapshotSts));
}

/*******************************************************
  * Name    :RteSys_UpdateMsdSrs
  * Author  :xiaojunzhong
  * Function:发布完整 SRS 接收观测或使其失效；接收不等于校验完整性通过。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_UpdateMsdSrs(const RteSysMsdSrs_t *pstSrs)
{
    portENTER_CRITICAL();
    if (pstSrs != NULL)
    {
        s_mRteMsdSrsSts = *pstSrs;
    }
    else
    {
        (void)memset(&s_mRteMsdSrsSts, 0, sizeof(s_mRteMsdSrsSts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_UpdateMsdGear
  * Author  :xiaojunzhong
  * Function:发布挡位及有效性；超时检查由快照资格判定完成。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_UpdateMsdGear(const RteSysMsdGear_t *pstGear)
{
    portENTER_CRITICAL();
    if (pstGear != NULL)
    {
        s_mRteMsdGearSts = *pstGear;
    }
    else
    {
        (void)memset(&s_mRteMsdGearSts, 0, sizeof(s_mRteMsdGearSts));
        s_mRteMsdGearSts.u8Gear = 0xFFU;
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdNavigation
  * Author  :xiaojunzhong
  * Function:发布导航数据组及来源时间，不能用查询时刻冒充采样时刻。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdNavigation(const RteSysMsdNavigation_t *pstNavigation)
{
    portENTER_CRITICAL();
    if (pstNavigation != NULL)
    {
        s_mRteMsdNavSts = *pstNavigation;
        s_mRteMsdHeadingSts.u32UpdateMs = pstNavigation->u32UpdateMs;
        s_mRteMsdHeadingSts.u16HeadingCdeg = pstNavigation->u16HeadingCdeg;
        s_mRteMsdHeadingSts.u8Valid = pstNavigation->u8HeadingValid;
    }
    else
    {
        (void)memset(&s_mRteMsdNavSts, 0, sizeof(s_mRteMsdNavSts));
        (void)memset(&s_mRteMsdHeadingSts, 0, sizeof(s_mRteMsdHeadingSts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdUtc
  * Author  :xiaojunzhong
  * Function:发布 UTC 日历及来源采样时间，读取时独立检查新鲜度。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdUtc(const RteSysMsdUtc_t *pstUtc)
{
    portENTER_CRITICAL();
    if (pstUtc != NULL)
    {
        s_mRteMsdUtcSts = *pstUtc;
    }
    else
    {
        (void)memset(&s_mRteMsdUtcSts, 0, sizeof(s_mRteMsdUtcSts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_ReadMsdSignals
  * Author  :xiaojunzhong
  * Function:保留原值复制接口；成功只表示取得副本，不保证所有字段有效。
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static uint8_t RteSys_IsMsdUtcValid(const RteSysMsdUtc_t *pstUtc)
{
    static const uint8_t au8MonthDays[12] = {31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U}; /* 公历各月基本天数表，闰年另行修正。 */
    uint8_t u8MonthDays; /* 当前月份实际天数。 */

    if ((pstUtc->u8Valid == 0U)
        || (pstUtc->u8Source == 0U)
        || (pstUtc->u16Year < 2000U)
        || (pstUtc->u16Year > 2235U)
        || (pstUtc->u8Month < 1U)
        || (pstUtc->u8Month > 12U)
        || (pstUtc->u8Hour > 23U)
        || (pstUtc->u8Minute > 59U)
        || (pstUtc->u8Second > 59U))
    {
        return 0U;
    }
    u8MonthDays = au8MonthDays[pstUtc->u8Month - 1U];
    if ((pstUtc->u8Month == 2U)
        && (((pstUtc->u16Year % 400U) == 0U)
            || (((pstUtc->u16Year % 4U) == 0U) && ((pstUtc->u16Year % 100U) != 0U))))
    {
        u8MonthDays = 29U;
    }
    return ((pstUtc->u8Day >= 1U) && (pstUtc->u8Day <= u8MonthDays)) ? 1U : 0U;
}

/*******************************************************
  * Name    :RteSys_CopyMsdSignalsLocked
  * Author  :xiaojunzhong
  * Function:在调用者持有临界区时复制全部数据组，不返回内部指针。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
static void RteSys_CopyMsdSignalsLocked(RteSysMsdSignals_t *pstSignals)
{
    pstSignals->stSrs = s_mRteMsdSrsSts;
    pstSignals->stGear = s_mRteMsdGearSts;
    pstSignals->stNavigation = s_mRteMsdNavSts;
    pstSignals->stUtc = s_mRteMsdUtcSts;
    pstSignals->stVehicle = s_mRteMsdVehicleCfg;
    pstSignals->stLoad = s_mRteMsdLoadSts;
    pstSignals->stImpact = s_mRteMsdImpactSts;
    pstSignals->stPositionQuality = s_mRteMsdQualitySts;
    pstSignals->stHistory = s_mRteMsdHistorySts;
    pstSignals->stNavigation.u16HeadingCdeg = s_mRteMsdHeadingSts.u16HeadingCdeg;
    pstSignals->stNavigation.u8HeadingValid = s_mRteMsdHeadingSts.u8Valid;
}

/*******************************************************
  * Name    :RteSys_QualifyMsdSignals
  * Author  :xiaojunzhong
  * Function:在临界区外使副本中的过期或非法字段失效，不改写来源缓存。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
static void RteSys_QualifyMsdSignals(RteSysMsdSignals_t *pstSignals, uint32_t u32NowMs, uint32_t u32HeadingUpdateMs)
{
    if ((u32NowMs - pstSignals->stSrs.u32UpdateMs) >= RTESYS_MSD_SRS_MAX_AGE_MS)
    {
        pstSignals->stSrs.u8Received = 0U;
    }
    if ((u32NowMs - pstSignals->stGear.u32UpdateMs) >= RTESYS_MSD_GEAR_MAX_AGE_MS)
    {
        pstSignals->stGear.u8Received = 0U;
    }
    if ((pstSignals->stGear.u8Received == 0U)
        || (pstSignals->stGear.u8Gear > 3U))
    {
        pstSignals->stGear.u8GearValid = 0U;
    }
    if ((u32NowMs - pstSignals->stNavigation.u32UpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
    {
        pstSignals->stNavigation.u8PositionValid = 0U;
        pstSignals->stNavigation.u8SpeedValid = 0U;
    }
    if ((pstSignals->stNavigation.i32LongitudeE7 < -1800000000L)
        || (pstSignals->stNavigation.i32LongitudeE7 > 1800000000L)
        || (pstSignals->stNavigation.i32LatitudeE7 < -900000000L)
        || (pstSignals->stNavigation.i32LatitudeE7 > 900000000L))
    {
        pstSignals->stNavigation.u8PositionValid = 0U;
    }
    if (((u32NowMs - u32HeadingUpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
        || (pstSignals->stNavigation.u16HeadingCdeg > 35999U))
    {
        pstSignals->stNavigation.u8HeadingValid = 0U;
    }
    if (((u32NowMs - pstSignals->stUtc.u32UpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
        || (RteSys_IsMsdUtcValid(&pstSignals->stUtc) == 0U))
    {
        pstSignals->stUtc.u8Valid = 0U;
    }
    if ((pstSignals->stNavigation.u8PositionValid == 0U)
        || ((u32NowMs - pstSignals->stHistory.u32UpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
        || (pstSignals->stHistory.u8Count > RTESYS_MSD_HISTORY_COUNT)
        || (pstSignals->stHistory.u32PositionUtcSeconds != pstSignals->stNavigation.u32UtcSeconds))
    {
        pstSignals->stHistory.u8Count = 0U;
    }
    if ((pstSignals->stLoad.u8Received == 0U)
        || ((u32NowMs - pstSignals->stLoad.u32UpdateMs) >= RTESYS_MSD_LOAD_MAX_AGE_MS))
    {
        pstSignals->stLoad.u8Received = 0U;
        pstSignals->stLoad.u8LoadValid = 0U;
        pstSignals->stLoad.u8OccupantsValid = 0U;
        pstSignals->stLoad.u8Occupants = RTESYS_MSD_OCCUPANT_UNKNOWN;
        pstSignals->stLoad.u8OccupantsLimited = 0U;
    }
    if ((u32NowMs - pstSignals->stImpact.u32UpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
    {
        pstSignals->stImpact.u8DeltaVLongitudinalValid = 0U;
        pstSignals->stImpact.u8DeltaVLateralValid = 0U;
        pstSignals->stImpact.u8RolloverValid = 0U;
    }
    if (pstSignals->stImpact.u8Rollover > 1U)
    {
        pstSignals->stImpact.u8RolloverValid = 0U;
    }
    if (((u32NowMs - pstSignals->stPositionQuality.u32UpdateMs) >= RTESYS_MSD_PUBLISH_MAX_AGE_MS)
        || (pstSignals->stPositionQuality.u8PositionTrusted > 1U))
    {
        pstSignals->stPositionQuality.u8Valid = 0U;
    }
}

/*******************************************************
  * Name    :RteSys_ReadMsdSignals
  * Author  :xiaojunzhong
  * Function:保留原值复制接口；成功只表示取得副本，不保证所有字段有效。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
uint8_t RteSys_ReadMsdSignals(RteSysMsdSignals_t *pstSignals)
{
    uint32_t u32NowMs; /* 当前单调毫秒计数，用无符号差值处理回绕。 */
    uint32_t u32HeadingUpdateMs; /* 独立航向来源更新时间，毫秒。 */

    if (pstSignals == NULL)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    u32NowMs = osKernelSysTick();
    RteSys_CopyMsdSignalsLocked(pstSignals);
    u32HeadingUpdateMs = s_mRteMsdHeadingSts.u32UpdateMs;
    portEXIT_CRITICAL();
    RteSys_QualifyMsdSignals(pstSignals, u32NowMs, u32HeadingUpdateMs);
    return 1U;
}

/*******************************************************
  * Name    :RteSys_IsMsdVinValid
  * Author  :xiaojunzhong
  * Function:检查 VIN 必须恰好十七字节且字符合法，不越界读输入。
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
uint8_t RteSys_IsMsdVinValid(const uint8_t *pu8Vin, uint8_t u8Length)
{
    uint8_t u8Index; /* 当前数组或通道索引。 */
    uint8_t u8Character; /* 当前校验的 VIN 字符。 */

    if ((pu8Vin == NULL) || (u8Length != RTESYS_MSD_VIN_LENGTH))
    {
        return 0U;
    }
    for (u8Index = 0U; u8Index < RTESYS_MSD_VIN_LENGTH; u8Index++)
    {
        u8Character = pu8Vin[u8Index];
        if ((((u8Character < '0') || (u8Character > '9'))
                && ((u8Character < 'A') || (u8Character > 'Z')))
            || (u8Character == 'I')
            || (u8Character == 'O')
            || (u8Character == 'Q'))
        {
            return 0U;
        }
    }
    return 1U;
}

/*******************************************************
  * Name    :RteSys_UpdateMsdVin
  * Author  :xiaojunzhong
  * Function:仅发布成功获得且语法合法的 VIN，非法输入保留上次有效值。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
uint8_t RteSys_UpdateMsdVin(const uint8_t *pu8Vin, uint8_t u8Length)
{
    if (RteSys_IsMsdVinValid(pu8Vin, u8Length) == 0U)
    {
        return 0U;
    }
    portENTER_CRITICAL();
    (void)memcpy(s_mRteMsdVehicleCfg.au8Vin, pu8Vin, RTESYS_MSD_VIN_LENGTH);
    s_mRteMsdVehicleCfg.u8VinValid = 1U;
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
    return 1U;
}

/*******************************************************
  * Name    :RteSys_PublishMsdVehicleProperties
  * Author  :xiaojunzhong
  * Function:发布经确认的车型及旧能源编码，不覆盖 VIN。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdVehicleProperties(uint8_t u8VehicleType, uint8_t u8EnergyType, uint8_t u8ValidMask)
{
    portENTER_CRITICAL();
    s_mRteMsdVehicleCfg.u8VehicleType = u8VehicleType;
    s_mRteMsdVehicleCfg.u8EnergyType = u8EnergyType;
    s_mRteMsdVehicleCfg.u8VehicleTypeValid = ((u8ValidMask & RTESYS_MSD_VEHICLE_TYPE_VALID) != 0U) ? 1U : 0U;
    s_mRteMsdVehicleCfg.u8EnergyTypeValid = ((u8ValidMask & RTESYS_MSD_ENERGY_TYPE_VALID) != 0U) ? 1U : 0U;
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_UpdateMsdLoad
  * Author  :xiaojunzhong
  * Function:保存公斤载重，按七十五公斤每人四舍五入估算，最多五人；估算不是实测人数。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_UpdateMsdLoad(const RteSysMsdLoad_t *pstLoad)
{
    uint32_t u32Occupants; /* 中间人数估算值，限幅前使用较宽类型。 */

    portENTER_CRITICAL();
    (void)memset(&s_mRteMsdLoadSts, 0, sizeof(s_mRteMsdLoadSts));
    s_mRteMsdLoadSts.u8Occupants = RTESYS_MSD_OCCUPANT_UNKNOWN;
    s_mRteMsdLoadSts.u8OccupantsEstimated = 1U;
    if (pstLoad != NULL)
    {
        s_mRteMsdLoadSts.u32UpdateMs = pstLoad->u32UpdateMs;
        s_mRteMsdLoadSts.u16LoadKg = pstLoad->u16LoadKg;
        s_mRteMsdLoadSts.u8Received = pstLoad->u8Received;
        s_mRteMsdLoadSts.u8LoadValid = pstLoad->u8LoadValid;
        if ((pstLoad->u8Received != 0U) && (pstLoad->u8LoadValid != 0U))
        {
            u32Occupants = pstLoad->u16LoadKg;
#if (RTESYS_MSD_OCCUPANT_ROUNDING == 1U)
            u32Occupants += RTESYS_MSD_OCCUPANT_MASS_KG / 2U;
#elif (RTESYS_MSD_OCCUPANT_ROUNDING == 2U)
            u32Occupants += RTESYS_MSD_OCCUPANT_MASS_KG - 1U;
#endif
            u32Occupants /= RTESYS_MSD_OCCUPANT_MASS_KG;
            if (u32Occupants > RTESYS_MSD_OCCUPANT_MAX)
            {
                u32Occupants = RTESYS_MSD_OCCUPANT_MAX;
                s_mRteMsdLoadSts.u8OccupantsLimited = 1U;
            }
            s_mRteMsdLoadSts.u8Occupants = (uint8_t)u32Occupants;
            s_mRteMsdLoadSts.u8OccupantsValid = 1U;
        }
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdPosition
  * Author  :xiaojunzhong
  * Function:更新位置，同时保留独立航向及其原始年龄。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdPosition(const RteSysMsdNavigation_t *pstNavigation)
{
    portENTER_CRITICAL();
    if (pstNavigation != NULL)
    {
        s_mRteMsdNavSts.i32LongitudeE7 = pstNavigation->i32LongitudeE7;
        s_mRteMsdNavSts.i32LatitudeE7 = pstNavigation->i32LatitudeE7;
        s_mRteMsdNavSts.u32UtcSeconds = pstNavigation->u32UtcSeconds;
        s_mRteMsdNavSts.u32UpdateMs = pstNavigation->u32UpdateMs;
        s_mRteMsdNavSts.u8PositionValid = pstNavigation->u8PositionValid;
        s_mRteMsdNavSts.u16SpeedCentiKmh = pstNavigation->u16SpeedCentiKmh;
        s_mRteMsdNavSts.u8SpeedValid = pstNavigation->u8SpeedValid;
    }
    else
    {
        s_mRteMsdNavSts.u8PositionValid = 0U;
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdHeading
  * Author  :xiaojunzhong
  * Function:发布业务提供的航向，单位为百分之一度，单独记录采样年龄。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdHeading(uint16_t u16HeadingCdeg, uint8_t u8Valid)
{
    portENTER_CRITICAL();
    s_mRteMsdHeadingSts.u32UpdateMs = osKernelSysTick();
    s_mRteMsdHeadingSts.u16HeadingCdeg = u16HeadingCdeg;
    s_mRteMsdHeadingSts.u8Valid = ((u8Valid != 0U) && (u16HeadingCdeg <= 35999U)) ? 1U : 0U;
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdImpact
  * Author  :xiaojunzhong
  * Function:发布冲击和翻滚观测；空指针明确使该组失效，不伪造默认成功。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdImpact(const RteSysMsdImpact_t *pstImpact)
{
    portENTER_CRITICAL();
    if (pstImpact != NULL)
    {
        s_mRteMsdImpactSts = *pstImpact;
    }
    else
    {
        (void)memset(&s_mRteMsdImpactSts, 0, sizeof(s_mRteMsdImpactSts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdPositionQuality
  * Author  :xiaojunzhong
  * Function:发布位置可信度及有效性，定位有效不等于位置可信。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdPositionQuality(const RteSysMsdPositionQuality_t *pstQuality)
{
    portENTER_CRITICAL();
    if (pstQuality != NULL)
    {
        s_mRteMsdQualitySts = *pstQuality;
    }
    else
    {
        (void)memset(&s_mRteMsdQualitySts, 0, sizeof(s_mRteMsdQualitySts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdPositionHistory
  * Author  :xiaojunzhong
  * Function:在同一临界区发布当前位置及其所属的真实历史点。
  * Version :V1.0.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdPositionHistory(const RteSysMsdNavigation_t *pstNavigation, const RteSysMsdHistory_t *pstHistory)
{
    portENTER_CRITICAL();
    if ((pstNavigation != NULL) && (pstHistory != NULL))
    {
        s_mRteMsdNavSts.i32LongitudeE7 = pstNavigation->i32LongitudeE7;
        s_mRteMsdNavSts.i32LatitudeE7 = pstNavigation->i32LatitudeE7;
        s_mRteMsdNavSts.u32UtcSeconds = pstNavigation->u32UtcSeconds;
        s_mRteMsdNavSts.u32UpdateMs = pstNavigation->u32UpdateMs;
        s_mRteMsdNavSts.u8PositionValid = pstNavigation->u8PositionValid;
        s_mRteMsdNavSts.u16SpeedCentiKmh = pstNavigation->u16SpeedCentiKmh;
        s_mRteMsdNavSts.u8SpeedValid = pstNavigation->u8SpeedValid;
        s_mRteMsdHistorySts = *pstHistory;
    }
    else
    {
        s_mRteMsdNavSts.u8PositionValid = 0U;
        (void)memset(&s_mRteMsdHistorySts, 0, sizeof(s_mRteMsdHistorySts));
    }
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_PublishMsdPropulsion
  * Author  :xiaojunzhong
  * Function:发布独立的七位组合能源配置，不从车辆运行状态推断能源。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
void RteSys_PublishMsdPropulsion(uint8_t u8PropulsionMask, uint8_t u8Valid)
{
    portENTER_CRITICAL();
    s_mRteMsdPropulsionCfg.u8Mask = u8PropulsionMask & RTESYS_MSD_PROPULSION_MASK;
    s_mRteMsdPropulsionCfg.u8Valid = ((u8Valid == 1U) && ((u8PropulsionMask & 0x80U) == 0U)) ? 1U : 0U;
    s_mRteMsdSnapshotSts.u32Revision++;
    portEXIT_CRITICAL();
}

/*******************************************************
  * Name    :RteSys_ReadMsdSnapshot
  * Author  :xiaojunzhong
  * Function:捕获同一发布版本的事故数据，锁外判定有效性并投影最多两个真实历史点。
  * Version :V1.1.0
  * Data    :2026.10.31
*******************************************************/
uint8_t RteSys_ReadMsdSnapshot(RteSysMsdSnapshot_t *pstSnapshot)
{
    RteSysMsdSignals_t stSignals; /* 完整数据组的调用者副本。 */
    uint32_t u32PreviousUtcSeconds; /* 前一个已选历史点 UTC，保证真实时间顺序。 */
    uint8_t u8Index; /* 当前数组或通道索引。 */

    if (pstSnapshot == NULL)
    {
        return 0U;
    }
    (void)memset(pstSnapshot, 0, sizeof(*pstSnapshot));
    portENTER_CRITICAL();
    pstSnapshot->u32SnapshotMs = osKernelSysTick();
    pstSnapshot->u32Revision = s_mRteMsdSnapshotSts.u32Revision;
    pstSnapshot->u32HeadingSampleMs = s_mRteMsdHeadingSts.u32UpdateMs;
    pstSnapshot->u8PropulsionMask = s_mRteMsdPropulsionCfg.u8Mask;
    pstSnapshot->u8PropulsionValid = s_mRteMsdPropulsionCfg.u8Valid;
    RteSys_CopyMsdSignalsLocked(&stSignals);
    portEXIT_CRITICAL();
    RteSys_QualifyMsdSignals(&stSignals, pstSnapshot->u32SnapshotMs, pstSnapshot->u32HeadingSampleMs);
    pstSnapshot->stNavigation = stSignals.stNavigation;
    pstSnapshot->stUtc = stSignals.stUtc;
    pstSnapshot->stVehicle = stSignals.stVehicle;
    pstSnapshot->stSrs = stSignals.stSrs;
    pstSnapshot->stGear = stSignals.stGear;
    pstSnapshot->stLoad = stSignals.stLoad;
    pstSnapshot->stPositionQuality = stSignals.stPositionQuality;
    /* A valid fix alone is not evidence of the standard trust criterion. */
    if (pstSnapshot->stNavigation.u8PositionValid == 0U)
    {
        pstSnapshot->stPositionQuality.u8Valid = 0U;
    }
    if ((pstSnapshot->stVehicle.u8VehicleType < 1U)
        || (pstSnapshot->stVehicle.u8VehicleType > 13U))
    {
        pstSnapshot->stVehicle.u8VehicleTypeValid = 0U;
    }
    u32PreviousUtcSeconds = stSignals.stNavigation.u32UtcSeconds;
    for (u8Index = 0U; (u8Index < stSignals.stHistory.u8Count) && (u8Index < RTESYS_MSD_CS_HISTORY_COUNT); u8Index++)
    {
        if ((stSignals.stHistory.ai32LatitudeE7[u8Index] < -900000000L)
            || (stSignals.stHistory.ai32LatitudeE7[u8Index] > 900000000L)
            || (stSignals.stHistory.ai32LongitudeE7[u8Index] < -1800000000L)
            || (stSignals.stHistory.ai32LongitudeE7[u8Index] > 1800000000L)
            || (stSignals.stHistory.au32UtcSeconds[u8Index] == 0U)
            || (stSignals.stHistory.au32UtcSeconds[u8Index] >= u32PreviousUtcSeconds))
        {
            break;
        }
        pstSnapshot->astRecentPosition[u8Index].i32LatitudeE7 = stSignals.stHistory.ai32LatitudeE7[u8Index];
        pstSnapshot->astRecentPosition[u8Index].i32LongitudeE7 = stSignals.stHistory.ai32LongitudeE7[u8Index];
        pstSnapshot->astRecentPosition[u8Index].u32UtcSeconds = stSignals.stHistory.au32UtcSeconds[u8Index];
        pstSnapshot->u8RecentPositionCount++;
        u32PreviousUtcSeconds = stSignals.stHistory.au32UtcSeconds[u8Index];
    }
    return 1U;
}
