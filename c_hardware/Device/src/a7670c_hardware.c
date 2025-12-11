#include "a7670c_hardware.h"

/* ====================================== STATIC DECLARATIONS ======================================= */
static void A7670C_TX_Init(void);
static void A7670C_RX_Init(void);
static void A7670C_USART_IRQ_Init(void);
static void A7670C_USART_Init(void);
static void A7670C_PWK_Init(void);
static void A7670C_NET_Init(void);
static void A7670C_PEN_Init(void);
/* ================================================================================================== */


void A7670C_USART_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_USART_CLK, ENABLE);
    
    USART_InitStructure.USART_BaudRate = A7670C_USART_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(A7670C_USART, &USART_InitStructure);
    USART_Cmd(A7670C_USART, ENABLE);
}


void A7670C_TX_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_TX_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = A7670C_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(A7670C_TX_PORT, &GPIO_InitStructure);
}

void A7670C_RX_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_RX_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = A7670C_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(A7670C_RX_PORT, &GPIO_InitStructure);
}

void A7670C_USART_IRQ_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(A7670C_USART_CLK, ENABLE);
    
    USART_InitStructure.USART_BaudRate = A7670C_USART_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(A7670C_USART, &USART_InitStructure);

    USART_ITConfig(A7670C_USART, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void A7670C_PWK_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_PWK_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = A7670C_PWK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(A7670C_PWK_PORT, &GPIO_InitStructure);
    GPIO_SetBits(A7670C_PWK_PORT, A7670C_PWK_PIN);
}


void A7670C_NET_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_NET_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = A7670C_NET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(A7670C_NET_PORT, &GPIO_InitStructure);
}


void A7670C_PEN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(A7670C_PEN_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = A7670C_PEN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(A7670C_PEN_PORT, &GPIO_InitStructure);
    GPIO_SetBits(A7670C_PEN_PORT, A7670C_PEN_PIN);
}



void a7670c_hardware_init(void)
{
    A7670C_TX_Init();
    A7670C_RX_Init();
    A7670C_USART_IRQ_Init();
    A7670C_USART_Init();
    A7670C_PWK_Init();
    A7670C_NET_Init();
    A7670C_PEN_Init();
}