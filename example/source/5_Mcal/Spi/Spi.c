/*******************************************************
  * Name    :Spi.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Spi_Cfg.h"
#include "Gpt.h"
/*-------------variable statement---------------------*/

/*-------------Function-------------------------------*/
/*******************************************************
  * Name    :Spi_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
void Spi_Init(void)
{
    stc_spi_init_t  stcSpiInit;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
    (void)GPIO_Init(GPIO_PORT_E, GPIO_PIN_12, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_E, GPIO_PIN_13, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_E, GPIO_PIN_14, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_D, GPIO_PIN_08, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_D, GPIO_PIN_09, &stcGpioInit);
    (void)GPIO_Init(GPIO_PORT_B, GPIO_PIN_15, &stcGpioInit);

    /* Configure Port */
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_12, GPIO_FUNC_43);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_13, GPIO_FUNC_41);
    GPIO_SetFunc(GPIO_PORT_E, GPIO_PIN_14, GPIO_FUNC_40);
    GPIO_SetFunc(GPIO_PORT_D, GPIO_PIN_08, GPIO_FUNC_44);
    GPIO_SetFunc(GPIO_PORT_D, GPIO_PIN_09, GPIO_FUNC_45);
    GPIO_SetFunc(GPIO_PORT_B, GPIO_PIN_15, GPIO_FUNC_47);

    /* Configuration SPI */
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI3, ENABLE);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI4, ENABLE);
    SPI_StructInit(&stcSpiInit);
    stcSpiInit.u32WireMode          = SPI_3_WIRE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32Parity            = SPI_PARITY_INVD;
    stcSpiInit.u32SpiMode           = SPI_MD_1;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV256;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_16BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    stcSpiInit.u32FrameLevel        = SPI_1_FRAME;
    (void)SPI_Init(CM_SPI3, &stcSpiInit);
    SPI_Cmd(CM_SPI3, ENABLE);

    SPI_StructInit(&stcSpiInit);
    stcSpiInit.u32WireMode          = SPI_3_WIRE;
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32Parity            = SPI_PARITY_INVD;
    stcSpiInit.u32SpiMode           = SPI_MD_0;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV256;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    stcSpiInit.u32FrameLevel        = SPI_1_FRAME;
    (void)SPI_Init(CM_SPI4, &stcSpiInit);
    SPI_Cmd(CM_SPI4, ENABLE);   //EEPROM
}

/*******************************************************
  * Name    :Spi_Init
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2021.12.7
*******************************************************/
void Low_Spi_DeInit(void)
{
    SPI_DeInit(CM_SPI3);
    SPI_DeInit(CM_SPI4);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI3, DISABLE);
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI4, DISABLE);
}

/*******************************************************
  * Name    :Spi3_send
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2021.12.7
*******************************************************/
void Spi3_send(uint16_t *sendbuff, uint16_t *recvbuff, uint16_t len)
{
    SPI_TransReceive(CM_SPI3, sendbuff, recvbuff, len, 0x20000000UL);
    Gpt_DelayUs(20);
}

/*******************************************************
  * Name    :Spi4_send
  * Author  :ZhuSenRong
  * Function:****
  * Version :V1.0.0
  * Data    :2021.12.7
*******************************************************/
void Spi4_send(uint8_t *sendbuff, uint8_t *recvbuff, uint16_t len)
{
    SPI_TransReceive(CM_SPI4, sendbuff, recvbuff, len, 0x20000000UL);
}
