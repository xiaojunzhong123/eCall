/*******************************************************
  * Name    :CAKY_TBOX_CANFD_TBOX_20261014_182806_V1.5.h
  * Author  :MatrixCreat @WangHu
  * Function:****
  * Version :V1.0.0
  * File    :CAKY_TBOX_CANFD_TBOX_20261014_182806_V1.5
  * Data    :2026.10.19 14:01:58
*******************************************************/
//DHU_Challenge
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t DHU_ChallengeFrame_7               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[1]
        bits_t DHU_ChallengeFrame_6               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[2]
        bits_t DHU_ChallengeFrame_5               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[3]
        bits_t DHU_ChallengeFrame_4               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[4]
        bits_t DHU_ChallengeFrame_3               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[5]
        bits_t DHU_ChallengeFrame_2               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[6]
        bits_t DHU_ChallengeFrame_1               :8;//DHU与TBOX认证密钥，DHU随机数
        //Byte[7]
        bits_t DHU_ChallengeFrame_0               :8;//DHU与TBOX认证密钥，DHU随机数
    }bits;
}CanMatrix_DHU_ChallengeMsg_Type;

//TBOX_AuthKey
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t TBOX_AuthKeyData_5                 :8;//认证钥匙
        //Byte[3]
        bits_t TBOX_AuthKeyData_4                 :8;//认证钥匙
        //Byte[4]
        bits_t TBOX_AuthKeyData_3                 :8;//认证钥匙
        //Byte[5]
        bits_t TBOX_AuthKeyData_2                 :8;//认证钥匙
        //Byte[6]
        bits_t TBOX_AuthKeyData_1                 :8;//认证钥匙
        //Byte[7]
        bits_t TBOX_AuthKeyData_0                 :8;//认证钥匙
    }bits;
}CanMatrix_TBOX_AuthKeyMsg_Type;

//IDCU_Challenge
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t IDCU_ChallengeFrame_7              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[1]
        bits_t IDCU_ChallengeFrame_6              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[2]
        bits_t IDCU_ChallengeFrame_5              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[3]
        bits_t IDCU_ChallengeFrame_4              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[4]
        bits_t IDCU_ChallengeFrame_3              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[5]
        bits_t IDCU_ChallengeFrame_2              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[6]
        bits_t IDCU_ChallengeFrame_1              :8;//IDCU与TBOX认证密钥，IDCU随机数
        //Byte[7]
        bits_t IDCU_ChallengeFrame_0              :8;//IDCU与TBOX认证密钥，IDCU随机数
    }bits;
}CanMatrix_IDCU_ChallengeMsg_Type;

//TBOX_RemCtl
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_AuthResponeSts                :8;//远程认证状态
        //Byte[1]
        bits_t TBOX_RemSentrySwt                  :2;//远程哨兵模式开关
        bits_t TBOX_BattAntidisassembly           :1;//动力电池防拆报警
        bits_t TBOX_RemCarSearchReq               :1;//远程寻车
        bits_t reserved12_15                      :4;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_TBOX_RemCtlMsg_Type;

//TBOX_RemReq
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_RemACOnOffReq                 :2;//远程空调控制请求
        bits_t TBOX_RemAcTempCSet                 :5;//远程空调温度设置（摄氏度）
        bits_t reserved7_7                        :1;
        //Byte[1]
        bits_t TBOX_RemExtremeColdReq             :2;//远程极速降温请求
        bits_t TBOX_RemExtremeHotReq              :2;//远程极速升温请求
        bits_t TBOX_RemAirColdReq                 :2;//远程空调制冷请求
        bits_t TBOX_RemAirHeatReq                 :2;//远程空调制热请求
        //Byte[2]
        bits_t TBOX_RemForbiddenCtrl              :2;//远程禁止启动请求
        bits_t TBOX_RemSpdLimReq                  :2;//远程车辆限速请求
        bits_t reserved20_23                      :4;
        //Byte[3]
        bits_t TBOX_RemDrvrSeatHeatReq            :3;//远程主驾座椅加热请求
        bits_t TBOX_RemDrvrSeatVentnReq           :3;//远程主驾座椅通风请求
        bits_t reserved30_31                      :2;
        //Byte[4]
        bits_t TBOX_RemPassSeatHeatReq            :3;//远程副驾座椅加热请求
        bits_t TBOX_RemPassSeatVentnReq           :3;//远程副驾座椅通风请求
        bits_t reserved38_39                      :2;
        //Byte[5]
        bits_t TBOX_RemTailgateSildeReq           :1;//远程侧滑门/尾门解锁请求
        bits_t TBOX_RemChrgFormSet                :2;//远程充电模式设置
        bits_t TBOX_RemLockCtrlReq                :2;//远程解闭锁请求
        bits_t TBOX_RemFrntDefrstReq              :2;//远程前除霜控制
        bits_t reserved47_47                      :1;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_TBOX_RemReqMsg_Type;

