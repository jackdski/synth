#ifndef DRV_GPIO_HPP_
#define DRV_GPIO_HPP_

#include "drv_GPIO.h"

#include "main.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum
{
    DRV_GPIO_CHANNEL_LED_BLINKY,  // PB14

    DRV_GPIO_CHANNEL_COUNT
} drv_GPIO_channel_E;

#endif  // DRV_GPIO_HPP_
