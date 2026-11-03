/*********************************************************************************
 * Copyright (C), 2008-2018, CJAE CO., LTD. All Rights Reserved
 * File name:   App_Memory.c
 * Author:      mobo
 * Version:     V0.1
 * Date:        2017.11.17
 * Description:
 * Others:
 * History:
 *********************************************************************************/

#include "App_Memory.h"
#include "M95040.h"
#include "Memory.h"
#include "Types.h"
#include "Nvm.h"

/*
 * 测试数组
 */
Type_EE8 testBuf[64]                                   = { 0 };

/*
 * EEPROM空闲标志位  0 空闲   1 忙
 */
Type_Eeprom_St g_Eeprom_Idle_Flag                      = EEPROM_IDLE;

/*
 *   EEPROM存储列表
 *
 *   存储标志位    :  此位置1 则表示有数据待写入EEPROM    置 0 则为空闲
 *   上电需获取EEPROM数据标志位 ： 此位应为常量 不能修改   上电需读取的参数
 *   存储起始地址   ： 数据写入开始地址
 *   存储终止地址   ： 数据写入结束地址
 *   存储数据长度   ：数据长度
 *   重试次数           ：写入异常  重写次数
 *   距下次写入时间长度 ： 此参数未使用
 *   写入数据地址  ：实际数据缓存区
 *   写入失败次数  ：写入失败次数
 *   失败故障码      ： 写入失败原因
 *
 */
static Type_Memory_Info g_Memory_Info_Tab[EE_LIST_NUM] = {
    {0u, 1u, 14, 14 + 17, 17, 1u, 0u,      (uint8_t *)&g_Uds_vin[0], 0u, 0u}
};

/*
 * 列表长度
 */
const Type_EE8 g_EE_List_Len                           = sizeof(g_Memory_Info_Tab) / sizeof(g_Memory_Info_Tab[0u]);

/*
 * 写入缓存
 */
static Type_EE8  g_Memory_Write_Info[EE_BUFFER_LENGHT] = { 0u };
static Type_EE16 g_Memory_Write_Addr                   = 0xffff;
static Type_EE16 g_Memory_Write_len                    = 0u;
Type_EE8        *g_Data_Ptr                            = NULL;

static Type_EE8 Memory_Write_Multi_Byte(void);

/***************************************************************
**  Function Name 	: 获取EEPROM工作状态
**  Description   	:
**  Input parameters: NONE
**  Return Value  	: EEPROM当前状态    0 空闲     1 正在进行EEPROM操作
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
Type_Eeprom_St Get_Eeprom_Work_St(void)
{
    return (g_Eeprom_Idle_Flag);
}

/***************************************************************
**  Function Name 	: 设置EEPROM工作状态
**  Description   	:
**  Input parameters: Type_Eeprom_St,状态
**  Return Value  	: NONE
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
void Set_Eeprom_Work_St(Type_Eeprom_St st)
{
    g_Eeprom_Idle_Flag = st;
}

/***************************************************************
**  Function Name 	: 设置数据结构体
**  Description   	:
**  Input parameters: list_num ：控制序列号
**  Return Value  	: 返回EEPROM是否完成写操作
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
Type_EE8 EE_Get_Write_Flag(Type_EE8 list_num)
{
    return (g_Memory_Info_Tab[list_num].flag);
}

/***************************************************************
**  Function Name 	: 设置数据结构体
**  Description   	:
**  Input parameters: list_num,列表序列号
**                    flag,存储写标志位
**                    *w_data,写入数据指针
**                    addr,写入地址
**                    len,写入长度
**  Return Value  	: NONE
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
void EE_Set_Write_Info(Type_EE8 list_num)
{
    //g_Memory_Info_Tab[ list_num ].addr_start = addr;
    g_Memory_Info_Tab[list_num].flag = 1u;
    //g_Memory_Info_Tab[ list_num ].len = len;
    //g_Memory_Info_Tab[ list_num ].w_data = w_data;
}

Type_EE8 EE_Set_Write_Info_Addr(Type_EE16 addr, Type_EE8 *w_data, Type_EE8 len)
{
    Type_EE8 i = 0u;

    for (i = 0u; i < g_EE_List_Len; i++)
    {
        if (addr == g_Memory_Info_Tab[i].addr_start)
        {
            break;
        }
    }

    if (i == g_EE_List_Len)
    {
        /* 起始地址不在列表中  返回错误  */

        return (0u);
    }

    g_Memory_Info_Tab[i].flag = 1u;
    Memory_Copy(g_Memory_Info_Tab[i].w_data, w_data, len);

    return (1u);
}

