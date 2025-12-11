#include "w25q32_hardware.h"


/* ====================================== STATIC DECLARATIONS ======================================= */
static void W25Q32_SPI_Init(void);
/* ================================================================================================== */

/**
 * @brief Init GPIOA and SPI1
 * 
 * @retval None
 */
void W25Q32_SPI_Init(void)
{
    RCC_APB2PeriphClockCmd(W25Q32_SPI_GPIO_CLK | W25Q32_SPI_CLK, ENABLE);

    GPIO_InitTypeDef gpio_cs = {
        .GPIO_Pin   = W25Q32_CS_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_Out_PP
    };

    GPIO_InitTypeDef gpio_sck_mosi = {
        .GPIO_Pin   = W25Q32_SCK_PIN | W25Q32_MOSI_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_AF_PP
    };

    GPIO_InitTypeDef gpio_miso = {
        .GPIO_Pin   = W25Q32_MISO_PIN,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode  = GPIO_Mode_IN_FLOATING
    };

    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_cs);
    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_sck_mosi);
    GPIO_Init(W25Q32_SPI_GPIO_PORT, &gpio_miso);

    W25Q32_CS_HIGH();

    SPI_InitTypeDef spi_config = {
        .SPI_Direction         = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode              = SPI_Mode_Master,
        .SPI_DataSize          = SPI_DataSize_8b,
        .SPI_CPOL              = SPI_CPOL_Low,
        .SPI_CPHA              = SPI_CPHA_1Edge,
        .SPI_NSS               = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4,
        .SPI_FirstBit          = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial     = 7
    };
    SPI_Init(W25Q32_SPI, &spi_config);
    SPI_Cmd(W25Q32_SPI, ENABLE);
}

void w25q32_hardware_init(void){
    W25Q32_SPI_Init();
}