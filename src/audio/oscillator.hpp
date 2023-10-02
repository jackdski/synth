#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "wavetables.hpp"

namespace Audio
{

class Oscillator
{
public:
    void selectWavetable(Wavetable::WavetableType wtType)
    {
        wavetableType = wtType;
        switch (wavetableType)
        {
            case Wavetable::WavetableType::SAW_WAVETABLE:
                wavetable = saw_wavetable;
                break;

            case Wavetable::WavetableType::SQUARE_WAVETABLE:
                wavetable = square_wavetable;
                break;

            case Wavetable::WavetableType::SINE_WAVETABLE:
            default:
                wavetable = sine_wavetable;
                break;
        }
    }

    uint32_t frequency = 0U;
    float phase = 0.0f;      // [-1.0, 1.0]
    float amplitude = 0.5f;  // [0.0,  1.0]

    Wavetable::WavetableType wavetableType;

    uint32_t currentSampleIndex;
    uint32_t wavetableSampleLength;

    void setFrequency(uint32_t newFrequency)
    {
        frequency = newFrequency;
    }

    float getSample(void)
    {
        float sample = 0.0f;

        if (frequency > 0U)
        {
            sample = wavetable[currentSampleIndex] * amplitude;
            currentSampleIndex++;
            if (currentSampleIndex > wavetableSampleLength)
            {
                currentSampleIndex = 0U;
            }
        }

        return sample;
    }

private:
    const float *wavetable;
};

}

#endif  // OSCILLATOR_H_
