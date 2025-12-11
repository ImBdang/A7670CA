#ifndef __GSM_TYPEDEF_H__
#define __GSM_TYPEDEF_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stdint.h"
#include "stdbool.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
#define AT_LWRB_SIZE        1024
#define SIM_EVENT_LWRB_SIZE 
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ======================================= */
typedef enum {
    GSM_STATE_DECISION,

    GSM_STATE_POWER_ON,

    GSM_STATE_AT_SYNC,      
    GSM_STATE_CONFIG,

    GSM_STATE_CHECK_SIM,        
    
    GSM_STATE_CHECK_NETWORK,     

    GSM_STATE_CHECK_GPRS,             

    GSM_STATE_INIT_PDP,       

    GSM_STATE_READY,             


    GSM_STATE_ERROR,          
} gsm_state_t;


typedef enum {
    GSM_CMD_CHECK_NETWORK,
    GSM_CMD_HTTP_POST,
    GSM_CMD_TCP_SEND,
    GSM_CMD_GET_IMEI,
} gsm_cmd_t;


typedef enum {
    GSM_CMD_STATE_OK,
    GSM_CMD_STATE_ERR,
    GSM_CMD_STATE_TIMEOUT,
    GSM_CMD_STATE_PRIMARY,
    GSM_CMD_STATE_INTERMEDIATE
} gsm_cmd_state_t;

typedef void (*gsm_callback_t)(gsm_cmd_state_t resp_state, const char *resp, uint8_t len);

typedef struct {
    char cmd[128];            
    char expect[32];
    uint32_t start_tick;          
    uint32_t timeout;      
    gsm_callback_t callback;         
} gsm_at_cmd_t;

typedef struct {
    bool power_on;
    bool sync_at;
    bool config;
    bool check_sim;
    bool reg_network;
    bool attach_gprs;
    bool activate_pdp;
    bool error;
    bool ready;
} gsm_decision_flag_t;
/* ============================================================================================= */


/* ====================================== API DECLARATIONS ==================================== */
/* ============================================================================================ */

#endif /* __GSM_TYPEDEF_H__ */
