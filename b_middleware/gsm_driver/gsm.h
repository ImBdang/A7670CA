#ifndef __GSM_H__
#define __GSM_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stdint.h"
#include "stdbool.h"

#include "a7670c_hardware.h"
#include "systick.h"
#include "debugger.h"
#include "lwrb.h"

#include "gsm_typedef.h"
#include "gsm_at_engine.h"
#include "gsm_event.h"
#include "gsm_common.h"


/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ======================================= */
/* ============================================================================================= */


/* ====================================== API DECLARATIONS ==================================== */
extern gsm_state_t gsm_state;
extern gsm_decision_flag_t decision_flag;

/*<! gsm.c */
bool gsm_is_ready(void);
void gsm_init(void);
void GSM_Manager(void);
/* ============================================================================================ */

#endif /* __GSM_H__ */