#include "led_hardware.h"

void led_hardware_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin   = LED_ONBOARD_PIN;        
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_ONBOARD_GPIO_PORT, &gpio);

    gpio.GPIO_Pin   = LED1_PIN;                
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED1_GPIO_PORT, &gpio);

    GPIO_SetBits(LED_ONBOARD_GPIO_PORT, LED_ONBOARD_PIN);
    GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN);
}
