#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

/* I N C L U D E S */

#include "features.h"

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "Utils.h"
#include "wavetables.h"

#if (FEATURE_OSC)

#ifdef __cplusplus
extern "C" {
#endif

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum
{
    OSCILLATOR_0,
    OSCILLATOR_1,
    OSCILLATOR_2,
    OSCILLATOR_3,
    OSCILLATOR_4,
    OSCILLATOR_5,
    OSCILLATOR_6,
    OSCILLATOR_7,
    OSCILLATOR_LFO,

    OSCILLATOR_COUNT,
} Oscillator_E;

typedef struct
{
    WavetableType_E wavetableType;
    float frequency;
    float phase;  // [-1.0, 1.0]
} OscillatorChannelConfig_S;

typedef struct
{
    OscillatorChannelConfig_S *channelConfig;
} OscillatorConfig_S;

/* P U B L I C   F U N C T I O N S */

void oscillator_init(const Oscillator_E oscillator, const WavetableType_E wavetableType, const float frequency);

float oscillator_getSample(Oscillator_E oscillator);

void oscillator_selectWavetable(const Oscillator_E oscillator, WavetableType_E wavetableType);
void oscillator_setFrequency(const Oscillator_E oscillator, float newFrequency);
float oscillator_getFrequency(const Oscillator_E oscillator);

uint32_t oscillator_getWavetableSteps(const Oscillator_E oscillator);

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_OSC
#endif  // OSCILLATOR_H_
