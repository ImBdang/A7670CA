#include "tty_hardware.h"

/* ====================================== STATIC DECLARATIONS ======================================= */
static void USART_Debugger_Init(void);
/* ================================================================================================== */

static void USART_Debugger_Init(void)
{
    RCC_APB2PeriphClockCmd(USART_DEBUGGER_RCC_GPIO, ENABLE);
    RCC_APB1PeriphClockCmd(USART_DEBUGGER_RCC_USART, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = USART_DEBUGGER_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
    GPIO_Init(USART_DEBUGGER_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = USART_DEBUGGER_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_DEBUGGER_GPIO_PORT, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = USART_DEBUGGER_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART_DEBUGGER_PORT, &USART_InitStructure);
    USART_Cmd(USART_DEBUGGER_PORT, ENABLE);
}

void tty_hardware_init(void)
{
    USART_Debugger_Init();
}