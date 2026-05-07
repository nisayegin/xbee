#ifndef TELEMETRY_RX_H
#define TELEMETRY_RX_H
 
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "protocol.h"
 
typedef struct {
    uint32_t packets_ok;
    uint32_t crc_errors;
    uint32_t length_errors;
    uint32_t resyncs;
    uint32_t bytes_processed;
} rx_stats_t;
 
typedef void (*rx_packet_handler_t)(uint8_t msg_id, uint8_t seq, const uint8_t *payload, uint8_t plen);
 
void              rx_init(rx_packet_handler_t handler);
void              rx_feed_byte(uint8_t b);
void              rx_feed(const uint8_t *data, size_t len);
void              rx_reset(void);
const rx_stats_t *rx_get_stats(void);
 
#endif
 
