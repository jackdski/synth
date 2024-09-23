#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WAVETABLE_NUM_SAMPLES 2048U

typedef enum
{
    WAVETABLE_TYPE_SINE,
    WAVETABLE_TYPE_SAW,
    WAVETABLE_TYPE_SQUARE,

    WAVETABLE_TYPE_COUNT,
} WavetableType_E;

typedef struct
{
    uint32_t numSamples;
    const float * wavetableData;
} WavetableConfig_S;


WavetableConfig_S * wavetable_getWavetableConfig(const WavetableType_E type);
float wavetable_getNextSample(const WavetableType_E type);
float wavetable_getSample(const WavetableType_E type, const uint32_t index);
uint32_t wavetable_getNumberOfSamples(const WavetableType_E type);
uint32_t wavetable_getIndex(const WavetableType_E type);

#ifdef __cplusplus
}
#endif

#endif // WAVETABLES_H_
