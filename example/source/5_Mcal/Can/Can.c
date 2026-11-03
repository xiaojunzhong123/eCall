/*******************************************************
  * Name    :Can.c
  * Author  :LoH
  * Function:HC32 CAN2 CAN FD driver, TJA1044 control and ISR receive ring.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
/*-------------include file---------------------------*/
#include "Can_Cfg.h"
#include "Can.h"
#include "Gpio.h"
#include <string.h>

/*-------------variable statement---------------------*/
typedef struct
{
    CanRxFrame_t astRxRing[CAN_CFG_RX_RING_SIZE];
    volatile uint8_t u8WriteSequence;
    volatile uint8_t u8ReadSequence;
} CanRuntime_t;

typedef struct
{
    volatile uint8_t u8BusOff;
	volatile uint8_t u8BusOffFlag;
	volatile uint8_t u8STBTxFlag;
	volatile uint8_t u8BusOffCount;
	volatile uint8_t u8noAck;
	volatile uint8_t u8RxFlag;
    volatile uint32_t u32BusOffCount;
    volatile uint32_t u32RxOverflowCount;
    volatile uint32_t u32TxErrorCount;
	
} CanStatus_t;

static CanRuntime_t s_mCanRun;
static CanStatus_t s_mCanSts;

/*-------------function-------------------------------*/
/*******************************************************
  * Name    :Can_EnablePhy
  * Author  :LoH
  * Function:Enable CAN2 power and set the TJA1044 to normal mode.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void Can_EnablePhy(void)
{
    GPIO_SetOutputLevel(E_OUTPUT_PWR_EN_CAN, HIGH);
    GPIO_SetOutputLevel(E_OUTPUT_CAN1_STB, LOW);
}

/*******************************************************
  * Name    :Can_DisablePhy
  * Author  :LoH
  * Function:Set the TJA1044 to standby mode.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void Can_DisablePhy(void)
{
    GPIO_SetOutputLevel(E_OUTPUT_CAN1_STB, HIGH);
}

/*******************************************************
  * Name    :Can_DlcToLength
  * Author  :LoH
  * Function:Convert a CAN data length code to payload bytes.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static uint8_t Can_DlcToLength(uint8_t u8Dlc)
{
    static const uint8_t au8DlcLength[16] =
    {
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U,
        8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U
    };

    if (u8Dlc >= 16U)
    {
        return 0U;
    }

    return au8DlcLength[u8Dlc];
}

/*******************************************************
  * Name    :Can_LengthToDlc
  * Author  :LoH
  * Function:Convert an exact supported payload length to a CAN DLC.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static uint8_t Can_LengthToDlc(uint8_t u8Length, uint8_t *pu8Dlc)
{
    if ((pu8Dlc == NULL) || (u8Length > CAN_RX_DATA_MAX_LENGTH))
    {
        return 0U;
    }

    if (u8Length <= 8U)
    {
        *pu8Dlc = u8Length;
        return 1U;
    }

    switch (u8Length)
    {
        case 12U:
            *pu8Dlc = CAN_DLC12;
            break;
        case 16U:
            *pu8Dlc = CAN_DLC16;
            break;
        case 20U:
            *pu8Dlc = CAN_DLC20;
            break;
        case 24U:
            *pu8Dlc = CAN_DLC24;
            break;
        case 32U:
            *pu8Dlc = CAN_DLC32;
            break;
        case 48U:
            *pu8Dlc = CAN_DLC48;
            break;
        case 64U:
            *pu8Dlc = CAN_DLC64;
            break;
        default:
            return 0U;
    }

    return 1U;
}

/*******************************************************
  * Name    :Can_PushRxFrameFromIsr
  * Author  :LoH
  * Function:Publish one hardware frame to the ISR-owned receive ring.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void Can_PushRxFrameFromIsr(const stc_can_rx_frame_t *pstcHwFrame)
{
    CanRxFrame_t *pstFrame;
    uint8_t u8Length;
    uint8_t u8WriteSequence;

    if (pstcHwFrame == NULL)
    {
        return;
    }

    u8WriteSequence = s_mCanRun.u8WriteSequence;
    if ((uint8_t)(u8WriteSequence - s_mCanRun.u8ReadSequence) >= CAN_CFG_RX_RING_SIZE)
    {
        s_mCanSts.u32RxOverflowCount++;
        return;
    }

    u8Length = Can_DlcToLength((uint8_t)pstcHwFrame->DLC);
    pstFrame = &s_mCanRun.astRxRing[u8WriteSequence & (CAN_CFG_RX_RING_SIZE - 1U)];
    pstFrame->u32Id = pstcHwFrame->u32ID;
    pstFrame->u8Length = u8Length;
    pstFrame->u8Ide = (uint8_t)pstcHwFrame->IDE;
    pstFrame->u8Fdf = (uint8_t)pstcHwFrame->FDF;
    pstFrame->u8Brs = (uint8_t)pstcHwFrame->BRS;
    if (u8Length > 0U)
    {
        (void)memcpy(pstFrame->au8Data, pstcHwFrame->au8Data, u8Length);
    }

    __DMB();
    s_mCanRun.u8WriteSequence = (uint8_t)(u8WriteSequence + 1U);
}

/*******************************************************
  * Name    :Can_DrainRxFromIsr
  * Author  :LoH
  * Function:Drain a bounded number of CAN2 receive frames in interrupt context.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void Can_DrainRxFromIsr(void)
{
    stc_can_rx_frame_t stcHwFrame;
    uint8_t u8Handled = 0U;

    while ((u8Handled < CAN_CFG_ISR_MAX_DRAIN_FRAMES)
           && (CAN_GetRxFrame(CM_CAN2, &stcHwFrame) == LL_OK))
    {
        Can_PushRxFrameFromIsr(&stcHwFrame);
        u8Handled++;
    }
}
#if 0
/*-------------function-------------------------------*/
static void CanRx(void)
{
    //uint8_t u8RxFrameNum = 0U;
    uint32_t i32Ret;

    /* Get all received frames. */
    do
    {
        i32Ret = CAN_GetRxFrame(CM_CAN2, &m_astRxFrame[0]);
        if (i32Ret == LL_OK)
        {
            CanTransciever_RxMsgHandler(CAN, m_astRxFrame[0].u32ID, m_astRxFrame[0].au8Data, m_astRxFrame[0].DLC);
        }
    } while (i32Ret == LL_OK);
}

