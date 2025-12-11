#include "hardware_init.h"

void hardware_init(void){
    SCB->VTOR = 0x08004000;
    tty_hardware_init();
    w25q32_hardware_init();
    a7670c_hardware_init();
    led_hardware_init();
    systick_init(1000);
}