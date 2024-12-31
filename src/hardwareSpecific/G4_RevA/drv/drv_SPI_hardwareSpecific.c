#include "drv_SPI.h"

/* I N C L U D E S */

#include "features.h"

#if FEATURE_SPI

#include "lvgl.h"

#include "spi.h"
#include "stm32g4xx_hal.h"

#include "drv_GPIO.h"

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static bool drv_SPI_private_writeBytesBus1(drv_SPI_TransactionConfig_S *xfer);
static bool drv_SPI_private_readBytesBus1(drv_SPI_TransactionConfig_S *xfer);
static bool drv_SPI_private_writeReadBytesBus1(drv_SPI_TransactionConfig_S *xfer);
static bool drv_SPI_private_isBusyBus1(void);

/* D A T A   D E F I N I T I O N S */

extern SPI_HandleTypeDef hspi1;

// clang-format off
static drv_SPI_busConfig_S spiBus1Config[DRV_SPI_BUS_COUNT] =
{
    [DRV_SPI_BUS_1] =
    {
        .writeBytes = drv_SPI_private_writeBytesBus1,
        .readBytes  = drv_SPI_private_readBytesBus1,
        .writeReadBytes = drv_SPI_private_writeReadBytesBus1,
        .isBusy = drv_SPI_private_isBusyBus1,
    },
};

static drv_SPI_deviceConfig_S spiDeviceConfig[DRV_SPI_DEVICE_COUNT] =
{
    [DRV_SPI_DEVICE_ST7789_DISPLAY] =
    {
        .csPin      = DRV_GPIO_CHANNEL_SPI_LCD_CS,
        .frameSize  = 1U,
        .bus        = DRV_SPI_BUS_1,
    },
    [DRV_SPI_DEVICE_SD_CARD] =
    {
        .csPin      = DRV_GPIO_CHANNEL_SPI_SD_CARD_CS,
        .frameSize  = 1U,
        .bus        = DRV_SPI_BUS_1,
    },
};

const drv_SPI_config_S drv_SPI_config =
{
    .busConfig = spiBus1Config,
    .deviceConfig = spiDeviceConfig,
};

// clang-format on

/* P R I V A T E   F U N C T I O N S */

static bool drv_SPI_private_writeBytesBus1(drv_SPI_TransactionConfig_S *xfer)
{
    bool ret = true;

    if ((xfer != NULL) && (xfer->txBuffer != NULL) && (xfer->length > 0U))
    {
        const drv_GPIO_channel_E csPin = drv_SPI_config.deviceConfig[xfer->spiDevice].csPin;
        drv_GPIO_setOutput(csPin, DRV_GPIO_LOW);

        if (HAL_SPI_Transmit_IT(&hspi1, xfer->txBuffer, xfer->length) != HAL_OK)
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}

static bool drv_SPI_private_writeReadBytesBus1(drv_SPI_TransactionConfig_S *xfer)
{
    bool ret = true;

    if ((xfer != NULL) && (xfer->txBuffer != NULL) && (xfer->rxBuffer != NULL) && (xfer->length > 0U))
    {
        const drv_GPIO_channel_E csPin = drv_SPI_config.deviceConfig[xfer->spiDevice].csPin;
        drv_GPIO_setOutput(csPin, DRV_GPIO_LOW);

        if (HAL_SPI_TransmitReceive_IT(&hspi1, xfer->txBuffer, xfer->rxBuffer, xfer->length) != HAL_OK)
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}

static bool drv_SPI_private_readBytesBus1(drv_SPI_TransactionConfig_S *xfer)
{
    bool ret = true;

    if ((xfer != NULL) && (xfer->rxBuffer != NULL) && (xfer->length > 0U))
    {
        const drv_GPIO_channel_E csPin = drv_SPI_config.deviceConfig[xfer->spiDevice].csPin;
        drv_GPIO_setOutput(csPin, DRV_GPIO_LOW);

        if (HAL_SPI_Receive_IT(&hspi1, xfer->rxBuffer, sizeof(xfer->rxBuffer)) != HAL_OK)
        {
            ret = false;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

static bool drv_SPI_private_isBusyBus1(void)
{
    return (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
}
/* P U B L I C   F U N C T I O N S */

void drv_SPI_hardwareSpecific_init(void)
{
    MX_SPI1_Init();
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
        drv_SPI_isrCallback(DRV_SPI_BUS_1);
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
        drv_SPI_isrCallback(DRV_SPI_BUS_1);
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1)
    {
        drv_SPI_isrCallback(DRV_SPI_BUS_1);
    }
}

// void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
// {
// }

#endif  // FEATURE_SPI
