#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "Utils.h"
#include "wavetables.hpp"

namespace Audio
{

class Oscillator
{
public:
    uint32_t frequency          = 440U;
    uint32_t currentSampleIndex = 0U;
    float phase                 = 0.0f;  // [-1.0, 1.0]
    float amplitude             = 0.5f;  // [0.0,  1.0]

    Wavetable::WavetableType wavetableType = Wavetable::WavetableType::SINE_WAVETABLE;

    void setFrequency(uint32_t newFrequency)
    {
        frequency          = newFrequency;
        wavetableSteps     = getWavetableSteps();
        currentSampleIndex = 0U;  // TODO: maybe delete?
    }

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

    float getSample(void)
    {
        float sample = 0.0f;
        if (frequency > 0U)
        {
            currentSampleIndex += wavetableSteps;
            if (currentSampleIndex > WAVETABLE_NUM_SAMPLES)
            {
                currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
            }

            sample = wavetable[currentSampleIndex] * amplitude;
        }

        return sample;
    }

private:
    const float *wavetable         = sine_wavetable;
    const uint32_t sampleFrequency = SYNTH_SAMPLE_FREQUENCY;
    uint32_t wavetableSteps        = getWavetableSteps();

    uint32_t getWavetableSteps(void)
    {
        const uint32_t wavelengthsPerSecond = (sampleFrequency / frequency);
        return (WAVETABLE_NUM_SAMPLES * wavelengthsPerSecond) / sampleFrequency;
    }
};

}

#endif  // OSCILLATOR_H_
