#include "wavetables.h"

/* I N C L U D E S */

#include <stddef.h>
#include <stdint.h>

#include "sine_wavetable.h"
#include "saw_wavetable.h"
#include "square_wavetable.h"

/* D E F I N E S */

/* D A T A T Y P E   D E F I N I T I O N S */

typedef struct
{
    uint32_t index;
} WavetableChannelData_S;

typedef struct
{
    WavetableChannelData_S channelData[WAVETABLE_TYPE_COUNT];
} WavetableData_S;


/* D A T A   D E F I N I T I O N S */

static WavetableData_S wavetableData =
{
    .channelData =
    {
        [WAVETABLE_TYPE_SINE] =
        {
            .index = 0U,
        },
        [WAVETABLE_TYPE_SAW] =
        {
            .index = 0U,
        },
        [WAVETABLE_TYPE_SQUARE] =
        {
            .index = 0U,
        },
    },
};

static WavetableConfig_S wavetableChannels[WAVETABLE_TYPE_COUNT] =
{
    [WAVETABLE_TYPE_SINE] =
    {
        .numSamples     = WAVETABLE_NUM_SAMPLES,
        .wavetableData  = sine_wavetable,
    },
    [WAVETABLE_TYPE_SAW] =
    {
        .numSamples     = WAVETABLE_NUM_SAMPLES,
        .wavetableData  = saw_wavetable,
    },
    [WAVETABLE_TYPE_SQUARE] =
    {
        .numSamples     = WAVETABLE_NUM_SAMPLES,
        .wavetableData  = square_wavetable,
    },
};

/* P R I V A T E    F U N C T I O N S */


/* P U B L I C   F U N C T I O N S */

float wavetable_getNextSample(const WavetableType_E type)
{
    WavetableChannelData_S * channelData = &wavetableData.channelData[type];
    const WavetableConfig_S * const channelConfig = &wavetableChannels[type];

    const float sample = channelConfig->wavetableData[channelData->index];

    if (channelData->index == WAVETABLE_NUM_SAMPLES)
    {
        channelData->index = 0U;
    }
    else
    {
        channelData->index++;
    }

    return sample;
}

float wavetable_getSample(const WavetableType_E type, const uint32_t index)
{
    return wavetableChannels[type].wavetableData[index];
}

uint32_t wavetable_getNumberOfSamples(const WavetableType_E type)
{
    const WavetableConfig_S * const channelConfig = &wavetableChannels[type];
    return channelConfig->numSamples;
}

uint32_t wavetable_getIndex(const WavetableType_E type)
{
    WavetableChannelData_S * channelData = &wavetableData.channelData[type];
    return channelData->index;
}


WavetableConfig_S * wavetable_getWavetableConfig(const WavetableType_E type)
{
    WavetableConfig_S * ret = NULL;
    if (type < WAVETABLE_TYPE_COUNT)
    {
        ret = &wavetableChannels[type];
    }
    return ret;
}
