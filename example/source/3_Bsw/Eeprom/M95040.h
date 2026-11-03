///////////////////////////////////////////////////////////////////////////////
//文件描述: eeprom芯片M95040头文件声明
//说明：
//芯片特性：32（页总数）*16（页大小） = 512字节，
//擦写1字节/1页的时间典型值4ms

#ifndef _M95040_H
    #define _M95040_H

    #include "Types.h"
    #include "Gpio.h"

    #define EEP_CHIP_M95040 1u /*M95040*/
    #define EEP_CHIP_M95080 2u /*M95080*/
    #define EEP_CHIP_M95160 3u /*M95160*/
    #define EEP_CHIPTYPE    EEP_CHIP_M95040

    #define EEP_GPIO_EMULATE_SPI STD_OFF

    #if (EEP_GPIO_EMULATE_SPI == STD_ON)

        //驱动配置
        #define M95040_CS_LOW()  0//GPIO_SetOutputState(E_OUTPUT_EE_SPI_CS, LOW)
        #define M95040_CS_HIGH() 0//GPIO_SetOutputState(E_OUTPUT_EE_SPI_CS, HIGH)
        #define M95040_SCLK_LOW() \
            do                    \
            {                     \
                ;                 \
            } while (0)   //pal_clearpad(EE_CLK_Port,EE_CLK_Pin)//{SIU.GPDO[107].B.PDO = 0;}//PG11, GPIO107
        #define M95040_SCLK_HIGH() \
            do                     \
            {                      \
                ;                  \
            } while (0)   //pal_setpad(EE_CLK_Port,EE_CLK_Pin)//{SIU.GPDO[107].B.PDO = 1;}
        #define M95040_MOSI_LOW() \
            do                    \
            {                     \
                ;                 \
            } while (0)   //pal_clearpad(EE_MOSI_Port,EE_MOSI_Pin)//{SIU.GPDO[35].B.PDO = 0;}//PC3, GPIO35
        #define M95040_MOSI_HIGH() \
            do                     \
            {                      \
                ;                  \
            } while (0)   //pal_setpad(EE_MOSI_Port,EE_MOSI_Pin)//{SIU.GPDO[35].B.PDO = 1;}
        #define M95040_MISO_LOW() \
            do                    \
            {                     \
                ;                 \
            } while (0)   //pal_clearpad(EE_MISO_Port,EE_MISO_Pin)//{SIU.GPDO[5].B.PDO = 0;}//PA5, GPIO5
        #define M95040_MISO_HIGH() \
            do                     \
            {                      \
                ;                  \
            } while (0)                      //pal_setpad(EE_MISO_Port,EE_MISO_Pin)//{SIU.GPDO[5].B.PDO = 1;}
        #define M95040_MISO_IS_HIGH() HIGH   //pal_readpad(EE_MISO_Port,EE_MISO_Pin)//(SIU.GPDI[5].B.PDI)
        #define M95040_W_LOW() \
            do                 \
            {                  \
                ;              \
            } while (0)   //pal_clearpad(EE_WP_Port,EE_WP_Pin)//{SIU.GPDO[34].B.PDO = 0;}//PC2, GPIO34
        #define M95040_W_HIGH() \
            do                  \
            {                   \
                ;               \
            } while (0)   //pal_setpad(EE_WP_Port,EE_WP_Pin)//{SIU.GPDO[34].B.PDO = 1;}

    #else

        #define M95040_CS_LOW()  GPIO_SetOutputState(E_OUTPUT_EE_SPI_CS, LOW)
        #define M95040_CS_HIGH() GPIO_SetOutputState(E_OUTPUT_EE_SPI_CS, HIGH)
        #define M95040_W_LOW() \
            do                 \
            {                  \
                ;              \
            } while (0)   //Dio_WriteChannel(DIO_EE_WP, STD_LOW)
        #define M95040_W_HIGH() \
            do                  \
            {                   \
                ;               \
            } while (0)   //Dio_WriteChannel(DIO_EE_WP, STD_HIGH)

    #endif

    //eeprom 地址起始
    #if (EEP_CHIPTYPE == EEP_CHIP_M95040)
        #define M95040_ADDR_START  0
        #define M95040_ADDR_END    511
        #define M95040_MEM_SIZE    512
        #define M95040_BUFF_LEN    (M95040_ADDR_END + 1)
        #define M95040_PAGE_SIZE   16
        #define M95040_PAGE_NB_MAX 32
        #define M95040_WR_TIME     4   //最大4ms
    #elif (EEP_CHIPTYPE == EEP_CHIP_M95080)
        #define M95040_ADDR_START  0
        #define M95040_ADDR_END    1023
        #define M95040_MEM_SIZE    1024
        #define M95040_BUFF_LEN    (M95040_ADDR_END + 1)
        #define M95040_PAGE_SIZE   32
        #define M95040_PAGE_NB_MAX 32
        #define M95040_WR_TIME     4   //最大4ms
    #else                              /*#elif (EEP_CHIPTYPE == EEP_CHIP_M95160)*/
        #define M95040_ADDR_START  0
        #define M95040_ADDR_END    2047
        #define M95040_MEM_SIZE    2048
        #define M95040_BUFF_LEN    (M95040_ADDR_END + 1)
        #define M95040_PAGE_SIZE   32
        #define M95040_PAGE_NB_MAX 64
        #define M95040_WR_TIME     4   //最大4ms
    #endif

//变量
//extern unsigned char g_m95040_buff[];
//extern unsigned char g_m95040_status;
//函数
extern unsigned char m95040_write_page(unsigned char page_ix, unsigned char *p_wr);
extern unsigned char m95040_read_page(unsigned char page_ix, unsigned char *p_save);
extern unsigned char m95040_read_status(void);
extern unsigned char m95040_rqst_read_mem(unsigned short int addr, unsigned char *p_save, unsigned short int len);

unsigned char m95040_write_Byte(unsigned short addr, unsigned char *p_wr, unsigned char len);
unsigned char m95040_Read_Byte(unsigned short addr, unsigned char *p_rd, unsigned char len);

    #if 0
extern void m95040_rqst_read_identification_page (unsigned char *p_save);
extern unsigned char m95040_rqst_write_mem (unsigned short int addr, unsigned char *p_wr, unsigned short int len);
extern unsigned char m95040_rqst_read_mem (unsigned short int addr, unsigned char *p_save, unsigned short int len);
extern void m95040_wr_main (void);
    #endif
#endif

/* Read Device identification(for Spi Communication test).*/
extern void Eep_ReadIdentificationPage(uint8_t *Dest);

