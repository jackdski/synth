#ifndef DRV_I2C_HARDWARE_SPECIFIC_H_
#define DRV_I2C_HARDWARE_SPECIFIC_H_

#include "drv_I2C.h"
#include "features.h"

#include "i2c.h"
#include "main.h"
#include "stm32g4xx_hal.h"

#if FEATURE_I2C

#define I2C_ASYNC 0

typedef enum
{
    DRV_I2C_DEVICE_SGTL5000,
    DRV_I2C_DEVICE_PORT_EXPANDER,
    DRV_I2C_DEVICE_LED_DRIVER,

    DRV_I2C_DEVICE_COUNT,
} drv_I2C_device_E;

typedef enum
{
    DRV_I2C_BUS_1,

    DRV_I2C_BUS_COUNT,
} drv_I2C_bus_E;

void drv_I2C_hardwareSpecific_init(void);

#endif  // FEATURE_I2C
#endif  // DRV_I2C_HARDWARE_SPECIFIC_H_
