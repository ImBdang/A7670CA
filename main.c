#include "stm32f10x.h"

#include "hardware_init.h"
#include "debugger.h"
#include "a7670c_hardware.h"
#include "systick.h"

#include "gsm_mqtt.h"

#include "task_control.h"
#include "ota_task.h"

// void W25Q32_DumpFirst128(void)
// {
//     uint8_t buf[128];

//     W25Q32_ReadData(0x000000, buf, 128);

//     DEBUG_PRINT("\r\n===== W25Q32 FIRST 128 BYTES =====\r\n");

//     for (int i = 0; i < 128; i++) {
//         DEBUG_PRINT("%02X ", buf[i]);
//         if ((i + 1) % 16 == 0) DEBUG_PRINT("\r\n");
//     }

//     DEBUG_PRINT("===== END FIRST 128 =====\r\n\r\n");
// }


// void W25Q32_DumpLast128(uint32_t fw_size)
// {
//     uint8_t buf[128];

//     if (fw_size < 128) {
//         DEBUG_PRINT("FW too small to dump last 128 bytes!\r\n");
//         return;
//     }

//     uint32_t addr = fw_size - 128;

//     W25Q32_ReadData(addr, buf, 128);

//     DEBUG_PRINT("===== W25Q32 LAST 128 BYTES (addr 0x%06lX) =====\r\n", addr);

//     for (int i = 0; i < 128; i++) {
//         DEBUG_PRINT("%02X ", buf[i]);
//         if ((i + 1) % 16 == 0) DEBUG_PRINT("\r\n");
//     }

//     DEBUG_PRINT("===== END LAST 128 =====\r\n\r\n");
// }

// void W25Q32_CheckFirmware(uint32_t fw_size)
// {
//     DEBUG_PRINT("\r\n========== CHECKING W25Q32 FIRMWARE ==========\r\n");

//     W25Q32_DumpFirst128();
//     W25Q32_DumpLast128(fw_size);

//     DEBUG_PRINT("========== END CHECK ==========\r\n\r\n");
// }

/* ====================================== EXTERNAL VARIABLES ======================================= */
/* ================================================================================================= */

extern char promt_want_to_send[64];

int main(void)
{
    __enable_irq();
    SCB->VTOR = 0x08004000;
    Clock_72MHz_HSE_Init();
    hardware_init();
    a7670c_hardware_init();
    gsm_init();

    //GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    LED1_ON();

    DEBUG_PRINT("APP RUN\r\n");
    while(1){
        // gsm_common_delete_all_sms();
        bool tmp = false;
        GSM_Manager();
        if (gsm_state == GSM_STATE_READY)
        {
            current_task_t cur_task = get_cur_task();
            switch (cur_task)
            {   
                case OTA_TASK:
                    ota_process();
                    break;

                case MQTT_START:
                    if (gsm_mqtt_process(0, "stm32f103", 1, 0, 4, 1, 60, true))
                        set_cur_task(IDLE_TASK);
                    break;

                case MQTT_STOP:
                    if (gsm_mqtt_close(0))
                        set_cur_task(IDLE_TASK);
                    break;


                case MQTT_SUB:
                    if (gsm_mqtt_sub(0, 24, 1))
                        set_cur_task(IDLE_TASK);
                    break;

                case IDLE_TASK:
                    break;
            }
        
        }
        delay_ms(250);
    }
    
    return 0;
}