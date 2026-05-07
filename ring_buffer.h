#ifndef RING_BUFFER_H
#define RING_BUFFER_H
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
 
typedef struct {
    uint8_t  *buf;
    uint16_t  size;
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_buffer_t;
 
void     rb_init(ring_buffer_t *rb, uint8_t *storage, uint16_t size);
bool     rb_put(ring_buffer_t *rb, uint8_t b);
bool     rb_get(ring_buffer_t *rb, uint8_t *out_byte);
uint16_t rb_count(const ring_buffer_t *rb);
void     rb_reset(ring_buffer_t *rb);
 
#endif