//SRS_Info
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_5                        :6;
        bits_t SRS_AirbagWarningSts               :2;//安全气囊故障状态
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t SRS_CrashOutPutSts                 :1;//碰撞信号
        bits_t reserved17_23                      :7;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t SRS_RollingCounter                 :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t SRS_Checksum                       :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_SRS_InfoMsg_Type;

//CCU_VehGear
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_8                        :1;
        bits_t CCU_ActSymGear                     :3;//实际挡位
        bits_t CCU_ActSymGearVld                  :1;//实际挡位有效性
        bits_t reserved13_15                      :3;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t CCU_116_RollingCounter             :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t CCU_116_Checksum                   :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_CCU_VehGearMsg_Type;

//BMS_RunSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_25                      :2;
        bits_t BMS_ChrgSts                        :2;//充电状态
        bits_t reserved28_31                      :4;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_RunStsMsg_Type;

//CCU_VacInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t CCU_AirP                           :7;//大气压力
        bits_t CCU_AirPValid                      :1;//大气压力有效性
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t CCU_201_RollingCounter             :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t CCU_201_Checksum                   :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_CCU_VacInfoMsg_Type;

//CCU_challenge
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_ChallengeFrame_7               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[1]
        bits_t CCU_ChallengeFrame_6               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[2]
        bits_t CCU_ChallengeFrame_5               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[3]
        bits_t CCU_ChallengeFrame_4               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[4]
        bits_t CCU_ChallengeFrame_3               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[5]
        bits_t CCU_ChallengeFrame_2               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[6]
        bits_t CCU_ChallengeFrame_1               :8;//CCU与TBOX认证密钥，CCU随机数
        //Byte[7]
        bits_t CCU_ChallengeFrame_0               :8;//CCU与TBOX认证密钥，CCU随机数
    }bits;
}CanMatrix_CCU_challengeMsg_Type;

//CCU_Info1
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_OBDSts                         :2;//诊断口连接状态
        bits_t CCU_CcuTboxResp                    :2;//CCU与TBOX认证结果
        bits_t CCU_RemLockFailResn                :1;//远程闭锁失败原因
        bits_t CCU_FindCarVld                     :2;//寻车执行状态
        bits_t CCU_RemForbiddenSts                :1;//远程禁止启动状态
        //Byte[1]
        bits_t CCU_SpdLim1_H                      :8;//车辆限速值
        //Byte[2]
        bits_t CCU_ChrgFormSts                    :1;//充电模式状态
        bits_t reserved17_17                      :1;
        bits_t CCU_SpdLimSts                      :1;//车辆限速状态
        bits_t CCU_SpdLim1_L                      :5;//车辆限速值
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_Info1Msg_Type;

//CCU_LiSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_11                       :4;
        bits_t CCU_DayRunngLiSts                  :1;//日行灯状态
        bits_t CCU_RvsLiSts                       :1;//倒车灯状态
        bits_t CCU_BrkLiSts                       :1;//制动灯状态
        bits_t reserved15_15                      :1;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_LiStsMsg_Type;

//CCU_WinSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_5                        :6;
        bits_t CCU_RainClsWinSts                  :2;//雨天关窗状态
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t CCU_FLWinPosnSts                   :7;//左前车窗位置
        bits_t reserved23_23                      :1;
        //Byte[3]
        bits_t CCU_FRWinPosnSts                   :7;//右前车窗位置
        bits_t reserved31_31                      :1;
        //Byte[4]
        bits_t CCU_RLWinPosnSts                   :7;//左后车窗位置
        bits_t reserved39_39                      :1;
        //Byte[5]
        bits_t CCU_RRWinPosnSts                   :7;//右后车窗位置
        bits_t reserved47_47                      :1;
        //Byte[6]
        bits_t reserved48_49                      :2;
        bits_t CCU_FrntWiprSwtSts                 :3;//前雨刮开关状态
        bits_t reserved53_55                      :3;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_WinStsMsg_Type;

//CCU_MirrSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_MirrSts                        :3;//外后视镜状态
        bits_t reserved3_7                        :5;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t CCU_LeMirrVertPosn                 :7;//左侧外后视镜垂直位置
        bits_t reserved23_23                      :1;
        //Byte[3]
        bits_t CCU_LeMirrHozlPosn                 :7;//左侧外后视镜水平位置
        bits_t reserved31_31                      :1;
        //Byte[4]
        bits_t CCU_RiMirrVertPosn                 :7;//右侧外后视镜垂直位置
        bits_t reserved39_39                      :1;
        //Byte[5]
        bits_t CCU_RiMirrHozlPosn                 :7;//右侧外后视镜水平位置
        bits_t reserved47_47                      :1;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_MirrStsMsg_Type;

