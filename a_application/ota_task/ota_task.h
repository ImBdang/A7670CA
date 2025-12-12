#ifndef __OTA_TASK_H__
#define __OTA_TASK_H__

/* ====================================== INCLUDE HEADER ====================================== */
#include "stm32f10x.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "task_control.h"

#include "systick.h"
#include "debugger.h"
#include "w25q32_driver.h"

#include "gsm_http.h"
#include "gsm_http_typedef.h"
#include "gsm_entity.h"


/* ============================================================================================ */

/* ====================================== DEFINTIONS ========================================== */
#define OTA_CHUNK               512
#define META_ADDR               0x08014000
#define FW_SIZE_ADDR            (META_ADDR)
#define FW_FLAG_ADDR            (META_ADDR + 4)

#define OTA_FLAG_NONE           0xFFFFFFFF
#define OTA_FLAG_READY          0xA5A5A5A5
/* ============================================================================================ */


/* ====================================== TYPEDEF STRUCT ====================================== */
/* ============================================================================================ */


/* ====================================== API DECLARATIONS ==================================== */
void ota_process(void);
/* ============================================================================================ */


#endif /* __OTA_TASK_H__ */