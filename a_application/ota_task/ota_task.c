#include "ota_task.h"

/* ====================================== GLOBAL VARIABES =========================================== */
extern http_action_entity_t http_action_entity;
extern current_task_t cur_task;
extern uint32_t http_reading_chunk;
extern volatile uint32_t cur_w25_addr;
extern uint8_t http_data_buff[HTTP_DATA_BUFFER];
extern uint8_t* http_buff_ptr;
/* ================================================================================================== */

/* ====================================== STATIC DECLARATIONS ======================================= */
static uint8_t sharedStep = 0;
static char ota_url[64] = "http://13.212.179.167/ota/z_build/a7670c.bin";

static uint32_t cur_offset = 0;
static uint32_t chunk = 0;

static void set_fw_size(uint32_t size);
static void set_fw_flag(uint32_t flag);
/* ================================================================================================== */

void ota_process(void)
{
    bool tmp = false;
    switch (sharedStep)
    {
        case 0:
            tmp = gsm_http_init();
            if (tmp)
                sharedStep++;
            break;

        case 1:
            tmp = gsm_http_set_url(ota_url);
            if (tmp)
                sharedStep++;
            break;
        case 2:
            tmp = gsm_http_action(0);
            if (tmp){
                sharedStep++;
            }
            break;

        case 3:
            if (http_action_entity.ready == 1)
            {
                set_fw_flag(OTA_FLAG_READY);
                set_fw_size(http_action_entity.data_length);
                DEBUG_PRINT("Erase W25Q32 chip\r\n");
                W25Q32_EraseChip();
                DEBUG_PRINT("Erase chip done\r\n");
                sharedStep++;
            }
            break;
        
        case 4:
            if (http_action_entity.ready == 1)
            {
                chunk = (http_action_entity.data_length > OTA_CHUNK) ? OTA_CHUNK : http_action_entity.data_length;
                http_reading_chunk = chunk;
                gsm_http_read(cur_offset, OTA_CHUNK);

                if (http_action_entity.read_done == 1)
                {
                    DEBUG_PRINT("Writing %d bytes to 0x%06lX\r\n", chunk, cur_w25_addr);
                    W25Q32_WriteData(cur_w25_addr, http_data_buff, chunk);
                    DEBUG_PRINT("Write done\r\n");
                    http_action_entity.data_length -= chunk;
                    cur_w25_addr += chunk;
                    cur_offset += chunk;
                }
                if (http_action_entity.data_length == 0)
                {
                    DEBUG_PRINT("FIRMWARE SAVED\r\n");
                    sharedStep++;
                }
            }
            break;

        case 6:
            tmp = gsm_http_term();
            if (tmp)
            {
                sharedStep = 0;
                cur_task = 0;
            }

            break;
    }
}

static void set_fw_size(uint32_t size){
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ProgramWord(FW_SIZE_ADDR, size);
    FLASH_Lock();
}

static void set_fw_flag(uint32_t flag){
    if (flag != OTA_FLAG_NONE && flag != OTA_FLAG_READY)
        return;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ProgramWord((uint32_t)FW_FLAG_ADDR, flag);
    FLASH_Lock();
}