//CCU_DoorSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_11                       :4;
        bits_t CCU_VehLockSts                     :1;//车辆解闭锁状态
        bits_t reserved13_13                      :1;
        bits_t CCU_TailgateLockSts                :2;//尾门锁状态
        //Byte[2]
        bits_t reserved16_17                      :2;
        bits_t CCU_LockRsn                        :5;//闭锁原因
        bits_t reserved23_23                      :1;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_DoorStsMsg_Type;

//CCU_VehMod
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_AntiTheftSts                   :2;//车辆防盗状态
        bits_t reserved2_3                        :2;
        bits_t CCU_LoUPwrMod                      :2;//低压电源模式，燃油车有4种电源模式，非燃油车只有OFF，ON
        bits_t reserved6_7                        :2;
        //Byte[1]
        bits_t CCU_VehMod                         :3;//车辆模式
        bits_t CCU_IntelligentChrgnSts            :2;//智能补电状态
        bits_t CCU_IntelligentChrgnExitResn       :3;//智能补电退出原因
        //Byte[2]
        bits_t CCU_LoUBattFltSts                  :1;//蓄电池故障状态
        bits_t CCU_AllwStrtSts                    :1;//允许启动状态
        bits_t CCU_AlcoholConcSnsrFltSts          :1;//酒精浓度传感器故障状态
        bits_t CCU_IntelligentChrgnHVONReq        :1;//智能补电上高压请求
        bits_t reserved20_23                      :4;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_VehModMsg_Type;

//CCU_SeatHeatSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_DrvrSeatHeatSts                :2;//主驾座椅加热状态
        bits_t CCU_DrvrSeatVentnSts               :2;//主驾座椅通风状态
        bits_t reserved4_7                        :4;
        //Byte[1]
        bits_t CCU_PassSeatHeatSts                :2;//副驾座椅加热状态
        bits_t CCU_PassSeatVentnSts               :2;//副驾座椅通风状态
        bits_t reserved12_15                      :4;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_SeatHeatStsMsg_Type;

//CCU_ACSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_AirCdnrWSts                    :1;//空调工作状态
        bits_t CCU_AirCdnrTSts                    :5;//空调温度状态
        bits_t CCU_ExtremeColdSts                 :1;//极速降温状态
        bits_t CCU_ExtremeHotSts                  :1;//极速升温状态
        //Byte[1]
        bits_t CCU_FrntDefrstSts                  :1;//前除霜状态
        bits_t reserved9_15                       :7;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_30                      :7;
        bits_t CCU_AirCdnrACSts                   :1;//空调AC状态
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_40                      :1;
        bits_t CCU_PTCSwtSts                      :1;//PTC开关状态
        bits_t reserved42_47                      :6;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_ACStsMsg_Type;

//CCU_KeyInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_RKEKeyLocn                     :3;//遥控钥匙位置
        bits_t CCU_BLEKeyLocn                     :3;//蓝牙钥匙位置
        bits_t reserved6_7                        :2;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_KeyInfoMsg_Type;

//TBOX_OTAInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_MobileNetTyp                  :3;//移动网络类型
        bits_t TBOX_MobileNetSwtFb                :1;//移动网络开关反馈（预留）
        bits_t TBOX_OTAModSts                     :1;//OTA模式状态
        bits_t TBOX_OTARes                        :2;//OTA升级结果
        bits_t TBOX_OTAModReq                     :1;//OTA进入请求
        //Byte[1]
        bits_t TBOX_MobileNetStr_H                :8;//移动网络信号强度
        //Byte[2]
        bits_t reserved16_21                      :6;
        bits_t TBOX_MobileNetStr_L                :2;//移动网络信号强度
        //Byte[3]
        bits_t TBOX_OTAUpgradeProgs_H             :8;//OTA升级进度
        //Byte[4]
        bits_t reserved32_37                      :6;
        bits_t TBOX_OTAUpgradeProgs_L             :2;//OTA升级进度
        //Byte[5]
        bits_t TBOX_OTALVReq                      :2;//OTA低压上下电请求
        bits_t TBOX_OTAHVReq                      :2;//OTA高压上下电请求
        bits_t TBOX_OTAFailResn                   :4;//OTA失败原因
        //Byte[6]
        bits_t TBOX_210_RollingCounter            :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t TBOX_210_Checksum                  :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_TBOX_OTAInfoMsg_Type;

