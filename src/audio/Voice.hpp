#ifndef VOICE_HPP_
#define VOICE_HPP_

#include "features.h"

#if (FEATURE_VOICE)

/* I N C L U D E S */

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "features.h"

#include "Sample.hpp"
#include "oscillator.h"

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

    Oscillator_E oscillatorChannel;
    Sample sample;

    VoiceConfig(void)
    {
        source = VoiceSource::OSCILLATOR;
        oscillatorChannel = (Oscillator_E)0U;
        sample = Sample();
    }

    VoiceConfig(Oscillator_E channel)
    {
        source = VoiceSource::OSCILLATOR;
        oscillatorChannel = channel;
    }

    VoiceConfig(Sample newSample)
    {
        source = VoiceSource::SAMPLE;
        sample = newSample;
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

    Voice(Oscillator_E channel)
    {
        config = VoiceConfig(channel);
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
                ret = oscillator_getSample(config.oscillatorChannel);
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


#endif // FEATURE_VOICE
#endif // VOICE_HPP_