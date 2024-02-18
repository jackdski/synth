#include "knob.hpp"

#include "main.h"
#include <stdint.h>

using namespace Devices;

void Knob::update(void)
{
    adcDriver.sample();
    position = adcDriver.getSampleValue();
    voltage  = (position * ADC_REFERENCE_VOLTAGE);
}
