#ifndef __GSM_MQTT_H__
#define __GSM_MQTT_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"


#include "gsm_at_engine.h"
/* ============================================================================================ */


/* ====================================== DEFINTIONS ========================================== */
#define MQTT_URL   "tcp://13.212.179.167:1883"
#define MQTT_USER  "imbdang"
#define MQTT_PASS  "Abc12345"
#define MQTT_TOPIC "device/stm32f103/control"
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ====================================== */
/* ============================================================================================= */


/* ====================================== API DECLARATIONS ======================================= */
bool gsm_mqtt_stop(void);
bool gsm_mqtt_start(void);
bool check_time(void);
bool check_internet(void);

bool gsm_mqtt_sni_config(uint8_t ssl_index, uint8_t client_index);
bool gsm_mqtt_sslver_config(uint8_t ssl_index, uint8_t mode);
bool gsm_mqtt_acquire_client(uint8_t client_index, const char* client_id, uint8_t server_type);
bool gsm_mqtt_release_client(uint8_t client_index);
bool gsm_mqtt_tls_config(uint8_t client_index, uint8_t ssl_index);
bool gsm_mqtt_connect_broker(uint8_t client_index, uint8_t keep_alive, bool clean);
/* ================================================================================================ */

#endif /* __GSM_MQTT_H__ */