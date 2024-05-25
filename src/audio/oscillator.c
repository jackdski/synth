#include "oscillator.h"
#include "wavetables.h"

#include <stdint.h>

typedef struct
{
    uint32_t currentSampleIndex;
    uint32_t wavetableSteps;  // getWavetableSteps();
} OscillatorChannelData_S;

typedef struct
{
    OscillatorChannelData_S channelData[OSCILLATOR_COUNT];
} OscillatorData_S;

static OscillatorChannelConfig_S oscillatorChannelConfig[OSCILLATOR_COUNT];

static OscillatorConfig_S oscillatorConfig = {
    .channelConfig = oscillatorChannelConfig,
};

static OscillatorData_S oscillatorData;

float oscillator_getSample(Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    OscillatorChannelData_S *channelData     = &oscillatorData.channelData[oscillator];

    float sample = 0.0f;
    if (channelConfig->frequency > 0U)
    {
        channelData->currentSampleIndex += channelData->wavetableSteps;
        if (channelData->currentSampleIndex > WAVETABLE_NUM_SAMPLES)
        {
            channelData->currentSampleIndex -= WAVETABLE_NUM_SAMPLES;
        }

        sample = wavetable_getSample(channelConfig->wavetableType, channelData->currentSampleIndex);
        sample *= channelConfig->amplitude;
    }

    return sample;
}

void oscillator_selectWavetable(const Oscillator_E oscillator, WavetableType_E wavetableType)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    channelConfig->wavetableType             = wavetableType;
}

void oscillator_setFrequency(const Oscillator_E oscillator, uint32_t newFrequency)
{
    OscillatorChannelData_S *channelData     = &oscillatorData.channelData[oscillator];
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];

    channelConfig->frequency        = newFrequency;
    channelData->wavetableSteps     = oscillator_getWavetableSteps(oscillator);
    channelData->currentSampleIndex = 0U;  // TODO: maybe delete?
}

float oscillator_getFrequency(const Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];
    return channelConfig->frequency;
}

uint32_t oscillator_getWavetableSteps(const Oscillator_E oscillator)
{
    OscillatorChannelConfig_S *channelConfig = &oscillatorConfig.channelConfig[oscillator];

    const uint32_t wavelengthsPerSecond = (channelConfig->sampleFrequency / channelConfig->frequency);
    return (WAVETABLE_NUM_SAMPLES * wavelengthsPerSecond) / channelConfig->sampleFrequency;
}
