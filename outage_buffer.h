#ifndef OUTAGE_BUFFER_H
#define OUTAGE_BUFFER_H
 
#include <stdint.h>
#include <stdbool.h>
#include "protocol.h"
 
#define OUTAGE_CAPACITY        64
#define OUTAGE_MAX_AGE_MS      60000UL
 
typedef struct {
    uint8_t  msg_id;
    uint8_t  plen;
    uint8_t  payload[PROTO_MAX_PAYLOAD];
    uint32_t enqueue_ms;
} outage_record_t;
 
void     outage_init(void);
bool     outage_push(uint32_t now_ms, uint8_t msg_id, const uint8_t *payload, uint8_t plen);
void     outage_age_off(uint32_t now_ms);
bool     outage_pop(outage_record_t *out);
uint16_t outage_count(void);
uint8_t  outage_fill_percent(void);
 
#endif
 