//TBOX_RemInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_RemSpdLim_H                   :5;//远程车辆限速
        bits_t TBOX_RemWinCtrlReq                 :2;//远程车窗控制请求
        bits_t TBOX_RemPwrCtrlReq                 :1;//远程电源请求
        //Byte[1]
        bits_t TBOX_RemSpdLim_L                   :8;//远程车辆限速
        //Byte[2]
        bits_t TBOX_RemChrgStrtTiHr               :5;//远程预约充电开始时间（小时时刻）
        bits_t TBOX_RemChrgStrtTiVld              :1;//远程预约充电开始时间有效位
        bits_t TBOX_RemBattPreheatSwt             :1;//远程电池预热开关
        bits_t TBOX_RemChrgEna                    :1;//远程预约充电使能
        //Byte[3]
        bits_t TBOX_RemBattPreheatReq             :1;//远程电池预热请求
        bits_t TBOX_RemChrgStrtTiMins             :6;//远程预约充电开始时间（分钟时刻）
        bits_t reserved31_31                      :1;
        //Byte[4]
        bits_t TBOX_RemChrgEndTiHr                :5;//远程预约充电结束时间（小时时刻）
        bits_t TBOX_RemChrgEndTiVld               :1;//远程预约充电结束时间有效位
        bits_t reserved38_39                      :2;
        //Byte[5]
        bits_t TBOX_RemChrgEndTiMins              :6;//远程预约充电结束时间（分钟时刻）
        bits_t reserved46_47                      :2;
        //Byte[6]
        bits_t TBOX_RemTrvlTiHr                   :5;//远程预约出行时间（小时时刻）
        bits_t TBOX_RemTrvlTiVld                  :1;//远程预约出行时间有效位
        bits_t TBOX_RemAuthnStrt                  :1;//远程授权启动
        bits_t reserved55_55                      :1;
        //Byte[7]
        bits_t TBOX_RemTrvlTiHrMins               :6;//远程预约出行时间（分钟时刻）
        bits_t reserved62_63                      :2;
    }bits;
}CanMatrix_TBOX_RemInfoMsg_Type;

//TBOX_Info
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_LongiInfo_3                   :8;//经度
        //Byte[1]
        bits_t TBOX_LongiInfo_2                   :8;//经度
        //Byte[2]
        bits_t TBOX_LongiInfo_1                   :8;//经度
        //Byte[3]
        bits_t TBOX_LongiInfo_0                   :8;//经度
        //Byte[4]
        bits_t TBOX_LatiInfo_3                    :8;//纬度
        //Byte[5]
        bits_t TBOX_LatiInfo_2                    :8;//纬度
        //Byte[6]
        bits_t TBOX_LatiInfo_1                    :8;//纬度
        //Byte[7]
        bits_t TBOX_LatiInfo_0                    :8;//纬度
    }bits;
}CanMatrix_TBOX_InfoMsg_Type;

//TBOX_Info2
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_AltiInfo_H                    :6;//海拔高度
        bits_t reserved6_7                        :2;
        //Byte[1]
        bits_t TBOX_AltiInfo_L                    :8;//海拔高度
        //Byte[2]
        bits_t TBOX_GPSSpd_H                      :8;//GPS速度
        //Byte[3]
        bits_t TBOX_GPSSpd_L                      :8;//GPS速度
        //Byte[4]
        bits_t TBOX_HdAg_H                        :8;//航向角
        //Byte[5]
        bits_t TBOX_HdAg_L                        :8;//航向角
        //Byte[6]
        bits_t TBOX_PosnVld                       :1;//定位有效性
        bits_t VehPosngAuthnSts                   :1;//车辆定位授权状态
        bits_t Tbox_ElectcfenceAlrm               :1;//电子围栏报警
        bits_t reserved51_55                      :5;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_TBOX_Info2Msg_Type;

//CCU_VehHVSt
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t CCU_VehHVSt                        :4;//整车高压状态
        bits_t CCU_VehRdySt                       :1;//车辆可行驶状态
        bits_t reserved21_23                      :3;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_37                      :6;
        bits_t CCU_MntnModSt                      :1;//维修模式状态
        bits_t reserved39_39                      :1;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t CCU_219_RollingCounter             :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t CCU_219_Checksum                   :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_CCU_VehHVStMsg_Type;

//TBOX_EcallInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_ECallSts                      :3;//ECall救援通话状态
        bits_t reserved3_7                        :5;
        //Byte[1]
        bits_t TBOX_EcallTiSec                    :8;//Ecall通话时间_秒
        //Byte[2]
        bits_t TBOX_EcallTiMin                    :8;//Ecall通话时间_分
        //Byte[3]
        bits_t TBOX_EcallTiHr                     :5;//Ecall通话时间_时
        bits_t reserved29_31                      :3;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_TBOX_EcallInfoMsg_Type;

//CCU_ThermMngt
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_1                        :2;
        bits_t CCU_CCUBattThermMngtSts            :2;//电池热管理状态
        bits_t CCU_CCUThermMngtSysFltSts          :1;//热管理系统故障状态
        bits_t reserved5_7                        :3;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t CCU_21F_RollingCounter             :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t CCU_21F_Checksum                   :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_CCU_ThermMngtMsg_Type;

