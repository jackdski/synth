#include "drv_SPI.h"

/* I N C L U D E S */

#include "features.h"

#include "i2c.h"
#include "lvgl.h"
#include "main.h"

#include "stm32g4xx_hal.h"

#include "drv_GPIO.h"

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static bool drv_SPI_private_writeBytesBus1(drv_SPI_TransactionConfig_S *xfer);
static bool drv_SPI_private_readBytesBus1(drv_SPI_TransactionConfig_S *xfer);

/* D A T A   D E F I N I T I O N S */

extern SPI_HandleTypeDef hspi1;
extern lv_color_t displayBuffer1;  // [BUFFER_SIZE];

// clang-format off
static drv_SPI_busConfig_S spiBus1Config[DRV_SPI_BUS_COUNT] = {
    [DRV_SPI_BUS_1] =
    {
        .writeBytes = drv_SPI_private_writeBytesBus1,
        .readBytes  = drv_SPI_private_readBytesBus1,
    },
};

static drv_SPI_deviceConfig_S spiDeviceConfig[DRV_SPI_DEVICE_COUNT] = {
    [DRV_SPI_DEVICE_ST7789_DISPLAY] =
    {
        .csPin      = DRV_GPIO_CHANNEL_SPI_LCD_CS,
        .frameSize  = 1U,
    },
    [DRV_SPI_DEVICE_SD_CARD] =
    {
        .csPin      = DRV_GPIO_CHANNEL_SPI_SD_CARD_CS,
        .frameSize  = 1U,
    },
};
// clang-format on

/* P R I V A T E   F U N C T I O N S */

static bool drv_SPI_private_writeBytesBus1(drv_SPI_TransactionConfig_S *xfer)
{
    bool ret = true;

    // if (HAL_SPI_TransmitReceive_IT(&hspi1, deviceAddress, xfer->txBuffer, xfer->length, 1U) != HAL_OK)
    // {
    //     ret = false;
    // }
    return ret;
}

static bool drv_SPI_private_readBytesBus1(drv_SPI_TransactionConfig_S *xfer)
{
    bool ret = true;

    // if ((xfer != NULL) || (xfer->rxBuffer != NULL) || (xfer->rxLength > 0U))
    // {
    //     if (HAL_SPI_Receive_IT(&hspi1, xfer->rxBuffer, sizeof(rxData)) != HAL_OK)
    //     {

    //     }
    // }
    // else
    // {
    //     ret = false;
    // }

    return ret;
}

/* P U B L I C   F U N C T I O N S */

void drv_SPI_hardwareSpecific_init(void)
{
    MX_SPI1_Init();
}

// void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
// {
// }
