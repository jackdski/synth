#ifndef GPIO_HPP_
#define DRV_GPIO_HPP_

#include "drv_GPIO.h"

#include "main.h"
#include "stm32g4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/* D E F I N E S */


/* T Y P E D E F S */

typedef enum
{
    DRV_GPIO_CHANNEL_SPI_SD_CARD_CS,
    DRV_GPIO_CHANNEL_LCD_DC,
    DRV_GPIO_CHANNEL_BUTTON_A,
    DRV_GPIO_CHANNEL_SPI_LCD_CS,
    DRV_GPIO_CHANNEL_LED_DRIVER_OE,
    DRV_GPIO_CHANNEL_LCD_RST, // PB9
    DRV_GPIO_CHANNEL_BUTTON_B, // PB11
    DRV_GPIO_CHANNEL_LED_BLINKY, // PB14

    DRV_GPIO_CHANNEL_COUNT
} drv_GPIO_channel_E;


#endif  // DRV_GPIO_HPP_