//CCU_PwrInfo2
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_DCDCMaxPwrLim                  :8;//DCDC最大功率限制
        //Byte[1]
        bits_t CCU_DrvMaxPwrLim_H                 :4;//驱动行驶最大允许功率限制
        bits_t reserved12_15                      :4;
        //Byte[2]
        bits_t CCU_DrvMaxPwrLim_L                 :8;//驱动行驶最大允许功率限制
        //Byte[3]
        bits_t CCU_ThermMngtMaxPwrLimHi           :8;//热管理高优先级最大功率限制
        //Byte[4]
        bits_t CCU_ThermMngtMaxPwrLimMid          :8;//热管理中优先级最大功率限制
        //Byte[5]
        bits_t CCU_ThermMngtMaxPwrLimLo           :8;//热管理低优先级最大功率限制
        //Byte[6]
        bits_t CCU_V2XMaxDisChrgPwrLim            :8;//V2X最大放电功率限制
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_PwrInfo2Msg_Type;

//BMS_Status
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_0                        :1;
        bits_t BMS_DisChrgSts                     :2;//放电状态
        bits_t reserved3_7                        :5;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_StatusMsg_Type;

//BMS_InslSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t BMS_SysInsulationR_H               :8;//系统绝缘阻值
        //Byte[3]
        bits_t BMS_SysInsulationR_L               :8;//系统绝缘阻值
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_InslStsMsg_Type;

//BMS_VoltSocInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t BMS_HVBatPackVoltage_H             :8;//动力电池总电压（继电器前电压）
        //Byte[1]
        bits_t BMS_HVBatPackVoltage_L             :8;//动力电池总电压（继电器前电压）
        //Byte[2]
        bits_t BMS_HVBatCurrent_H                 :8;//动力电池实际输入输出电流（负为流入，正为流出）
        //Byte[3]
        bits_t BMS_HVBatCurrent_L                 :8;//动力电池实际输入输出电流（负为流入，正为流出）
        //Byte[4]
        bits_t BMS_HVBatSOC                       :8;//动力电池显示SOC
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_VoltSocInfoMsg_Type;

//BMS_FtList1
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t BMS_FtCellTempDiff                 :2;//单体温差大故障
        bits_t BMS_FtCellOvertemp                 :2;//单体过温故障
        bits_t BMS_FtSysUndervolt                 :2;//母线欠压故障
        bits_t BMS_FtSysOvervolt                  :2;//母线过压故障
        //Byte[1]
        bits_t BMS_FtSocLow                       :2;//SOC低故障
        bits_t BMS_FtCellOvervolt                 :2;//单体过压故障
        bits_t BMS_FtCellUndervolt                :2;//单体欠压故障
        bits_t BMS_InsulationFltSts               :2;//绝缘故障状态
        //Byte[2]
        bits_t BMS_FtCellConsistency              :2;//单体一致性差故障
        bits_t BMS_FtOverchg                      :2;//电池过充故障
        bits_t BMS_FtSocHigh                      :2;//SOC过高故障
        bits_t BMS_FtSocJump                      :1;//SOC跳变故障
        bits_t reserved23_23                      :1;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t BMS_FtNum                          :4;//BMS同时发生的故障数量
        bits_t reserved44_47                      :4;
        //Byte[6]
        bits_t BMS_DTC_H                          :8;//BMS故障码
        //Byte[7]
        bits_t BMS_DTC_L                          :8;//BMS故障码
    }bits;
}CanMatrix_BMS_FtList1Msg_Type;

//BMS_CellVolt2
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t BMS_PmCellVoltN                    :8;//单体电压信息帧序号n
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t BMS_PmCellVolt3N2_H                :8;//第3n-2个单体电压值
        //Byte[3]
        bits_t BMS_PmCellVolt3N2_L                :8;//第3n-2个单体电压值
        //Byte[4]
        bits_t BMS_PmCellVolt3N1_H                :8;//第3n-1个单体电压值
        //Byte[5]
        bits_t BMS_PmCellVolt3N1_L                :8;//第3n-1个单体电压值
        //Byte[6]
        bits_t BMS_PmCellVolt3N_H                 :8;//第3n个单体电压值
        //Byte[7]
        bits_t BMS_PmCellVolt3N_L                 :8;//第3n个单体电压值
    }bits;
}CanMatrix_BMS_CellVolt2Msg_Type;

//BMS_ACChgInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t BMS_ACChrgGunSts                   :1;//慢充枪连接状态
        bits_t reserved57_63                      :7;
    }bits;
}CanMatrix_BMS_ACChgInfoMsg_Type;

//ACU_DCDCSymRun
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t IPU_DCDCRunSt                      :4;//DCDC运行状态
        bits_t reserved4_7                        :4;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_ACU_DCDCSymRunMsg_Type;

