/*******************************************************
  * Name    :Uart.c
  * Author  :WangHu
  * Function:MCAL 串口初始化及收发；接收由中断写入、单任务读取。
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Uart_Cfg.h"
#include "hc32_ll_interrupts.h"
/*-------------variable statement---------------------*/
#define UART_RX_BUFFER_LENGTH    2048U

uint8_t data[50] = { 0 };
/* 接收槽由 USART3 中断写入，单一消费任务读取，写索引发布后数据有效。 */
static uint8_t m_au8RxBuffer[UART_RX_BUFFER_LENGTH];
/* 读索引由消费任务更新、中断读取；写索引由中断更新、任务读取。
 * 索引为对齐的十六位访问，范围零至缓冲区长度减一；初始化须在接收前完成。
 * 不支持多任务并行读取或运行期与接收并行重新初始化。 */
static volatile uint16_t m_u16RxReadIndex = 0U;
static volatile uint16_t m_u16RxWriteIndex = 0U;
static volatile uint16_t m_u16RxOverflowCount = 0U;
/*-------------Function-------------------------------*/

/*******************************************************
  * Name    :USART3_RxError_IrqCallback
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void USART3_RxError_IrqCallback(void)
{
    if (SET == USART_GetStatus(CM_USART3, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR)))
    {
        (void)USART_ReadData(CM_USART3);
    }

    USART_ClearStatus(CM_USART3, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
}

/*******************************************************
  * Name    :USART3_RxFull_IrqCallback
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
static void USART3_RxFull_IrqCallback(void)
{
    uint8_t u8Data = (uint8_t)USART_ReadData(CM_USART3);
    uint16_t u16NextWriteIndex = m_u16RxWriteIndex + 1U;

    if (u16NextWriteIndex >= UART_RX_BUFFER_LENGTH)
    {
        u16NextWriteIndex = 0U;
    }
    if (u16NextWriteIndex != m_u16RxReadIndex)
    {
        m_au8RxBuffer[m_u16RxWriteIndex] = u8Data;
        m_u16RxWriteIndex = u16NextWriteIndex;
    }
    else
    {
        m_u16RxOverflowCount++;
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

    m_u16RxReadIndex = 0U;
    m_u16RxWriteIndex = 0U;
    m_u16RxOverflowCount = 0U;

    GPIO_SetFunc(GPIO_PORT_F, GPIO_PIN_06, GPIO_FUNC_32);//TX
    GPIO_SetFunc(GPIO_PORT_F, GPIO_PIN_07, GPIO_FUNC_33);//RX

    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_USART3, ENABLE);

    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv      = USART_CLK_DIV4;
    stcUartInit.u32Baudrate      = 115200;//230400;//460800;
    stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
    if (LL_OK != USART_UART_Init(CM_USART3, &stcUartInit, NULL))
    {
        for (;;)
        {
        }
    }
    /* Register USART3 RX error and RX full IRQ handlers with checked installation. */
    if ((LL_OK != INTC_IrqInstallHandle(INT020_IRQn, INT_SRC_USART3_EI,
                                        DDL_IRQ_PRIO_DEFAULT, &USART3_RxError_IrqCallback))
        || (LL_OK != INTC_IrqInstallHandle(INT021_IRQn, INT_SRC_USART3_RI,
                                           DDL_IRQ_PRIO_DEFAULT, &USART3_RxFull_IrqCallback)))
    {
        for (;;)
        {
        }
    }

    USART_FuncCmd(CM_USART3, (USART_RX | USART_INT_RX | USART_TX), ENABLE);
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
        USART_WriteData(CM_USART3, *p_buff++);
        while (RESET == USART_GetStatus(CM_USART3, USART_FLAG_TX_EMPTY))
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
  * Author  :xiaojunzhong
  * Function:按确定顺序读取接收索引，非阻塞取走当前可用字节。
  * Version :V1.1.0
  * Data    :2026.11.05
  * Input   :接收缓冲区及最大长度，缓冲区容量不得小于最大长度。
  * Output  :返回实际读取长度，范围零至请求长度，单位为字节。
  * Caller  :单一消费任务；当前尚无生产调用者，不允许多任务并行读取。
  * Precond :Uart_Init 已完成；接收中断独占写索引，任务独占读索引。
  * Failure :空指针、零长度或队列为空时返回零，不等待新数据。
  * Effect  :复制后发布新读索引；不关中断、不改变写索引和溢出策略。
*******************************************************/
uint8_t Uart_ReadRxData(uint8_t *pu8Buffer, uint8_t u8Length)
{
    uint8_t u8ReadLength = 0U; /* 本次已复制字节数，不跨调用保留。 */
    uint16_t u16ReadIndex;    /* 当前读位置及复制后的下一位置。 */
    uint16_t u16WriteIndex;   /* 每次判空前取得的中断写位置快照。 */

    if (pu8Buffer == NULL)
    {
        return 0U;
    }
    while (u8ReadLength < u8Length)
    {
        /* 分开读取两个 volatile 对象，比较只使用本地快照。 */
        u16ReadIndex = m_u16RxReadIndex;
        u16WriteIndex = m_u16RxWriteIndex;
        if (u16ReadIndex == u16WriteIndex)
        {
            break;
        }

        pu8Buffer[u8ReadLength] = m_au8RxBuffer[u16ReadIndex];
        u16ReadIndex++;
        if (u16ReadIndex >= UART_RX_BUFFER_LENGTH)
        {
            u16ReadIndex = 0U;
        }
        /* 复制完成后才释放槽位，中断不得覆盖尚未取走的字节。 */
        m_u16RxReadIndex = u16ReadIndex;
        u8ReadLength++;
    }
    return u8ReadLength;
}

/*******************************************************
  * Name    :Uart_GetRxOverflowCount
  * Author  :LoH
  * Function:Return USART3 RX ring-buffer overflow count.
  * Version :V1.0.0
  * Data    :2026.08.27
*******************************************************/
uint16_t Uart_GetRxOverflowCount(void)
{
    return m_u16RxOverflowCount;
}

