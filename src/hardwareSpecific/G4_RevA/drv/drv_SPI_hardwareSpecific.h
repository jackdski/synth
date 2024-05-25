#ifndef SPI_HARDWARE_SPECIFIC_HPP_
#define SPI_HARDWARE_SPECIFIC_SPI_HPP_

#include "features.h"
#include "drv_SPI.h"

#include "i2c.h"
#include "main.h"
#include "stm32g4xx_hal.h"

#if (FEATURE_SPI)

typedef enum
{
    DRV_SPI_DEVICE_ST7789_DISPLAY,
    DRV_SPI_DEVICE_SD_CARD,

    DRV_SPI_DEVICE_COUNT,
}  drv_SPI_device_E;

typedef enum
{
    DRV_SPI_MAILBOX_DISPLAY_COMMAND,
    DRV_SPI_MAILBOX_DISPLAY_DRAW,

    DRV_SPI_MAILBOX_DISPLAY_COUNT
} drv_SPI_mailbox_E;

typedef enum
{
    DRV_SPI_BUS_1,

    DRV_SPI_BUS_COUNT,
} drv_SPI_bus_E;


void drv_SPI_hardwareSpecific_init(void);


#endif  // FEATURE_SPI
#endif  // SPI_HARDWARE_SPECIFIC_SPI_HPP_
