#include "PCA9685.h"

#if FEATURE_PCA9685

#include "drv_GPIO.h"
#include "drv_I2C.h"

#include <stdbool.h>

PCA9685_config_S PCA9685_config = {
    .i2cDevice    = DRV_I2C_DEVICE_LED_DRIVER,
    .gpioOE       = DRV_GPIO_CHANNEL_LED_DRIVER_OE,
    .updateOnAck  = false,
    .invertOutput = true,
};

#endif  // FEATURE_PCA9685
