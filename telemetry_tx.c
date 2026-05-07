#include "telemetry_tx.h"
#include "crc16.h"
#include "stm32f4xx_hal.h"
#include <string.h>
 
static UART_HandleTypeDef *s_huart = NULL;
static uint8_t             s_seq   = 0;
static const uint32_t      TX_TIMEOUT_MS = 100;
 
uint16_t tx_build_packet(uint8_t msg_id, const uint8_t *payload, uint8_t plen, uint8_t *out_buf)
{
    if (out_buf == NULL || plen > PROTO_MAX_PAYLOAD) return 0;
 
    uint16_t pos = 0;
    out_buf[pos++] = PROTO_SYNC1;
    out_buf[pos++] = PROTO_SYNC2;
 
    uint8_t len = (uint8_t)(2 + plen);
    out_buf[pos++] = len;
    out_buf[pos++] = msg_id;
    out_buf[pos++] = s_seq++;
 
    if (plen > 0 && payload != NULL) {
        memcpy(&out_buf[pos], payload, plen);
        pos = (uint16_t)(pos + plen);
    }
 
    uint16_t crc = crc16_ccitt(&out_buf[2], (size_t)(1 + 2 + plen));
    out_buf[pos++] = (uint8_t)(crc >> 8);
    out_buf[pos++] = (uint8_t)(crc & 0xFF);
    return pos;
}
 
static bool tx_send_raw(uint8_t msg_id, const uint8_t *payload, uint8_t plen)
{
    if (s_huart == NULL) return false;
 
    uint8_t  pkt[PROTO_MAX_PACKET];
    uint16_t len = tx_build_packet(msg_id, payload, plen, pkt);
    if (len == 0) return false;
 
    return HAL_UART_Transmit(s_huart, pkt, len, TX_TIMEOUT_MS) == HAL_OK;
}
 
void tx_init(UART_HandleTypeDef *huart)
{
    s_huart = huart;
    s_seq   = 0;
}
 
bool tx_send_electro(const telemetry_electro_t *data)
{
    if (data == NULL) return false;
    return tx_send_raw(MSG_TELEMETRY_ELECTRO, (const uint8_t *)data, TELEMETRY_ELECTRO_SIZE);
}
 
bool tx_send_hydro(const telemetry_hydro_t *data)
{
    if (data == NULL) return false;
    return tx_send_raw(MSG_TELEMETRY_HYDRO, (const uint8_t *)data, TELEMETRY_HYDRO_SIZE);
}
 
bool tx_send_recovery(const uint8_t *payload, uint8_t payload_len)
{
    return tx_send_raw(MSG_OUTAGE_RECOVERY, payload, payload_len);
}
 
uint8_t tx_peek_seq(void)
{
    return s_seq;
}
 
