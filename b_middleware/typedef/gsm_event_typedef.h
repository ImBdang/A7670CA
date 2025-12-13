#ifndef __GSM_EVENT_TYPEDEF_H__
#define __GSM_EVENT_TYPEDEF_H__

/* ====================================== INCLDE HEADER ======================================= */
#include "stdint.h"
/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
#define EVENT_RESPONSE_QUEUE_SIZE           512
#define EVENT_URC_QUEUE_SIZE                512
/* ============================================================================================ */


/* ====================================== TYPEDEF DEFINITION ================================== */
typedef enum {
    RESPONSE = 0,
    URC
} event_queue_type_t;

typedef enum {
    EVT_OK = 0,
    EVT_ERR,
    EVT_TIMEOUT,
    EVT_PRIMARY
} response_event_t;

typedef enum {
    URC_SMS_NEW = 0,
    URC_SMS_RECEIVED,
    
    URC_HTTPACTION,
    URC_HTTPREAD,

    URC_RING
} urc_event_t;

typedef union  {
    response_event_t response;
    urc_event_t urc;
} event_t;


typedef struct {
    union {
        struct {
            response_event_t data[EVENT_RESPONSE_QUEUE_SIZE];
        } response;

        struct {
            urc_event_t data[EVENT_URC_QUEUE_SIZE];
        } urc;
    } queue;
    event_queue_type_t type;
    uint16_t queue_size;
    uint8_t head;
    uint8_t tail;
} event_queue_t;
/* ============================================================================================ */

#endif /* __GSM_EVENT_TYPEDEF_H__ */