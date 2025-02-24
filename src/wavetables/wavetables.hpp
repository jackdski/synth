#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#include "features.h"

#if FEATURE_WAVETABLE

#include <stdint.h>

#define WAVETABLE_NUM_SAMPLES 2048U

namespace Audio
{

enum class WavetableType
{
    SINE,
    SAW,
    SQUARE,

    COUNT,
};

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


WavetableConfig_S * wavetable_getWavetableConfig(const WavetableType type);
float wavetable_getSample(const WavetableType type, const uint32_t index);
uint32_t wavetable_getNumberOfSamples(const WavetableType type);

}

#endif // FEATURE_WAVETABLE
#endif // WAVETABLES_H_
