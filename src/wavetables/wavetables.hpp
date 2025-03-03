#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#include "features.h"

#if FEATURE_WAVETABLE

#include "waveforms.hpp"

#include <stdint.h>

#define WAVETABLE_NUM_SAMPLES 4096U

namespace Audio
{

struct WavetableConfig_S
{
    uint32_t numSamples;
    const float * wavetableData;

    static WavetableConfig_S create(uint32_t numSamples, const float * wavetableData)
    {
        WavetableConfig_S config;
        config.numSamples = numSamples;
        config.wavetableData = wavetableData;
        return config;
    }
};


WavetableConfig_S * wavetable_getWavetableConfig(const WaveformType type);
float wavetable_getSample(const WaveformType type, const uint32_t index);
uint32_t wavetable_getNumberOfSamples(const WaveformType type);

}

#endif // FEATURE_WAVETABLE
#endif // WAVETABLES_H_
