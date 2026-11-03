///////////////////////////////////////////////////////////////////////////////
//文件描述: eeprom芯片M95040模块文件
//版本: 0.1
//作者: 张祥
//日期: 20150906
//说明: 创建初始版本
///////////////////////////////////////////////////////////////////////////////
//版本: 0.2
//作者: 张祥
//日期: 20151119
//说明: 驱动文件去掉状态机，将读写状态机封装到eeprom.c中实现，将驱动部分独立
///////////////////////////////////////////////////////////////////////////////

#include "m95040.h"

//eeprom cmd
//write enable
#define CMD_WREN  0x06   //0000 x110
//write disable
#define CMD_WRDI  0x04   //0000 x100
//read status reg
#define CMD_RDSR  0x05   //0000 x101
//write status reg
#define CMD_WRSR  0x01   //0000 x001
//read memory
#define CMD_READ  0x03   //0000 A8011
//write memory
#define CMD_WRITE 0x02   //0000 A8010
//read identification page
#define CMD_RDID  0x83   //1000 0011
//WRITE identification page
#define CMD_WRID  0x82   //1000 0010
//EEPROM页属性
#if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    #define PAGE_SIZE   16
    #define PAGE_NB_MAX 32
#elif (EEP_CHIPTYPE == EEP_CHIP_M95080)
    #define PAGE_SIZE   32
    #define PAGE_NB_MAX 32
#else /*#elif (EEP_CHIPTYPE == EEP_CHIP_M95160)*/
    #define PAGE_SIZE   32
    #define PAGE_NB_MAX 64
#endif
//直接读/写
//#define DIRECT_WR_RD 1
//状态寄存器未定义
#define STATUS_REG_BIT_WIP 0x01   //正在写指示位
#define STATUS_REG_BIT_WEL 0x02   //写使能位
#define STATUS_REG_BIT_BP  0xc0   //保护位指示

///////////////////////////////////////////////////////////////////////////////
//
void m95040_dummly_delay(unsigned short int x)
{
    while (x--)
    {
    }
}

void m95040_lowpower(void)
{
}

///////////////////////////////////////////////////////////////////////////////
//说明：M95040支持SPI模式0(CPOL=0, CPHA=0)和模式3(CPOL=1, CPHA=1). MSb高位先发
unsigned char m95040_spi_transfer_byte(unsigned char mosi)
{
    // #if (EEP_GPIO_EMULATE_SPI == STD_ON)
    //     unsigned char i = 0, miso = 0;

    //     for (i = 0, miso = 0; i < 8; i++)
    //     {
    //         //上升沿采样输入
    //         if (mosi & 0x80)
    //         {
    //             M95040_MOSI_HIGH ();
    //         }
    //         else
    //         {
    //             M95040_MOSI_LOW ();
    //         }
    //         M95040_SCLK_HIGH ();
    //         m95040_dummly_delay  (5);
    //         //dummly_delay  (1);
    //         miso <<= 1;
    //         mosi <<= 1;
    //         //下降沿输出
    //         if (M95040_MISO_IS_HIGH())
    //         {
    //             miso |= 0x01;
    //         }
    //         M95040_SCLK_LOW ();
    //     }
    //     M95040_MOSI_LOW ();
    //     M95040_SCLK_LOW ();
    // #else
    uint8_t miso;

    (void)Spi4_send(&mosi, &miso, 1);

    // #endif
    return miso;
}

///////////////////////////////////////////////////////////////////////////////
//说明：单命令执行（WREN, WRDIS）
void m95040_strobe_command(unsigned char cmd)
{
    M95040_CS_LOW();
    m95040_spi_transfer_byte(cmd);
    M95040_CS_HIGH();
}

///////////////////////////////////////////////////////////////////////////////
//说明：读取状态寄存器
unsigned char m95040_read_status(void)
{
    unsigned char status = 0;
    M95040_CS_LOW();
    status = m95040_spi_transfer_byte(CMD_RDSR);
    status = m95040_spi_transfer_byte(0);
    M95040_CS_HIGH();
    return status;
}

