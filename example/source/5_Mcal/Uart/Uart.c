/*******************************************************
  * Name    :Uart.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Uart_Cfg.h"
/*-------------variable statement---------------------*/
#define UART_RX_BUFFER_LENGTH    128U

uint8_t data[50] = { 0 };
static uint8_t m_au8RxBuffer[UART_RX_BUFFER_LENGTH];
static volatile uint8_t m_u8RxReadIndex = 0U;
static volatile uint8_t m_u8RxWriteIndex = 0U;
static volatile uint16_t m_u16RxOverflowCount = 0U;
/*-------------Function-------------------------------*/

/*******************************************************
  * Name    :USART4_RxError_IrqCallback
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void USART4_RxError_IrqCallback(void)
{
    if (SET == USART_GetStatus(CM_USART4, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR)))
    {
        (void)USART_ReadData(CM_USART4);
    }

    USART_ClearStatus(CM_USART4, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
}

/*******************************************************
  * Name    :USART4_RxFull_IrqCallback
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void USART4_RxFull_IrqCallback(void)
{
    uint8_t u8Data = (uint8_t)USART_ReadData(CM_USART4);
    uint8_t u8NextWriteIndex = m_u8RxWriteIndex + 1U;

    if (u8NextWriteIndex >= UART_RX_BUFFER_LENGTH)
    {
        u8NextWriteIndex = 0U;
    }
    if (u8NextWriteIndex != m_u8RxReadIndex)
    {
        m_au8RxBuffer[m_u8RxWriteIndex] = u8Data;
        m_u8RxWriteIndex = u8NextWriteIndex;
    }
    else
    {
        m_u16RxOverflowCount++;
    }
}

/*******************************************************
  * Name    :INTC_IrqInstalHandler
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void INTC_IrqInstalHandler(const stc_irq_signin_config_t *pstcConfig, uint32_t u32Priority)
{
    if (NULL != pstcConfig)
    {
        (void)INTC_IrqSignIn(pstcConfig);
        NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
        NVIC_SetPriority(pstcConfig->enIRQn, u32Priority);
        NVIC_EnableIRQ(pstcConfig->enIRQn);
    }
}

/*******************************************************
  * Name    :Uart_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Uart_Init(void)
{
    stc_usart_uart_init_t   stcUartInit;
    stc_irq_signin_config_t stcIrqSigninConfig;

    m_u8RxReadIndex = 0U;
    m_u8RxWriteIndex = 0U;
    m_u16RxOverflowCount = 0U;

    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_06, GPIO_FUNC_32);//TX
    GPIO_SetFunc(GPIO_PORT_D, GPIO_PIN_06, GPIO_FUNC_33);//RX

    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_USART4, ENABLE);

    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv      = USART_CLK_DIV4;
    stcUartInit.u32Baudrate      = 230400;//460800;
    stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
    if (LL_OK != USART_UART_Init(CM_USART4, &stcUartInit, NULL))
    {
        for (;;)
        {
        }
    }
    /* Register RX error IRQ handler && configure NVIC. */
    stcIrqSigninConfig.enIRQn      = INT020_IRQn;
    stcIrqSigninConfig.enIntSrc    = INT_SRC_USART4_EI;
    stcIrqSigninConfig.pfnCallback = &USART4_RxError_IrqCallback;
    INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

    /* Register RX full IRQ handler && configure NVIC. */
    stcIrqSigninConfig.enIRQn      = INT021_IRQn;
    stcIrqSigninConfig.enIntSrc    = INT_SRC_USART4_RI;
    stcIrqSigninConfig.pfnCallback = &USART4_RxFull_IrqCallback;
    INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

    USART_FuncCmd(CM_USART4, (USART_RX | USART_INT_RX | USART_TX), ENABLE);
}

/*******************************************************
  * Name    :Uart_DeInit
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Uart_DeInit(void)
{

}

/*******************************************************
  * Name    :Uart_TxData
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Uart_TxData(uint8_t *p_buff, uint8_t len)
{
    while (len--)
    {
        USART_WriteData(CM_USART4, *p_buff++);
        while (RESET == USART_GetStatus(CM_USART4, USART_FLAG_TX_EMPTY))
        {
        }
    }
    return TRUE;
}

/*******************************************************
  * Name    :Uart_RxData
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
Bool Uart_RxData(uint8_t *p_buff, uint8_t len)
{
    return TRUE;
}

/*******************************************************
  * Name    :Uart_ReadRxData
  * Author  :LoH
  * Function:Read available USART4 RX ring-buffer bytes.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
uint8_t Uart_ReadRxData(uint8_t *pu8Buffer, uint8_t u8Length)
{
    uint8_t u8ReadLength = 0U;

    if (pu8Buffer == NULL)
    {
        return 0U;
    }
    while ((u8ReadLength < u8Length) && (m_u8RxReadIndex != m_u8RxWriteIndex))
    {
        pu8Buffer[u8ReadLength] = m_au8RxBuffer[m_u8RxReadIndex];
        m_u8RxReadIndex++;
        if (m_u8RxReadIndex >= UART_RX_BUFFER_LENGTH)
        {
            m_u8RxReadIndex = 0U;
        }
        u8ReadLength++;
    }
    return u8ReadLength;
}

/*******************************************************
  * Name    :Uart_GetRxOverflowCount
  * Author  :LoH
  * Function:Return USART4 RX ring-buffer overflow count.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
uint16_t Uart_GetRxOverflowCount(void)
{
    return m_u16RxOverflowCount;
}

