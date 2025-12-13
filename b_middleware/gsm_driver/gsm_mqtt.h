#ifndef __GSM_MQTT_H__
#define __GSM_MQTT_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"


#include "gsm_at_engine.h"
/* ============================================================================================ */


/* ====================================== DEFINTIONS ========================================== */
//#define MQTT_URL   "tcp://13.212.179.167:1883"
#define MQTT_URL                "tcp://f6fa5bcc03514f218dfb2b84d9c1925b.s1.eu.hivemq.cloud:8883"
#define MQTT_USER               "imbdang"
#define MQTT_PASS               "Abc12345"
#define MQTT_CONTROL_TOPIC      "device/stm32f103/control"
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ====================================== */
/* ============================================================================================= */


/* ====================================== API DECLARATIONS ======================================= */
bool gsm_mqtt_process(uint8_t client_index,
                    const char *client_id,
                    uint8_t server_type,
                    uint8_t ssl_index,
                    uint8_t sslversion_mode,
                    uint8_t enable_sni,
                    uint8_t keep_alive,
                    bool clean_session);

bool gsm_mqtt_close(uint8_t client_index);


bool gsm_mqtt_sub(uint8_t client_index,
                  uint8_t req_len,
                  uint8_t qos);
/* ================================================================================================ */

#endif /* __GSM_MQTT_H__ */