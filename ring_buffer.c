#include "ring_buffer.h"
 
void rb_init(ring_buffer_t *rb, uint8_t *storage, uint16_t size)
{
    rb->buf  = storage;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}
 
bool rb_put(ring_buffer_t *rb, uint8_t b)
{
    uint16_t next = (uint16_t)(rb->head + 1);
    if (next >= rb->size) next = 0;
    if (next == rb->tail) return false;
 
    rb->buf[rb->head] = b;
    rb->head = next;
    return true;
}
 
bool rb_get(ring_buffer_t *rb, uint8_t *out_byte)
{
    if (rb->head == rb->tail) return false;
 
    *out_byte = rb->buf[rb->tail];
    uint16_t next = (uint16_t)(rb->tail + 1);
    if (next >= rb->size) next = 0;
    rb->tail = next;
    return true;
}
 
uint16_t rb_count(const ring_buffer_t *rb)
{
    int32_t diff = (int32_t)rb->head - (int32_t)rb->tail;
    if (diff < 0) diff += rb->size;
    return (uint16_t)diff;
}
 
void rb_reset(ring_buffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}
 
