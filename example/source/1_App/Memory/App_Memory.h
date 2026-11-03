/*********************************************************************************
 * Copyright (C), 2008-2018, CJAE CO., LTD. All Rights Reserved
 * File name:   App_Memory.h
 * Author:      mobo
 * Version:     V0.1
 * Date:        2017.11.17
 * Description:
 * Others:
 * History:
 *********************************************************************************/

#ifndef __APP_MEMORY_H__
#define __APP_MEMORY_H__

/*
 * 数据类型定义
 */
typedef unsigned char  Type_EE8;
typedef unsigned short Type_EE16;
typedef unsigned int   Type_EE32;

/*
 * 存储列表长度
 */
#define EE_LIST_NUM (1u)

/*
 * 数据临时缓存长度
 */
#define EE_BUFFER_LENGHT (M95040_MEM_SIZE)

/*
 * eeprom 存储工作状态
 */
typedef enum
{
    EEPROM_IDLE = 0u,
    EEPROM_BUSY,
} Type_Eeprom_St;

/*
 * 存储列表
 */

typedef enum
{
    //peps存储
    Eeprom_vin = 0
} eEeprom_List;

/*
 *EEPROM操作错误类型定义
 */
#define _EEPROM_NONE_ERR        0u /*  eeprom操作成功  */
#define _EEPROM_SOURCE_DATA_ERR 1u /*  写入数据源错误  */
#define _EEPROM_OVER_RANGE_ERR  2u /*  写入数据长度超出范围  */
#define _EEPROM_TIMEOUT_ERR     3u /*  EEPROM写入超时错误  */
#define _EEPROM_WRITE_ERR       4u /*  写入数据错误  */
#define _EEPROM_VERIFY_ERR      5u /*  写入错误数据  */

/*
 *方式1
 */
#define App_Write_Info_To_EEPROM(id) EE_Set_Write_Info(id)

/*
 *方式2
 */
#define App_Write_Info_To_EEPROM_Addr(addr, w_data, len) EE_Set_Write_Info_Addr(addr, w_data, len)

/*
 * EEPROM读取数据
 */
#define App_Read_Info_From_Eeprom(addr, data, len) Eeprom_Read_Data(addr, data, len)

/*
 *信息存储结构体
 */
typedef struct
{
    /*  存储标志位  */
    Type_EE8 flag;

    /*  上电需读取此参数标志  */
    Type_EE8 powerup;

    /*  存储起始地址  */
    Type_EE16 addr_start;

    /*  存储中止地址  */
    Type_EE16 addr_end;

    /*  存储数据长度  */
    Type_EE16 len;

    /*  重试次数  */
    Type_EE8 retry;

    /*  距下次写入时间长度  */
    Type_EE8 tick;

    /*  写入数据地址  */
    Type_EE8 *w_data;

    /*  写入失败次数   此次数在写入成功呢后自动清零  */
    Type_EE8 failure_tick;

    /*  EEPROM故障标志  当某一处EEPROM写入失败次数 连续达到10次(根据需求定)后 则置该标志位  */
    Type_EE8 eeprom_err;

} Type_Memory_Info;

extern void     Memory_Main(void);
extern Type_EE8 EE_Get_Write_Flag(Type_EE8 list_num);
extern void     EE_Set_Write_Info(Type_EE8 list_num);
extern Type_EE8 EE_Set_Write_Info_Addr(Type_EE16 addr, Type_EE8 *w_data, Type_EE8 len);
extern Type_EE8 Eeprom_Read_Data(Type_EE16 addr, Type_EE8 *data, Type_EE8 len);

extern void    Eeprom_Powerup_Read_Parameter(void);
Type_Eeprom_St Get_Eeprom_Work_St(void);
void           Set_Eeprom_Work_St(Type_Eeprom_St st);

extern void Test_Memory(void);
extern void Memory_Test(void);

#endif
