#include "oscillator.h"

/* I N C L U D E S */

#include "constants.h"
#include "wavetables.h"

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (FEATURE_OSC)

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    uint32_t currentSampleIndex;
    uint32_t wavetableSteps;  // getWavetableSteps();
} OscillatorChannelData_S;

typedef struct
{
    OscillatorChannelData_S channelData[OSCILLATOR_COUNT];
} OscillatorData_S;

/* D A T A   D E F I N I T I O N S */

static OscillatorChannelConfig_S oscillatorChannelConfig[OSCILLATOR_COUNT];

static OscillatorConfig_S oscillatorConfig = {
    .channelConfig = oscillatorChannelConfig,
};

static OscillatorData_S oscillatorData;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void oscillator_init(const Oscillator_E oscillator, const WavetableType_E wavetableType, const float frequency)
{
    memset(&oscillatorData, 0U, sizeof(oscillatorData));
    oscillatorConfig.channelConfig[oscillator].wavetableType = wavetableType;
    oscillator_setFrequency(oscillator, frequency);
}

float oscillator_getSample(Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    OscillatorChannelData_S *channelData     = &oscillatorData.channelData[oscillator];

    float sample = 0.0f;
    if (channelConfig->frequency > 0U)
    {
        sample = wavetable_getSample(channelConfig->wavetableType, channelData->currentSampleIndex);

        channelData->currentSampleIndex += channelData->wavetableSteps;
        if (channelData->currentSampleIndex > WAVETABLE_NUM_SAMPLES)
        {
            channelData->currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
        }
    }

    return sample;
}

void oscillator_selectWavetable(const Oscillator_E oscillator, WavetableType_E wavetableType)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    channelConfig->wavetableType             = wavetableType;
}

void oscillator_setFrequency(const Oscillator_E oscillator, float newFrequency)
{
    OscillatorChannelData_S *channelData     = &oscillatorData.channelData[oscillator];
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];

    channelConfig->frequency        = newFrequency;
    channelData->wavetableSteps     = oscillator_getWavetableSteps(oscillator);
    channelData->currentSampleIndex = 0U;
}

float oscillator_getFrequency(const Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    return channelConfig->frequency;
}

uint32_t oscillator_getWavetableSteps(const Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];

    const uint32_t wavelengthsPerSecond = (uint32_t)(SYNTH_SAMPLE_FREQUENCY / channelConfig->frequency);
    return (WAVETABLE_NUM_SAMPLES * wavelengthsPerSecond) / SYNTH_SAMPLE_FREQUENCY;
}

#endif // FEATURE_OSC

#ifdef __cplusplus
}
#endif
