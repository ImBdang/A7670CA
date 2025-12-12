#ifndef __GSM_SMS_H__
#define __GSM_SMS_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"

#include "task_control.h"

#include "gsm_http_typedef.h"
#include "gsm_typedef.h"
#include "gsm_at_engine.h"
#include "gsm_cb.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ====================================== */
typedef struct {
    uint8_t idx;
    char content[128];
} sms_entity_t;

typedef enum {
    SMS_IDLE = 0,
    SMS_READING
} sms_state_t;
/* ============================================================================================= */

/* ====================================== API DECLARATIONS ======================================= */
bool send_sms(const char* str, const char* phone);
void read_sms(uint8_t idx);
void delete_sms(uint8_t idx);

void gsm_common_delete_all_sms(void);

void check_sms(const char* str);
void set_new_sms_idx(uint8_t idx);
void set_new_sms_content(const char* content);
/* ================================================================================================== */

#endif /* __GSM_SMS_H__ */