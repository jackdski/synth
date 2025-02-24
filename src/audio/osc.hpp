#ifndef OSC_HPP_
#define OSC_HPP_

#include "features.h"

#if FEATURE_OSC

#include "constants.h"
#include "wavetables.hpp"

#include <stdint.h>
#include <stdbool.h>

#define WAVETABLE_NUM_SAMPLES 2048U

namespace Audio {

class Oscillator
{
private:
    WavetableType   wavetableType;
    uint32_t        currentSampleIndex = 0U;
    uint32_t        wavetableStep = 0U;

    float frequency;
    float phase;

public:
    Oscillator(float frequency, float phase, WavetableType wavetableType) : wavetableType(wavetableType), frequency(frequency), phase(phase)
    {
        updateWavetableSteps();
    }

    Oscillator(void)
    {
        wavetableType = WavetableType::SINE;
        frequency = 0.0f;
    }

    Oscillator(WavetableType wavetableType, float frequency): wavetableType(wavetableType), frequency(frequency)
    {
        setFrequency(frequency);
    }

    void setFrequency(float freq)
    {
        frequency = freq;
        updateWavetableSteps();
    }

    void updateWavetableSteps(void)
    {
        const uint32_t wavelengthsPerSecond = (uint32_t)(SYNTH_SAMPLE_FREQUENCY / frequency);
        wavetableStep = (Audio::wavetable_getNumberOfSamples(wavetableType) * wavelengthsPerSecond) / SYNTH_SAMPLE_FREQUENCY;
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (frequency > 0.0F)
        {
            sample = Audio::wavetable_getSample(wavetableType, currentSampleIndex);

            currentSampleIndex += wavetableStep;
            if (currentSampleIndex > WAVETABLE_NUM_SAMPLES)
            {
                currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
            }
        }
        return sample;
    }
};

}
#endif // FEATURE_OSC
#endif // OSC_HPP_