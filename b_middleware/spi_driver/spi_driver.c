#include "spi_driver.h"

/**
 * @brief Transfer data and receive reply
 * 
 * @param data: Data that need to send
 * 
 * @retval Data from DR register after RXNE (not empty) is set
 */
uint16_t SPI1_transfer_w25q32(uint16_t data){
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}
