#ifndef __GSM_AT_ENGINE_H__
#define __GSM_AT_ENGINE_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "lwrb.h"
#include "systick.h"
#include "string_handler.h"
#include "debugger.h"

#include "gsm_http_typedef.h"
#include "gsm_event.h"
#include "gsm_entity.h"
#include "gsm_typedef.h"
#include "gsm.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ======================================= */

/* ============================================================================================= */
/* ====================================== API DECLARATIONS ==================================== */
void gsm_at_process(void);
bool send_at_cmd(gsm_at_cmd_t cmd);
void at_engine_init(void);
/* ============================================================================================ */

#endif /* __GSM_AT_ENGINE_H__ */