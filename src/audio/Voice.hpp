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

namespace Audio
{

/* D E F I N E S */


/* T Y P E D E F S */

enum class VoiceSource
{
    OSCILLATOR,
    SAMPLE,
};

class VoiceConfig
{
public:
    VoiceSource source;

#if FEATURE_OSC
    Oscillator oscillator = Oscillator();
#endif
#if FEATURE_SAMPLE
    Samples::Sample     sample;
#endif
    // ADSR

    VoiceConfig(void)
    {
        source = VoiceSource::OSCILLATOR;
        sample = Samples::Sample();
    }

    VoiceConfig(VoiceSource source): source(source)
    {
    }

    VoiceConfig(WavetableType wavetableType, float frequency)
    {
        source = VoiceSource::OSCILLATOR;
#if FEATURE_OSC
        oscillator = Oscillator(wavetableType, frequency);
#endif
    }

    VoiceConfig(Samples::SampleType newSample)
    {
        source = VoiceSource::SAMPLE;
        sample = globalSamples_getSample(newSample);
        // sample = Samples::Sample(GlobalData::hi_hat_0_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::hi_hat_0_wavetable));
    }
};

class Voice
{
public:
    VoiceConfig config;

    Voice(void)
    {
        config = VoiceConfig();
    }

    Voice(WavetableType wavetableType, float frequency)
    {
        config = VoiceConfig(wavetableType, frequency);
    }

    Voice(Samples::SampleType sample)
    {
        config = VoiceConfig(sample);
    }

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
                ret = config.oscillator.getSample();
#endif
                break;

            case VoiceSource::SAMPLE:
                ret = config.sample.getSample(restart);
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