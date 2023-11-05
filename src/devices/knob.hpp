#ifndef KNOB_HPP
#define KNOB_HPP

#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc_ex.h"

#include "drv_ADC.hpp"

#define KNOB_POSITION_MAX 1.0f
#define KNOB_POSITION_MIN 0.0f

namespace Devices
{

class Knob
{
private:
    float voltage  = 0.0f;
    float position = KNOB_POSITION_MIN;

public:
    friend class Drivers::drv_ADC;
    Drivers::drv_ADC &adcDriver;

    float getVoltage(void)
    {
        return voltage;
    }

    float getPosition(void)
    {
        return position;
    }

    void update(void);

    Knob(Drivers::drv_ADC &adcDriver) : adcDriver(adcDriver)
    {
    }
};

}

#endif /* KNOB_HPP */
