#include "note.h"

/* I N C L U D E S */

#include "float.h"
#include "stdint.h"

#if (FEATURE_NOTE)

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    bool active;
} Note_data_S;

/* D A T A   D E F I N I T I O N S */

// clang-format off
static Note_config_S noteConfig[NOTE_CHANNEL_COUNT] =
{
    [NOTE_CHANNEL_0] =
    {
        .osc  = OSCILLATOR_0,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_1] =
    {
        .osc  = OSCILLATOR_1,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_2] =
    {
        .osc  = OSCILLATOR_2,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_3] =
    {
        .osc  = OSCILLATOR_3,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_4] =
    {
        .osc  = OSCILLATOR_4,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_5] =
    {
        .osc  = OSCILLATOR_5,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_6] =
    {
        .osc  = OSCILLATOR_6,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
    [NOTE_CHANNEL_7] =
    {
        .osc  = OSCILLATOR_7,
        .adsr = DEFAULT_ADSR_SETTINGS,
        .wavetableType = WAVETABLE_TYPE_SINE,
    },
};
// clang-format on

static Note_data_S noteData[NOTE_CHANNEL_COUNT];

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void Note_init(void)
{
    for (Note_channel_E channel = (Note_channel_E)0U; channel < NOTE_CHANNEL_COUNT; channel++)
    {
        oscillator_init(noteConfig[channel].osc, noteConfig[channel].wavetableType, 0.0f);
    }
}

float Note_update(const Note_channel_E channel)
{
    float sample       = oscillator_getSample(noteConfig[channel].osc);
    const bool noteOff = (noteData[channel].active == false);
    sample *= ADSR_update(&noteConfig[channel].adsr, noteOff);

    return sample;
}

void Note_setAdsrSettings(const Note_channel_E channel, const AdsrSettings_S adsrSettings)
{
    ADSR_S *adsr   = &noteConfig[channel].adsr;
    adsr->settings = adsrSettings;
}

void Note_setAdsrMode(const Note_channel_E channel, const AdsrMode_E adsrMode)
{
    ADSR_S *adsr = &noteConfig[channel].adsr;
    adsr->mode   = adsrMode;
}

void Note_setAdsrModeSettings(const Note_channel_E channel, const ADSR_data_U modeSettings)
{
    ADSR_S *adsr = &noteConfig[channel].adsr;
    adsr->data   = modeSettings;
}

void Note_setAdsrAmplitude(const Note_channel_E channel, const float amplitude)
{
    ADSR_S *adsr    = &noteConfig[channel].adsr;
    adsr->amplitude = amplitude;
    // might need to reset ADSR?
}

void Note_setActive(const Note_channel_E channel, bool active)
{
    if (channel < NOTE_CHANNEL_COUNT)
    {
        noteData[channel].active = active;
    }
}

void Note_setFrequency(const Note_channel_E channel, float frequency)
{
    oscillator_setFrequency(noteConfig[channel].osc, frequency);
}

bool Note_isActive(const Note_channel_E channel)
{
    return noteData[channel].active;
}

float Note_getFrequency(const Note_channel_E channel)
{
    return oscillator_getFrequency(noteConfig[channel].osc);
}

#endif  // FEATURE_NOTE
