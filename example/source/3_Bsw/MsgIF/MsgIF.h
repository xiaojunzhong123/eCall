/*******************************************************
  * Name    :MsgIF.h
  * Author  :Robsen
  * Function:****
  * Version :V1.0.0
  * Data    :2023/10/11
*******************************************************/
#ifndef _MSGIF_H_
#define _MSGIF_H_
/*-------------include file---------------------------*/
#include "Types.h"
/*-------------function port--------------------------*/
#define BUFFER_SIZE 2048  // 缓冲区大小

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    int head;        // 读指针
    int tail;        // 写指针
    int count;       // 当前元素数量
}RingBuffer;
void ring_buffer_init(RingBuffer *rb);
Bool ring_buffer_is_empty(RingBuffer *rb);
Bool ring_buffer_is_full(RingBuffer *rb);
Bool ring_buffer_push(RingBuffer *rb, uint8_t value);
Bool ring_buffer_pop(RingBuffer *rb, uint8_t *value);
#endif

