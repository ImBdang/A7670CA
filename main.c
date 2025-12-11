#include "stm32f10x.h"

#include "hardware_init.h"
#include "debugger.h"
#include "a7670c_hardware.h"
#include "systick.h"

#include "task_config.h"
#include "ota_task.h"

current_task_t cur_task = IDLE;

/* ====================================== EXTERNAL VARIABLES ======================================= */
/* ================================================================================================= */

int main(void)
{
    Clock_72MHz_HSE_Init();
    hardware_init();
    a7670c_hardware_init();
    gsm_init();
    DEBUG_PRINT("APP RUN\r\n");
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    // GPIO_SetBits(GPIOC, GPIO_Pin_13);
    while(1){
        bool tmp = false;
        GSM_Manager();
        if (gsm_state == GSM_STATE_READY)
        {
            switch (cur_task)
            {   
                case OTA:
                    ota_process();
                    break;

                case IDLE:
                    break;
            }
            delay_ms(150);
        }
    }
    
    return 0;
}