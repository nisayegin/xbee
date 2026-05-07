#include "telemetry_rx.h"
#include "crc16.h"
#include <string.h>
 
typedef enum {
    ST_WAIT_SYNC1 = 0,
    ST_WAIT_SYNC2,
    ST_WAIT_LEN,
    ST_READ_BODY,
    ST_READ_CRC_HIGH,
    ST_READ_CRC_LOW
} rx_state_t;
 
static rx_state_t          s_state   = ST_WAIT_SYNC1;
static rx_packet_handler_t s_handler = NULL;
static rx_stats_t          s_stats   = {0};
 
static uint8_t  s_len     = 0;
static uint8_t  s_body[PROTO_MAX_PAYLOAD + 2];
static uint8_t  s_body_idx = 0;
static uint16_t s_crc_recv = 0;
 
void rx_init(rx_packet_handler_t handler)
{
    s_handler = handler;
    rx_reset();
    memset(&s_stats, 0, sizeof(s_stats));
}
 
void rx_reset(void)
{
    s_state    = ST_WAIT_SYNC1;
    s_len      = 0;
    s_body_idx = 0;
    s_crc_recv = 0;
}
 
const rx_stats_t *rx_get_stats(void)
{
    return &s_stats;
}
 
void rx_feed_byte(uint8_t b)
{
    s_stats.bytes_processed++;
 
    switch (s_state) {
    case ST_WAIT_SYNC1:
        if (b == PROTO_SYNC1) s_state = ST_WAIT_SYNC2;
        break;
 
    case ST_WAIT_SYNC2:
        if (b == PROTO_SYNC2) {
            s_state = ST_WAIT_LEN;
        } else if (b == PROTO_SYNC1) {
            s_state = ST_WAIT_SYNC2;
        } else {
            s_stats.resyncs++;
            s_state = ST_WAIT_SYNC1;
        }
        break;
 
    case ST_WAIT_LEN:
        if (b < 2 || b > (PROTO_MAX_PAYLOAD + 2)) {
            s_stats.length_errors++;
            rx_reset();
            return;
        }
        s_len      = b;
        s_body_idx = 0;
        s_state    = ST_READ_BODY;
        break;
 
    case ST_READ_BODY:
        s_body[s_body_idx++] = b;
        if (s_body_idx >= s_len) s_state = ST_READ_CRC_HIGH;
        break;
 
    case ST_READ_CRC_HIGH:
        s_crc_recv = (uint16_t)b << 8;
        s_state    = ST_READ_CRC_LOW;
        break;
 
    case ST_READ_CRC_LOW: {
        s_crc_recv |= (uint16_t)b;
 
        uint16_t crc = crc16_ccitt_update(0xFFFF, &s_len, 1);
        crc = crc16_ccitt_update(crc, s_body, s_len);
 
        if (crc == s_crc_recv) {
            s_stats.packets_ok++;
            uint8_t msg_id = s_body[0];
            uint8_t seq    = s_body[1];
            uint8_t plen   = (uint8_t)(s_len - 2);
            const uint8_t *payload = (plen > 0) ? &s_body[2] : NULL;
            if (s_handler) s_handler(msg_id, seq, payload, plen);
        } else {
            s_stats.crc_errors++;
        }
        rx_reset();
        break;
    }
 
    default:
        rx_reset();
        break;
    }
}
 
void rx_feed(const uint8_t *data, size_t len)
{
    while (len--) rx_feed_byte(*data++);
}
 
