#ifndef __STRING_HANDLER_H__
#define __STRING_HANDLER_H__

/* ====================================== INCLUDE HEADER ======================================= */
#include "string.h"
#include "stdint.h"
/* ============================================================================================= */



/* ====================================== DEFINITIONS ========================================== */
/* ============================================================================================= */


/* ====================================== API ================================================== */
uint32_t fast_atoi(const char *s);
void fast_itoa(uint32_t value, char *buf);
uint32_t parse_ota_size(const char *line);

void gsm_parse_cpin(const char *line, char *status);
void gsm_parse_creg(const char* line, char* status);
uint8_t new_sms_dispatch(const char *s);

uint32_t gsm_parse_httpread_len(const char *line);
void gsm_parse_httpget(const char *str, uint8_t *method, uint16_t *status, uint32_t *length);
/* ============================================================================================= */

#endif /* __STRING_HANDLER_H__ */