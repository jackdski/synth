#ifndef DRV_ADC_HPP_
#define DRV_ADC_HPP_

#include "i2c.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

namespace Drivers
{

class drv_I2C
{
public:
    drv_I2C(I2C_HandleTypeDef *i2cHandle) : i2c(i2cHandle)
    {
    }

private:
    I2C_HandleTypeDef *i2c;
};

}

#endif  //  DRV_ADC_HPP_
