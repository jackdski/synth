#ifndef I2C_HPP_
#define I2C_HPP_

#include "drv_I2C_base.hpp"

#include "i2c.h"
#include "main.h"
#include "stm32g4xx_hal.h"

#if FEATURE_I2C

namespace Drivers
{

enum class I2CDevices
{
    I2C_DEVICE_SGTL5000,
    I2C_DEVICE_COUNT,
};

class I2CBus : public I2CBus_base
{
public:
    I2CBus(I2C_HandleTypeDef *i2c) : i2c(i2c)
    {
    }

private:
    I2C_HandleTypeDef *i2c;

    bool _writeI2CBytes(I2C_Transaction xfer)
    {
        bool ret = true;
        if (HAL_I2C_Master_Transmit(i2c, xfer._i2cDevice.deviceAddress, xfer.txBuffer, sizeof(xfer.txBuffer), 1U) !=
            HAL_OK)
        {
            ret = false;
        }
        return ret;
    }

    bool _readI2CBytes(I2C_Transaction xfer)
    {
        bool ret = true;
        if (HAL_I2C_Master_Transmit(i2c, xfer._i2cDevice.deviceAddress, xfer.txBuffer, sizeof(xfer.txBuffer), 1) ==
            HAL_OK)
        {
            ret = (HAL_I2C_Master_Receive_IT(
                       i2c, xfer._i2cDevice.deviceAddress, xfer.rxBuffer, sizeof(xfer.rxBuffer)) == HAL_OK);
        }
        else
        {
            ret = false;
        }
        return ret;
    }
};

}

#endif  // FEATURE_I2C

#endif  //  I2C_HPP_
