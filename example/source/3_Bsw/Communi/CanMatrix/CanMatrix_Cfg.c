/*******************************************************
  * Name    :CanMatrix_Cfg.c
  * Author  :xiaojunzhong
  * Function:TBOX CAN matrix configuration
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
/* Source: CAKY_TBOX_CANFD_TBOX_20261014_182806_V1.5.xlsx,
 * sheet TBOX_TBOX_CANFD. Each message/signal cites its source row below.
 * Def.h is used unchanged. All 55 layouts were checked against matrix bits.
 *
 * 应用表仅含 38 Rx + 12 Tx。2 个 NM 和 3 个诊断报文由专用模块处理。
 * 外部配置需同步：CANNM_ID=0x405，UDS_PHYS_ADDR_ID=0x705，
 * UDS_RESP_ADDR_ID=0x785；UDS_FUNC_ADDR_ID=0x7DF。本文件未修改外部模块。
 * 必须同时替换配套 CanMatrix.c；旧核心不支持此版本的超时与发送行为。
 * 周期报文按 CANFD 规范 7.6 的通信超时规则配置；事件报文不做周期超时。
 * 本规则是通信监测基线，不替代功能特定的信号新鲜度要求。
 
 * 已配置 CCU 关键报文 0x116 的节点状态监测，不假定网关源节点在线。
 * legacy DTC 表仍为 0；CCU 状态通过回调/查询提供，需绑定 DTC 服务。
 * 上层需通过 CanMatrix_SetRxMonitorAllowed 接入上电、供电与恢复门控。
 * 初始监控关闭；RTE 绑定、CRC/计数器验证与更新仍需接入。
 * 信号空白初始值及未定义的保留位暂用 0，已在初始化代码中逐项标注。
 */
/*-------------include file---------------------------*/
#include "CanMatrix_Cfg.h"
#include <string.h>

/*-------------variable statement---------------------*/
/* Legacy table buffer symbols retained to match the reference convention. */
static CanMatrix_DHU_ChallengeMsg_Type g_RxDHU_ChallengeMsg;
static CanMatrix_TBOX_AuthKeyMsg_Type g_TxTBOX_AuthKeyMsg;
static CanMatrix_IDCU_ChallengeMsg_Type g_RxIDCU_ChallengeMsg;
static CanMatrix_TBOX_RemCtlMsg_Type g_TxTBOX_RemCtlMsg;
static CanMatrix_TBOX_RemReqMsg_Type g_TxTBOX_RemReqMsg;
static CanMatrix_SRS_InfoMsg_Type g_RxSRS_InfoMsg;
static CanMatrix_CCU_VehGearMsg_Type g_RxCCU_VehGearMsg;
static CanMatrix_BMS_RunStsMsg_Type g_RxBMS_RunStsMsg;
static CanMatrix_CCU_VacInfoMsg_Type g_RxCCU_VacInfoMsg;
static CanMatrix_CCU_challengeMsg_Type g_RxCCU_challengeMsg;
static CanMatrix_CCU_Info1Msg_Type g_RxCCU_Info1Msg;
static CanMatrix_CCU_LiStsMsg_Type g_RxCCU_LiStsMsg;
static CanMatrix_CCU_WinStsMsg_Type g_RxCCU_WinStsMsg;
static CanMatrix_CCU_MirrStsMsg_Type g_RxCCU_MirrStsMsg;
static CanMatrix_CCU_DoorStsMsg_Type g_RxCCU_DoorStsMsg;
static CanMatrix_CCU_VehModMsg_Type g_RxCCU_VehModMsg;
static CanMatrix_CCU_SeatHeatStsMsg_Type g_RxCCU_SeatHeatStsMsg;
static CanMatrix_CCU_ACStsMsg_Type g_RxCCU_ACStsMsg;
static CanMatrix_CCU_KeyInfoMsg_Type g_RxCCU_KeyInfoMsg;
static CanMatrix_TBOX_OTAInfoMsg_Type g_TxTBOX_OTAInfoMsg;
static CanMatrix_TBOX_RemInfoMsg_Type g_TxTBOX_RemInfoMsg;
static CanMatrix_TBOX_InfoMsg_Type g_TxTBOX_InfoMsg;
static CanMatrix_TBOX_Info2Msg_Type g_TxTBOX_Info2Msg;
static CanMatrix_CCU_VehHVStMsg_Type g_RxCCU_VehHVStMsg;
static CanMatrix_TBOX_EcallInfoMsg_Type g_TxTBOX_EcallInfoMsg;
static CanMatrix_CCU_ThermMngtMsg_Type g_RxCCU_ThermMngtMsg;
static CanMatrix_CCU_PwrInfo2Msg_Type g_RxCCU_PwrInfo2Msg;
static CanMatrix_BMS_StatusMsg_Type g_RxBMS_StatusMsg;
static CanMatrix_BMS_InslStsMsg_Type g_RxBMS_InslStsMsg;
static CanMatrix_BMS_VoltSocInfoMsg_Type g_RxBMS_VoltSocInfoMsg;
static CanMatrix_BMS_FtList1Msg_Type g_RxBMS_FtList1Msg;
static CanMatrix_BMS_CellVolt2Msg_Type g_RxBMS_CellVolt2Msg;
static CanMatrix_BMS_ACChgInfoMsg_Type g_RxBMS_ACChgInfoMsg;
static CanMatrix_ACU_DCDCSymRunMsg_Type g_RxACU_DCDCSymRunMsg;
static CanMatrix_IDCU_SentryMsg_Type g_RxIDCU_SentryMsg;
static CanMatrix_IDCU_ChrgTimeMsg_Type g_RxIDCU_ChrgTimeMsg;
static CanMatrix_IDCU_TotMsg_Type g_RxIDCU_TotMsg;
static CanMatrix_IDCU_Tot1Msg_Type g_TxIDCU_Tot1Msg;
static CanMatrix_CCU_LoadCalMsg_Type g_RxCCU_LoadCalMsg;
static CanMatrix_CCU_AutoParkMsg_Type g_RxCCU_AutoParkMsg;
static CanMatrix_TBOX_TimeMsg_Type g_TxTBOX_TimeMsg;
static CanMatrix_ADCU_SenInfoMsg_Type g_RxADCU_SenInfoMsg;
static CanMatrix_TBOX_BattHeatMsg_Type g_TxTBOX_BattHeatMsg;
static CanMatrix_ADCU_ChallengeMsg_Type g_RxADCU_ChallengeMsg;
static CanMatrix_BMS_VCVRemoteStsMsg_Type g_RxBMS_VCVRemoteStsMsg;
static CanMatrix_TBOX_VIUCmdMsg_Type g_TxTBOX_VIUCmdMsg;
static CanMatrix_BMS_CellTempMsg_Type g_RxBMS_CellTempMsg;
static CanMatrix_MCU_TempInfoMsg_Type g_RxMCU_TempInfoMsg;
static CanMatrix_BMS_BrtyIDMsg_Type g_RxBMS_BrtyIDMsg;
static CanMatrix_BMS_AttInfoMsg_Type g_RxBMS_AttInfoMsg;

/* Event request flags are owned by the single CAN task, not ISR/shared state. */
static Bool s_mCanMatrixEventPending[CANMATRIX_TX_LIST_NUM];

