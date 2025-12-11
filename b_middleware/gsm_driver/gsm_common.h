#ifndef __GSM_COMMON_H__
#define __GSM_COMMON_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stdint.h"
#include "stdbool.h"

#include "a7670c_hardware.h"
#include "systick.h"
#include "debugger.h"
#include "lwrb.h"

#include "gsm_event.h"
#include "gsm_cb.h"
#include "gsm_at_engine.h"
#include "gsm_typedef.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ======================================= */
/* ============================================================================================= */


/* ====================================== API DECLARATIONS ==================================== */
void gsm_common_sync_at(void);
void gsm_common_config(void);
void gsm_common_check_sim(void);
void gsm_common_check_network(void);
void gsm_common_check_gprs(void);
void gsm_common_init_pdp(void);
void gsm_common_error();
/* ============================================================================================ */

#endif /* __GSM_COMMON_H__ */