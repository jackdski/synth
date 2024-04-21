#include "wavetables.hpp"

#include <stdint.h>

namespace Wavetables
{

static Wavetable sineWavetable(WavetableType::SINE_WAVETABLE, sine_wavetable);
static Wavetable sawWavetable(WavetableType::SAW_WAVETABLE, saw_wavetable);
static Wavetable squareWavetable(WavetableType::SQUARE_WAVETABLE, square_wavetable);

Wavetable * _wavetables[static_cast<uint8_t>(WavetableType::WAVETABLE_COUNT)] =
{
    &sineWavetable,
    &sawWavetable,
    &squareWavetable,
};

float Wavetable::getNextSample(void)
{
    float ret = wavetableData[index];

    if (index == WAVETABLE_NUM_SAMPLES)
    {
        index = 0U;
    }
    else
    {
        index++;
    }

    return ret;
}

uint32_t Wavetable::getNumberOfSamples(void)
{
    return numSamples;
}

uint32_t Wavetable::getIndex(void)
{
    return index;
}


Wavetable * getWavetable(const WavetableType type)
{
    return _wavetables[static_cast<uint8_t>(type)];
}

}