/*!
*************************************************************************************************
** Service name: Eep_ReadIdentificationPage
**   Sync/Async: Synchronous
**   Reentrancy: Non Reentrant
** ----------------------------------------------------------------------------------------------
**    Param[in]: Dest.
**   Param[out]: None.
** ----------------------------------------------------------------------------------------------
** Return value: None.
**         \ret: 0x20(ST Manufacturer code).
** ----------------------------------------------------------------------------------------------
**  Description: Read Device identification(for Spi Communication test)
**      History:
**    - YuGui    2018-10-31       Primary edition.
*************************************************************************************************
*/
void Eep_ReadIdentificationPage(uint8_t *Dest)
{
    uint8_t identIdx;

    M95040_CS_LOW();
    (void)m95040_spi_transfer_byte(CMD_RDID);
    (void)m95040_spi_transfer_byte(0U);
    for (identIdx = 0u; identIdx < 3U; ++identIdx)
    {
        *Dest = m95040_spi_transfer_byte(0U);
    }
    M95040_CS_HIGH();
}

///////////////////////////////////////////////////////////////////////////////
//输入：page_ix, 指定的页
//      p_wr, 数据源
//返回：执行结果。1，参数page_ix超限；0，成功
//说明：在指定页起始的地址处写入一页的数据(p_wr)
unsigned char m95040_write_page(unsigned char page_ix, unsigned char *p_wr)
{
    unsigned char      cmd, i;
    unsigned short int addr;
    if (page_ix >= PAGE_NB_MAX)
    {
        return 1;
    }
    //写使能
    m95040_strobe_command(CMD_WREN);
    //写数据
    addr = (unsigned short int)(page_ix * PAGE_SIZE);
    //addr = addr & 0x01FF;
    M95040_CS_LOW();
#if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    cmd = (unsigned char)(CMD_WRITE | ((addr >> 5) & 0x08));
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#else
    cmd = (unsigned char)(CMD_WRITE);
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)((addr >> 8) & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#endif
    for (i = 0; i < PAGE_SIZE; i++)
    {
        (uint8_t) m95040_spi_transfer_byte(*(p_wr + i));
    }
    M95040_CS_HIGH();
    return 0;
}

//测试    2018/02/24
unsigned char m95040_write_Byte(unsigned short addr, unsigned char *p_wr, unsigned char len)
{
    unsigned char cmd, i;

    if (addr >= (PAGE_SIZE * PAGE_NB_MAX))
    {
        return 1;
    }
    //写使能
    m95040_strobe_command(CMD_WREN);
    //写数据
    //addr = addr & 0x01FF;

    M95040_CS_LOW();
#if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    cmd = (unsigned char)(CMD_WRITE | ((addr >> 5) & 0x08));
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#else
    cmd = (unsigned char)(CMD_WRITE);
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)((addr >> 8) & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#endif

    for (i = 0; i < len; i++)
    {
        (uint8_t) m95040_spi_transfer_byte(*(p_wr + i));
    }

    M95040_CS_HIGH();

    return 0;
}