static void CAN_IrqCallback(void)
{
    stc_can_error_info_t stcErr;

    (void)CAN_GetErrorInfo(CM_CAN2, &stcErr);

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_BUS_OFF) == SET)
    {
        // SEGGER_RTT_printf(0,"BUS OFF.\r\n");
        s_mCanSts.u8BusOffFlag = TRUE;
        s_mCanSts.u32BusOffCount++;
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_BUF_OVF) == SET)
    {
        HMI_LOG_CAN("RX overflow.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX_BUF_OVF);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_TX_GOING) == SET)
    {
        HMI_LOG_CAN("TX GOING.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_TX_GOING);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_GOING) == SET)
    {
        HMI_LOG_CAN("RX GOING.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_TX_GOING);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_BUF_OVF) == SET)
    {
        HMI_LOG_CAN("RX BUF OVF.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX_BUF_OVF);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_TX_BUF_FULL) == SET)
    {
        HMI_LOG_CAN("TX buffer full.\r\n");
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_TX_ABORTED) == SET)
    {
        HMI_LOG_CAN("TX aborted.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_TX_ABORTED);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_STB_TX) == SET)
    {
        HMI_LOG_CAN("STB transmitted.\r\n");
        s_mCanSts.u8STBTxFlag = 1u;
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_STB_TX);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_PTB_TX) == SET)
    {
        HMI_LOG_CAN("PTB transmitted.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_PTB_TX);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX) == SET)
    {
        /* Received frame can be read here. */
        HMI_LOG_CAN("Received a frame.\r\n");
        s_mCanSts.u8RxFlag = 1U;
        CanRx();
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_BUF_WARN) == SET)
    {
        /* Received frames can be read here. */
        HMI_LOG_CAN("RX buffer warning.\r\n");
        s_mCanSts.u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX_BUF_WARN);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_BUF_FULL) == SET)
    {
        /* Received frames can be read here. */
        HMI_LOG_CAN("RX buffer full.\r\n");
        s_mCanSts.u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX_BUF_FULL);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_RX_OVERRUN) == SET)
    {
        HMI_LOG_CAN("RX buffer overrun.\r\n");
        s_mCanSts.u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_RX_OVERRUN);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_TEC_REC_WARN) == SET)
    {
        HMI_LOG_CAN("TEC or REC reached warning limit.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_TEC_REC_WARN);
    }

    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_BUS_ERR) == SET)
    {
        HMI_LOG_CAN("FLAG BUS ERR.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_BUS_ERR);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_ARBITR_LOST) == SET)
    {
        HMI_LOG_CAN("FLAG ARBITR LOST.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_ARBITR_LOST);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_ERR_PASSIVE) == SET)
    {
        HMI_LOG_CAN("FLAG ERR PASSIVE.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_ERR_PASSIVE);
    }
    if (CAN_GetStatus(CM_CAN2, CAN_FLAG_ERR_PASSIVE_NODE) == SET)
    {
        HMI_LOG_CAN("FLAG ERR PASSIVE NODE.\r\n");
        CAN_ClearStatus(CM_CAN2, CAN_FLAG_ERR_PASSIVE_NODE);
        //g_noAck_flag = 1u;
    }
}
#endif

/*******************************************************
  * Name    :Can_IrqCallback
  * Author  :LoH
  * Function:Handle CAN2 receive, bus-off and transmit error status.CAN中断回调函数
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
static void Can_IrqCallback(void)
{
    uint32_t u32Status;
    uint32_t u32RxFlags;

    u32Status = CAN_GetStatusValue(CM_CAN2);
    u32RxFlags = CAN_FLAG_RX | CAN_FLAG_RX_BUF_WARN | CAN_FLAG_RX_BUF_FULL | CAN_FLAG_RX_OVERRUN;

    if ((u32Status & u32RxFlags) != 0U)
    {
        Can_DrainRxFromIsr();
    }
    if ((u32Status & CAN_FLAG_BUS_OFF) != 0U)
    {
        s_mCanSts.u8BusOff = 1U;
        s_mCanSts.u32BusOffCount++;
    }
    if ((u32Status & CAN_FLAG_RX_OVERRUN) != 0U)
    {
        s_mCanSts.u32RxOverflowCount++;
    }
	if (u32Status & CAN_FLAG_STB_TX != 0U)
    {
        s_mCanSts.u8STBTxFlag = 1u;
        //CAN_ClearStatus(CM_CAN2, CAN_FLAG_STB_TX);
    }
    if (((u32Status & CAN_FLAG_PTB_TX) == 0U)
        && ((u32Status & (CAN_FLAG_TX_ABORTED | CAN_FLAG_BUS_ERR)) != 0U))
    {
        s_mCanSts.u32TxErrorCount++;
    }
    if (u32Status != 0U)
    {
        CAN_ClearStatus(CM_CAN2, u32Status);
    }
}

/*******************************************************
  * Name    :Can_Init
  * Author  :LoH
  * Function:Initialize CAN2, PE2/PE3 pins and the TJA1044 PHY for 500K/2M CAN FD.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void Can_Init(void)
{
    stc_can_init_t stcCanInit;
    stc_canfd_config_t stcCanFd;
    stc_irq_signin_config_t stcIrq;
    stc_can_filter_config_t astcFilter[1] =
    {
        {CAN_CFG_RX_SRS_INFO_ID, 0x00U, CAN_ID_STD}
    };

    (void)memset(&s_mCanRun, 0, sizeof(s_mCanRun));
    (void)memset(&s_mCanSts, 0, sizeof(s_mCanSts));

    LL_PERIPH_WRITE_ENABLE();
    CLK_SetCANClockSrc(CLK_CAN2, CLK_CANCLK_SYSCLK_DIV5);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_02, GPIO_FUNC_62);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_03, GPIO_FUNC_63);
    Can_EnablePhy();

    (void)CAN_StructInit(&stcCanInit);
    stcCanInit.stcBitCfg.u32Prescaler = CAN_CFG_NOMINAL_PRESCALER;
    stcCanInit.stcBitCfg.u32TimeSeg1 = CAN_CFG_NOMINAL_TIME_SEG1;
    stcCanInit.stcBitCfg.u32TimeSeg2 = CAN_CFG_NOMINAL_TIME_SEG2;
    stcCanInit.stcBitCfg.u32SJW = CAN_CFG_NOMINAL_SJW;
    stcCanInit.pstcFilter = astcFilter;
    stcCanInit.u16FilterSelect = CAN_FILTER1;
    stcCanInit.u8WorkMode = CAN_WORK_MD_NORMAL;

    (void)CAN_FD_StructInit(&stcCanFd);
    stcCanFd.stcBitCfg.u32Prescaler = CAN_CFG_DATA_PRESCALER;
    stcCanFd.stcBitCfg.u32TimeSeg1 = CAN_CFG_DATA_TIME_SEG1;
    stcCanFd.stcBitCfg.u32TimeSeg2 = CAN_CFG_DATA_TIME_SEG2;
    stcCanFd.stcBitCfg.u32SJW = CAN_CFG_DATA_SJW;
    stcCanFd.u8TDC = CAN_FD_TDC_ENABLE;
    stcCanFd.u8SSPOffset = CAN_CFG_DATA_TIME_SEG1;
    stcCanInit.pstcCanFd = &stcCanFd;

    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN2, DISABLE);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN2, ENABLE);
    CAN_FD_Cmd(CM_CAN2, ENABLE);
    (void)CAN_Init(CM_CAN2, &stcCanInit);
    CAN_ClearStatus(CM_CAN2, CAN_FLAG_ALL);
    CAN_IntCmd(CM_CAN2, CAN_INT_ALL, DISABLE);
    CAN_IntCmd(CM_CAN2, CAN_INT_ERR_INT | CAN_INT_PTB_TX | CAN_INT_RX_BUF_WARN |
               CAN_INT_RX_BUF_FULL | CAN_INT_RX_OVERRUN | CAN_INT_RX | CAN_INT_BUS_ERR, ENABLE);

    stcIrq.enIntSrc = INT_SRC_CAN2_HOST;
    stcIrq.enIRQn = INT092_IRQn;
    stcIrq.pfnCallback = &Can_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    NVIC_ClearPendingIRQ(stcIrq.enIRQn);
    NVIC_SetPriority(stcIrq.enIRQn, DDL_IRQ_PRIO_06);
    NVIC_EnableIRQ(stcIrq.enIRQn);
    LL_PERIPH_WRITE_DISABLE();
}

/*******************************************************
  * Name    :Net_Can_DeInit
  * Author  :LoH
  * Function:Stop CAN2 and set the TJA1044 to standby mode.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void Net_Can_DeInit(void)
{
    LL_PERIPH_WRITE_ENABLE();
    NVIC_DisableIRQ(INT092_IRQn);
    CAN_IntCmd(CM_CAN2, CAN_INT_ALL, DISABLE);
    CAN_DeInit(CM_CAN2);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN2, DISABLE);
    Can_DisablePhy();
    LL_PERIPH_WRITE_DISABLE();
}

/*******************************************************
  * Name    :LowPower_Can_DeInit
  * Author  :LoH
  * Function:Use the normal CAN2 shutdown path before low power.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void LowPower_Can_DeInit(void)
{
    Net_Can_DeInit();
}

/*******************************************************
  * Name    :Can_TakeRxFrame
  * Author  :LoH
  * Function:Copy one complete frame from the ISR receive ring.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint8_t Can_TakeRxFrame(CanRxFrame_t *pstFrame)
{
    uint8_t u8ReadSequence;

    if (pstFrame == NULL)
    {
        return 0U;
    }

    u8ReadSequence = s_mCanRun.u8ReadSequence;
    if (u8ReadSequence == s_mCanRun.u8WriteSequence)
    {
        return 0U;
    }

    __DMB();
    *pstFrame = s_mCanRun.astRxRing[u8ReadSequence & (CAN_CFG_RX_RING_SIZE - 1U)];
    __DMB();
    s_mCanRun.u8ReadSequence = (uint8_t)(u8ReadSequence + 1U);
    return 1U;
}

/*******************************************************
  * Name    :Can_TxMsg
  * Author  :LoH
  * Function:Validate and send one standard classic CAN or CAN FD+BRS frame by PTB.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
Bool Can_TxMsg(Frame_Type enFrame, uint32_t u32Id, const uint8_t *pu8Data, uint8_t u8Length)
{
    stc_can_tx_frame_t stcTxFrame;
    uint8_t u8Dlc;

    if ((u32Id > 0x7FFU)
        || ((u8Length > 0U) && (pu8Data == NULL))
        || (Can_LengthToDlc(u8Length, &u8Dlc) == 0U)
        || ((enFrame == CAN) && (u8Length > 8U)))
    {
        return FALSE;
    }

    (void)memset(&stcTxFrame, 0, sizeof(stcTxFrame));
    stcTxFrame.u32ID = u32Id;
    stcTxFrame.IDE = 0U;
    stcTxFrame.DLC = u8Dlc;
    stcTxFrame.FDF = (enFrame == CANFD) ? 1U : 0U;
    stcTxFrame.BRS = (enFrame == CANFD) ? 1U : 0U;
    if (u8Length > 0U)
    {
        (void)memcpy(stcTxFrame.au8Data, pu8Data, u8Length);
    }

    if (CAN_FillTxFrame(CM_CAN2, CAN_TX_BUF_PTB, &stcTxFrame) != LL_OK)
    {
        s_mCanSts.u32TxErrorCount++;
        return FALSE;
    }

    CAN_StartTx(CM_CAN2, CAN_TX_REQ_PTB);
    return TRUE;
}

/*******************************************************
  * Name    :Can_ClearNoAckFlag
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Can_ClearNoAckFlag(void)
{
    s_mCanSts.u8noAck = FALSE;
}

/*******************************************************
  * Name    :Can_GetNoAckFlag
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Can_GetNoAckFlag(void)
{
    return (Bool)s_mCanSts.u8noAck;
}

/*******************************************************
  * Name    :Can_ClearBusOffFlag
  * Author  :xiaojunzhong
  * Function:****
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
void Can_ClearBusOffFlag(void)
{
    s_mCanSts.u8BusOffFlag = FALSE;
}

/*******************************************************
  * Name    :Can_GetBusOffFlag
  * Author  :LoH
  * Function:Return whether CAN2 has entered bus-off since initialization.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint8_t Can_GetBusOffFlag(void)
{
    return s_mCanSts.u8BusOff;
}

/*******************************************************
  * Name    :Can_ClearBusOffCnt
  * Author  :xiaojunzhong
  * Function:****
  * Version :V1.0.0
  * Data    :2026.10.21
*******************************************************/
void Can_ClearBusOffCnt(void)
{
    s_mCanSts.u8BusOffCount = 0;
}

/*******************************************************
  * Name    :Can_GetBusOffCount
  * Author  :LoH
  * Function:Return the accumulated CAN2 bus-off count.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint32_t Can_GetBusOffCount(void)
{
    return s_mCanSts.u32BusOffCount;
}

/*******************************************************
  * Name    :Can_GetRxOverflowCount
  * Author  :LoH
  * Function:Return receive ring overflow events.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint32_t Can_GetRxOverflowCount(void)
{
    return s_mCanSts.u32RxOverflowCount;
}

/*******************************************************
  * Name    :Can_GetTxErrorCount
  * Author  :LoH
  * Function:Return recorded CAN2 error events.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
uint32_t Can_GetTxErrorCount(void)
{
    return s_mCanSts.u32TxErrorCount;
}

/*******************************************************
  * Name    :Can_HasEmptyMail
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Can_HasEmptyMail(void)
{
    return TRUE;
}

/*******************************************************
  * Name    :Can_ClearMail
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Can_ClearMail(void)
{
    CAN_AbortTx(CM_CAN2, CAN_TX_BUF_STB);
    CAN_AbortTx(CM_CAN2, CAN_TX_BUF_PTB);
}

/*******************************************************
  * Name    :Can_GetBusOffCnt
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
uint16_t Can_GetBusOffCnt(void)
{
    return s_mCanSts.u8BusOffCount;
}

/*******************************************************
  * Name    :Can_CheckIfTxOK
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Can_CheckIfTxOK(void)
{
    Bool txokflag = FALSE;
    txokflag      = (s_mCanSts.u8STBTxFlag == 1) ? TRUE : FALSE;
    return txokflag;
}