/***************************************************************
**  Function Name 	: 数据写入
**  Description   	:
**  Input parameters: list_num,列表序列号
**  Return Value  	: 写入字节数
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
static Type_EE8 Memory_Write_Multi_Byte(void)
{
    /* 返回值   写入数据长度  */
    Type_EE8 l_ret      = 0u;
    /* 该页剩余长度  */
    Type_EE8 l_left_len = 0u;
    /* 已使用字节数  */
    Type_EE8 l_used_len = 0u;

    /* 获取用掉长度  */
    l_used_len          = g_Memory_Write_Addr % M95040_PAGE_SIZE;

    /* 写入数据会跨页  */
    if ((l_used_len + g_Memory_Write_len) > M95040_PAGE_SIZE)
    {
        /* 计算剩余长度  */
        l_left_len = M95040_PAGE_SIZE - l_used_len;
        m95040_write_Byte(g_Memory_Write_Addr, g_Data_Ptr, l_left_len);
    }
    /*  写入数据不会跨页    */
    else
    {
        l_left_len = g_Memory_Write_len;
        m95040_write_Byte(g_Memory_Write_Addr, g_Data_Ptr, l_left_len);
    }

    l_ret = l_left_len;

    return (l_ret);
}

/***************************************************************
**  Function Name 	: 存储模块主程序
**  Description   	:
**  Input parameters: NONE
**  Return Value  	: NONE
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
void Memory_Main(void)
{
    static enum
    {
        eeprom_init = 0u, /* 写入初始化 */
        eeprom_write,     /* 写入数据 */
        eeprom_verify,    /* 数据验证 */
        eeprom_end        /* 写入结束 */
    } state;

    /* 写入列表序列   */
    static Type_EE8 list_num        = 0u;
    /* 超时计数器  */
    static Type_EE16 l_timeout_tick = 0u;
    /*  重试次数  */
    static Type_EE8 l_retry_tick    = 0u;
    /* 数据写入长度  */
    static Type_EE16 l_wr_data_len  = 0u;
    /* 错误码  */
    static Type_EE8 l_error_code    = _EEPROM_NONE_ERR;
    /* 临时数据缓存 */
    Type_EE8 l_tmp_data[64u]        = { 0u };

    if (g_Memory_Info_Tab[list_num].flag)
    {
        switch (state)
        {
                /* 初始化阶段  将需要写入的数据拷贝到数据缓存区  */
            case eeprom_init:

                /* EEPROM当前处于忙状态  一直等待  */
                if (EEPROM_BUSY == g_Eeprom_Idle_Flag)
                {
                    break;
                }

                if (0)   //bat not ok
                {
                    break;
                }

                l_error_code = _EEPROM_NONE_ERR;

                /* 判断数据源是否正确  */
                if (NULL == g_Memory_Info_Tab[list_num].w_data)
                {
                    /* 写入数据源错误  */
                    g_Memory_Info_Tab[list_num].eeprom_err = _EEPROM_SOURCE_DATA_ERR;
                    l_error_code                           = _EEPROM_SOURCE_DATA_ERR;
                    state                                  = eeprom_end;
                    break;
                }

                /* 判断数据域是否超出定义范围  */
                if (g_Memory_Info_Tab[list_num].addr_end != (g_Memory_Info_Tab[list_num].addr_start + g_Memory_Info_Tab[list_num].len))
                {
                    /* 数据域过短或者超出范围  */
                    g_Memory_Info_Tab[list_num].eeprom_err = _EEPROM_OVER_RANGE_ERR;
                    l_error_code                           = _EEPROM_OVER_RANGE_ERR;
                    state                                  = eeprom_end;
                    break;
                }

                /* 将需要写入的数据拷贝到数据缓存区  */
                Memory_Copy(g_Memory_Write_Info, g_Memory_Info_Tab[list_num].w_data, g_Memory_Info_Tab[list_num].len);
                g_Memory_Write_Addr = g_Memory_Info_Tab[list_num].addr_start;
                g_Memory_Write_len  = g_Memory_Info_Tab[list_num].len;
                g_Data_Ptr          = g_Memory_Write_Info;

                Set_Eeprom_Work_St(EEPROM_BUSY);

                state = eeprom_write;

                break;

            case eeprom_write:

                /* 写数据  */
                l_wr_data_len = Memory_Write_Multi_Byte();

                /* 写数据完成  */
                if (l_wr_data_len > 0u)
                {
                    l_timeout_tick = 0u;
                    state          = eeprom_verify;
                }
                else
                {
                    /* 返回写长度为0  则认为写入错误  */
                    state                                  = eeprom_end;
                    g_Memory_Info_Tab[list_num].eeprom_err = _EEPROM_WRITE_ERR;
                    l_error_code                           = _EEPROM_WRITE_ERR;
                }

                break;

                /*  读出数据进行校验   验证通过后 检测是否还有剩余数据需写入  若有继续执行step2  */
            case eeprom_verify:

                /* 等待写入完成  */
                if (!(m95040_read_status() & 0x01))
                {
                    l_timeout_tick = 0u;
                    m95040_Read_Byte(g_Memory_Write_Addr, l_tmp_data, l_wr_data_len);
                    //比较读出的数据和写入的数据  若不相同则认为写入失败
                    if (Memory_Compare(l_tmp_data, g_Data_Ptr, l_wr_data_len))
                    {
                        /* 更新地址和剩余长度  */
                        g_Memory_Write_Addr += l_wr_data_len;
                        g_Memory_Write_len  -= l_wr_data_len;
                        g_Data_Ptr          += l_wr_data_len;
                        l_wr_data_len        = 0u;
                        /* 若剩余长度为0   则认为写入完成  */
                        if (0u == g_Memory_Write_len)
                        {
                            state        = eeprom_end;
                            l_error_code = _EEPROM_NONE_ERR;
                        }
                        else
                        {
                            state = eeprom_write;
                            break;
                        }
                    }
                    else
                    {
                        state                                  = eeprom_end;
                        g_Memory_Info_Tab[list_num].eeprom_err = _EEPROM_VERIFY_ERR;
                        l_error_code                           = _EEPROM_VERIFY_ERR;
                    }
                }
                /*  写入超时  */
                else if (l_timeout_tick++ > 10u)
                {
                    state                                  = eeprom_end;
                    g_Memory_Info_Tab[list_num].eeprom_err = _EEPROM_TIMEOUT_ERR;
                    l_error_code                           = _EEPROM_TIMEOUT_ERR;
                }

                break;

            /* 数据写入完成  列表序号向下移一位  */
            case eeprom_end:

                /* 判断是否写入完成  若写入失败 则重试  超过重试次数  则退出该次写入  */
                if (l_error_code > _EEPROM_NONE_ERR)
                {
                    if (l_retry_tick < g_Memory_Info_Tab[list_num].retry)
                    {
                        /* 重试次数累加  */
                        l_retry_tick++;
                        /* 清故障码  */
                        l_error_code = 0u;
                        state        = eeprom_write;
                        break;
                    }
                }

                Memory_Fill(g_Memory_Write_Info, 0u, 128u);
                /* 清除写入标志位  */
                g_Memory_Info_Tab[list_num].flag = 0u;

                /* 设置EEPROM空闲  */
                Set_Eeprom_Work_St(EEPROM_IDLE);

                /* 序列号递加  */
                list_num++;

                /* 清除重试次数 */
                l_retry_tick = 0u;
                state        = eeprom_init;

                break;

            default:

                state = eeprom_init;

                break;
        }
    }
    else
    {
        list_num++;
    }

    if (EE_LIST_NUM == list_num)
    {
        list_num = 0u;
    }
}

