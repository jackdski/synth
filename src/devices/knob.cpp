#include "knob.hpp"

#include <stdint.h>

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc_ex.h"

using namespace Devices;

void Knob::update(void)
{
    position = adcDriver.sampleValue();
}
