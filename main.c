#include "stm32f10x.h"

#include "hardware_init.h"
#include "debugger.h"
#include "a7670c_hardware.h"
#include "systick.h"

#include "task_config.h"
#include "ota_task.h"

void W25Q32_DumpFirst128(void)
{
    uint8_t buf[128];

    W25Q32_ReadData(0x000000, buf, 128);

    DEBUG_PRINT("\r\n===== W25Q32 FIRST 128 BYTES =====\r\n");

    for (int i = 0; i < 128; i++) {
        DEBUG_PRINT("%02X ", buf[i]);
        if ((i + 1) % 16 == 0) DEBUG_PRINT("\r\n");
    }

    DEBUG_PRINT("===== END FIRST 128 =====\r\n\r\n");
}


void W25Q32_DumpLast128(uint32_t fw_size)
{
    uint8_t buf[128];

    if (fw_size < 128) {
        DEBUG_PRINT("FW too small to dump last 128 bytes!\r\n");
        return;
    }

    uint32_t addr = fw_size - 128;

    W25Q32_ReadData(addr, buf, 128);

    DEBUG_PRINT("===== W25Q32 LAST 128 BYTES (addr 0x%06lX) =====\r\n", addr);

    for (int i = 0; i < 128; i++) {
        DEBUG_PRINT("%02X ", buf[i]);
        if ((i + 1) % 16 == 0) DEBUG_PRINT("\r\n");
    }

    DEBUG_PRINT("===== END LAST 128 =====\r\n\r\n");
}

void W25Q32_CheckFirmware(uint32_t fw_size)
{
    DEBUG_PRINT("\r\n========== CHECKING W25Q32 FIRMWARE ==========\r\n");

    W25Q32_DumpFirst128();
    W25Q32_DumpLast128(fw_size);

    DEBUG_PRINT("========== END CHECK ==========\r\n\r\n");
}

extern uint32_t __stack_chk;

void dbg_dump_sp(void)
{
    register uint32_t sp_val __asm("sp");
    static uint32_t last = 0;

    if ((sp_val & 0xFF000000) != 0x20000000) {
        DEBUG_PRINT("SP CORRUPT!!! sp=0x%08lX\r\n", sp_val);
        while (1);
    }

    if (last == 0) {
        last = sp_val;
    }
}


current_task_t cur_task = IDLE;

/* ====================================== EXTERNAL VARIABLES ======================================= */
/* ================================================================================================= */

int main(void)
{
    SCB->VTOR = 0x08004000;
    Clock_72MHz_HSE_Init();
    hardware_init();
    a7670c_hardware_init();
    gsm_init();
    DEBUG_PRINT("APP RUN\r\n");
    // W25Q32_CheckFirmware(21512);
    //GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    cur_task = 1;
    __enable_irq();
    while(1){
        dbg_dump_sp();
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