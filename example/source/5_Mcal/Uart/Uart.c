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
uint8_t data[50] = { 0 };
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

    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_06, GPIO_FUNC_32);//TX
    GPIO_SetFunc(GPIO_PORT_D, GPIO_PIN_06, GPIO_FUNC_33);//RX

    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_USART4, ENABLE);

    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv      = USART_CLK_DIV4;
    stcUartInit.u32Baudrate      = 460800;
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

