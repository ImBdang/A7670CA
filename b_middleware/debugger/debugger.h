#ifndef __DEBUG_H__
#define __DEBUG_H__

/* ====================================== INCLUDE HEADER ======================================= */
#include "stdint.h"
#include <stdarg.h>
#include <stdio.h>
#include "usart_driver.h"
#include "tty_hardware.h"
#include "stm32f10x.h"
/* ============================================================================================= */



/* ====================================== DEFINITIONS ========================================== */
#define DEBUG_PRINT(fmt, ...) debug_printf(fmt, ##__VA_ARGS__)

// #ifdef DEBUG_ENABLE
//     #define DEBUG_PRINT(fmt, ...) debug_printf(fmt, ##__VA_ARGS__)
// #else
//     #define DEBUG_PRINT(fmt, ...)
// #endif
/* ============================================================================================= */



/* ========================================= API =============================================== */
void debug_printf(const char* fmt, ...);
void breakp(void);
/* ============================================================================================= */

#endif /* __DEBUG_H__ */