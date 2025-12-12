#ifndef __LED_HW_H__
#define __LED_HW_H__

/* ====================================== INCLUDE HEADER =============================== */
#include "stm32f10x.h"
/* ===================================================================================== */

/* ====================================== DEFINE ======================================= */
#define LED_ONBOARD_GPIO_PORT           GPIOC
#define LED_ONBOARD_PIN                 GPIO_Pin_13

#define LED1_GPIO_PORT                  GPIOA
#define LED1_PIN                        GPIO_Pin_11

#define LED_ON()                        GPIO_ResetBits(LED_ONBOARD_GPIO_PORT, LED_ONBOARD_PIN)
#define LED_OFF()                       GPIO_SetBits(LED_ONBOARD_GPIO_PORT, LED_ONBOARD_PIN)

#define LED1_ON()                       GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF()                      GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN)
                           
/* ===================================================================================== */

/* ====================================== API ========================================== */
void led_hardware_init(void);
/* ===================================================================================== */

#endif /* __LED_HW_H__ */