//IDCU_Sentry
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t IDCU_SentrySwtFb                   :1;//哨兵模式开关反馈
        bits_t IDCU_SentrySts                     :4;//哨兵模式工作状态
        bits_t IDCU_APPPromptText                 :2;//哨兵模式开启失败手机APP提示
        bits_t reserved7_7                        :1;
        //Byte[1]
        bits_t IDCU_IdcuTboxResp                  :2;//IDCU与TBOX认证结果
        bits_t IDCU_DhuTboxResp                   :2;//DHU与TBOX认证结果
        bits_t IDCU_LocalBattPreheatSwt           :1;//本地电池预热开关
        bits_t IDCU_MobileNetSwt                  :2;//移动网络开关（预留）
        bits_t reserved15_15                      :1;
        //Byte[2]
        bits_t IDCU_LocalTrvlTiHr                 :5;//本地预约出行时间（小时时刻）
        bits_t reserved21_23                      :3;
        //Byte[3]
        bits_t IDCU_LocalTrvlTiHrMins             :6;//本地预约出行时间（分钟时刻）
        bits_t IDCU_LocalTrvlTiVld                :1;//本地预约出行时间有效位
        bits_t reserved31_31                      :1;
        //Byte[4]
        bits_t IDCU_DistractionLvl                :3;//分心报警等级（IC不发）
        bits_t IDCU_DrowsinessLvl                 :3;//疲劳等级（IC不发）
        bits_t IDCU_VehPosngAuthnSwt              :2;//车辆定位授权开关
        //Byte[5]
        bits_t IDCU_SmokingLvl                    :3;//抽烟报警等级（IC不发）
        bits_t IDCU_PhoneLvl                      :3;//打电话报警等级（IC不发）
        bits_t reserved46_46                      :1;
        bits_t IDCU_NavOperSts                    :1;//车机导航运行状态（IC不发）
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_IDCU_SentryMsg_Type;

//IDCU_ChrgTime
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t IDCU_LocalChrgStrtTiHr             :8;//本地预约充电开始时间（小时时刻）
        //Byte[1]
        bits_t IDCU_LocalChrgStrtTiMins           :8;//本地预约充电开始时间（分钟时刻）
        //Byte[2]
        bits_t IDCU_LoacalChrgStrtTiVld           :1;//本地预约充电开始时间有效位
        bits_t reserved17_23                      :7;
        //Byte[3]
        bits_t IDCU_LocalChrgEndTiHr              :8;//本地预约充电结束时间（小时时刻）
        //Byte[4]
        bits_t IDCU_LocalChrgEndTiMins            :8;//本地预约充电结束时间（分钟时刻）
        //Byte[5]
        bits_t IDCU_LoacalChrgEndTiVld            :1;//本地预约充电结束时间有效位
        bits_t reserved41_47                      :7;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_IDCU_ChrgTimeMsg_Type;

//IDCU_Tot
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_6                        :7;
        bits_t IDCU_TotMilgRequest                :1;//累计总里程请求
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_IDCU_TotMsg_Type;

//IDCU_Tot1
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_TotMilg_3                     :8;//累计总里程反馈
        //Byte[1]
        bits_t TBOX_TotMilg_2                     :8;//累计总里程反馈
        //Byte[2]
        bits_t TBOX_TotMilg_1                     :8;//累计总里程反馈
        //Byte[3]
        bits_t TBOX_TotMilg_0                     :8;//累计总里程反馈
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_IDCU_Tot1Msg_Type;

//CCU_LoadCal
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t CCU_LoadCalcn_H                    :8;//载重估算
        //Byte[2]
        bits_t CCU_LoadCalcn_L                    :8;//载重估算
        //Byte[3]
        bits_t CCU_LoadCalcnVld                   :1;//载重估算有效性
        bits_t CCU_VehWorkMod                     :3;//车辆工作模式
        bits_t reserved28_31                      :4;
        //Byte[4]
        bits_t CCU_VehHVSts                       :8;//整车高压运行状态
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_CCU_LoadCalMsg_Type;

//CCU_AutoPark
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t CCU_ExtnRngPwrReq_H                :6;//增程器发电功率请求
        bits_t reserved30_31                      :2;
        //Byte[4]
        bits_t reserved32_33                      :2;
        bits_t CCU_ExtnRngPwrReq_L                :6;//增程器发电功率请求
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t CCU_28E_RollingCounter             :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t CCU_28E_Checksum                   :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_CCU_AutoParkMsg_Type;

//TBOX_Time
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_TiYr_H                        :8;//年
        //Byte[1]
        bits_t TBOX_TiYr_L                        :8;//年
        //Byte[2]
        bits_t TBOX_TiMth                         :8;//月
        //Byte[3]
        bits_t TBOX_TiDay                         :8;//日
        //Byte[4]
        bits_t TBOX_TiHr                          :8;//时
        //Byte[5]
        bits_t TBOX_TIMins                        :8;//分
        //Byte[6]
        bits_t TBOX_TiSec                         :8;//秒
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_TBOX_TimeMsg_Type;

