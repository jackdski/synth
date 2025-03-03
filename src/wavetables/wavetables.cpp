#include "wavetables.hpp"

#if (FEATURE_WAVETABLE)

/* I N C L U D E S */

#include <stddef.h>
#include <stdint.h>

using namespace Audio;

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
    WavetableChannelData_S channelData[(int)WaveformType::COUNT];
} WavetableData_S;


/* D A T A   D E F I N I T I O N S */

static WavetableConfig_S sineWavetableConfig    = WavetableConfig_S::create(WAVETABLE_NUM_SAMPLES, sine_wavetable);
static WavetableConfig_S sawWavetableConfig     = WavetableConfig_S::create(WAVETABLE_NUM_SAMPLES, saw_wavetable);
static WavetableConfig_S squareWavetableConfig  = WavetableConfig_S::create(WAVETABLE_NUM_SAMPLES, square_wavetable);

static WavetableConfig_S wavetableChannels[] =
{
    sineWavetableConfig,
    sawWavetableConfig,
    squareWavetableConfig,
};

/* P R I V A T E    F U N C T I O N S */


/* P U B L I C   F U N C T I O N S */

float Audio::wavetable_getSample(const WaveformType type, const uint32_t index)
{
    return wavetableChannels[(int)type].wavetableData[index];
}

uint32_t Audio::wavetable_getNumberOfSamples(const WaveformType type)
{
    const WavetableConfig_S * const channelConfig = &wavetableChannels[(int)type];
    return channelConfig->numSamples;
}

WavetableConfig_S * Audio::wavetable_getWavetableConfig(const WaveformType type)
{
    WavetableConfig_S * ret = NULL;
    if (type < WaveformType::COUNT)
    {
        ret = &wavetableChannels[(int)type];
    }
    return ret;
}

#endif // FEATURE_WAVETABLE
