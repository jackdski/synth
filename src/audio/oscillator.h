#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "Utils.h"
#include "wavetables.h"

typedef enum
{
    OSCILLATOR_0,
    OSCILLATOR_1,
    OSCILLATOR_2,
    OSCILLATOR_3,

    OSCILLATOR_COUNT,
} Oscillator_E;

typedef struct
{
    WavetableType_E wavetableType;
    uint32_t frequency;
    float phase;               // [-1.0, 1.0]
    float amplitude;           // [0.0,  1.0]
    uint32_t sampleFrequency;  //   = SYNTH_SAMPLE_FREQUENCY;
} OscillatorChannelConfig_S;

typedef struct
{
    OscillatorChannelConfig_S *channelConfig;
} OscillatorConfig_S;

float oscillator_getSample(Oscillator_E oscillator);

void oscillator_selectWavetable(const Oscillator_E oscillator, WavetableType_E wavetableType);
void oscillator_setFrequency(const Oscillator_E oscillator, uint32_t newFrequency);

uint32_t oscillator_getWavetableSteps(const Oscillator_E oscillator);

#endif  // OSCILLATOR_H_
