#ifndef VOICE_HPP_
#define VOICE_HPP_

#include "features.h"

#if (FEATURE_VOICE)

/* I N C L U D E S */

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "GlobalSamples.hpp"
#include "Sample.hpp"
#include "osc.hpp"
#include "wavetables.hpp"
#include "waveforms.hpp"

namespace Audio
{

/* D E F I N E S */

#define MAX_CONCURRENT_OSCILLATORS  4U


/* T Y P E D E F S */

enum class VoiceSource
{
    OSCILLATOR,
    OSCILLATOR_CHORD,
    SAMPLE,
};

class VoiceConfig
{
public:
    VoiceSource source;

#if FEATURE_OSC
    Oscillator oscillator[MAX_CONCURRENT_OSCILLATORS]; // = Oscillator();
    uint8_t    activeOscillatorCount = 0U;
#endif
#if FEATURE_SAMPLE
    Samples::Sample     sample;
#endif
    // ADSR

    VoiceConfig(void)
    {
        source = VoiceSource::OSCILLATOR;
#if FEATURE_SAMPLE
        sample = Samples::Sample();
#elif FEATURE_OSC
        oscillator[0U] = Oscillator(WaveformType::SINE, 440.0f);
        activeOscillatorCount = 1U;
#endif
    }

    VoiceConfig(VoiceSource source): source(source)
    {
    }

    VoiceConfig(float frequency)
    {
        source = VoiceSource::OSCILLATOR;
#if FEATURE_OSC
        oscillator[0U] = Oscillator(frequency);
        activeOscillatorCount = 1U;
#endif
    }

    VoiceConfig(WaveformType waveformType, float frequency)
    {
        source = VoiceSource::OSCILLATOR;
#if FEATURE_OSC
        oscillator[0U] = Oscillator(waveformType, frequency);
        activeOscillatorCount = 1U;
#endif
    }

#if FEATURE_SAMPLE
    VoiceConfig(Samples::SampleType newSample)
    {
        source = VoiceSource::SAMPLE;
        sample = globalSamples_getSample(newSample);
        // sample = Samples::Sample(GlobalData::hi_hat_0_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::hi_hat_0_wavetable));
    }
#endif
};

class Voice
{
public:
    VoiceConfig config;

    Voice(void)
    {
        config = VoiceConfig();
    }

    Voice(float frequency)
    {
        config = VoiceConfig(frequency);
    }

    Voice(WaveformType waveformType, float frequency)
    {
        config = VoiceConfig(waveformType, frequency);
    }

#if FEATURE_SAMPLE
    Voice(Samples::SampleType sample)
    {
        config = VoiceConfig(sample);
    }
#endif

    void setConfig(const VoiceConfig voiceConfig)
    {
        config = voiceConfig;
    }

    float getSample(const bool restart = false)
    {
        float ret = 0.0F;
        switch (config.source)
        {
            case VoiceSource::OSCILLATOR:
#if FEATURE_OSC
                ret = config.oscillator[0U].getSample();
#endif
                break;

            case VoiceSource::OSCILLATOR_CHORD:
            {
#if FEATURE_OSC
                for (uint8_t i = 0U; i < config.activeOscillatorCount; i++)
                {
                    ret += config.oscillator[i].getSample();
                }
                return (ret / (float)config.activeOscillatorCount);
#endif
            }
                break;

            case VoiceSource::SAMPLE:
#if FEATURE_SAMPLE
                ret = config.sample.getSample(restart);
#endif
                break;

            default:
                // nothing
                break;
        }

        return ret;
    }
};

/* P U B L I C   F U N C T I O N S */

}

#endif // FEATURE_VOICE
#endif // VOICE_HPP_