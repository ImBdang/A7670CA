#include "led_hardware.h"

void led_hardware_init(void)
{
    RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin   = LED_ONBOARD_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_ONBOARD_GPIO_PORT, &gpio);
}