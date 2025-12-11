#include "hardware_init.h"

void hardware_init(void){
    tty_hardware_init();
    w25q32_hardware_init();
    a7670c_hardware_init();
    systick_init(1000);
}