/***************************************************************
**  Function Name 	: 获取存储内容
**  Description   	:
**  Input parameters: data 输出参数，保存读取的数据
**                    addr 读取数据起始地址
**                    len  读取数据长度
**
**  Return Value  	: 0 : 读取失败
**                    1 ：正在读取中
**                    2 : 读取成功
**
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
Type_EE8 Eeprom_Read_Data_bak(Type_EE16 addr, Type_EE8 *data, Type_EE8 len)
{
    Type_Eeprom_St l_tmp_ee_st = EEPROM_IDLE;

    /* 超时计数器 */
    Type_EE8 l_ee_counter      = 0u;

    /* 获取EEPROM状态 */
    l_tmp_ee_st                = Get_Eeprom_Work_St();

    if (EEPROM_IDLE == l_tmp_ee_st)
    {
        l_ee_counter = 0u;
        m95040_Read_Byte(addr, data, len);

        /* 读取成功返回2 */
        return (2u);
    }
    else
    {
        l_ee_counter++;
    }

    /* 此处做2个调度周期的延时处理  超时则返回读取失败 */
    if (l_ee_counter > 2u)
    {
        l_ee_counter = 0u;
        return (0u);
    }

    /* 等待EEPROM空闲返回1 */

    return (1u);
}

Type_EE8 Eeprom_Read_Data(Type_EE16 addr, Type_EE8 *data, Type_EE8 len)
{
    //此处需加判断   判断读取数据长度与输出参数的长度  判断是否越界
    //m95040_Read_Byte( addr , data , len );
    //重试次数
    unsigned int retryTime = 0;

    //实测最大需要循环等待325次
    while (retryTime < 1000)
    {
        retryTime++;
        //读写操作忙状态
        if (!(m95040_read_status() & 0x01))
        {
            //写操作完成后才能读取，否则得到的数据有错误 (全为0或者0xff)
            m95040_Read_Byte(addr, data, len);
            return (2u);
        }
    }

    //超时返回全0xff
    Memory_Fill(data, 0xff, len);

    return (0u);
}

