#ifndef TELEMETRY_TX_H
#define TELEMETRY_TX_H
 
#include <stdint.h>
#include <stdbool.h>
#include "protocol.h"
 
struct __UART_HandleTypeDef;
typedef struct __UART_HandleTypeDef UART_HandleTypeDef;
 
void     tx_init(UART_HandleTypeDef *huart);
bool     tx_send_electro(const telemetry_electro_t *data);
bool     tx_send_hydro(const telemetry_hydro_t *data);
bool     tx_send_recovery(const uint8_t *payload, uint8_t payload_len);
uint16_t tx_build_packet(uint8_t msg_id, const uint8_t *payload, uint8_t plen, uint8_t *out_buf);
uint8_t  tx_peek_seq(void);
 
#endif
