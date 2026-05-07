#include "outage_buffer.h"
#include <string.h>
 
static outage_record_t s_buf[OUTAGE_CAPACITY];
static uint16_t        s_head;
static uint16_t        s_tail;
static uint16_t        s_count;
 
void outage_init(void)
{
    s_head  = 0;
    s_tail  = 0;
    s_count = 0;
    memset(s_buf, 0, sizeof(s_buf));
}
 
bool outage_push(uint32_t now_ms, uint8_t msg_id, const uint8_t *payload, uint8_t plen)
{
    if (plen > PROTO_MAX_PAYLOAD) return false;
 
    if (s_count == OUTAGE_CAPACITY) {
        s_tail = (uint16_t)((s_tail + 1) % OUTAGE_CAPACITY);
        s_count--;
    }
 
    outage_record_t *r = &s_buf[s_head];
    r->msg_id     = msg_id;
    r->plen       = plen;
    r->enqueue_ms = now_ms;
    if (plen > 0 && payload) memcpy(r->payload, payload, plen);
 
    s_head = (uint16_t)((s_head + 1) % OUTAGE_CAPACITY);
    s_count++;
    return true;
}
 
void outage_age_off(uint32_t now_ms)
{
    while (s_count > 0) {
        outage_record_t *r = &s_buf[s_tail];
        if ((now_ms - r->enqueue_ms) <= OUTAGE_MAX_AGE_MS) break;
        s_tail = (uint16_t)((s_tail + 1) % OUTAGE_CAPACITY);
        s_count--;
    }
}
 
bool outage_pop(outage_record_t *out)
{
    if (s_count == 0 || out == NULL) return false;
 
    *out = s_buf[s_tail];
    s_tail = (uint16_t)((s_tail + 1) % OUTAGE_CAPACITY);
    s_count--;
    return true;
}
 
uint16_t outage_count(void)        { return s_count; }
uint8_t  outage_fill_percent(void) { return (uint8_t)((uint32_t)s_count * 100U / OUTAGE_CAPACITY); }
