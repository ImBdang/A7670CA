#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"
#include <stdint.h>

extern volatile uint32_t msTicks;

void systick_init(uint32_t ticks_per_second);
void delay_ms(uint32_t ms);
uint32_t get_systick(void);

#endif
