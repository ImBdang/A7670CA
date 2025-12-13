#ifndef __GSM_ENTITY_H__
#define __GSM_ENTITY_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stdint.h"
#include "stdbool.h"

#include "a7670c_hardware.h"
#include "systick.h"
#include "debugger.h"
#include "lwrb.h"

/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ======================================= */
/*<! STRUCT STATUS FOR SIM */
typedef enum {
    SIM_READY = 0,
    SIM_PIN,
    SIM_PUK,
    SIM_UNKNOW
} gsm_sim_state_t;
typedef enum {
    CREG_NOT_REGISTERED,        // 0,0
    CREG_REGISTERED_HOME,       // 0,1
    CREG_SEARCHING,             // 0,2
    CREG_REG_DENIED,            // 0,3
    CREG_UNKNOWN,               // 0,4
    CREG_REGISTERED_ROAMING,    // 0,5
    CREG_REGISTERED_SMS,        // 0,6
    CREG_REGISTERED_SMS_ROAM    // 0,7
} gsm_sim_creg_state_t;

typedef struct {
    char iccid[24];  
    gsm_sim_state_t state;
    gsm_sim_creg_state_t creg;
} sim_entity_t;

/*<! STRUCT DATA FOR HTTP */
typedef struct {
    uint16_t status_code;
    uint32_t data_length;
    uint8_t method;
    uint8_t  ready;

    uint8_t read_done;
    uint8_t post_done;
} http_action_entity_t;


/*<! STRUCT DATA FOR MQTT */
typedef struct {
    
} mqtt_entity_t;

/* ============================================================================================= */


/* ====================================== API DECLARATIONS ==================================== */
gsm_sim_state_t get_sim_state(void);
gsm_sim_creg_state_t get_sim_creg_state(void);
void set_sim_state(gsm_sim_state_t state);
void set_sim_creg_state(gsm_sim_creg_state_t creg);

void set_http_action(uint8_t ready, uint8_t method, uint16_t status, uint32_t data_length);
/* ============================================================================================ */

#endif /* __GSM_ENTITY_H__ */