//ADCU_SenInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t ADCU_AdcuTboxResp                  :2;//ADCU与TBOX认证结果
        bits_t reserved2_7                        :6;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_ADCU_SenInfoMsg_Type;

//TBOX_BattHeat
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_TBOXRemBattHeatReq            :2;//远程电池加热请求
        bits_t reserved2_7                        :6;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t TBOX_2C5_RollingCounter            :4;//Message counter for the message 报文计数器
        bits_t reserved52_55                      :4;
        //Byte[7]
        bits_t TBOX_2C5_Checksum                  :8;//CRC8 The checksum is build based on SAE J1850 for byte 0 to Byte 6. polynom:x8+x4+x3+x2+1(0x1D)
    }bits;
}CanMatrix_TBOX_BattHeatMsg_Type;

//ADCU_Challenge
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t ADCU_ChallengeFrame_7              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[1]
        bits_t ADCU_ChallengeFrame_6              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[2]
        bits_t ADCU_ChallengeFrame_5              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[3]
        bits_t ADCU_ChallengeFrame_4              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[4]
        bits_t ADCU_ChallengeFrame_3              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[5]
        bits_t ADCU_ChallengeFrame_2              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[6]
        bits_t ADCU_ChallengeFrame_1              :8;//ADCU与TBOX认证密钥，ADCU随机数
        //Byte[7]
        bits_t ADCU_ChallengeFrame_0              :8;//ADCU与TBOX认证密钥，ADCU随机数
    }bits;
}CanMatrix_ADCU_ChallengeMsg_Type;

//BMS_VCVRemoteSts
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_2                        :3;
        bits_t BMS_VCVSts                         :1;//VCV功能状态
        bits_t reserved4_7                        :4;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_VCVRemoteStsMsg_Type;

//TBOX_VIUCmd
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t Tbox_LockInstructPer               :2;//锁车指令许可
        bits_t Tbox_SoftVersionChangePer          :2;//软件版本切换许可
        bits_t Tbox_LimitSpeedClass               :2;//限速等级
        bits_t Tbox_ChargeConditionJudge          :2;//充电/非充电条件判断
        //Byte[1]
        bits_t Tbox_MaintenanceRe                 :4;//保养提示
        bits_t Tbox_PaymentRe                     :4;//缴费提示
        //Byte[2]
        bits_t Tbox_WhistleAndFlashing            :1;//鸣笛/双闪
        bits_t Tbox_V2VStatus                     :1;//V2V开启/关闭状态
        bits_t Tbox_ReplenishReq                  :1;//补电请求
        bits_t Tbox_BatteryMaintainInd            :1;//动力电池维护指令
        bits_t Tbox_NetworkAnomalyPre             :1;//网络异常提示
        bits_t Tbox_AnnualInspectionAndViolation  :3;//年审违章提示
        //Byte[3]
        bits_t reserved24_25                      :2;
        bits_t Tbox_PowerInterceptCmd             :1;//动力阻断指令
        bits_t reserved27_31                      :5;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t Tbox_GPRSLIFE                      :8;//Gprs2模块LIFE值
    }bits;
}CanMatrix_TBOX_VIUCmdMsg_Type;

//NM_TBOX
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_SourceNodeIdentifier          :8;//节点地址
        //Byte[1]
        bits_t TBOX_RepeatMessageRequestBit       :1;//Repeat Message Request Bit
        bits_t reserved9_11                       :3;
        bits_t TBOX_ActiveWakeupBit               :1;//Active Wakeup Bit
        bits_t reserved13_15                      :3;
        //Byte[2]
        bits_t TBOX_Wakeup_reasons                :8;//Wakeup reasons
        //Byte[3]
        bits_t TBOX_NMSts                         :1;//The bit is used to indicate network management state. It's only used for test.
        bits_t reserved25_31                      :7;
        //Byte[4]
        bits_t TBOX_Stayawake_reasons             :8;//Stayawake reasons
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_NM_TBOXMsg_Type;

//NM_CCU
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t CCU_SourceNodeIdentifier           :8;//节点地址
        //Byte[1]
        bits_t CCU_RepeatMessageRequestBit        :1;//Repeat Message Request Bit
        bits_t reserved9_11                       :3;
        bits_t CCU_ActiveWakeupBit                :1;//Active Wakeup Bit
        bits_t reserved13_15                      :3;
        //Byte[2]
        bits_t CCU_Wakeup_reasons                 :8;//Wakeup reasons
        //Byte[3]
        bits_t CCU_NMSts                          :1;//The bit is used to indicate network management state. It's only used for test.
        bits_t reserved25_31                      :7;
        //Byte[4]
        bits_t CCU_Stayawake_reasons              :8;//Stayawake reasons
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_NM_CCUMsg_Type;

