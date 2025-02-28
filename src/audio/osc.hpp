#ifndef OSC_HPP_
#define OSC_HPP_

#include "features.h"

#if FEATURE_OSC

#include "constants.h"
#include "wavetables.hpp"

#include <stdint.h>
#include <stdbool.h>

extern "C"
{
#include "arm_math.h"
}

namespace Audio {

enum class OscillatorType
{
    WAVETABLE,
    CALCULATED,
};

class Oscillator
{
private:
    OscillatorType  type = OscillatorType::WAVETABLE;
    WavetableType   wavetableType;
    uint32_t        currentSampleIndex = 0U;
    uint32_t        wavetableStep = 0U;

    float           currentCalculatedStep = 0.0F;
    float           calculatedStep = 0.0F;

    float frequency;
    float phase;

    float getWavetableSample()
    {
        float sample = Audio::wavetable_getSample(wavetableType, currentSampleIndex);

        currentSampleIndex += wavetableStep;
        if (currentSampleIndex >= WAVETABLE_NUM_SAMPLES)
        {
            currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
        }
        return sample;
    }

    float getCalculatedSample()
    {
        float sample = arm_sin_f32(currentCalculatedStep);

        currentCalculatedStep += calculatedStep;
        if (currentCalculatedStep >= TWO_PI)
        {
            currentCalculatedStep -= TWO_PI;
        }
        return sample;
    }

public:
    Oscillator(float frequency, float phase, WavetableType wavetableType) : wavetableType(wavetableType), frequency(frequency), phase(phase)
    {
        updateStepSize();
    }

    Oscillator(void)
    {
        type = OscillatorType::CALCULATED;
        wavetableType = WavetableType::SINE;
        frequency = 0.0f;
    }

    Oscillator(float frequency)
    {
        type = OscillatorType::CALCULATED;
        wavetableType = WavetableType::SINE;
        setFrequency(frequency);
    }

    Oscillator(WavetableType wavetableType, float frequency): wavetableType(wavetableType), frequency(frequency)
    {
        type = OscillatorType::WAVETABLE;
        setFrequency(frequency);

    }

    void setFrequency(float freq)
    {
        frequency = freq;
        updateStepSize();
    }

    void updateStepSize(void)
    {
        switch (type)
        {
            case OscillatorType::CALCULATED:
                calculatedStep = TWO_PI / (SYNTH_SAMPLE_FREQUENCY / frequency);
                break;

            case OscillatorType::WAVETABLE:
            default:
                wavetableStep = (uint32_t)((float)(Audio::wavetable_getNumberOfSamples(wavetableType) * frequency) / SYNTH_SAMPLE_FREQUENCY);
                break;
        }
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (frequency > 0.0F)
        {
            switch (type)
            {
                case OscillatorType::CALCULATED:
                    sample = getCalculatedSample();
                    break;

                case OscillatorType::WAVETABLE:
                default:
                    sample = getWavetableSample();
                    break;
            }
        }
        return sample;
    }
};

}
#endif // FEATURE_OSC
#endif // OSC_HPP_