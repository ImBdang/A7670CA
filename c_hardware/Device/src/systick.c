#include "systick.h"

volatile uint32_t msTicks = 0;

void systick_init(uint32_t ticks_per_second) {
    SystemCoreClockUpdate();

    uint32_t reload = SystemCoreClock / ticks_per_second;
    if (reload > SysTick_LOAD_RELOAD_Msk) {
        while (1);
    }

    SysTick_Config(reload);
}

void delay_ms(uint32_t ms) {
    uint32_t start = msTicks;
    while ((msTicks - start) < ms) {}
}

uint32_t get_systick(void) {
    return msTicks;
}
