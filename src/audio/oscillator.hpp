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
    uint32_t frequency          = 0U;
    uint32_t currentSampleIndex = 0U;
    float phase                 = 0.0f;  // [-1.0, 1.0]
    float amplitude             = 0.5f;  // [0.0,  1.0]

    Wavetables::WavetableType wavetableType = Wavetables::WavetableType::SINE_WAVETABLE;

    void setFrequency(uint32_t newFrequency)
    {
        frequency          = newFrequency;
        wavetableSteps     = getWavetableSteps();
        currentSampleIndex = 0U;  // TODO: maybe delete?
    }

    float getFrequency(void)
    {
        return frequency;
    }

    void selectWavetable(Wavetables::WavetableType wtType)
    {
        wavetable = Wavetables::getWavetable(wtType);
    }

    float getSample(void)
    {
        float sample = 0.0f;
        if ((frequency > 0U) && (wavetable != nullptr))
        {
            currentSampleIndex += wavetableSteps;
            if (currentSampleIndex > WAVETABLE_NUM_SAMPLES)
            {
                currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
            }

            sample = wavetable->wavetableData[currentSampleIndex] * amplitude;
        }

        return sample;
    }

    Oscillator(void)
    {
        wavetable = Wavetables::getWavetable(Wavetables::WavetableType::SINE_WAVETABLE);
    }

private:
    Wavetables::Wavetable *wavetable = nullptr;
    const uint32_t sampleFrequency  = SYNTH_SAMPLE_FREQUENCY;
    uint32_t wavetableSteps         = getWavetableSteps();

    uint32_t getWavetableSteps(void)
    {
        const uint32_t wavelengthsPerSecond = (sampleFrequency / frequency);
        return (WAVETABLE_NUM_SAMPLES * wavelengthsPerSecond) / sampleFrequency;
    }
};

}

#endif  // OSCILLATOR_H_