unsigned char m95040_Read_Byte(unsigned short addr, unsigned char *p_rd, unsigned char len)
{
    unsigned char cmd, i;

    if (addr >= (PAGE_SIZE * PAGE_NB_MAX))
    {
        return 1;
    }

    //写数据
    //addr = addr & 0x01FF;

    M95040_CS_LOW();

#if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    cmd = (unsigned char)(CMD_READ | ((addr >> 5) & 0x08));
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#else
    cmd = (unsigned char)(CMD_READ);
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)((addr >> 8) & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#endif

    for (i = 0; i < len; i++)
    {
        *(p_rd + i) = (uint8_t)m95040_spi_transfer_byte(0);
    }

    M95040_CS_HIGH();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//输入：addr, page的起始地址
//      p_save, 数据保存的地方
//返回：执行结果。1，参数page_ix超限；0，成功
//说明：读出一页的数据出来放在p_save
unsigned char m95040_read_page(unsigned char page_ix, unsigned char *p_save)
{
    unsigned char      cmd, i;
    unsigned short int addr;
    if (page_ix >= PAGE_NB_MAX)
    {
        return 1;
    }
    //读数据
    addr = (unsigned short int)(page_ix * PAGE_SIZE);
    //addr = addr & 0x01FF;
    M95040_CS_LOW();
#if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    cmd = (unsigned char)(CMD_READ | ((addr >> 5) & 0x08));
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#else
    cmd = (unsigned char)(CMD_READ);
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)((addr >> 8) & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
#endif
    for (i = 0; i < PAGE_SIZE; i++)
    {
        *(p_save + i) = m95040_spi_transfer_byte(0);
    }
    M95040_CS_HIGH();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//输入：addr, 有效b8--b0
//      p_wr, 数据源
//      len，数据长度
//返回：M95040状态。=0，请求OK；1=当前正在写/读
//说明：从地址addr处读出len个字节数据保存在p_save
unsigned char m95040_rqst_read_mem(unsigned short int addr, unsigned char *p_save, unsigned short int len)
{
#if 1   //def DIRECT_WR_RD
    unsigned char      cmd = 0;
    unsigned short int i;
    M95040_CS_LOW();
    #if (EEP_CHIPTYPE == EEP_CHIP_M95040)
    cmd = (unsigned char)(CMD_READ | ((addr >> 5) & 0x08));
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    #else
    cmd = (unsigned char)(CMD_READ);
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)((addr >> 8) & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    cmd = (unsigned char)(addr & 0x00ff);   //lsb addr
    (uint8_t) m95040_spi_transfer_byte(cmd);
    #endif
    for (i = 0; i < len; i++)
    {
        *(p_save + i) = m95040_spi_transfer_byte(0);
    }
    M95040_CS_HIGH();
    return 0;
#else
    unsigned char cmd;
    if (0 == g_m95040_status)
    {
        g_addr     = addr & 0x01FF;
        g_buff_len = len;
        mem_copy(&g_m95040_buff[0], p_save, len);
        g_m95040_status = 1;
        return 0;
    }
    else
    {
        return 1;
    }
#endif
}

#if 0
///////////////////////////////////////////////////////////////////////////////
//输入：addr, 有效b8--b0
//      p_wr, 数据源
//      len，数据长度
//返回：M95040状态。=0，请求OK；1=当前正在写/读；2，地址/长度超限
//说明：在地址addr处写入len个字节数据(p_buff)
unsigned char m95040_rqst_write_mem (unsigned short int addr, unsigned char *p_write, unsigned short int len)
{
    #ifdef DIRECT_WR_RD
    unsigned short int i, data_len, data_pos = 0
    unsigned char result = 0;
    unsigned char sector_index, sector_ofst_start;
    static unsigned char sector_wr[PAGE_SIZE], sector_rd[PAGE_SIZE];
    if ((addr > M95040_ADDR_END) || (addr < M95040_ADDR_START))
    {
        return 1;
    }
    if ((addr + len) > M95040_ADDR_END)
    {
        return 1;
    }
    //写数据
    for (i = 0, data_pos = 0; i < len; )
    {
        sector_index = (unsigned char)((addr - M95040_ADDR_START) / PAGE_SIZE);
        sector_ofst_start = (unsigned char)((addr - M95040_ADDR_START) % PAGE_SIZE);
        data_len = len - i;
        if (sector_ofst_start > 0)
        {
        	if ((sector_ofst_start + data_len) >= PAGE_SIZE)
        	{//
        		data_pos = 1;
        		data_len = (unsigned char)(PAGE_SIZE - sector_ofst_start);
        	}
        	else
        	{
        		data_pos = 3;
        	}
        }
        else
        {
        	if (data_len >= PAGE_SIZE)
        	{
        		data_pos = 0;
        		data_len = PAGE_SIZE;
        	}
        	else
        	{
        		data_pos = 2;
        	}
        }

        switch (data_pos)
        {
            case 1: //前半截
                result = m95040_read_page(sector_index, sector_wr);
                if (result > 0)
                {
                    return 2;
                }
                mem_copy (&sector_wr[PAGE_SIZE - data_len], p_write + i, data_len);
                break;

            case 2: //后半截
                result = m95040_read_page(sector_index, sector_wr);
                if (result > 0)
                {
                    return 2;
                }
                mem_copy (&sector_wr[0], p_write + i, data_len);
                break;

	        case 3: //中间
                result = m95040_read_page(sector_index, sector_wr);
                if (result > 0)
                {
                    return 2;
                }
                mem_copy (&sector_wr[sector_ofst_start], p_write + i, data_len);
	        	break;

            default: //sector
                mem_copy (&sector_wr[0], p_write + i, data_len);
                break;
        }

        //写使能
        m95040_strobe_command (CMD_WREN);
        result = m95040_write_page(sector_index, sector_wr);
        if (result > 0)
        {
            return 3;
        }
        i += data_len;
        addr += data_len;
    }
    return 0;
    #else
    if (0 == g_m95040_status)
    {
        if ((addr > M95040_ADDR_END) || (addr < M95040_ADDR_START))
        {
            return 2;
        }
        if ((addr + len) > M95040_ADDR_END)
        {
            return 2;
        }
        g_addr = addr & 0x01FF;
        g_buff_len = len;
        mem_copy (&g_m95040_buff[0], p_write, len);
        g_m95040_status = 1;
        return 0;
    }
    else
    {
        return 1;
    }
    #endif
}



///////////////////////////////////////////////////////////////////////////////
//输入：p_save, 数据保存处
//返回：无
//说明：读取额外的标识页数据（长度16字节）
void m95040_rqst_read_identification_page (unsigned char *p_save)
{
    unsigned char cmd, i;
    cmd = CMD_RDID ;
    M95040_CS_LOW ();
    m95040_spi_transfer_byte (0);
    m95040_spi_transfer_byte (0); //lsb addr
    for (i = 0; i < 16; i++)
    {
        p_save[i] = m95040_spi_transfer_byte (0);
    }
    M95040_CS_HIGH ();
}



///////////////////////////////////////////////////////////////////////////////
//说明：M95040写功能状态机
void m95040_wr_main (void)
{
    static unsigned short int i, data_len = 0, data_pos = 0;
    static unsigned char sector_index, sector_ofst_start;
    static unsigned char sector_wr[PAGE_SIZE], sector_rd[PAGE_SIZE];
    static unsigned short int timeout = 0, tick = 0;
    unsigned char result = 0, status = 0;

    static unsigned char state = 0;
    switch (state)
    {
        case 0:
        default:
            if (g_m95040_status)
            {
                state = 1;
                //清零
                g_m95040_wr_result = 0;
                data_pos = 0;
                i = 0;
            }
            break;

        case 1: //写页
            result = 0;
            //获取页码
            sector_index = (unsigned char)((g_addr - M95040_ADDR_START) / PAGE_SIZE);
            sector_ofst_start = (unsigned char)((g_addr - M95040_ADDR_START) % PAGE_SIZE);
            data_len = g_buff_len - i;
            if (sector_ofst_start > 0)
            {
            	if ((sector_ofst_start + data_len) >= PAGE_SIZE)
            	{//
            		data_pos = 1;
            		data_len = (unsigned char)(PAGE_SIZE - sector_ofst_start);
            	}
            	else
            	{
            		data_pos = 3;
            	}
            }
            else
            {
            	if (data_len >= PAGE_SIZE)
            	{
            		data_pos = 0;
            		data_len = PAGE_SIZE;
            	}
            	else
            	{
            		data_pos = 2;
            	}
            }
            //准备数据
            switch (data_pos)
            {
                case 1: //前半截
                    result = m95040_read_page(sector_index, sector_wr);
                    if (result > 0)
                    {
                        result = 2;
                        break;
                    }
                    mem_copy (&sector_wr[PAGE_SIZE - data_len], &g_m95040_buff[i], data_len);
                    break;

                case 2: //后半截
                    result = m95040_read_page(sector_index, sector_wr);
                    if (result > 0)
                    {
                        result = 2;
                        break;
                    }
                    mem_copy (&sector_wr[0], &g_m95040_buff[i], data_len);
                    break;

    	        case 3: //中间
                    result = m95040_read_page(sector_index, sector_wr);
                    if (result > 0)
                    {
                        result =  2;
                        break;
                    }
                    mem_copy (&sector_wr[sector_ofst_start], &g_m95040_buff[i], data_len);
    	        	break;

                default: //sector
                    mem_copy (&sector_wr[0], &g_m95040_buff[i], data_len);
                    break;
            }
            //写页
            m95040_strobe_command (CMD_WREN);
            result = (uint8_t)m95040_write_page(sector_index, sector_wr);
            if (result > 0)
            {
                result = 3;
            }
            if (result > 0)
            {
                state = 0;
                g_m95040_wr_result = 1; //读写页失败
                g_m95040_status = 0;
            }
            else
            {
                state = 2;
                timeout = 0;
                tick = 0;
            }
            break;

        case 2: //等待写结束
            if (++tick > 2000)
            {
                tick = 0;
                status = m95040_read_status ();
                if (0 == (status & STATUS_REG_BIT_WIP))
                {
                    i += data_len;
                    g_addr += data_len;
                    if (i >= g_buff_len)
                    {//写完毕
                        state = 0;
                        g_m95040_wr_result = 0; //写成功
                        g_m95040_status = 0;
                    }
                    else
                    {
                        state = 1;
                    }
                }
            }
            if (++timeout > 50000)
            {
                state = 0;
                timeout = 0;
                g_m95040_wr_result = 2; //写超时
                g_m95040_status = 0;
            }
            break;
    }
}
#endif

