/**
  ******************************************************************************
  * @file    stm32f1xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_it.h"

extern lwrb_t at_rb;
extern volatile uint32_t msTicks;

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                          */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
__attribute__((naked))
void HardFault_Handler(void)
{
    __asm volatile
    (
        "tst lr, #4            \n"
        "ite eq                \n"
        "mrseq r0, msp         \n" // r0 = pointer tới stack frame
        "mrsne r0, psp         \n"
        "b HardFault_Handler_C \n"
    );
}

void HardFault_Handler_C(uint32_t *stack)
{
    uint32_t r0  = stack[0];
    uint32_t r1  = stack[1];
    uint32_t r2  = stack[2];
    uint32_t r3  = stack[3];
    uint32_t r12 = stack[4];
    uint32_t lr  = stack[5];       // return address
    uint32_t pc  = stack[6];       // chỗ crash
    uint32_t psr = stack[7];

    uint32_t cfsr = SCB->CFSR;
    uint32_t hfsr = SCB->HFSR;

    DEBUG_PRINT("\r\n[HardFault]\r\n");
    DEBUG_PRINT(" PC  = 0x%08lX\r\n", pc);
    DEBUG_PRINT(" LR  = 0x%08lX\r\n", lr);
    DEBUG_PRINT(" CFSR=0x%08lX HFSR=0x%08lX\r\n", cfsr, hfsr);
    DEBUG_PRINT(" r0=0x%08lX r1=0x%08lX r2=0x%08lX r3=0x%08lX r12=0x%08lX\r\n",
                r0, r1, r2, r3, r12);

    while (1);
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  uint32_t cfsr = SCB->CFSR;
  uint32_t bfar = SCB->BFAR;
  DEBUG_PRINT("BusFault! CFSR=0x%08lX, BFAR=0x%08lX\r\n", cfsr, bfar);
  while (1) {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


void SysTick_Handler(void)
{
  msTicks++;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
      uint8_t data = USART_ReceiveData(USART1);
      lwrb_write(&at_rb, &data, 1);
    }
}


/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_md.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
