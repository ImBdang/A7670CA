#ifndef __W25Q32_H__
#define __W25Q32_H__

/* ====================================== INCLUDE HEADER =============================== */
#include "stm32f10x.h"
#include "spi_driver.h"
#include "w25q32_hardware.h"
/* ===================================================================================== */

/* ====================================== DEFINE ======================================= */
/* ===================================================================================== */


/* ====================================== TYPEDEF STRUCT =============================== */
/* W25Q32 Commands */
#define W25Q32_CMD_WRITE_ENABLE       0x06
#define W25Q32_CMD_WRITE_DISABLE      0x04
#define W25Q32_CMD_READ_STATUS_REG1   0x05
#define W25Q32_CMD_READ_STATUS_REG2   0x35
#define W25Q32_CMD_WRITE_STATUS_REG   0x01
#define W25Q32_CMD_PAGE_PROGRAM       0x02
#define W25Q32_CMD_QUAD_PAGE_PROGRAM  0x32
#define W25Q32_CMD_BLOCK_ERASE_32K    0x52
#define W25Q32_CMD_SECTOR_ERASE       0x20
#define W25Q32_CMD_BLOCK_ERASE_64K    0xD8
#define W25Q32_CMD_CHIP_ERASE         0xC7
#define W25Q32_CMD_READ_DATA          0x03
#define W25Q32_CMD_FAST_READ          0x0B
#define W25Q32_CMD_READ_ID            0x9F
#define W25Q32_CMD_POWER_DOWN         0xB9
#define W25Q32_CMD_RELEASE_POWER_DOWN 0xAB
#define W25Q32_CMD_DEVICE_ID          0xAB

/* W25Q32 Parameters */
#define W25Q32_PAGE_SIZE              256
#define W25Q32_SECTOR_SIZE            4096
#define W25Q32_BLOCK_SIZE_32K         32768
#define W25Q32_BLOCK_SIZE_64K         65536
#define W25Q32_TOTAL_SIZE             4194304  // 4MB

/* Status Register Bits */
#define W25Q32_STATUS_BUSY            0x01
#define W25Q32_STATUS_WEL             0x02  // Write Enable Latch
/* ===================================================================================== */


/* ====================================== API ========================================== */
void     W25Q32_ReadID(uint8_t *manufacturer, uint16_t *deviceID);

uint8_t  W25Q32_ReadStatusReg(void);
void     W25Q32_WaitBusy(void);

void     W25Q32_WriteEnable(void);
void     W25Q32_WriteDisable(void);

void     W25Q32_EraseSector(uint32_t address);
void     W25Q32_EraseChip(void);

void     W25Q32_WritePage(uint32_t address, uint8_t *data, uint16_t size);
void     W25Q32_WriteData(uint32_t addr, const uint8_t *buf, uint32_t len);
void     W25Q32_ReadData(uint32_t address, uint8_t *buffer, uint32_t size);

void     W25Q32_PowerDown(void);
void     W25Q32_WakeUp(void);
/* ===================================================================================== */

#endif /* __W25Q32_H__ */