/***************************************************************
**  Function Name 	: 上电读取配置参数
**  Description   	:
**  Input parameters: NONE
**  Return Value  	: NONE
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
void Eeprom_Powerup_Read_Parameter(void)
{
    Type_EE8 list_num = 0u;

    /* 从eeprom中获取初始数据  */
    for (list_num = 0; list_num < EE_LIST_NUM; list_num++)
    {
        /*  判断是否需要上电获取该配置   */
        if (g_Memory_Info_Tab[list_num].powerup)
        {
            Eeprom_Read_Data(g_Memory_Info_Tab[list_num].addr_start, g_Memory_Info_Tab[list_num].w_data, g_Memory_Info_Tab[list_num].len);
            /* 两次读  间隔4ms */
            /* Timer_Delay_Ms( 4u );*/
        }
    }
}

/***************************************************************
**  Function Name 	: 存储模块测试
**  Description   	:
**  Input parameters: NONE
**  Return Value  	: NONE
**  Version       	: V0.1
**  Author        	: mobo
***************************************************************/
Type_EE8 g_memory_test_arr[64];
void     Memory_Test(void)
{
    uint8_t          i           = 0u;
    static Type_EE16 l_time_tick = 0u;
    static Type_EE8  list        = 0u;
    static Type_EE8  l_pre_sig   = 0u;
    Type_EE8         l_cur_sig   = 0u;

    static enum
    {
        eeprom_init = 0,
        eeprom_run,
        eeprom_next
    } state;

    if (l_time_tick > 8u)
    {
        l_time_tick -= 8u;
    }
    else
    {
        l_time_tick = 0u;
    }

    switch (state)
    {
        case eeprom_init:

            if (l_cur_sig != l_pre_sig)
            {
                for (i = 0; i < 64; i++)
                {
                    testBuf[i]++;
                }
                EE_Set_Write_Info(i);

                state = eeprom_run;
            }

            break;

        case eeprom_run:

            l_time_tick = 2000u;
            state       = eeprom_next;

            break;

        case eeprom_next:

            if (0u == l_time_tick)
            {
                Eeprom_Read_Data(g_Memory_Info_Tab[list].addr_start, g_memory_test_arr, g_Memory_Info_Tab[list].len);
                state = eeprom_init;
                list++;
            }

            break;

        default:

            state = eeprom_init;

            break;
    }
    l_pre_sig = l_cur_sig;
}

