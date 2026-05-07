#ifndef PROTOCOL_H
#define PROTOCOL_H
 
#include <stdint.h>
 
#define PROTO_SYNC1            0xAA
#define PROTO_SYNC2            0x55
#define PROTO_HEADER_SIZE      5
#define PROTO_CRC_SIZE         2
#define PROTO_MAX_PAYLOAD      64
#define PROTO_MAX_PACKET       (PROTO_HEADER_SIZE + PROTO_MAX_PAYLOAD + PROTO_CRC_SIZE)
 
typedef enum {
    MSG_TELEMETRY_ELECTRO  = 0x01,
    MSG_TELEMETRY_HYDRO    = 0x02,
    MSG_OUTAGE_RECOVERY    = 0x10,
    MSG_HEARTBEAT_ACK      = 0xFE
} msg_id_t;
 
#pragma pack(push, 1)
 
typedef struct {
    uint32_t zaman_ms;
    uint16_t hiz_kmh;
    int16_t  T_bat_x10;
    uint16_t V_bat_x10;
    uint16_t kalan_enerji_Wh;
} telemetry_electro_t;
 
typedef struct {
    uint32_t zaman_ms;
    uint16_t hiz_kmh;
    int16_t  T_bat_x10;
    int16_t  T_tank_x10;
    uint16_t V_bat_x10;
    uint16_t kalan_enerji_Wh;
} telemetry_hydro_t;
 
#pragma pack(pop)
 
#define TELEMETRY_ELECTRO_SIZE   ((uint8_t)sizeof(telemetry_electro_t))
#define TELEMETRY_HYDRO_SIZE     ((uint8_t)sizeof(telemetry_hydro_t))
 
#endif
