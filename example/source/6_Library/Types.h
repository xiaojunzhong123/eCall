/*******************************************************
  * Name    :Types.h
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
#ifndef _TYPES_H_
#define _TYPES_H_
/*-------------typedef--------------------------------*/
#include <stdint.h>
typedef unsigned char bits_t;
//typedef signed char                         int8_t;
//typedef signed short                        int16_t;
//typedef signed long                         int32_t;
//typedef unsigned char                       uint8_t;
//typedef unsigned short                      uint16_t;
//typedef unsigned long                       uint32_t;
/*-------------define---------------------------------*/
#ifndef NULL
    #define NULL ((void *)0)
#endif
#define OFF                       0
#define ON                        1
#define TP_DISABLE                   0
#define TP_ENABLE                    1
#define CRC_16                    0
#define CRC_32                    1
#define ArraySize(arr)            sizeof(arr) / sizeof((arr)[0])
#define LL_PERIPH_WRITE_ENABLE()  LL_PERIPH_WE(0XFF)
#define LL_PERIPH_WRITE_DISABLE() LL_PERIPH_WP(0XFF)
/*-------------define user----------------------------*/
#define LED_NUM                   31
/*-------------enum and struct------------------------*/
typedef enum
{
    FALSE = 0,
    TRUE
} Bool;
typedef enum
{
    LOW = 0x00,
    HIGH
} IO_Type;
typedef enum
{
    CAN = 0,
    CANFD
} Frame_Type;

typedef enum
{
    EVENT = 0x00,
    CYCLE,
    CYCEV
} Msg_Type;

typedef enum
{
    RET_OK = 0x00,
    RET_NOT_OK,
    RET_RCRRP,
    RET_PENDING,
} Return_Type;

typedef enum
{
    NO_ERROR = 0,
    ERROR_RX_PID,
    ERROR_RX_DATA,
    ERROR_RX_FRAME,
    ERROR_RX_CHECKSUM,
    ERROR_RX_SYNC_FILED,
    ERROR_NOT_RESPONE,
} Error_Type;

typedef struct
{
    Frame_Type frame;
    uint32_t   id;
    uint8_t    len;
    uint8_t    buff[64];
} Can_Type;
#endif