//BMS_CellTemp
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t BMS_CellTempN                      :8;//单体温度信息帧序号
        //Byte[1]
        bits_t BMS_CellTemp6N5                    :8;//第6n-5个单体温度值
        //Byte[2]
        bits_t BMS_CellTemp6N4                    :8;//第6n-4个单体温度值
        //Byte[3]
        bits_t BMS_CellTemp6N3                    :8;//第6n-3个单体温度值
        //Byte[4]
        bits_t BMS_CellTemp6N2                    :8;//第6n-2个单体温度值
        //Byte[5]
        bits_t BMS_CellTemp6N1                    :8;//第6n-1个单体温度值
        //Byte[6]
        bits_t BMS_CellTemp6N                     :8;//第6n个单体温度值
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_CellTempMsg_Type;

//MCU_TempInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t reserved16_23                      :8;
        //Byte[3]
        bits_t reserved24_31                      :8;
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t MCU_DrvMotNrGb                     :2;//驱动电机序号
        bits_t reserved58_63                      :6;
    }bits;
}CanMatrix_MCU_TempInfoMsg_Type;

//BMS_BrtyID
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t BMS_PmReessIdNFrame                :8;//电池编码帧序号
        //Byte[1]
        bits_t BMS_PmReessIdLength                :8;//电池编码总长度值
        //Byte[2]
        bits_t BMS_PmReessId171319                :8;//第6n-5位ID
        //Byte[3]
        bits_t BMS_PmReessId281420                :8;//第6n-4位ID
        //Byte[4]
        bits_t BMS_PmReessId391521                :8;//第6n-3位ID
        //Byte[5]
        bits_t BMS_PmReessId4101622               :8;//第6n-2位ID
        //Byte[6]
        bits_t BMS_PmReessId5111723               :8;//第6n-1位ID
        //Byte[7]
        bits_t BMS_PmReessId6121824               :8;//第6n位ID
    }bits;
}CanMatrix_BMS_BrtyIDMsg_Type;

//BMS_AttInfo
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t reserved0_7                        :8;
        //Byte[1]
        bits_t reserved8_15                       :8;
        //Byte[2]
        bits_t BMS_PmCellSerialNum                :8;//电芯串联数
        //Byte[3]
        bits_t BMS_PmTempProbeNum                 :8;//温度探针数
        //Byte[4]
        bits_t reserved32_39                      :8;
        //Byte[5]
        bits_t reserved40_47                      :8;
        //Byte[6]
        bits_t reserved48_55                      :8;
        //Byte[7]
        bits_t reserved56_63                      :8;
    }bits;
}CanMatrix_BMS_AttInfoMsg_Type;

//Diag_TBOX_Phy_Req
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_Phy_Req_Data0                 :8;
        //Byte[1]
        bits_t TBOX_Phy_Req_Data1                 :8;
        //Byte[2]
        bits_t TBOX_Phy_Req_Data2                 :8;
        //Byte[3]
        bits_t TBOX_Phy_Req_Data3                 :8;
        //Byte[4]
        bits_t TBOX_Phy_Req_Data4                 :8;
        //Byte[5]
        bits_t TBOX_Phy_Req_Data5                 :8;
        //Byte[6]
        bits_t TBOX_Phy_Req_Data6                 :8;
        //Byte[7]
        bits_t TBOX_Phy_Req_Data7                 :8;
    }bits;
}CanMatrix_Diag_TBOX_Phy_ReqMsg_Type;

//Diag_TBOX_Phy_Resp
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t TBOX_Phy_Resp_Data0                :8;
        //Byte[1]
        bits_t TBOX_Phy_Resp_Data1                :8;
        //Byte[2]
        bits_t TBOX_Phy_Resp_Data2                :8;
        //Byte[3]
        bits_t TBOX_Phy_Resp_Data3                :8;
        //Byte[4]
        bits_t TBOX_Phy_Resp_Data4                :8;
        //Byte[5]
        bits_t TBOX_Phy_Resp_Data5                :8;
        //Byte[6]
        bits_t TBOX_Phy_Resp_Data6                :8;
        //Byte[7]
        bits_t TBOX_Phy_Resp_Data7                :8;
    }bits;
}CanMatrix_Diag_TBOX_Phy_RespMsg_Type;

//Diag_Fun_Req
typedef union
{
    uint8_t data[8];
    struct
    {
        //Byte[0]
        bits_t Fun_Req_Data0                      :8;
        //Byte[1]
        bits_t Fun_Req_Data1                      :8;
        //Byte[2]
        bits_t Fun_Req_Data2                      :8;
        //Byte[3]
        bits_t Fun_Req_Data3                      :8;
        //Byte[4]
        bits_t Fun_Req_Data4                      :8;
        //Byte[5]
        bits_t Fun_Req_Data5                      :8;
        //Byte[6]
        bits_t Fun_Req_Data6                      :8;
        //Byte[7]
        bits_t Fun_Req_Data7                      :8;
    }bits;
}CanMatrix_Diag_Fun_ReqMsg_Type;



