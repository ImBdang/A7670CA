#include "w25q32_driver.h"

/* ====================================== GLOBAL VARIABLES ======================================= */
volatile uint32_t cur_w25_addr = 0x000000;                  /*<! 24bit addr */
/* =============================================================================================== */

/**
 * @brief  Read manufacurer ID and device ID
 * @param  manufacturer: Pointer to save manufacturer
 * @param  deviceID: Pointer to save ID device
 */
void W25Q32_ReadID(uint8_t *manufacturer, uint16_t *deviceID) {
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_READ_ID);
    *manufacturer = SPI1_transfer_w25q32(0xFF);
    *deviceID = (SPI1_transfer_w25q32(0xFF) << 8);
    *deviceID |= SPI1_transfer_w25q32(0xFF);
    W25Q32_CS_HIGH();
}

/**
 * @brief  Read status register
 * @retval Status reg of w25q32
 */
uint8_t W25Q32_ReadStatusReg(void) {
    uint8_t status;
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_READ_STATUS_REG1);
    status = SPI1_transfer_w25q32(0xFF);
    W25Q32_CS_HIGH();
    return status;
}

/**
 * @brief  Wait chip out of busy
 */
void W25Q32_WaitBusy(void) {
    while(W25Q32_ReadStatusReg() & W25Q32_STATUS_BUSY);
}


/**
 * @brief Cmd to enable W25Q32
 */
void W25Q32_WriteEnable(void) {
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_WRITE_ENABLE);
    W25Q32_CS_HIGH();
}
/**
 * @brief Cmd to disable W25Q32
 */
void W25Q32_WriteDisable(void) {
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_WRITE_DISABLE);
    W25Q32_CS_HIGH();
}

/**
 * @brief  Delete 1 sector
 * @param  address: Address of sector
 */
void W25Q32_EraseSector(uint32_t address) {
    W25Q32_WaitBusy();
    W25Q32_WriteEnable();
    
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_SECTOR_ERASE);
    SPI1_transfer_w25q32((address >> 16) & 0xFF);
    SPI1_transfer_w25q32((address >> 8) & 0xFF);
    SPI1_transfer_w25q32(address & 0xFF);
    W25Q32_CS_HIGH();
    W25Q32_WaitBusy();
}

/**
 * @brief   Erase all chip
 */
void W25Q32_EraseChip(void) {
    W25Q32_WaitBusy();
    W25Q32_WriteEnable();
    
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_CHIP_ERASE); 
    W25Q32_CS_HIGH();
    W25Q32_WaitBusy();
}



/**
 * @brief Write data in 1 page
 * 
 * @param address: addr start of page
 * @param data: data that you need to write, in this case my data is an array so its an pointer to arr
 * @param size: size of the data
 */
void W25Q32_WritePage(uint32_t address, uint8_t *data, uint16_t size) {
    if(size > W25Q32_PAGE_SIZE) size = W25Q32_PAGE_SIZE;
    
    W25Q32_WaitBusy();
    W25Q32_WriteEnable();
    
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_PAGE_PROGRAM);
    SPI1_transfer_w25q32((address >> 16) & 0xFF);
    SPI1_transfer_w25q32((address >> 8) & 0xFF);
    SPI1_transfer_w25q32(address & 0xFF);
    
    for(uint16_t i = 0; i < size; i++) {
        SPI1_transfer_w25q32(data[i]);
    }

    W25Q32_CS_HIGH();
    W25Q32_WaitBusy();
}

/**
 * @brief   Use to write many data in to multi page
 * 
 * @param   addr: Addr where you start to write in w25
 * @param   buff: Pointer of the fist data 
 */
void W25Q32_WriteData(uint32_t addr, const uint8_t *buf, uint32_t len) {
    while (len > 0) {

        uint32_t page_offset = addr % W25Q32_PAGE_SIZE;
        uint32_t bytes_in_page = W25Q32_PAGE_SIZE - page_offset;

        uint32_t write_now = (len < bytes_in_page) ? len : bytes_in_page;

        W25Q32_WritePage(addr, (uint8_t*)buf, write_now);

        addr += write_now;
        buf  += write_now;
        len  -= write_now;
    }
}


/**
 * @brief  Normal mode
 * @param  address: Start addr
 * @param  buffer: Buffer to save data
 * @param  size: Byte you need to read 
 */
void W25Q32_ReadData(uint32_t address, uint8_t *buffer, uint32_t size) {
    W25Q32_WaitBusy();
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_READ_DATA);
    SPI1_transfer_w25q32((address >> 16) & 0xFF);
    SPI1_transfer_w25q32((address >> 8) & 0xFF);
    SPI1_transfer_w25q32(address & 0xFF);
    
    for(uint32_t i = 0; i < size; i++) {
        buffer[i] = SPI1_transfer_w25q32(0xFF);
    }
    W25Q32_CS_HIGH();
}

/**
 * @brief Power down for w25q32
 */
void W25Q32_PowerDown(void) {
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_POWER_DOWN);
    W25Q32_CS_HIGH();
}

/**
 * @brief Wake w25q32 from power down up
 */
void W25Q32_WakeUp(void) {
    W25Q32_CS_LOW();
    SPI1_transfer_w25q32(W25Q32_CMD_RELEASE_POWER_DOWN);
    W25Q32_CS_HIGH();
}
