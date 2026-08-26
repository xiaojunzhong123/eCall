/*******************************************************
  * Name    :MsgIF.c
  * Author  :Robsen
  * Function:****
  * Version :V1.0.0
  * Data    :2023/10/11
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
#include "MsgIF.h"
/*-------------variable statement---------------------*/

/* 初始化环形缓冲区 */
void ring_buffer_init(RingBuffer *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

/* 判断是否为空 */
Bool ring_buffer_is_empty(RingBuffer *rb) {
    return (Bool)(rb->count == 0);
}

/* 判断是否已满 */
Bool ring_buffer_is_full(RingBuffer *rb) {
    return (Bool)(rb->count == BUFFER_SIZE);
}

/* 写入数据（入队） */
Bool ring_buffer_push(RingBuffer *rb, uint8_t value) {
    if (ring_buffer_is_full(rb))
    {
        return FALSE;
    }
    rb->buffer[rb->tail] = value;
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    rb->count++;
    return TRUE;
}

/* 读取数据（出队） */
Bool ring_buffer_pop(RingBuffer *rb, uint8_t *value) {
    if (ring_buffer_is_empty(rb))
    {
        return FALSE;
    }
    *value = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    rb->count--;
    return TRUE;
}
