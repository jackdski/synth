#ifndef OSC_HPP_
#define OSC_HPP_

#include "features.h"

#if FEATURE_OSC

#include "constants.h"
#include "wavetables.hpp"
#include "waveforms.hpp"

#include <stdint.h>
#include <stdbool.h>

extern "C"
{
#include "arm_math.h"
}

#define WAVEFORM_MIN_AMPLITUDE      (-1.0F)
#define WAVEFORM_MAX_AMPLITUDE      (1.0F)
#define WAVEFORM_AMPLITUDE_RANGE    (2.0F)

namespace Audio {

enum class OscillatorType
{
    WAVETABLE,
    CALCULATED,
};

class Oscillator
{
private:
    OscillatorType  type = OscillatorType::CALCULATED;
    WaveformType    waveformType;
    uint32_t        currentSampleIndex = 0U;
    uint32_t        wavetableStep = 0U;

    float           currentCalculatedStep = 0.0F;
    float           calculatedStep = 0.0F;

    float frequency;
    float phase;

#if FEATURE_WAVETABLE
    float getWavetableSample()
    {
        float sample = Audio::wavetable_getSample(waveformType, currentSampleIndex);

        currentSampleIndex += wavetableStep;
        if (currentSampleIndex >= WAVETABLE_NUM_SAMPLES)
        {
            currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
        }
        return sample;
    }
#endif

    float getCalculatedSample()
    {
        float sample = 0.0f;

        switch (waveformType)
        {
            case WaveformType::SAW:
            {
                sample = currentCalculatedStep;
                currentCalculatedStep += calculatedStep;

                if (currentCalculatedStep >= WAVEFORM_AMPLITUDE_RANGE)
                {
                    currentCalculatedStep = WAVEFORM_MIN_AMPLITUDE;
                }
                break;
            }

            case WaveformType::SQUARE:
            {
                const float sine = arm_sin_f32(currentCalculatedStep);
                if (sine > 0.0f)
                {
                    sample = 1.0F;
                }
                else
                {
                    sample = -1.0F;
                }

                currentCalculatedStep += calculatedStep;
                if (currentCalculatedStep >= TWO_PI)
                {
                    currentCalculatedStep -= TWO_PI;
                }
                break;
            }

            case WaveformType::SINE:
            default:
            {
                sample = arm_sin_f32(currentCalculatedStep);

                currentCalculatedStep += calculatedStep;
                if (currentCalculatedStep >= TWO_PI)
                {
                    currentCalculatedStep -= TWO_PI;
                }
                break;
            }
        }
        return sample;
    }

    void updateStepSizeCalculatedWaveform(void)
    {
        switch (waveformType)
        {
            case WaveformType::SAW:
            {
                calculatedStep = 2.0f / frequency;
                break;
            }

            case WaveformType::SQUARE:
                calculatedStep = TWO_PI / (SYNTH_SAMPLE_FREQUENCY / frequency);
                break;

            case WaveformType::SINE:
            default:
                calculatedStep = TWO_PI / (SYNTH_SAMPLE_FREQUENCY / frequency);
                break;
        }
    }

    void updateStepSize(void)
    {
        switch (type)
        {
#if FEATURE_WAVETABLE
            case OscillatorType::WAVETABLE:
                wavetableStep = (uint32_t)((float)(Audio::wavetable_getNumberOfSamples(waveformType) * frequency) / SYNTH_SAMPLE_FREQUENCY);
                break;
#endif

            case OscillatorType::CALCULATED:
            default:
                updateStepSizeCalculatedWaveform();
                break;
        }
    }

public:
    Oscillator(float frequency, float phase, WaveformType waveformType) : waveformType(waveformType), frequency(frequency), phase(phase)
    {
        type = OscillatorType::CALCULATED;
        updateStepSize();
    }

    Oscillator(void)
    {
        type = OscillatorType::CALCULATED;
        waveformType = WaveformType::SINE;
        frequency = 0.0f;
    }

    Oscillator(float frequency)
    {
        type = OscillatorType::CALCULATED;
        waveformType = WaveformType::SINE;
        setFrequency(frequency);
    }

    Oscillator(WaveformType waveformType, float frequency): waveformType(waveformType), frequency(frequency)
    {
        type = OscillatorType::WAVETABLE;
        setFrequency(frequency);
    }

    void setFrequency(float freq)
    {
        frequency = freq;
        updateStepSize();
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (frequency > 0.0F)
        {
            switch (type)
            {
#if FEATURE_WAVETABLE
                case OscillatorType::WAVETABLE:
                    sample = getWavetableSample();
                    break;
#endif

                case OscillatorType::CALCULATED:
                default:
                    sample = getCalculatedSample();
                    break;

            }
        }
        return sample;
    }
};

}
#endif // FEATURE_OSC
#endif // OSC_HPP_