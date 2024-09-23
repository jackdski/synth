#ifndef OSC_HPP_
#define OSC_HPP_

#include "features.h"

#if FEATURE_OSC

#include "constants.h"
#include "wavetables.h"

#include <stdint.h>
#include <stdbool.h>

namespace Audio {

class Oscillator
{
private:
    float frequency;
    float phase;

    WavetableType_E wavetableType;

    uint32_t        currentSampleIndex;
    uint32_t        wavetableStep;

public:
    Oscillator(float frequency, float phase, WavetableType_E wavetableType) : frequency(frequency), phase(phase), wavetableType(wavetableType)
    {
        updateWavetableSteps();
    }

    Oscillator(void)
    {
        wavetableType = WAVETABLE_TYPE_SINE;
        frequency = 0.0F;
        phase = 0.0F;
    }

    void setFrequency(float freq)
    {
        frequency = freq;
        updateWavetableSteps();
    }

    void updateWavetableSteps(void)
    {
        const uint32_t wavelengthsPerSecond = (uint32_t)(SYNTH_SAMPLE_FREQUENCY / frequency);
        wavetableStep = (wavetable_getNumberOfSamples(wavetableType) * wavelengthsPerSecond) / SYNTH_SAMPLE_FREQUENCY;
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (frequency > 0.0F)
        {
            sample = wavetable_getSample(wavetableType, currentSampleIndex);

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