/* Compile-time size checks protect bitfield packing assumptions in Def.h. */
typedef char CanMatrix_SizeCheck_DHU_Challenge[(sizeof(CanMatrix_DHU_ChallengeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_AuthKey[(sizeof(CanMatrix_TBOX_AuthKeyMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_IDCU_Challenge[(sizeof(CanMatrix_IDCU_ChallengeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_RemCtl[(sizeof(CanMatrix_TBOX_RemCtlMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_RemReq[(sizeof(CanMatrix_TBOX_RemReqMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_SRS_Info[(sizeof(CanMatrix_SRS_InfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_VehGear[(sizeof(CanMatrix_CCU_VehGearMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_RunSts[(sizeof(CanMatrix_BMS_RunStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_VacInfo[(sizeof(CanMatrix_CCU_VacInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_challenge[(sizeof(CanMatrix_CCU_challengeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_Info1[(sizeof(CanMatrix_CCU_Info1Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_LiSts[(sizeof(CanMatrix_CCU_LiStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_WinSts[(sizeof(CanMatrix_CCU_WinStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_MirrSts[(sizeof(CanMatrix_CCU_MirrStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_DoorSts[(sizeof(CanMatrix_CCU_DoorStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_VehMod[(sizeof(CanMatrix_CCU_VehModMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_SeatHeatSts[(sizeof(CanMatrix_CCU_SeatHeatStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_ACSts[(sizeof(CanMatrix_CCU_ACStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_KeyInfo[(sizeof(CanMatrix_CCU_KeyInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_OTAInfo[(sizeof(CanMatrix_TBOX_OTAInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_RemInfo[(sizeof(CanMatrix_TBOX_RemInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_Info[(sizeof(CanMatrix_TBOX_InfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_Info2[(sizeof(CanMatrix_TBOX_Info2Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_VehHVSt[(sizeof(CanMatrix_CCU_VehHVStMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_EcallInfo[(sizeof(CanMatrix_TBOX_EcallInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_ThermMngt[(sizeof(CanMatrix_CCU_ThermMngtMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_PwrInfo2[(sizeof(CanMatrix_CCU_PwrInfo2Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_Status[(sizeof(CanMatrix_BMS_StatusMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_InslSts[(sizeof(CanMatrix_BMS_InslStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_VoltSocInfo[(sizeof(CanMatrix_BMS_VoltSocInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_FtList1[(sizeof(CanMatrix_BMS_FtList1Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_CellVolt2[(sizeof(CanMatrix_BMS_CellVolt2Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_ACChgInfo[(sizeof(CanMatrix_BMS_ACChgInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_ACU_DCDCSymRun[(sizeof(CanMatrix_ACU_DCDCSymRunMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_IDCU_Sentry[(sizeof(CanMatrix_IDCU_SentryMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_IDCU_ChrgTime[(sizeof(CanMatrix_IDCU_ChrgTimeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_IDCU_Tot[(sizeof(CanMatrix_IDCU_TotMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_IDCU_Tot1[(sizeof(CanMatrix_IDCU_Tot1Msg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_LoadCal[(sizeof(CanMatrix_CCU_LoadCalMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_CCU_AutoPark[(sizeof(CanMatrix_CCU_AutoParkMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_Time[(sizeof(CanMatrix_TBOX_TimeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_ADCU_SenInfo[(sizeof(CanMatrix_ADCU_SenInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_BattHeat[(sizeof(CanMatrix_TBOX_BattHeatMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_ADCU_Challenge[(sizeof(CanMatrix_ADCU_ChallengeMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_VCVRemoteSts[(sizeof(CanMatrix_BMS_VCVRemoteStsMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_TBOX_VIUCmd[(sizeof(CanMatrix_TBOX_VIUCmdMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_CellTemp[(sizeof(CanMatrix_BMS_CellTempMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_MCU_TempInfo[(sizeof(CanMatrix_MCU_TempInfoMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_BrtyID[(sizeof(CanMatrix_BMS_BrtyIDMsg_Type) == 8) ? 1 : -1];
typedef char CanMatrix_SizeCheck_BMS_AttInfo[(sizeof(CanMatrix_BMS_AttInfoMsg_Type) == 8) ? 1 : -1];

/*******************************************************
  * Name    :CanMatrix_DefaultDataInit
  * Author  :xiaojunzhong
  * Function:Initialize all application buffers from matrix initial values
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
void CanMatrix_DefaultDataInit(void)
{
    uint8_t i;

    for (i = 0; i < CANMATRIX_RX_LIST_NUM; i++)
    {
        CanMatrix_Fill(g_CanMatrixRxList[i].p_buff, 0x00, g_CanMatrixRxList[i].len);
    }
    for (i = 0; i < CANMATRIX_TX_LIST_NUM; i++)
    {
        CanMatrix_Fill(g_CanMatrixTxList[i].p_buff, 0x00, g_CanMatrixTxList[i].len);
        s_mCanMatrixEventPending[i] = FALSE;
    }
    /* Unspecified reserved bits remain zero. Values below are raw, not physical.
     * For 48/64-bit challenge/key fields, numeric 0xFFFF sets the low 16 bits,
     * rather than filling the entire signal with 0xFF.
     */

    /* DHU_Challenge; message row 3. */
    /* Row 4: DHU_ChallengeFrame; initial=0xFFFF. */
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_7 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_6 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_5 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_4 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_3 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_2 = 0x00;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_1 = 0xFF;
    g_RxDHU_ChallengeMsg.bits.DHU_ChallengeFrame_0 = 0xFF;

    /* TBOX_AuthKey; message row 6. */
    /* Row 7: TBOX_AuthKeyData; initial=0xFFFF. */
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_5 = 0x00;
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_4 = 0x00;
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_3 = 0x00;
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_2 = 0x00;
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_1 = 0xFF;
    g_TxTBOX_AuthKeyMsg.bits.TBOX_AuthKeyData_0 = 0xFF;

    /* IDCU_Challenge; message row 9. */
    /* Row 10: IDCU_ChallengeFrame; initial=0xFFFF. */
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_7 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_6 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_5 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_4 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_3 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_2 = 0x00;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_1 = 0xFF;
    g_RxIDCU_ChallengeMsg.bits.IDCU_ChallengeFrame_0 = 0xFF;

    /* TBOX_RemCtl; message row 12. */
    /* Row 13: TBOX_AuthResponeSts; initial=0xFF. */
    g_TxTBOX_RemCtlMsg.bits.TBOX_AuthResponeSts = 0xFF;
    /* Row 14: TBOX_RemSentrySwt; initial=0x0. */
    g_TxTBOX_RemCtlMsg.bits.TBOX_RemSentrySwt = 0x00;
    /* Row 15: TBOX_BattAntidisassembly; initial=0x0. */
    g_TxTBOX_RemCtlMsg.bits.TBOX_BattAntidisassembly = 0x00;
    /* Row 16: TBOX_RemCarSearchReq; initial=0x0. */
    g_TxTBOX_RemCtlMsg.bits.TBOX_RemCarSearchReq = 0x00;

    /* TBOX_RemReq; message row 18. */
    /* Row 19: TBOX_RemACOnOffReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemACOnOffReq = 0x00;
    /* Row 20: TBOX_RemAcTempCSet; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemAcTempCSet = 0x00;
    /* Row 21: TBOX_RemExtremeColdReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemExtremeColdReq = 0x00;
    /* Row 22: TBOX_RemExtremeHotReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemExtremeHotReq = 0x00;
    /* Row 23: TBOX_RemAirColdReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemAirColdReq = 0x00;
    /* Row 24: TBOX_RemAirHeatReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemAirHeatReq = 0x00;
    /* Row 25: TBOX_RemForbiddenCtrl; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemForbiddenCtrl = 0x00;
    /* Row 26: TBOX_RemSpdLimReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemSpdLimReq = 0x00;
    /* Row 27: TBOX_RemDrvrSeatHeatReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemDrvrSeatHeatReq = 0x00;
    /* Row 28: TBOX_RemDrvrSeatVentnReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemDrvrSeatVentnReq = 0x00;
    /* Row 29: TBOX_RemPassSeatHeatReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemPassSeatHeatReq = 0x00;
    /* Row 30: TBOX_RemPassSeatVentnReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemPassSeatVentnReq = 0x00;
    /* Row 31: TBOX_RemTailgateSildeReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemTailgateSildeReq = 0x00;
    /* Row 32: TBOX_RemChrgFormSet; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemChrgFormSet = 0x00;
    /* Row 33: TBOX_RemLockCtrlReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemLockCtrlReq = 0x00;
    /* Row 34: TBOX_RemFrntDefrstReq; initial=0x0. */
    g_TxTBOX_RemReqMsg.bits.TBOX_RemFrntDefrstReq = 0x00;

    /* SRS_Info; message row 36. */
    /* Row 37: SRS_AirbagWarningSts; initial=0x0. */
    g_RxSRS_InfoMsg.bits.SRS_AirbagWarningSts = 0x00;
    /* Row 38: SRS_CrashOutPutSts; initial=0x0. */
    g_RxSRS_InfoMsg.bits.SRS_CrashOutPutSts = 0x00;
    /* Row 39: SRS_RollingCounter; initial=0x0. */
    g_RxSRS_InfoMsg.bits.SRS_RollingCounter = 0x00;
    /* Row 40: SRS_Checksum; initial=0x0. */
    g_RxSRS_InfoMsg.bits.SRS_Checksum = 0x00;

    /* CCU_VehGear; message row 42. */
    /* Row 43: CCU_ActSymGear; initial=0x0. */
    g_RxCCU_VehGearMsg.bits.CCU_ActSymGear = 0x00;
    /* Row 44: CCU_ActSymGearVld; initial=0x0. */
    g_RxCCU_VehGearMsg.bits.CCU_ActSymGearVld = 0x00;
    /* Row 45: CCU_116_RollingCounter; initial=0x0. */
    g_RxCCU_VehGearMsg.bits.CCU_116_RollingCounter = 0x00;
    /* Row 46: CCU_116_Checksum; initial=0x0. */
    g_RxCCU_VehGearMsg.bits.CCU_116_Checksum = 0x00;

    /* BMS_RunSts; message row 48. */
    /* Row 49: BMS_ChrgSts; initial=0x0. */
    g_RxBMS_RunStsMsg.bits.BMS_ChrgSts = 0x00;

    /* CCU_VacInfo; message row 51. */
    /* Row 52: CCU_AirP; initial cell blank, temporary raw zero. */
    g_RxCCU_VacInfoMsg.bits.CCU_AirP = 0x00;
    /* Row 53: CCU_AirPValid; initial=0x0. */
    g_RxCCU_VacInfoMsg.bits.CCU_AirPValid = 0x00;
    /* Row 54: CCU_201_RollingCounter; initial=0x0. */
    g_RxCCU_VacInfoMsg.bits.CCU_201_RollingCounter = 0x00;
    /* Row 55: CCU_201_Checksum; initial=0x0. */
    g_RxCCU_VacInfoMsg.bits.CCU_201_Checksum = 0x00;

    /* CCU_challenge; message row 57. */
    /* Row 58: CCU_ChallengeFrame; initial=0xFFFF. */
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_7 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_6 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_5 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_4 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_3 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_2 = 0x00;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_1 = 0xFF;
    g_RxCCU_challengeMsg.bits.CCU_ChallengeFrame_0 = 0xFF;

    /* CCU_Info1; message row 60. */
    /* Row 61: CCU_OBDSts; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_OBDSts = 0x00;
    /* Row 62: CCU_CcuTboxResp; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_CcuTboxResp = 0x00;
    /* Row 63: CCU_RemLockFailResn; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_RemLockFailResn = 0x00;
    /* Row 64: CCU_FindCarVld; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_FindCarVld = 0x00;
    /* Row 65: CCU_RemForbiddenSts; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_RemForbiddenSts = 0x00;
    /* Row 66: CCU_ChrgFormSts; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_ChrgFormSts = 0x00;
    /* Row 67: CCU_SpdLimSts; initial=0x0. */
    g_RxCCU_Info1Msg.bits.CCU_SpdLimSts = 0x00;
    /* Row 68: CCU_SpdLim1; initial=0x12C0. */
    g_RxCCU_Info1Msg.bits.CCU_SpdLim1_H = 0x96;
    g_RxCCU_Info1Msg.bits.CCU_SpdLim1_L = 0x00;

    /* CCU_LiSts; message row 70. */
    /* Row 71: CCU_DayRunngLiSts; initial=0x0. */
    g_RxCCU_LiStsMsg.bits.CCU_DayRunngLiSts = 0x00;
    /* Row 72: CCU_RvsLiSts; initial=0x0. */
    g_RxCCU_LiStsMsg.bits.CCU_RvsLiSts = 0x00;
    /* Row 73: CCU_BrkLiSts; initial=0x0. */
    g_RxCCU_LiStsMsg.bits.CCU_BrkLiSts = 0x00;

    /* CCU_WinSts; message row 75. */
    /* Row 76: CCU_RainClsWinSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_RainClsWinSts = 0x00;
    /* Row 77: CCU_FLWinPosnSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_FLWinPosnSts = 0x00;
    /* Row 78: CCU_FRWinPosnSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_FRWinPosnSts = 0x00;
    /* Row 79: CCU_RLWinPosnSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_RLWinPosnSts = 0x00;
    /* Row 80: CCU_RRWinPosnSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_RRWinPosnSts = 0x00;
    /* Row 81: CCU_FrntWiprSwtSts; initial=0x0. */
    g_RxCCU_WinStsMsg.bits.CCU_FrntWiprSwtSts = 0x00;

    /* CCU_MirrSts; message row 83. */
    /* Row 84: CCU_MirrSts; initial=0x0. */
    g_RxCCU_MirrStsMsg.bits.CCU_MirrSts = 0x00;
    /* Row 85: CCU_LeMirrVertPosn; initial=0x7E. */
    g_RxCCU_MirrStsMsg.bits.CCU_LeMirrVertPosn = 0x7E;
    /* Row 86: CCU_LeMirrHozlPosn; initial=0x7E. */
    g_RxCCU_MirrStsMsg.bits.CCU_LeMirrHozlPosn = 0x7E;
    /* Row 87: CCU_RiMirrVertPosn; initial=0x7E. */
    g_RxCCU_MirrStsMsg.bits.CCU_RiMirrVertPosn = 0x7E;
    /* Row 88: CCU_RiMirrHozlPosn; initial=0x7E. */
    g_RxCCU_MirrStsMsg.bits.CCU_RiMirrHozlPosn = 0x7E;

    /* CCU_DoorSts; message row 90. */
    /* Row 91: CCU_VehLockSts; initial=0x0. */
    g_RxCCU_DoorStsMsg.bits.CCU_VehLockSts = 0x00;
    /* Row 92: CCU_TailgateLockSts; initial=0x0. */
    g_RxCCU_DoorStsMsg.bits.CCU_TailgateLockSts = 0x00;
    /* Row 93: CCU_LockRsn; initial=0x0. */
    g_RxCCU_DoorStsMsg.bits.CCU_LockRsn = 0x00;

    /* CCU_VehMod; message row 95. */
    /* Row 96: CCU_AntiTheftSts; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_AntiTheftSts = 0x00;
    /* Row 97: CCU_LoUPwrMod; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_LoUPwrMod = 0x00;
    /* Row 98: CCU_VehMod; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_VehMod = 0x00;
    /* Row 99: CCU_IntelligentChrgnSts; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_IntelligentChrgnSts = 0x00;
    /* Row 100: CCU_IntelligentChrgnExitResn; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_IntelligentChrgnExitResn = 0x00;
    /* Row 101: CCU_LoUBattFltSts; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_LoUBattFltSts = 0x00;
    /* Row 102: CCU_AllwStrtSts; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_AllwStrtSts = 0x00;
    /* Row 103: CCU_AlcoholConcSnsrFltSts; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_AlcoholConcSnsrFltSts = 0x00;
    /* Row 104: CCU_IntelligentChrgnHVONReq; initial=0x0. */
    g_RxCCU_VehModMsg.bits.CCU_IntelligentChrgnHVONReq = 0x00;

    /* CCU_SeatHeatSts; message row 106. */
    /* Row 107: CCU_DrvrSeatHeatSts; initial=0x0. */
    g_RxCCU_SeatHeatStsMsg.bits.CCU_DrvrSeatHeatSts = 0x00;
    /* Row 108: CCU_DrvrSeatVentnSts; initial=0x0. */
    g_RxCCU_SeatHeatStsMsg.bits.CCU_DrvrSeatVentnSts = 0x00;
    /* Row 109: CCU_PassSeatHeatSts; initial=0x0. */
    g_RxCCU_SeatHeatStsMsg.bits.CCU_PassSeatHeatSts = 0x00;
    /* Row 110: CCU_PassSeatVentnSts; initial=0x0. */
    g_RxCCU_SeatHeatStsMsg.bits.CCU_PassSeatVentnSts = 0x00;

    /* CCU_ACSts; message row 112. */
    /* Row 113: CCU_AirCdnrWSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_AirCdnrWSts = 0x00;
    /* Row 114: CCU_AirCdnrTSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_AirCdnrTSts = 0x00;
    /* Row 115: CCU_ExtremeColdSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_ExtremeColdSts = 0x00;
    /* Row 116: CCU_ExtremeHotSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_ExtremeHotSts = 0x00;
    /* Row 117: CCU_FrntDefrstSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_FrntDefrstSts = 0x00;
    /* Row 118: CCU_AirCdnrACSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_AirCdnrACSts = 0x00;
    /* Row 119: CCU_PTCSwtSts; initial=0x0. */
    g_RxCCU_ACStsMsg.bits.CCU_PTCSwtSts = 0x00;

    /* CCU_KeyInfo; message row 121. */
    /* Row 122: CCU_RKEKeyLocn; initial=0x0. */
    g_RxCCU_KeyInfoMsg.bits.CCU_RKEKeyLocn = 0x00;
    /* Row 123: CCU_BLEKeyLocn; initial=0x0. */
    g_RxCCU_KeyInfoMsg.bits.CCU_BLEKeyLocn = 0x00;

    /* TBOX_OTAInfo; message row 125. */
    /* Row 126: TBOX_MobileNetTyp; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_MobileNetTyp = 0x00;
    /* Row 127: TBOX_MobileNetSwtFb; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_MobileNetSwtFb = 0x00;
    /* Row 128: TBOX_OTAModSts; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAModSts = 0x00;
    /* Row 129: TBOX_OTARes; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTARes = 0x00;
    /* Row 130: TBOX_OTAModReq; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAModReq = 0x00;
    /* Row 131: TBOX_MobileNetStr; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_MobileNetStr_H = 0x00;
    g_TxTBOX_OTAInfoMsg.bits.TBOX_MobileNetStr_L = 0x00;
    /* Row 132: TBOX_OTAUpgradeProgs; initial cell blank, temporary raw zero. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAUpgradeProgs_H = 0x00;
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAUpgradeProgs_L = 0x00;
    /* Row 133: TBOX_OTALVReq; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTALVReq = 0x00;
    /* Row 134: TBOX_OTAHVReq; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAHVReq = 0x00;
    /* Row 135: TBOX_OTAFailResn; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_OTAFailResn = 0x00;
    /* Row 136: TBOX_210_RollingCounter; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_210_RollingCounter = 0x00;
    /* Row 137: TBOX_210_Checksum; initial=0x0. */
    g_TxTBOX_OTAInfoMsg.bits.TBOX_210_Checksum = 0x00;

    /* TBOX_RemInfo; message row 139. */
    /* Row 140: TBOX_RemWinCtrlReq; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemWinCtrlReq = 0x00;
    /* Row 141: TBOX_RemPwrCtrlReq; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemPwrCtrlReq = 0x00;
    /* Row 142: TBOX_RemSpdLim; initial=0x12C0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemSpdLim_H = 0x12;
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemSpdLim_L = 0xC0;
    /* Row 143: TBOX_RemChrgStrtTiHr; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgStrtTiHr = 0x00;
    /* Row 144: TBOX_RemChrgStrtTiVld; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgStrtTiVld = 0x00;
    /* Row 145: TBOX_RemBattPreheatSwt; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemBattPreheatSwt = 0x00;
    /* Row 146: TBOX_RemChrgEna; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgEna = 0x00;
    /* Row 147: TBOX_RemBattPreheatReq; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemBattPreheatReq = 0x00;
    /* Row 148: TBOX_RemChrgStrtTiMins; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgStrtTiMins = 0x00;
    /* Row 149: TBOX_RemChrgEndTiHr; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgEndTiHr = 0x00;
    /* Row 150: TBOX_RemChrgEndTiVld; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgEndTiVld = 0x00;
    /* Row 151: TBOX_RemChrgEndTiMins; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemChrgEndTiMins = 0x00;
    /* Row 152: TBOX_RemTrvlTiHr; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemTrvlTiHr = 0x00;
    /* Row 153: TBOX_RemTrvlTiVld; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemTrvlTiVld = 0x00;
    /* Row 154: TBOX_RemAuthnStrt; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemAuthnStrt = 0x00;
    /* Row 155: TBOX_RemTrvlTiHrMins; initial=0x0. */
    g_TxTBOX_RemInfoMsg.bits.TBOX_RemTrvlTiHrMins = 0x00;

    /* TBOX_Info; message row 157. */
    /* Row 158: TBOX_LongiInfo; initial=0xFFFFFFFF. */
    g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_3 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_2 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_1 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_0 = 0xFF;
    /* Row 159: TBOX_LatiInfo; initial=0xFFFFFFFF. */
    g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_3 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_2 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_1 = 0xFF;
    g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_0 = 0xFF;

    /* TBOX_Info2; message row 161. */
    /* Row 162: TBOX_AltiInfo; initial=0x0. */
    g_TxTBOX_Info2Msg.bits.TBOX_AltiInfo_H = 0x00;
    g_TxTBOX_Info2Msg.bits.TBOX_AltiInfo_L = 0x00;
    /* Row 163: TBOX_GPSSpd; initial=0xFFFF. */
    g_TxTBOX_Info2Msg.bits.TBOX_GPSSpd_H = 0xFF;
    g_TxTBOX_Info2Msg.bits.TBOX_GPSSpd_L = 0xFF;
    /* Row 164: TBOX_HdAg; initial=0x8C00. */
    g_TxTBOX_Info2Msg.bits.TBOX_HdAg_H = 0x8C;
    g_TxTBOX_Info2Msg.bits.TBOX_HdAg_L = 0x00;
    /* Row 165: TBOX_PosnVld; initial=0x0. */
    g_TxTBOX_Info2Msg.bits.TBOX_PosnVld = 0x00;
    /* Row 166: VehPosngAuthnSts; initial=0x0. */
    g_TxTBOX_Info2Msg.bits.VehPosngAuthnSts = 0x00;
    /* Row 167: Tbox_ElectcfenceAlrm; initial cell blank, temporary raw zero. */
    g_TxTBOX_Info2Msg.bits.Tbox_ElectcfenceAlrm = 0x00;

    /* CCU_VehHVSt; message row 169. */
    /* Row 170: CCU_VehHVSt; initial=0x0. */
    g_RxCCU_VehHVStMsg.bits.CCU_VehHVSt = 0x00;
    /* Row 171: CCU_VehRdySt; initial=0x0. */
    g_RxCCU_VehHVStMsg.bits.CCU_VehRdySt = 0x00;
    /* Row 172: CCU_MntnModSt; initial=0x0. */
    g_RxCCU_VehHVStMsg.bits.CCU_MntnModSt = 0x00;
    /* Row 173: CCU_219_RollingCounter; initial=0x0. */
    g_RxCCU_VehHVStMsg.bits.CCU_219_RollingCounter = 0x00;
    /* Row 174: CCU_219_Checksum; initial=0x0. */
    g_RxCCU_VehHVStMsg.bits.CCU_219_Checksum = 0x00;

    /* TBOX_EcallInfo; message row 176. */
    /* Row 177: TBOX_ECallSts; initial=0x0. */
    g_TxTBOX_EcallInfoMsg.bits.TBOX_ECallSts = 0x00;
    /* Row 178: TBOX_EcallTiSec; initial=0x0. */
    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiSec = 0x00;
    /* Row 179: TBOX_EcallTiMin; initial=0x0. */
    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiMin = 0x00;
    /* Row 180: TBOX_EcallTiHr; initial=0x0. */
    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiHr = 0x00;

    /* CCU_ThermMngt; message row 182. */
    /* Row 183: CCU_CCUBattThermMngtSts; initial=0x0. */
    g_RxCCU_ThermMngtMsg.bits.CCU_CCUBattThermMngtSts = 0x00;
    /* Row 184: CCU_CCUThermMngtSysFltSts; initial=0x0. */
    g_RxCCU_ThermMngtMsg.bits.CCU_CCUThermMngtSysFltSts = 0x00;
    /* Row 185: CCU_21F_RollingCounter; initial=0x0. */
    g_RxCCU_ThermMngtMsg.bits.CCU_21F_RollingCounter = 0x00;
    /* Row 186: CCU_21F_Checksum; initial=0x0. */
    g_RxCCU_ThermMngtMsg.bits.CCU_21F_Checksum = 0x00;

    /* CCU_PwrInfo2; message row 188. */
    /* Row 189: CCU_DCDCMaxPwrLim; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_DCDCMaxPwrLim = 0x00;
    /* Row 190: CCU_DrvMaxPwrLim; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_DrvMaxPwrLim_H = 0x00;
    g_RxCCU_PwrInfo2Msg.bits.CCU_DrvMaxPwrLim_L = 0x00;
    /* Row 191: CCU_ThermMngtMaxPwrLimHi; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_ThermMngtMaxPwrLimHi = 0x00;
    /* Row 192: CCU_ThermMngtMaxPwrLimMid; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_ThermMngtMaxPwrLimMid = 0x00;
    /* Row 193: CCU_ThermMngtMaxPwrLimLo; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_ThermMngtMaxPwrLimLo = 0x00;
    /* Row 194: CCU_V2XMaxDisChrgPwrLim; initial=0x0. */
    g_RxCCU_PwrInfo2Msg.bits.CCU_V2XMaxDisChrgPwrLim = 0x00;

    /* BMS_Status; message row 196. */
    /* Row 197: BMS_DisChrgSts; initial=0x0. */
    g_RxBMS_StatusMsg.bits.BMS_DisChrgSts = 0x00;

    /* BMS_InslSts; message row 199. */
    /* Row 200: BMS_SysInsulationR; initial=0x0. */
    g_RxBMS_InslStsMsg.bits.BMS_SysInsulationR_H = 0x00;
    g_RxBMS_InslStsMsg.bits.BMS_SysInsulationR_L = 0x00;

    /* BMS_VoltSocInfo; message row 202. */
    /* Row 203: BMS_HVBatPackVoltage; initial=0x0. */
    g_RxBMS_VoltSocInfoMsg.bits.BMS_HVBatPackVoltage_H = 0x00;
    g_RxBMS_VoltSocInfoMsg.bits.BMS_HVBatPackVoltage_L = 0x00;
    /* Row 204: BMS_HVBatCurrent; initial=0x2710. */
    g_RxBMS_VoltSocInfoMsg.bits.BMS_HVBatCurrent_H = 0x27;
    g_RxBMS_VoltSocInfoMsg.bits.BMS_HVBatCurrent_L = 0x10;
    /* Row 205: BMS_HVBatSOC; initial=0x0. */
    g_RxBMS_VoltSocInfoMsg.bits.BMS_HVBatSOC = 0x00;

    /* BMS_FtList1; message row 207. */
    /* Row 208: BMS_FtCellTempDiff; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtCellTempDiff = 0x00;
    /* Row 209: BMS_FtCellOvertemp; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtCellOvertemp = 0x00;
    /* Row 210: BMS_FtSysUndervolt; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtSysUndervolt = 0x00;
    /* Row 211: BMS_FtSysOvervolt; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtSysOvervolt = 0x00;
    /* Row 212: BMS_FtSocLow; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtSocLow = 0x00;
    /* Row 213: BMS_FtCellOvervolt; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtCellOvervolt = 0x00;
    /* Row 214: BMS_FtCellUndervolt; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtCellUndervolt = 0x00;
    /* Row 215: BMS_InsulationFltSts; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_InsulationFltSts = 0x00;
    /* Row 216: BMS_FtCellConsistency; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtCellConsistency = 0x00;
    /* Row 217: BMS_FtOverchg; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtOverchg = 0x00;
    /* Row 218: BMS_FtSocHigh; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtSocHigh = 0x00;
    /* Row 219: BMS_FtSocJump; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtSocJump = 0x00;
    /* Row 220: BMS_FtNum; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_FtNum = 0x00;
    /* Row 221: BMS_DTC; initial=0x0. */
    g_RxBMS_FtList1Msg.bits.BMS_DTC_H = 0x00;
    g_RxBMS_FtList1Msg.bits.BMS_DTC_L = 0x00;

    /* BMS_CellVolt2; message row 223. */
    /* Row 224: BMS_PmCellVoltN; initial=0x1. */
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVoltN = 0x01;
    /* Row 225: BMS_PmCellVolt3N2; initial=0x0. */
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N2_H = 0x00;
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N2_L = 0x00;
    /* Row 226: BMS_PmCellVolt3N1; initial=0x0. */
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N1_H = 0x00;
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N1_L = 0x00;
    /* Row 227: BMS_PmCellVolt3N; initial=0x0. */
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N_H = 0x00;
    g_RxBMS_CellVolt2Msg.bits.BMS_PmCellVolt3N_L = 0x00;

    /* BMS_ACChgInfo; message row 229. */
    /* Row 230: BMS_ACChrgGunSts; initial cell blank, temporary raw zero. */
    g_RxBMS_ACChgInfoMsg.bits.BMS_ACChrgGunSts = 0x00;

    /* ACU_DCDCSymRun; message row 232. */
    /* Row 233: IPU_DCDCRunSt; initial=0x0. */
    g_RxACU_DCDCSymRunMsg.bits.IPU_DCDCRunSt = 0x00;

    /* IDCU_Sentry; message row 235. */
    /* Row 236: IDCU_SentrySwtFb; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_SentrySwtFb = 0x00;
    /* Row 237: IDCU_SentrySts; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_SentrySts = 0x00;
    /* Row 238: IDCU_APPPromptText; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_APPPromptText = 0x00;
    /* Row 239: IDCU_IdcuTboxResp; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_IdcuTboxResp = 0x00;
    /* Row 240: IDCU_DhuTboxResp; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_DhuTboxResp = 0x00;
    /* Row 241: IDCU_LocalBattPreheatSwt; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_LocalBattPreheatSwt = 0x00;
    /* Row 242: IDCU_MobileNetSwt; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_MobileNetSwt = 0x00;
    /* Row 243: IDCU_LocalTrvlTiHr; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_LocalTrvlTiHr = 0x00;
    /* Row 244: IDCU_LocalTrvlTiHrMins; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_LocalTrvlTiHrMins = 0x00;
    /* Row 245: IDCU_LocalTrvlTiVld; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_LocalTrvlTiVld = 0x00;
    /* Row 246: IDCU_DistractionLvl; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_DistractionLvl = 0x00;
    /* Row 247: IDCU_DrowsinessLvl; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_DrowsinessLvl = 0x00;
    /* Row 248: IDCU_VehPosngAuthnSwt; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_VehPosngAuthnSwt = 0x00;
    /* Row 249: IDCU_SmokingLvl; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_SmokingLvl = 0x00;
    /* Row 250: IDCU_PhoneLvl; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_PhoneLvl = 0x00;
    /* Row 251: IDCU_NavOperSts; initial=0x0. */
    g_RxIDCU_SentryMsg.bits.IDCU_NavOperSts = 0x00;

    /* IDCU_ChrgTime; message row 253. */
    /* Row 254: IDCU_LocalChrgStrtTiHr; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LocalChrgStrtTiHr = 0x00;
    /* Row 255: IDCU_LocalChrgStrtTiMins; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LocalChrgStrtTiMins = 0x00;
    /* Row 256: IDCU_LoacalChrgStrtTiVld; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LoacalChrgStrtTiVld = 0x00;
    /* Row 257: IDCU_LocalChrgEndTiHr; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LocalChrgEndTiHr = 0x00;
    /* Row 258: IDCU_LocalChrgEndTiMins; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LocalChrgEndTiMins = 0x00;
    /* Row 259: IDCU_LoacalChrgEndTiVld; initial=0x0. */
    g_RxIDCU_ChrgTimeMsg.bits.IDCU_LoacalChrgEndTiVld = 0x00;

    /* IDCU_Tot; message row 261. */
    /* Row 262: IDCU_TotMilgRequest; initial=0x0. */
    g_RxIDCU_TotMsg.bits.IDCU_TotMilgRequest = 0x00;

    /* IDCU_Tot1; message row 264. */
    /* Row 265: TBOX_TotMilg; initial=0x0. */
    g_TxIDCU_Tot1Msg.bits.TBOX_TotMilg_3 = 0x00;
    g_TxIDCU_Tot1Msg.bits.TBOX_TotMilg_2 = 0x00;
    g_TxIDCU_Tot1Msg.bits.TBOX_TotMilg_1 = 0x00;
    g_TxIDCU_Tot1Msg.bits.TBOX_TotMilg_0 = 0x00;

    /* CCU_LoadCal; message row 267. */
    /* Row 268: CCU_LoadCalcn; initial=0x0. */
    g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcn_H = 0x00;
    g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcn_L = 0x00;
    /* Row 269: CCU_LoadCalcnVld; initial=0x0. */
    g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcnVld = 0x00;
    /* Row 270: CCU_VehWorkMod; initial=0x0. */
    g_RxCCU_LoadCalMsg.bits.CCU_VehWorkMod = 0x00;
    /* Row 271: CCU_VehHVSts; initial=0x0. */
    g_RxCCU_LoadCalMsg.bits.CCU_VehHVSts = 0x00;

    /* CCU_AutoPark; message row 273. */
    /* Row 274: CCU_ExtnRngPwrReq; initial=0x0. */
    g_RxCCU_AutoParkMsg.bits.CCU_ExtnRngPwrReq_H = 0x00;
    g_RxCCU_AutoParkMsg.bits.CCU_ExtnRngPwrReq_L = 0x00;
    /* Row 275: CCU_28E_RollingCounter; initial=0x0. */
    g_RxCCU_AutoParkMsg.bits.CCU_28E_RollingCounter = 0x00;
    /* Row 276: CCU_28E_Checksum; initial=0x0. */
    g_RxCCU_AutoParkMsg.bits.CCU_28E_Checksum = 0x00;

    /* TBOX_Time; message row 278. */
    /* Row 279: TBOX_TiYr; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TiYr_H = 0x00;
    g_TxTBOX_TimeMsg.bits.TBOX_TiYr_L = 0x00;
    /* Row 280: TBOX_TiMth; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TiMth = 0x00;
    /* Row 281: TBOX_TiDay; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TiDay = 0x00;
    /* Row 282: TBOX_TiHr; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TiHr = 0x00;
    /* Row 283: TBOX_TIMins; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TIMins = 0x00;
    /* Row 284: TBOX_TiSec; initial=0x0. */
    g_TxTBOX_TimeMsg.bits.TBOX_TiSec = 0x00;

    /* ADCU_SenInfo; message row 286. */
    /* Row 287: ADCU_AdcuTboxResp; initial=0x0. */
    g_RxADCU_SenInfoMsg.bits.ADCU_AdcuTboxResp = 0x00;

    /* TBOX_BattHeat; message row 289. */
    /* Row 290: TBOX_TBOXRemBattHeatReq; initial=0x0. */
    g_TxTBOX_BattHeatMsg.bits.TBOX_TBOXRemBattHeatReq = 0x00;
    /* Row 291: TBOX_2C5_RollingCounter; initial=0x0. */
    g_TxTBOX_BattHeatMsg.bits.TBOX_2C5_RollingCounter = 0x00;
    /* Row 292: TBOX_2C5_Checksum; initial=0x0. */
    g_TxTBOX_BattHeatMsg.bits.TBOX_2C5_Checksum = 0x00;

    /* ADCU_Challenge; message row 294. */
    /* Row 295: ADCU_ChallengeFrame; initial=0xFFFF. */
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_7 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_6 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_5 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_4 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_3 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_2 = 0x00;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_1 = 0xFF;
    g_RxADCU_ChallengeMsg.bits.ADCU_ChallengeFrame_0 = 0xFF;

    /* BMS_VCVRemoteSts; message row 297. */
    /* Row 298: BMS_VCVSts; initial=0x0. */
    g_RxBMS_VCVRemoteStsMsg.bits.BMS_VCVSts = 0x00;

    /* TBOX_VIUCmd; message row 300. */
    /* Row 301: Tbox_LockInstructPer; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_LockInstructPer = 0x00;
    /* Row 302: Tbox_SoftVersionChangePer; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_SoftVersionChangePer = 0x00;
    /* Row 303: Tbox_LimitSpeedClass; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_LimitSpeedClass = 0x00;
    /* Row 304: Tbox_ChargeConditionJudge; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_ChargeConditionJudge = 0x00;
    /* Row 305: Tbox_MaintenanceRe; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_MaintenanceRe = 0x00;
    /* Row 306: Tbox_PaymentRe; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_PaymentRe = 0x00;
    /* Row 307: Tbox_WhistleAndFlashing; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_WhistleAndFlashing = 0x00;
    /* Row 308: Tbox_V2VStatus; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_V2VStatus = 0x00;
    /* Row 309: Tbox_ReplenishReq; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_ReplenishReq = 0x00;
    /* Row 310: Tbox_BatteryMaintainInd; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_BatteryMaintainInd = 0x00;
    /* Row 311: Tbox_NetworkAnomalyPre; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_NetworkAnomalyPre = 0x00;
    /* Row 312: Tbox_AnnualInspectionAndViolation; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_AnnualInspectionAndViolation = 0x00;
    /* Row 313: Tbox_PowerInterceptCmd; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_PowerInterceptCmd = 0x00;
    /* Row 314: Tbox_GPRSLIFE; initial cell blank, temporary raw zero. */
    g_TxTBOX_VIUCmdMsg.bits.Tbox_GPRSLIFE = 0x00;

    /* BMS_CellTemp; message row 332. */
    /* Row 333: BMS_CellTempN; initial=0x0. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTempN = 0x00;
    /* Row 334: BMS_CellTemp6N5; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N5 = 0x28;
    /* Row 335: BMS_CellTemp6N4; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N4 = 0x28;
    /* Row 336: BMS_CellTemp6N3; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N3 = 0x28;
    /* Row 337: BMS_CellTemp6N2; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N2 = 0x28;
    /* Row 338: BMS_CellTemp6N1; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N1 = 0x28;
    /* Row 339: BMS_CellTemp6N; initial=0x28. */
    g_RxBMS_CellTempMsg.bits.BMS_CellTemp6N = 0x28;

    /* MCU_TempInfo; message row 341. */
    /* Row 342: MCU_DrvMotNrGb; initial=0x0. */
    g_RxMCU_TempInfoMsg.bits.MCU_DrvMotNrGb = 0x00;

    /* BMS_BrtyID; message row 344. */
    /* Row 345: BMS_PmReessIdNFrame; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessIdNFrame = 0x00;
    /* Row 346: BMS_PmReessIdLength; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessIdLength = 0x00;
    /* Row 347: BMS_PmReessId171319; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId171319 = 0x00;
    /* Row 348: BMS_PmReessId281420; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId281420 = 0x00;
    /* Row 349: BMS_PmReessId391521; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId391521 = 0x00;
    /* Row 350: BMS_PmReessId4101622; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId4101622 = 0x00;
    /* Row 351: BMS_PmReessId5111723; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId5111723 = 0x00;
    /* Row 352: BMS_PmReessId6121824; initial=0x0. */
    g_RxBMS_BrtyIDMsg.bits.BMS_PmReessId6121824 = 0x00;

    /* BMS_AttInfo; message row 354. */
    /* Row 355: BMS_PmCellSerialNum; initial=0x1. */
    g_RxBMS_AttInfoMsg.bits.BMS_PmCellSerialNum = 0x01;
    /* Row 356: BMS_PmTempProbeNum; initial=0x1. */
    g_RxBMS_AttInfoMsg.bits.BMS_PmTempProbeNum = 0x01;
    /* TODO: finalize CRC/rolling-counter initialization per the E2E profile.
     * Matrix checksum initial cells contain zero; do not treat zero as a valid
     * calculated checksum for arbitrary payloads. Supply the full CRC profile.
     */
}

/*******************************************************
  * Name    :CanMatrix_RequestEventTx
  * Author  :xiaojunzhong
  * Function:Queue one application event on the CAN owner task
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
Bool CanMatrix_RequestEventTx(uint32_t id)
{
    uint8_t i;

    if ((CanMatrix_CheckStartAppTimerIsOk() == FALSE)
        || (CanMatrix_CheckNMIfAllowTxMsg() == FALSE)
        || (CanMatrix_CheckIsInBusOff() == TRUE)
        || (CanMatrix_CheckIsInNoAck() == TRUE))
    {
        return FALSE;
    }
    for (i = 0; i < CANMATRIX_TX_LIST_NUM; i++)
    {
        if ((g_CanMatrixTxList[i].id == id)
            && (g_CanMatrixTxList[i].type == EVENT))
        {
            s_mCanMatrixEventPending[i] = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/*******************************************************
  * Name    :CanMatrix_ClearPendingEventTx
  * Author  :xiaojunzhong
  * Function:Discard unsent event requests when communication is inhibited
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
void CanMatrix_ClearPendingEventTx(void)
{
    uint8_t i;

    for (i = 0; i < CANMATRIX_TX_LIST_NUM; i++)
    {
        s_mCanMatrixEventPending[i] = FALSE;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxDHU_ChallengeMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx DHU_Challenge, matrix row 3
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxDHU_ChallengeMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 4: DHU_ChallengeFrame; phys=raw*1+(0); fields=DHU_ChallengeFrame_7, DHU_ChallengeFrame_6, DHU_ChallengeFrame_5, DHU_ChallengeFrame_4, DHU_ChallengeFrame_3, DHU_ChallengeFrame_2, DHU_ChallengeFrame_1, DHU_ChallengeFrame_0. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_AuthKeyMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_AuthKey, matrix row 6
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_AuthKeyMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            /* Request API schedules one initial send. Matrix repetitions=0.
             * The existing core interprets CHECK's return value as send count.
             */
            if (s_mCanMatrixEventPending[0] == TRUE)
            {
                s_mCanMatrixEventPending[0] = FALSE;
                return 1;
            }
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 7: TBOX_AuthKeyData; phys=raw*1+(0); fields=TBOX_AuthKeyData_5, TBOX_AuthKeyData_4, TBOX_AuthKeyData_3, TBOX_AuthKeyData_2, TBOX_AuthKeyData_1, TBOX_AuthKeyData_0. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_RxIDCU_ChallengeMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx IDCU_Challenge, matrix row 9
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxIDCU_ChallengeMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 10: IDCU_ChallengeFrame; phys=raw*1+(0); fields=IDCU_ChallengeFrame_7, IDCU_ChallengeFrame_6, IDCU_ChallengeFrame_5, IDCU_ChallengeFrame_4, IDCU_ChallengeFrame_3, IDCU_ChallengeFrame_2, IDCU_ChallengeFrame_1, IDCU_ChallengeFrame_0. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_RemCtlMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_RemCtl, matrix row 12
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_RemCtlMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            /* Request API schedules one initial send. Matrix repetitions=0.
             * The existing core interprets CHECK's return value as send count.
             */
            if (s_mCanMatrixEventPending[1] == TRUE)
            {
                s_mCanMatrixEventPending[1] = FALSE;
                return 1;
            }
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 13: TBOX_AuthResponeSts; phys=raw*1+(0); fields=TBOX_AuthResponeSts. */
            /* Row 14: TBOX_RemSentrySwt; phys=raw*1+(0); fields=TBOX_RemSentrySwt. */
            /* Row 15: TBOX_BattAntidisassembly; phys=raw*1+(0); fields=TBOX_BattAntidisassembly. */
            /* Row 16: TBOX_RemCarSearchReq; phys=raw*1+(0); fields=TBOX_RemCarSearchReq. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_RemReqMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_RemReq, matrix row 18
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_RemReqMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            /* Request API schedules one initial send. Matrix repetitions=0.
             * The existing core interprets CHECK's return value as send count.
             */
            if (s_mCanMatrixEventPending[2] == TRUE)
            {
                s_mCanMatrixEventPending[2] = FALSE;
                return 1;
            }
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 19: TBOX_RemACOnOffReq; phys=raw*1+(0); fields=TBOX_RemACOnOffReq. */
            /* Row 20: TBOX_RemAcTempCSet; phys=raw*1+(0); fields=TBOX_RemAcTempCSet. */
            /* Row 21: TBOX_RemExtremeColdReq; phys=raw*1+(0); fields=TBOX_RemExtremeColdReq. */
            /* Row 22: TBOX_RemExtremeHotReq; phys=raw*1+(0); fields=TBOX_RemExtremeHotReq. */
            /* Row 23: TBOX_RemAirColdReq; phys=raw*1+(0); fields=TBOX_RemAirColdReq. */
            /* Row 24: TBOX_RemAirHeatReq; phys=raw*1+(0); fields=TBOX_RemAirHeatReq. */
            /* Row 25: TBOX_RemForbiddenCtrl; phys=raw*1+(0); fields=TBOX_RemForbiddenCtrl. */
            /* Row 26: TBOX_RemSpdLimReq; phys=raw*1+(0); fields=TBOX_RemSpdLimReq. */
            /* Row 27: TBOX_RemDrvrSeatHeatReq; phys=raw*1+(0); fields=TBOX_RemDrvrSeatHeatReq. */
            /* Row 28: TBOX_RemDrvrSeatVentnReq; phys=raw*1+(0); fields=TBOX_RemDrvrSeatVentnReq. */
            /* Row 29: TBOX_RemPassSeatHeatReq; phys=raw*1+(0); fields=TBOX_RemPassSeatHeatReq. */
            /* Row 30: TBOX_RemPassSeatVentnReq; phys=raw*1+(0); fields=TBOX_RemPassSeatVentnReq. */
            /* Row 31: TBOX_RemTailgateSildeReq; phys=raw*1+(0); fields=TBOX_RemTailgateSildeReq. */
            /* Row 32: TBOX_RemChrgFormSet; phys=raw*1+(0); fields=TBOX_RemChrgFormSet. */
            /* Row 33: TBOX_RemLockCtrlReq; phys=raw*1+(0); fields=TBOX_RemLockCtrlReq. */
            /* Row 34: TBOX_RemFrntDefrstReq; phys=raw*1+(0); fields=TBOX_RemFrntDefrstReq. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/* SRS_Info由CanTask中的CanService发布，接收表仅保留报文缓存与匹配。 */

/*******************************************************
  * Name    :CanMatrix_RxCCU_VehGearMsgFun
  * Author  :LoH
  * Function:Decode CAN observations into the corresponding RTE group.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static void CanMatrix_RxCCU_VehGearMsgFun(CanMatrix_RxState_Type state)
{
    RteSysMsdGear_t stSignal;

    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
        case CANMATRIX_RX_STATE_LOST:
            RteSys_UpdateMsdGear(NULL);
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            (void)memset(&stSignal, 0, sizeof(stSignal));
            stSignal.u32UpdateMs = osKernelSysTick();
            stSignal.u8Received = 1U;
            /* CRC/counter verification has no confirmed implementation yet. */
            stSignal.u8IntegrityVerified = 0U;
            stSignal.u8Gear = (uint8_t)g_RxCCU_VehGearMsg.bits.CCU_ActSymGear;
            stSignal.u8GearValid = (uint8_t)g_RxCCU_VehGearMsg.bits.CCU_ActSymGearVld;
            stSignal.u8Counter = (uint8_t)g_RxCCU_VehGearMsg.bits.CCU_116_RollingCounter;
            stSignal.u8Checksum = (uint8_t)g_RxCCU_VehGearMsg.bits.CCU_116_Checksum;
            RteSys_UpdateMsdGear(&stSignal);
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_RunStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_RunSts, matrix row 48
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_RunStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 49: BMS_ChrgSts; phys=raw*1+(0); fields=BMS_ChrgSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_VacInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_VacInfo, matrix row 51
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_VacInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 52: CCU_AirP; phys=raw*0.2+(0); fields=CCU_AirP. */
            /* Row 53: CCU_AirPValid; phys=raw*1+(0); fields=CCU_AirPValid. */
            /* Row 54: CCU_201_RollingCounter; phys=raw*1+(0); fields=CCU_201_RollingCounter. */
            /* Row 55: CCU_201_Checksum; phys=raw*1+(0); fields=CCU_201_Checksum. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_challengeMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_challenge, matrix row 57
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_challengeMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 58: CCU_ChallengeFrame; phys=raw*1+(0); fields=CCU_ChallengeFrame_7, CCU_ChallengeFrame_6, CCU_ChallengeFrame_5, CCU_ChallengeFrame_4, CCU_ChallengeFrame_3, CCU_ChallengeFrame_2, CCU_ChallengeFrame_1, CCU_ChallengeFrame_0. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_Info1MsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_Info1, matrix row 60
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_Info1MsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 61: CCU_OBDSts; phys=raw*1+(0); fields=CCU_OBDSts. */
            /* Row 62: CCU_CcuTboxResp; phys=raw*1+(0); fields=CCU_CcuTboxResp. */
            /* Row 63: CCU_RemLockFailResn; phys=raw*1+(0); fields=CCU_RemLockFailResn. */
            /* Row 64: CCU_FindCarVld; phys=raw*1+(0); fields=CCU_FindCarVld. */
            /* Row 65: CCU_RemForbiddenSts; phys=raw*1+(0); fields=CCU_RemForbiddenSts. */
            /* Row 66: CCU_ChrgFormSts; phys=raw*1+(0); fields=CCU_ChrgFormSts. */
            /* Row 67: CCU_SpdLimSts; phys=raw*1+(0); fields=CCU_SpdLimSts. */
            /* Row 68: CCU_SpdLim1; phys=raw*0.05625+(0); fields=CCU_SpdLim1_H, CCU_SpdLim1_L. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_LiStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_LiSts, matrix row 70
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_LiStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 71: CCU_DayRunngLiSts; phys=raw*1+(0); fields=CCU_DayRunngLiSts. */
            /* Row 72: CCU_RvsLiSts; phys=raw*1+(0); fields=CCU_RvsLiSts. */
            /* Row 73: CCU_BrkLiSts; phys=raw*1+(0); fields=CCU_BrkLiSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_WinStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_WinSts, matrix row 75
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_WinStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 76: CCU_RainClsWinSts; phys=raw*1+(0); fields=CCU_RainClsWinSts. */
            /* Row 77: CCU_FLWinPosnSts; phys=raw*1+(0); fields=CCU_FLWinPosnSts. */
            /* Row 78: CCU_FRWinPosnSts; phys=raw*1+(0); fields=CCU_FRWinPosnSts. */
            /* Row 79: CCU_RLWinPosnSts; phys=raw*1+(0); fields=CCU_RLWinPosnSts. */
            /* Row 80: CCU_RRWinPosnSts; phys=raw*1+(0); fields=CCU_RRWinPosnSts. */
            /* Row 81: CCU_FrntWiprSwtSts; phys=raw*1+(0); fields=CCU_FrntWiprSwtSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_MirrStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_MirrSts, matrix row 83
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_MirrStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 84: CCU_MirrSts; phys=raw*1+(0); fields=CCU_MirrSts. */
            /* Row 85: CCU_LeMirrVertPosn; phys=raw*1+(0); fields=CCU_LeMirrVertPosn. */
            /* Row 86: CCU_LeMirrHozlPosn; phys=raw*1+(0); fields=CCU_LeMirrHozlPosn. */
            /* Row 87: CCU_RiMirrVertPosn; phys=raw*1+(0); fields=CCU_RiMirrVertPosn. */
            /* Row 88: CCU_RiMirrHozlPosn; phys=raw*1+(0); fields=CCU_RiMirrHozlPosn. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_DoorStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_DoorSts, matrix row 90
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_DoorStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 91: CCU_VehLockSts; phys=raw*1+(0); fields=CCU_VehLockSts. */
            /* Row 92: CCU_TailgateLockSts; phys=raw*1+(0); fields=CCU_TailgateLockSts. */
            /* Row 93: CCU_LockRsn; phys=raw*1+(0); fields=CCU_LockRsn. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_VehModMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_VehMod, matrix row 95
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_VehModMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 96: CCU_AntiTheftSts; phys=raw*1+(0); fields=CCU_AntiTheftSts. */
            /* Row 97: CCU_LoUPwrMod; phys=raw*1+(0); fields=CCU_LoUPwrMod. */
            /* Row 98: CCU_VehMod; phys=raw*1+(0); fields=CCU_VehMod. */
            /* Row 99: CCU_IntelligentChrgnSts; phys=raw*1+(0); fields=CCU_IntelligentChrgnSts. */
            /* Row 100: CCU_IntelligentChrgnExitResn; phys=raw*1+(0); fields=CCU_IntelligentChrgnExitResn. */
            /* Row 101: CCU_LoUBattFltSts; phys=raw*1+(0); fields=CCU_LoUBattFltSts. */
            /* Row 102: CCU_AllwStrtSts; phys=raw*1+(0); fields=CCU_AllwStrtSts. */
            /* Row 103: CCU_AlcoholConcSnsrFltSts; phys=raw*1+(0); fields=CCU_AlcoholConcSnsrFltSts. */
            /* Row 104: CCU_IntelligentChrgnHVONReq; phys=raw*1+(0); fields=CCU_IntelligentChrgnHVONReq. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_SeatHeatStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_SeatHeatSts, matrix row 106
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_SeatHeatStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 107: CCU_DrvrSeatHeatSts; phys=raw*1+(0); fields=CCU_DrvrSeatHeatSts. */
            /* Row 108: CCU_DrvrSeatVentnSts; phys=raw*1+(0); fields=CCU_DrvrSeatVentnSts. */
            /* Row 109: CCU_PassSeatHeatSts; phys=raw*1+(0); fields=CCU_PassSeatHeatSts. */
            /* Row 110: CCU_PassSeatVentnSts; phys=raw*1+(0); fields=CCU_PassSeatVentnSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_ACStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_ACSts, matrix row 112
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_ACStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 113: CCU_AirCdnrWSts; phys=raw*1+(0); fields=CCU_AirCdnrWSts. */
            /* Row 114: CCU_AirCdnrTSts; phys=raw*1+(0); fields=CCU_AirCdnrTSts. */
            /* Row 115: CCU_ExtremeColdSts; phys=raw*1+(0); fields=CCU_ExtremeColdSts. */
            /* Row 116: CCU_ExtremeHotSts; phys=raw*1+(0); fields=CCU_ExtremeHotSts. */
            /* Row 117: CCU_FrntDefrstSts; phys=raw*1+(0); fields=CCU_FrntDefrstSts. */
            /* Row 118: CCU_AirCdnrACSts; phys=raw*1+(0); fields=CCU_AirCdnrACSts. */
            /* Row 119: CCU_PTCSwtSts; phys=raw*1+(0); fields=CCU_PTCSwtSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_KeyInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_KeyInfo, matrix row 121
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_KeyInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 122: CCU_RKEKeyLocn; phys=raw*1+(0); fields=CCU_RKEKeyLocn. */
            /* Row 123: CCU_BLEKeyLocn; phys=raw*1+(0); fields=CCU_BLEKeyLocn. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_OTAInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_OTAInfo, matrix row 125
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_OTAInfoMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 126: TBOX_MobileNetTyp; phys=raw*1+(0); fields=TBOX_MobileNetTyp. */
            /* Row 127: TBOX_MobileNetSwtFb; phys=raw*1+(0); fields=TBOX_MobileNetSwtFb. */
            /* Row 128: TBOX_OTAModSts; phys=raw*1+(0); fields=TBOX_OTAModSts. */
            /* Row 129: TBOX_OTARes; phys=raw*1+(0); fields=TBOX_OTARes. */
            /* Row 130: TBOX_OTAModReq; phys=raw*1+(0); fields=TBOX_OTAModReq. */
            /* Row 131: TBOX_MobileNetStr; phys=raw*0.1+(0); fields=TBOX_MobileNetStr_H, TBOX_MobileNetStr_L. */
            /* Row 132: TBOX_OTAUpgradeProgs; phys=raw*0.1+(0); fields=TBOX_OTAUpgradeProgs_H, TBOX_OTAUpgradeProgs_L. */
            /* Row 133: TBOX_OTALVReq; phys=raw*1+(0); fields=TBOX_OTALVReq. */
            /* Row 134: TBOX_OTAHVReq; phys=raw*1+(0); fields=TBOX_OTAHVReq. */
            /* Row 135: TBOX_OTAFailResn; phys=raw*1+(0); fields=TBOX_OTAFailResn. */
            /* Row 136: TBOX_210_RollingCounter; phys=raw*1+(0); fields=TBOX_210_RollingCounter. */
            /* Row 137: TBOX_210_Checksum; phys=raw*1+(0); fields=TBOX_210_Checksum. */
            /* TODO: update RollingCounter and calculate checksum AFTER payload update. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_RemInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_RemInfo, matrix row 139
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_RemInfoMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 140: TBOX_RemWinCtrlReq; phys=raw*1+(0); fields=TBOX_RemWinCtrlReq. */
            /* Row 141: TBOX_RemPwrCtrlReq; phys=raw*1+(0); fields=TBOX_RemPwrCtrlReq. */
            /* Row 142: TBOX_RemSpdLim; phys=raw*0.05625+(0); fields=TBOX_RemSpdLim_H, TBOX_RemSpdLim_L. */
            /* Row 143: TBOX_RemChrgStrtTiHr; phys=raw*1+(0); fields=TBOX_RemChrgStrtTiHr. */
            /* Row 144: TBOX_RemChrgStrtTiVld; phys=raw*1+(0); fields=TBOX_RemChrgStrtTiVld. */
            /* Row 145: TBOX_RemBattPreheatSwt; phys=raw*1+(0); fields=TBOX_RemBattPreheatSwt. */
            /* Row 146: TBOX_RemChrgEna; phys=raw*1+(0); fields=TBOX_RemChrgEna. */
            /* Row 147: TBOX_RemBattPreheatReq; phys=raw*1+(0); fields=TBOX_RemBattPreheatReq. */
            /* Row 148: TBOX_RemChrgStrtTiMins; phys=raw*1+(0); fields=TBOX_RemChrgStrtTiMins. */
            /* Row 149: TBOX_RemChrgEndTiHr; phys=raw*1+(0); fields=TBOX_RemChrgEndTiHr. */
            /* Row 150: TBOX_RemChrgEndTiVld; phys=raw*1+(0); fields=TBOX_RemChrgEndTiVld. */
            /* Row 151: TBOX_RemChrgEndTiMins; phys=raw*1+(0); fields=TBOX_RemChrgEndTiMins. */
            /* Row 152: TBOX_RemTrvlTiHr; phys=raw*1+(0); fields=TBOX_RemTrvlTiHr. */
            /* Row 153: TBOX_RemTrvlTiVld; phys=raw*1+(0); fields=TBOX_RemTrvlTiVld. */
            /* Row 154: TBOX_RemAuthnStrt; phys=raw*1+(0); fields=TBOX_RemAuthnStrt. */
            /* Row 155: TBOX_RemTrvlTiHrMins; phys=raw*1+(0); fields=TBOX_RemTrvlTiHrMins. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_InfoMsgFun
  * Author  :LoH
  * Function:Encode RTE coordinates using matrix offsets and byte order.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static uint8_t CanMatrix_TxTBOX_InfoMsgFun(CanMatrix_TxState_Type state)
{
    RteSysMsdSignals_t stSignals;
    uint32_t u32LongitudeRaw = 0xFFFFFFFFUL;
    uint32_t u32LatitudeRaw = 0xFFFFFFFFUL;

    switch (state)
    {
        case CANMATRIX_TX_STATE_UPDATE:
            if ((RteSys_ReadMsdSignals(&stSignals) != 0U)
                && (stSignals.stNavigation.u8PositionValid != 0U))
            {
                /* Unsigned addition avoids signed overflow at positive longitude limits. */
                u32LongitudeRaw = (uint32_t)stSignals.stNavigation.i32LongitudeE7 + 1800000000UL;
                u32LatitudeRaw = (uint32_t)stSignals.stNavigation.i32LatitudeE7 + 900000000UL;
            }
            g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_3 = (uint8_t)(u32LongitudeRaw >> 24);
            g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_2 = (uint8_t)(u32LongitudeRaw >> 16);
            g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_1 = (uint8_t)(u32LongitudeRaw >> 8);
            g_TxTBOX_InfoMsg.bits.TBOX_LongiInfo_0 = (uint8_t)u32LongitudeRaw;
            g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_3 = (uint8_t)(u32LatitudeRaw >> 24);
            g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_2 = (uint8_t)(u32LatitudeRaw >> 16);
            g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_1 = (uint8_t)(u32LatitudeRaw >> 8);
            g_TxTBOX_InfoMsg.bits.TBOX_LatiInfo_0 = (uint8_t)u32LatitudeRaw;
            break;
        default:
            break;
    }
    return 0U;
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_Info2MsgFun
  * Author  :LoH
  * Function:Encode RTE navigation and retain defaults for unavailable signals.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static uint8_t CanMatrix_TxTBOX_Info2MsgFun(CanMatrix_TxState_Type state)
{
    RteSysMsdSignals_t stSignals;
    uint16_t u16HeadingRaw = 0x8C00U;
    uint16_t u16SpeedRaw = 0xFFFFU;

    switch (state)
    {
        case CANMATRIX_TX_STATE_UPDATE:
            (void)RteSys_ReadMsdSignals(&stSignals);
            if (stSignals.stNavigation.u8HeadingValid != 0U)
            {
                u16HeadingRaw = stSignals.stNavigation.u16HeadingCdeg;
            }
            if (stSignals.stNavigation.u8SpeedValid != 0U)
            {
                u16SpeedRaw = stSignals.stNavigation.u16SpeedCentiKmh;
            }
            g_TxTBOX_Info2Msg.bits.TBOX_HdAg_H = (uint8_t)(u16HeadingRaw >> 8);
            g_TxTBOX_Info2Msg.bits.TBOX_HdAg_L = (uint8_t)u16HeadingRaw;
            g_TxTBOX_Info2Msg.bits.TBOX_GPSSpd_H = (uint8_t)(u16SpeedRaw >> 8);
            g_TxTBOX_Info2Msg.bits.TBOX_GPSSpd_L = (uint8_t)u16SpeedRaw;
            g_TxTBOX_Info2Msg.bits.TBOX_PosnVld = (stSignals.stNavigation.u8PositionValid != 0U) ? 1U : 0U;
            /* Altitude, authorization and geofence retain their existing ownership. */
            break;
        default:
            break;
    }
    return 0U;
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_VehHVStMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_VehHVSt, matrix row 169
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_VehHVStMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 170: CCU_VehHVSt; phys=raw*1+(0); fields=CCU_VehHVSt. */
            /* Row 171: CCU_VehRdySt; phys=raw*1+(0); fields=CCU_VehRdySt. */
            /* Row 172: CCU_MntnModSt; phys=raw*1+(0); fields=CCU_MntnModSt. */
            /* Row 173: CCU_219_RollingCounter; phys=raw*1+(0); fields=CCU_219_RollingCounter. */
            /* Row 174: CCU_219_Checksum; phys=raw*1+(0); fields=CCU_219_Checksum. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_EcallInfoMsgFun
  * Author  :xiaojunzhong
  * Function:发送前绑定模组真实通话观测；仅发布状态，由车机自行静音。
  * Version :V1.3.0
  * Data    :2026.11.07
  * Input   :矩阵发送阶段；只在UPDATE阶段读取RTE。
  * Output  :零表示无额外事件重发；原报文继续按一百毫秒周期处理。
  * Caller  :CanManageTask中的CanMatrix_TxMsgMain，唯一写本报文缓存的任务。
  * Precond :模组协议任务已发布合法状态；首次无来源时沿用矩阵零初始值。
  * Failure :来源无效时保留最后发送内容，不因ACK或MCU待机伪造通话结束。
  * Effect  :只改本报文四个业务字段，保留其余位；不操作GPIO、音频电源或UART。
*******************************************************/
static uint8_t CanMatrix_TxTBOX_EcallInfoMsgFun(CanMatrix_TxState_Type state)
{
    RteSysModemCallStatus_t stCallStatus; /* 同一次短临界区取得的状态和累计秒数，防止跨版本混用。 */

    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            if ((RteSys_ReadModemCallStatus(&stCallStatus) != 0U)
                && (stCallStatus.u8SourceValid != 0U))
            {
                g_TxTBOX_EcallInfoMsg.bits.TBOX_ECallSts = stCallStatus.u8CallState;
                if (stCallStatus.u8DurationValid != 0U)
                {
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiSec = (uint8_t)(stCallStatus.u32DurationSeconds % 60U);
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiMin = (uint8_t)((stCallStatus.u32DurationSeconds / 60U) % 60U);
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiHr = (uint8_t)(stCallStatus.u32DurationSeconds / 3600U);
                }
                else
                {
                    /* 矩阵第178至180行：秒和分为FF，小时为五位1F，均表示不可用。 */
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiSec = 0xFFU;
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiMin = 0xFFU;
                    g_TxTBOX_EcallInfoMsg.bits.TBOX_EcallTiHr = 0x1FU;
                }
            }
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* 周期发送完成不代表通话结束，不清除上次模组状态。 */
            break;
        default:
            break;
    }
    return 0U;
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_ThermMngtMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_ThermMngt, matrix row 182
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_ThermMngtMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 183: CCU_CCUBattThermMngtSts; phys=raw*1+(0); fields=CCU_CCUBattThermMngtSts. */
            /* Row 184: CCU_CCUThermMngtSysFltSts; phys=raw*1+(0); fields=CCU_CCUThermMngtSysFltSts. */
            /* Row 185: CCU_21F_RollingCounter; phys=raw*1+(0); fields=CCU_21F_RollingCounter. */
            /* Row 186: CCU_21F_Checksum; phys=raw*1+(0); fields=CCU_21F_Checksum. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_PwrInfo2MsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_PwrInfo2, matrix row 188
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_PwrInfo2MsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 189: CCU_DCDCMaxPwrLim; phys=raw*0.1+(0); fields=CCU_DCDCMaxPwrLim. */
            /* Row 190: CCU_DrvMaxPwrLim; phys=raw*0.1+(0); fields=CCU_DrvMaxPwrLim_H, CCU_DrvMaxPwrLim_L. */
            /* Row 191: CCU_ThermMngtMaxPwrLimHi; phys=raw*0.1+(0); fields=CCU_ThermMngtMaxPwrLimHi. */
            /* Row 192: CCU_ThermMngtMaxPwrLimMid; phys=raw*0.1+(0); fields=CCU_ThermMngtMaxPwrLimMid. */
            /* Row 193: CCU_ThermMngtMaxPwrLimLo; phys=raw*0.1+(0); fields=CCU_ThermMngtMaxPwrLimLo. */
            /* Row 194: CCU_V2XMaxDisChrgPwrLim; phys=raw*0.1+(0); fields=CCU_V2XMaxDisChrgPwrLim. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_StatusMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_Status, matrix row 196
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_StatusMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 197: BMS_DisChrgSts; phys=raw*1+(0); fields=BMS_DisChrgSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_InslStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_InslSts, matrix row 199
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_InslStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 200: BMS_SysInsulationR; phys=raw*1+(0); fields=BMS_SysInsulationR_H, BMS_SysInsulationR_L. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_VoltSocInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_VoltSocInfo, matrix row 202
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_VoltSocInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 203: BMS_HVBatPackVoltage; phys=raw*0.1+(0); fields=BMS_HVBatPackVoltage_H, BMS_HVBatPackVoltage_L. */
            /* Row 204: BMS_HVBatCurrent; phys=raw*0.1+(-1000); fields=BMS_HVBatCurrent_H, BMS_HVBatCurrent_L. */
            /* Row 205: BMS_HVBatSOC; phys=raw*1+(0); fields=BMS_HVBatSOC. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_FtList1MsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_FtList1, matrix row 207
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_FtList1MsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 208: BMS_FtCellTempDiff; phys=raw*1+(0); fields=BMS_FtCellTempDiff. */
            /* Row 209: BMS_FtCellOvertemp; phys=raw*1+(0); fields=BMS_FtCellOvertemp. */
            /* Row 210: BMS_FtSysUndervolt; phys=raw*1+(0); fields=BMS_FtSysUndervolt. */
            /* Row 211: BMS_FtSysOvervolt; phys=raw*1+(0); fields=BMS_FtSysOvervolt. */
            /* Row 212: BMS_FtSocLow; phys=raw*1+(0); fields=BMS_FtSocLow. */
            /* Row 213: BMS_FtCellOvervolt; phys=raw*1+(0); fields=BMS_FtCellOvervolt. */
            /* Row 214: BMS_FtCellUndervolt; phys=raw*1+(0); fields=BMS_FtCellUndervolt. */
            /* Row 215: BMS_InsulationFltSts; phys=raw*1+(0); fields=BMS_InsulationFltSts. */
            /* Row 216: BMS_FtCellConsistency; phys=raw*1+(0); fields=BMS_FtCellConsistency. */
            /* Row 217: BMS_FtOverchg; phys=raw*1+(0); fields=BMS_FtOverchg. */
            /* Row 218: BMS_FtSocHigh; phys=raw*1+(0); fields=BMS_FtSocHigh. */
            /* Row 219: BMS_FtSocJump; phys=raw*1+(0); fields=BMS_FtSocJump. */
            /* Row 220: BMS_FtNum; phys=raw*1+(0); fields=BMS_FtNum. */
            /* Row 221: BMS_DTC; phys=raw*1+(0); fields=BMS_DTC_H, BMS_DTC_L. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_CellVolt2MsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_CellVolt2, matrix row 223
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_CellVolt2MsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 224: BMS_PmCellVoltN; phys=raw*1+(0); fields=BMS_PmCellVoltN. */
            /* Row 225: BMS_PmCellVolt3N2; phys=raw*0.001+(0); fields=BMS_PmCellVolt3N2_H, BMS_PmCellVolt3N2_L. */
            /* Row 226: BMS_PmCellVolt3N1; phys=raw*0.001+(0); fields=BMS_PmCellVolt3N1_H, BMS_PmCellVolt3N1_L. */
            /* Row 227: BMS_PmCellVolt3N; phys=raw*0.001+(0); fields=BMS_PmCellVolt3N_H, BMS_PmCellVolt3N_L. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_ACChgInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_ACChgInfo, matrix row 229
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_ACChgInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 230: BMS_ACChrgGunSts; phys=raw*1+(0); fields=BMS_ACChrgGunSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxACU_DCDCSymRunMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx ACU_DCDCSymRun, matrix row 232
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxACU_DCDCSymRunMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 233: IPU_DCDCRunSt; phys=raw*1+(0); fields=IPU_DCDCRunSt. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxIDCU_SentryMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx IDCU_Sentry, matrix row 235
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxIDCU_SentryMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 236: IDCU_SentrySwtFb; phys=raw*1+(0); fields=IDCU_SentrySwtFb. */
            /* Row 237: IDCU_SentrySts; phys=raw*1+(0); fields=IDCU_SentrySts. */
            /* Row 238: IDCU_APPPromptText; phys=raw*1+(0); fields=IDCU_APPPromptText. */
            /* Row 239: IDCU_IdcuTboxResp; phys=raw*1+(0); fields=IDCU_IdcuTboxResp. */
            /* Row 240: IDCU_DhuTboxResp; phys=raw*1+(0); fields=IDCU_DhuTboxResp. */
            /* Row 241: IDCU_LocalBattPreheatSwt; phys=raw*1+(0); fields=IDCU_LocalBattPreheatSwt. */
            /* Row 242: IDCU_MobileNetSwt; phys=raw*1+(0); fields=IDCU_MobileNetSwt. */
            /* Row 243: IDCU_LocalTrvlTiHr; phys=raw*1+(0); fields=IDCU_LocalTrvlTiHr. */
            /* Row 244: IDCU_LocalTrvlTiHrMins; phys=raw*1+(0); fields=IDCU_LocalTrvlTiHrMins. */
            /* Row 245: IDCU_LocalTrvlTiVld; phys=raw*1+(0); fields=IDCU_LocalTrvlTiVld. */
            /* Row 246: IDCU_DistractionLvl; phys=raw*1+(0); fields=IDCU_DistractionLvl. */
            /* Row 247: IDCU_DrowsinessLvl; phys=raw*1+(0); fields=IDCU_DrowsinessLvl. */
            /* Row 248: IDCU_VehPosngAuthnSwt; phys=raw*1+(0); fields=IDCU_VehPosngAuthnSwt. */
            /* Row 249: IDCU_SmokingLvl; phys=raw*1+(0); fields=IDCU_SmokingLvl. */
            /* Row 250: IDCU_PhoneLvl; phys=raw*1+(0); fields=IDCU_PhoneLvl. */
            /* Row 251: IDCU_NavOperSts; phys=raw*1+(0); fields=IDCU_NavOperSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxIDCU_ChrgTimeMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx IDCU_ChrgTime, matrix row 253
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxIDCU_ChrgTimeMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 254: IDCU_LocalChrgStrtTiHr; phys=raw*1+(0); fields=IDCU_LocalChrgStrtTiHr. */
            /* Row 255: IDCU_LocalChrgStrtTiMins; phys=raw*1+(0); fields=IDCU_LocalChrgStrtTiMins. */
            /* Row 256: IDCU_LoacalChrgStrtTiVld; phys=raw*1+(0); fields=IDCU_LoacalChrgStrtTiVld. */
            /* Row 257: IDCU_LocalChrgEndTiHr; phys=raw*1+(0); fields=IDCU_LocalChrgEndTiHr. */
            /* Row 258: IDCU_LocalChrgEndTiMins; phys=raw*1+(0); fields=IDCU_LocalChrgEndTiMins. */
            /* Row 259: IDCU_LoacalChrgEndTiVld; phys=raw*1+(0); fields=IDCU_LoacalChrgEndTiVld. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxIDCU_TotMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx IDCU_Tot, matrix row 261
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxIDCU_TotMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 262: IDCU_TotMilgRequest; phys=raw*1+(0); fields=IDCU_TotMilgRequest. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxIDCU_Tot1MsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx IDCU_Tot1, matrix row 264
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxIDCU_Tot1MsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 265: TBOX_TotMilg; phys=raw*0.1+(0); fields=TBOX_TotMilg_3, TBOX_TotMilg_2, TBOX_TotMilg_1, TBOX_TotMilg_0. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_LoadCalMsgFun
  * Author  :LoH
  * Function:Decode confirmed kg load and publish qualified inputs for occupant estimation.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static void CanMatrix_RxCCU_LoadCalMsgFun(CanMatrix_RxState_Type state)
{
    RteSysMsdLoad_t stLoad;

    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
        case CANMATRIX_RX_STATE_LOST:
            RteSys_UpdateMsdLoad(NULL);
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            (void)memset(&stLoad, 0, sizeof(stLoad));
            stLoad.u32UpdateMs = osKernelSysTick();
            stLoad.u8Received = 1U;
            stLoad.u16LoadKg = (uint16_t)(((uint16_t)g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcn_H << 8)
                                         | (uint16_t)g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcn_L);
            stLoad.u8LoadValid = (uint8_t)g_RxCCU_LoadCalMsg.bits.CCU_LoadCalcnVld;
            RteSys_UpdateMsdLoad(&stLoad);
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxCCU_AutoParkMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx CCU_AutoPark, matrix row 273
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxCCU_AutoParkMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 274: CCU_ExtnRngPwrReq; phys=raw*0.1+(0); fields=CCU_ExtnRngPwrReq_H, CCU_ExtnRngPwrReq_L. */
            /* Row 275: CCU_28E_RollingCounter; phys=raw*1+(0); fields=CCU_28E_RollingCounter. */
            /* Row 276: CCU_28E_Checksum; phys=raw*1+(0); fields=CCU_28E_Checksum. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_TimeMsgFun
  * Author  :LoH
  * Function:Encode RTE UTC using matrix scaling and unavailable values.
  * Version :V1.0.0
  * Data    :2026.10.29
*******************************************************/
static uint8_t CanMatrix_TxTBOX_TimeMsgFun(CanMatrix_TxState_Type state)
{
    RteSysMsdSignals_t stSignals;

    switch (state)
    {
        case CANMATRIX_TX_STATE_UPDATE:
            (void)RteSys_ReadMsdSignals(&stSignals);
            if (stSignals.stUtc.u8Valid != 0U)
            {
                /* Calendar year; matrix day/second resolution is 0.25. */
                g_TxTBOX_TimeMsg.bits.TBOX_TiYr_H = (uint8_t)(stSignals.stUtc.u16Year >> 8);
                g_TxTBOX_TimeMsg.bits.TBOX_TiYr_L = (uint8_t)stSignals.stUtc.u16Year;
                g_TxTBOX_TimeMsg.bits.TBOX_TiMth = stSignals.stUtc.u8Month;
                g_TxTBOX_TimeMsg.bits.TBOX_TiDay = (uint8_t)(stSignals.stUtc.u8Day * 4U);
                g_TxTBOX_TimeMsg.bits.TBOX_TiHr = stSignals.stUtc.u8Hour;
                g_TxTBOX_TimeMsg.bits.TBOX_TIMins = stSignals.stUtc.u8Minute;
                g_TxTBOX_TimeMsg.bits.TBOX_TiSec = (uint8_t)(stSignals.stUtc.u8Second * 4U);
            }
            else
            {
                g_TxTBOX_TimeMsg.bits.TBOX_TiYr_H = 0x00U;
                g_TxTBOX_TimeMsg.bits.TBOX_TiYr_L = 0xFFU;
                g_TxTBOX_TimeMsg.bits.TBOX_TiMth = 0xFFU;
                g_TxTBOX_TimeMsg.bits.TBOX_TiDay = 0xFFU;
                g_TxTBOX_TimeMsg.bits.TBOX_TiHr = 0xFFU;
                g_TxTBOX_TimeMsg.bits.TBOX_TIMins = 0xFFU;
                g_TxTBOX_TimeMsg.bits.TBOX_TiSec = 0xFFU;
            }
            break;
        default:
            break;
    }
    return 0U;
}

/*******************************************************
  * Name    :CanMatrix_RxADCU_SenInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx ADCU_SenInfo, matrix row 286
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxADCU_SenInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 287: ADCU_AdcuTboxResp; phys=raw*1+(0); fields=ADCU_AdcuTboxResp. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_BattHeatMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_BattHeat, matrix row 289
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_BattHeatMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 290: TBOX_TBOXRemBattHeatReq; phys=raw*1+(0); fields=TBOX_TBOXRemBattHeatReq. */
            /* Row 291: TBOX_2C5_RollingCounter; phys=raw*1+(0); fields=TBOX_2C5_RollingCounter. */
            /* Row 292: TBOX_2C5_Checksum; phys=raw*1+(0); fields=TBOX_2C5_Checksum. */
            /* TODO: update RollingCounter and calculate checksum AFTER payload update. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_RxADCU_ChallengeMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx ADCU_Challenge, matrix row 294
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxADCU_ChallengeMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 295: ADCU_ChallengeFrame; phys=raw*1+(0); fields=ADCU_ChallengeFrame_7, ADCU_ChallengeFrame_6, ADCU_ChallengeFrame_5, ADCU_ChallengeFrame_4, ADCU_ChallengeFrame_3, ADCU_ChallengeFrame_2, ADCU_ChallengeFrame_1, ADCU_ChallengeFrame_0. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_VCVRemoteStsMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_VCVRemoteSts, matrix row 297
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_VCVRemoteStsMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 298: BMS_VCVSts; phys=raw*1+(0); fields=BMS_VCVSts. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_TxTBOX_VIUCmdMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Tx TBOX_VIUCmd, matrix row 300
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static uint8_t CanMatrix_TxTBOX_VIUCmdMsgFun(CanMatrix_TxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_TX_STATE_CHECK:
            break;
        case CANMATRIX_TX_STATE_UPDATE:
            /* TODO: bind application/RTE values; use raw encoding shown below.
             * Caller may prefill the table's Tx buffer on the same owner task.
             */
            /* Row 301: Tbox_LockInstructPer; phys=raw*1+(0); fields=Tbox_LockInstructPer. */
            /* Row 302: Tbox_SoftVersionChangePer; phys=raw*1+(0); fields=Tbox_SoftVersionChangePer. */
            /* Row 303: Tbox_LimitSpeedClass; phys=raw*1+(0); fields=Tbox_LimitSpeedClass. */
            /* Row 304: Tbox_ChargeConditionJudge; phys=raw*1+(0); fields=Tbox_ChargeConditionJudge. */
            /* Row 305: Tbox_MaintenanceRe; phys=raw*1+(0); fields=Tbox_MaintenanceRe. */
            /* Row 306: Tbox_PaymentRe; phys=raw*1+(0); fields=Tbox_PaymentRe. */
            /* Row 307: Tbox_WhistleAndFlashing; phys=raw*1+(0); fields=Tbox_WhistleAndFlashing. */
            /* Row 308: Tbox_V2VStatus; phys=raw*1+(0); fields=Tbox_V2VStatus. */
            /* Row 309: Tbox_ReplenishReq; phys=raw*1+(0); fields=Tbox_ReplenishReq. */
            /* Row 310: Tbox_BatteryMaintainInd; phys=raw*1+(0); fields=Tbox_BatteryMaintainInd. */
            /* Row 311: Tbox_NetworkAnomalyPre; phys=raw*1+(0); fields=Tbox_NetworkAnomalyPre. */
            /* Row 312: Tbox_AnnualInspectionAndViolation; phys=raw*1+(0); fields=Tbox_AnnualInspectionAndViolation. */
            /* Row 313: Tbox_PowerInterceptCmd; phys=raw*1+(0); fields=Tbox_PowerInterceptCmd. */
            /* Row 314: Tbox_GPRSLIFE; phys=raw*1+(0); fields=Tbox_GPRSLIFE. */
            break;
        case CANMATRIX_TX_STATE_CLEAR:
            /* Do not erase remote commands without an application-level policy. */
            break;
        default:
            break;
    }
    return 0;
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_CellTempMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_CellTemp, matrix row 332
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_CellTempMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 333: BMS_CellTempN; phys=raw*1+(0); fields=BMS_CellTempN. */
            /* Row 334: BMS_CellTemp6N5; phys=raw*1+(-40); fields=BMS_CellTemp6N5. */
            /* Row 335: BMS_CellTemp6N4; phys=raw*1+(-40); fields=BMS_CellTemp6N4. */
            /* Row 336: BMS_CellTemp6N3; phys=raw*1+(-40); fields=BMS_CellTemp6N3. */
            /* Row 337: BMS_CellTemp6N2; phys=raw*1+(-40); fields=BMS_CellTemp6N2. */
            /* Row 338: BMS_CellTemp6N1; phys=raw*1+(-40); fields=BMS_CellTemp6N1. */
            /* Row 339: BMS_CellTemp6N; phys=raw*1+(-40); fields=BMS_CellTemp6N. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxMCU_TempInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx MCU_TempInfo, matrix row 341
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxMCU_TempInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 342: MCU_DrvMotNrGb; phys=raw*1+(0); fields=MCU_DrvMotNrGb. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_BrtyIDMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_BrtyID, matrix row 344
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_BrtyIDMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 345: BMS_PmReessIdNFrame; phys=raw*1+(0); fields=BMS_PmReessIdNFrame. */
            /* Row 346: BMS_PmReessIdLength; phys=raw*1+(0); fields=BMS_PmReessIdLength. */
            /* Row 347: BMS_PmReessId171319; phys=raw*1+(0); fields=BMS_PmReessId171319. */
            /* Row 348: BMS_PmReessId281420; phys=raw*1+(0); fields=BMS_PmReessId281420. */
            /* Row 349: BMS_PmReessId391521; phys=raw*1+(0); fields=BMS_PmReessId391521. */
            /* Row 350: BMS_PmReessId4101622; phys=raw*1+(0); fields=BMS_PmReessId4101622. */
            /* Row 351: BMS_PmReessId5111723; phys=raw*1+(0); fields=BMS_PmReessId5111723. */
            /* Row 352: BMS_PmReessId6121824; phys=raw*1+(0); fields=BMS_PmReessId6121824. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/*******************************************************
  * Name    :CanMatrix_RxBMS_AttInfoMsgFun
  * Author  :xiaojunzhong
  * Function:Handle Rx BMS_AttInfo, matrix row 354
  * Version :V1.2.0
  * Data    :2026.10.20
*******************************************************/
static void CanMatrix_RxBMS_AttInfoMsgFun(CanMatrix_RxState_Type state)
{
    switch (state)
    {
        case CANMATRIX_RX_STATE_INIT:
            /* TODO: initialize application validity/state through confirmed RTE. */
            break;
        case CANMATRIX_RX_STATE_NORMAL:
            /* TODO: validate any CRC/counter before publishing application data.
             * IsChangeIn=FALSE preserves every frame, including repeated commands.
             */
            /* Row 355: BMS_PmCellSerialNum; phys=raw*1+(0); fields=BMS_PmCellSerialNum. */
            /* Row 356: BMS_PmTempProbeNum; phys=raw*1+(0); fields=BMS_PmTempProbeNum. */
            break;
        case CANMATRIX_RX_STATE_LOST:
            /* TODO: publish specified loss/fallback state. No DTC mapping invented. */
            break;
        default:
            break;
    }
}

/* No DTC requirement in the communication matrix. Inactive storage only. */
CanMatrix_DtcList_Type g_CanMatrixDtcList[CANMATRIX_DTC_LIST_STORAGE_NUM] =
{
    {0, 0, 0, 0}
};

/* Direct CCU key message: 0x116, 20 ms, timeout 200 ms.
 * Only one key is selected; equal-period multi-key groups can be added after
 * project confirmation and are handled with ALL-lost aggregation by the core.
 */
static const uint8_t s_mCanMatrixCcuKeyRxIndices[] = {3U};
const CanMatrix_NodeMonitorList_Type g_CanMatrixNodeMonitorList[CANMATRIX_NODE_MONITOR_LIST_NUM] =
{
    {CANMATRIX_DTC_CCU_LOST, s_mCanMatrixCcuKeyRxIndices, 1U}
};

/* clang-format off */
CanMatrix_RxList_Type g_CanMatrixRxList[CANMATRIX_RX_LIST_NUM] =
{
/* 1*/ {CANFD, 0x21 ,    0, 8, FALSE, g_RxDHU_ChallengeMsg.data, CanMatrix_RxDHU_ChallengeMsgFun},
/* 2*/ {CANFD, 0x36 ,    0, 8, FALSE, g_RxIDCU_ChallengeMsg.data, CanMatrix_RxIDCU_ChallengeMsgFun},
/* 3*/ {CAN  , 0x10C, RTESYS_MSD_SRS_MAX_AGE_MS, 8, FALSE, g_RxSRS_InfoMsg.data, NULL},
/* 4*/ {CANFD, 0x116, RTESYS_MSD_GEAR_MAX_AGE_MS, 8, FALSE, g_RxCCU_VehGearMsg.data, CanMatrix_RxCCU_VehGearMsgFun},
/* 5*/ {CANFD, 0x130,  500, 8, FALSE, g_RxBMS_RunStsMsg.data, CanMatrix_RxBMS_RunStsMsgFun},
/* 6*/ {CANFD, 0x201, 2000, 8, FALSE, g_RxCCU_VacInfoMsg.data, CanMatrix_RxCCU_VacInfoMsgFun},
/* 7*/ {CANFD, 0x202,    0, 8, FALSE, g_RxCCU_challengeMsg.data, CanMatrix_RxCCU_challengeMsgFun},
/* 8*/ {CANFD, 0x203, 1000, 8, FALSE, g_RxCCU_Info1Msg.data, CanMatrix_RxCCU_Info1MsgFun},
/* 9*/ {CANFD, 0x204, 1000, 8, FALSE, g_RxCCU_LiStsMsg.data, CanMatrix_RxCCU_LiStsMsgFun},
/*10*/ {CANFD, 0x206, 1000, 8, FALSE, g_RxCCU_WinStsMsg.data, CanMatrix_RxCCU_WinStsMsgFun},
/*11*/ {CANFD, 0x207, 1000, 8, FALSE, g_RxCCU_MirrStsMsg.data, CanMatrix_RxCCU_MirrStsMsgFun},
/*12*/ {CANFD, 0x208, 1000, 8, FALSE, g_RxCCU_DoorStsMsg.data, CanMatrix_RxCCU_DoorStsMsgFun},
/*13*/ {CANFD, 0x209, 1000, 8, FALSE, g_RxCCU_VehModMsg.data, CanMatrix_RxCCU_VehModMsgFun},
/*14*/ {CANFD, 0x20A, 1000, 8, FALSE, g_RxCCU_SeatHeatStsMsg.data, CanMatrix_RxCCU_SeatHeatStsMsgFun},
/*15*/ {CANFD, 0x20B, 1000, 8, FALSE, g_RxCCU_ACStsMsg.data, CanMatrix_RxCCU_ACStsMsgFun},
/*16*/ {CANFD, 0x20C, 1000, 8, FALSE, g_RxCCU_KeyInfoMsg.data, CanMatrix_RxCCU_KeyInfoMsgFun},
/*17*/ {CANFD, 0x219, 1000, 8, FALSE, g_RxCCU_VehHVStMsg.data, CanMatrix_RxCCU_VehHVStMsgFun},
/*18*/ {CANFD, 0x21F, 1000, 8, FALSE, g_RxCCU_ThermMngtMsg.data, CanMatrix_RxCCU_ThermMngtMsgFun},
/*19*/ {CANFD, 0x22C, 1000, 8, FALSE, g_RxCCU_PwrInfo2Msg.data, CanMatrix_RxCCU_PwrInfo2MsgFun},
/*20*/ {CANFD, 0x231, 1000, 8, FALSE, g_RxBMS_StatusMsg.data, CanMatrix_RxBMS_StatusMsgFun},
/*21*/ {CANFD, 0x233, 2000, 8, FALSE, g_RxBMS_InslStsMsg.data, CanMatrix_RxBMS_InslStsMsgFun},
/*22*/ {CANFD, 0x234, 1000, 8, FALSE, g_RxBMS_VoltSocInfoMsg.data, CanMatrix_RxBMS_VoltSocInfoMsgFun},
/*23*/ {CANFD, 0x235, 1000, 8, FALSE, g_RxBMS_FtList1Msg.data, CanMatrix_RxBMS_FtList1MsgFun},
/*24*/ {CANFD, 0x238, 2000, 8, FALSE, g_RxBMS_CellVolt2Msg.data, CanMatrix_RxBMS_CellVolt2MsgFun},
/*25*/ {CANFD, 0x23A, 1000, 8, FALSE, g_RxBMS_ACChgInfoMsg.data, CanMatrix_RxBMS_ACChgInfoMsgFun},
/*26*/ {CANFD, 0x240, 1000, 8, FALSE, g_RxACU_DCDCSymRunMsg.data, CanMatrix_RxACU_DCDCSymRunMsgFun},
/*27*/ {CANFD, 0x245, 1000, 8, FALSE, g_RxIDCU_SentryMsg.data, CanMatrix_RxIDCU_SentryMsgFun},
/*28*/ {CANFD, 0x246, 1000, 8, FALSE, g_RxIDCU_ChrgTimeMsg.data, CanMatrix_RxIDCU_ChrgTimeMsgFun},
/*29*/ {CANFD, 0x25F, 1000, 8, FALSE, g_RxIDCU_TotMsg.data, CanMatrix_RxIDCU_TotMsgFun},
/*30*/ {CANFD, 0x28B, RTESYS_MSD_LOAD_MAX_AGE_MS, 8, FALSE, g_RxCCU_LoadCalMsg.data, CanMatrix_RxCCU_LoadCalMsgFun},
/*31*/ {CANFD, 0x28E, 1000, 8, FALSE, g_RxCCU_AutoParkMsg.data, CanMatrix_RxCCU_AutoParkMsgFun},
/*32*/ {CANFD, 0x2B0, 1000, 8, FALSE, g_RxADCU_SenInfoMsg.data, CanMatrix_RxADCU_SenInfoMsgFun},
/*33*/ {CANFD, 0x2EA,    0, 8, FALSE, g_RxADCU_ChallengeMsg.data, CanMatrix_RxADCU_ChallengeMsgFun},
/*34*/ {CANFD, 0x367, 5000, 8, FALSE, g_RxBMS_VCVRemoteStsMsg.data, CanMatrix_RxBMS_VCVRemoteStsMsgFun},
/*35*/ {CANFD, 0x531, 5000, 8, FALSE, g_RxBMS_CellTempMsg.data, CanMatrix_RxBMS_CellTempMsgFun},
/*36*/ {CANFD, 0x623, 5000, 8, FALSE, g_RxMCU_TempInfoMsg.data, CanMatrix_RxMCU_TempInfoMsgFun},
/*37*/ {CANFD, 0x63D, 5000, 8, FALSE, g_RxBMS_BrtyIDMsg.data, CanMatrix_RxBMS_BrtyIDMsgFun},
/*38*/ {CANFD, 0x63F, 5000, 8, FALSE, g_RxBMS_AttInfoMsg.data, CanMatrix_RxBMS_AttInfoMsgFun},
};

CanMatrix_TxList_Type g_CanMatrixTxList[CANMATRIX_TX_LIST_NUM] =
{
/* 1*/ {CANFD, 0x28 ,  0,   0, 8, EVENT, g_TxTBOX_AuthKeyMsg.data, CanMatrix_TxTBOX_AuthKeyMsgFun},
/* 2*/ {CANFD, 0x48 ,  0,   0, 8, EVENT, g_TxTBOX_RemCtlMsg.data, CanMatrix_TxTBOX_RemCtlMsgFun},
/* 3*/ {CANFD, 0x68 ,  0,   0, 8, EVENT, g_TxTBOX_RemReqMsg.data, CanMatrix_TxTBOX_RemReqMsgFun},
/* 4*/ {CANFD, 0x210,  0, 100, 8, CYCLE, g_TxTBOX_OTAInfoMsg.data, CanMatrix_TxTBOX_OTAInfoMsgFun},
/* 5*/ {CANFD, 0x212,  0, 100, 8, CYCLE, g_TxTBOX_RemInfoMsg.data, CanMatrix_TxTBOX_RemInfoMsgFun},
/* 6*/ {CANFD, 0x214,  0, 100, 8, CYCLE, g_TxTBOX_InfoMsg.data, CanMatrix_TxTBOX_InfoMsgFun},
/* 7*/ {CANFD, 0x216,  0, 100, 8, CYCLE, g_TxTBOX_Info2Msg.data, CanMatrix_TxTBOX_Info2MsgFun},
/* 8*/ {CANFD, 0x21C,  0, 100, 8, CYCLE, g_TxTBOX_EcallInfoMsg.data, CanMatrix_TxTBOX_EcallInfoMsgFun},
/* 9*/ {CANFD, 0x26F,  0, 100, 8, CYCLE, g_TxIDCU_Tot1Msg.data, CanMatrix_TxIDCU_Tot1MsgFun},
/*10*/ {CANFD, 0x2A3,  0, 100, 8, CYCLE, g_TxTBOX_TimeMsg.data, CanMatrix_TxTBOX_TimeMsgFun},
/*11*/ {CANFD, 0x2C5,  0, 500, 8, CYCLE, g_TxTBOX_BattHeatMsg.data, CanMatrix_TxTBOX_BattHeatMsgFun},
/*12*/ {CANFD, 0x3F0,  0, 500, 8, CYCLE, g_TxTBOX_VIUCmdMsg.data, CanMatrix_TxTBOX_VIUCmdMsgFun},
};
/* clang-format on */
