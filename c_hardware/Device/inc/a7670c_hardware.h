#ifndef __A7670C_HW_H__
#define __A7670C_HW_H__

#include "stm32f10x.h"

/* ====================================== DEFINITIONS ================================================ */
#define A7670C_USART               USART1
#define A7670C_USART_CLK           RCC_APB2Periph_USART1
#define A7670C_USART_BAUD          115200

#define A7670C_TX_PORT             GPIOA
#define A7670C_TX_PIN              GPIO_Pin_9
#define A7670C_TX_CLK              RCC_APB2Periph_GPIOA

#define A7670C_RX_PORT             GPIOA
#define A7670C_RX_PIN              GPIO_Pin_10
#define A7670C_RX_CLK              RCC_APB2Periph_GPIOA

#define A7670C_PWK_PORT            GPIOA
#define A7670C_PWK_PIN             GPIO_Pin_11
#define A7670C_PWK_CLK             RCC_APB2Periph_GPIOA

#define A7670C_NET_PORT            GPIOA
#define A7670C_NET_PIN             GPIO_Pin_12
#define A7670C_NET_CLK             RCC_APB2Periph_GPIOA

#define A7670C_PEN_PORT            GPIOA
#define A7670C_PEN_PIN             GPIO_Pin_15
#define A7670C_PEN_CLK             RCC_APB2Periph_GPIOA

#define SIM_PWKEY_LOW()            GPIO_ResetBits(A7670C_PWK_PORT, A7670C_PWK_PIN)
#define SIM_PWKEY_HIGH()           GPIO_SetBits(A7670C_PWK_PORT, A7670C_PWK_PIN)
#define SIM_STATUS_READ()          GPIO_ReadInputDataBit(A7670C_NET_PORT, A7670C_NET_PIN)
/* ================================================================================================== */




/* ====================================== API ========================================== */
void a7670c_hardware_init(void);
/* ===================================================================================== */

#endif /* __A7670C_HW_H__ */
