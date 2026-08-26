/*******************************************************
  * Name    :Can.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Can_Cfg.h"
#include "CanIf.h"
#include "CanTransciever.h"
/*-------------variable statement---------------------*/
static uint8_t            g_busOff_flag = 0, g_noAck_flag = 0;
static uint8_t            g_busOff_count = 0;
static __IO uint8_t       m_u8RxFlag     = 0U;
static __IO uint8_t       m_u8STBTxFlag  = 0U;
static __IO uint8_t       m_u8PTBTxFlag  = 0U;
static stc_can_rx_frame_t m_astRxFrame[2];
/*-------------function-------------------------------*/

static void CanRx(void)
{
    //uint8_t u8RxFrameNum = 0U;
    int32_t i32Ret;

    /* Get all received frames. */
    do
    {
        i32Ret = CAN_GetRxFrame(CM_CAN, &m_astRxFrame[0]);
        if (i32Ret == LL_OK)
        {
            CanTransciever_RxMsgHandler(CAN, m_astRxFrame[0].u32ID, m_astRxFrame[0].au8Data, m_astRxFrame[0].DLC);
        }
    } while (i32Ret == LL_OK);
}

static void CAN_IrqCallback(void)
{
    stc_can_error_info_t stcErr;

    (void)CAN_GetErrorInfo(CM_CAN, &stcErr);

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_BUS_OFF) == SET)
    {
        // SEGGER_RTT_printf(0,"BUS OFF.\r\n");
        g_busOff_flag = TRUE;
        g_busOff_count++;
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_BUF_OVF) == SET)
    {
        DDL_Printf("RX overflow.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX_BUF_OVF);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_TX_GOING) == SET)
    {
        DDL_Printf("TX GOING.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_TX_GOING);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_GOING) == SET)
    {
        DDL_Printf("RX GOING.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_TX_GOING);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_BUF_OVF) == SET)
    {
        DDL_Printf("RX BUF OVF.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX_BUF_OVF);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_TX_BUF_FULL) == SET)
    {
        DDL_Printf("TX buffer full.\r\n");
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_TX_ABORTED) == SET)
    {
        DDL_Printf("TX aborted.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_TX_ABORTED);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_STB_TX) == SET)
    {
        DDL_Printf("STB transmitted.\r\n");
        m_u8STBTxFlag = 1u;
        CAN_ClearStatus(CM_CAN, CAN_FLAG_STB_TX);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_PTB_TX) == SET)
    {
        DDL_Printf("PTB transmitted.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_PTB_TX);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX) == SET)
    {
        /* Received frame can be read here. */
        DDL_Printf("Received a frame.\r\n");
        m_u8RxFlag = 1U;
        CanRx();
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_BUF_WARN) == SET)
    {
        /* Received frames can be read here. */
        DDL_Printf("RX buffer warning.\r\n");
        m_u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX_BUF_WARN);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_BUF_FULL) == SET)
    {
        /* Received frames can be read here. */
        DDL_Printf("RX buffer full.\r\n");
        m_u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX_BUF_FULL);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_RX_OVERRUN) == SET)
    {
        DDL_Printf("RX buffer overrun.\r\n");
        m_u8RxFlag = 1U;
        CAN_ClearStatus(CM_CAN, CAN_FLAG_RX_OVERRUN);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_TEC_REC_WARN) == SET)
    {
        DDL_Printf("TEC or REC reached warning limit.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_TEC_REC_WARN);
    }

    if (CAN_GetStatus(CM_CAN, CAN_FLAG_BUS_ERR) == SET)
    {
        DDL_Printf("FLAG BUS ERR.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_BUS_ERR);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_ARBITR_LOST) == SET)
    {
        DDL_Printf("FLAG ARBITR LOST.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_ARBITR_LOST);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_ERR_PASSIVE) == SET)
    {
        DDL_Printf("FLAG ERR PASSIVE.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_ERR_PASSIVE);
    }
    if (CAN_GetStatus(CM_CAN, CAN_FLAG_ERR_PASSIVE_NODE) == SET)
    {
        DDL_Printf("FLAG ERR PASSIVE NODE.\r\n");
        CAN_ClearStatus(CM_CAN, CAN_FLAG_ERR_PASSIVE_NODE);
        //g_noAck_flag = 1u;
    }
}

/*******************************************************
  * Name    :Can_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Can_Init(void)
{
    stc_can_init_t          stcCanInit;
    stc_irq_signin_config_t stcIrq;
    stc_can_filter_config_t astcFilter[6] = {
        {0x330, 0x00,     CAN_ID_STD},
        {0x30c, 0x00,     CAN_ID_STD},
        {0x302, 0x00,     CAN_ID_STD},
        {0x210, 0x00,     CAN_ID_STD},
        {0x7df, 0x00, CAN_ID_STD_EXT},
        {0x791, 0x00, CAN_ID_STD_EXT}
    };
    LL_PERIPH_WRITE_ENABLE();
    GPIO_SetFunc(GPIO_PORT_C, GPIO_PIN_07, GPIO_FUNC_50);
    GPIO_SetFunc(GPIO_PORT_C, GPIO_PIN_08, GPIO_FUNC_51);

    /* Initializes CAN. */
    (void)CAN_StructInit(&stcCanInit);
    stcCanInit.stcBitCfg.u32Prescaler = 1U;
    stcCanInit.stcBitCfg.u32TimeSeg1  = 12U;
    stcCanInit.stcBitCfg.u32TimeSeg2  = 4U;
    stcCanInit.stcBitCfg.u32SJW       = 4U;
    stcCanInit.pstcFilter             = astcFilter;
    stcCanInit.u16FilterSelect        = CAN_FILTER1 | CAN_FILTER2 | CAN_FILTER3 | CAN_FILTER4 | CAN_FILTER5 | CAN_FILTER6;
    stcCanInit.u8WorkMode             = CAN_WORK_MD_NORMAL;

    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN, DISABLE);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN, ENABLE);
    (void)CAN_Init(CM_CAN, &stcCanInit);
    /* Enable the interrupts, the status flags can be read. */
    CAN_IntCmd(CM_CAN, CAN_INT_ALL, DISABLE);
    CAN_IntCmd(CM_CAN, (CAN_INT_STB_TX | CAN_INT_RX_BUF_WARN | CAN_INT_RX_BUF_FULL | CAN_INT_RX_OVERRUN | CAN_INT_RX | CAN_INT_BUS_ERR), ENABLE);

    stcIrq.enIntSrc    = INT_SRC_CAN_INT;
    stcIrq.enIRQn      = INT122_IRQn;
    stcIrq.pfnCallback = &CAN_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrq);
    NVIC_ClearPendingIRQ(stcIrq.enIRQn);
    NVIC_SetPriority(stcIrq.enIRQn, DDL_IRQ_PRIO_03);
    NVIC_EnableIRQ(stcIrq.enIRQn);
    LL_PERIPH_WRITE_DISABLE();
}

/*******************************************************
  * Name    :Can_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Net_Can_DeInit(void)
{
    LL_PERIPH_WRITE_ENABLE();
    CAN_DeInit(CM_CAN);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN, DISABLE);
    LL_PERIPH_WRITE_DISABLE();
}

/*******************************************************
  * Name    :LowPower_Can_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void LowPower_Can_DeInit(void)
{
    CAN_DeInit(CM_CAN);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_CAN, DISABLE);
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
    g_noAck_flag = FALSE;
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
    return (Bool)g_noAck_flag;
}

/*******************************************************
  * Name    :Can_ClearBusOffFlag
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Can_ClearBusOffFlag(void)
{
    g_busOff_flag = FALSE;
}

/*******************************************************
  * Name    :Can_GetBusOffFlag
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Can_GetBusOffFlag(void)
{
    return (Bool)g_busOff_flag;
}

/*******************************************************
  * Name    :Can_ClearBusOffCnt
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Can_ClearBusOffCnt(void)
{
    g_busOff_count = 0;
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
    return g_busOff_count;
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
    CAN_AbortTx(CM_CAN, CAN_TX_BUF_STB);
    CAN_AbortTx(CM_CAN, CAN_TX_BUF_PTB);
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
    txokflag      = (m_u8STBTxFlag == 1) ? TRUE : FALSE;
    return txokflag;
}

/*******************************************************
  * Name    :Can_TxMsg
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Can_TxMsg(Frame_Type frame, uint32_t id, const uint8_t *p_buff, uint8_t len)
{
    stc_can_tx_frame_t stcTx1;
    stcTx1.u32Ctrl    = 0x0UL;
    stcTx1.u32ID      = id;
    stcTx1.IDE        = 0;   //stand format
    stcTx1.DLC        = len;
    stcTx1.au8Data[0] = *(p_buff + 0);
    stcTx1.au8Data[1] = *(p_buff + 1);
    stcTx1.au8Data[2] = *(p_buff + 2);
    stcTx1.au8Data[3] = *(p_buff + 3);
    stcTx1.au8Data[4] = *(p_buff + 4);
    stcTx1.au8Data[5] = *(p_buff + 5);
    stcTx1.au8Data[6] = *(p_buff + 6);
    stcTx1.au8Data[7] = *(p_buff + 7);

    if (id == 0x799)
    {
        (void)CAN_FillTxFrame(CM_CAN, CAN_TX_BUF_PTB, &stcTx1);
    }
    else
    {
        (void)CAN_FillTxFrame(CM_CAN, CAN_TX_BUF_STB, &stcTx1);
    }
    if (id == 0x545)
    {
        m_u8STBTxFlag = 0u;
    }

    if (id == 0x799)
    {
        CAN_StartTx(CM_CAN, CAN_TX_REQ_PTB);
    }
    else
    {
        CAN_StartTx(CM_CAN, CAN_TX_REQ_STB_ONE);
    }

    return TRUE;
}

