#ifndef __GSM_EVENT_H__
#define __GSM_EVENT_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"

#include "gsm_event_typedef.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ====================================== */
/* ============================================================================================= */

/* ====================================== API DECLARATIONS ======================================= */
extern event_queue_t response_event_queue;
extern event_queue_t urc_event_queue;

bool push_event(event_queue_t* queue, event_t evt);
bool pop_event(event_queue_t* queue, event_t* evt);
bool event_queue_is_empty(event_queue_t* queue);
bool event_queue_is_full(event_queue_t* queue);
void event_queue_clear(event_queue_t* queue);
/* ================================================================================================== */


#endif /* __GSM_EVENT_H__ */