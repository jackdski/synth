#ifndef NOTE_HPP_
#define NOTE_HPP_

#include "features.h"

#if (FEATURE_NOTE)

/* I N C L U D E S */

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "adsr.h"
#include "oscillator.h"

/* D E F I N E S */

// clang-format off
#define DEFAULT_ADSR_SETTINGS                       \
    {                                               \
        .mode = ADSR_MODE_LINEAR,                   \
        .data.linearData.step = 0.00001f,           \
        .stage                = ADSR_STAGE_ATTACK,  \
        .settings.attack      = 0.5f,               \
        .settings.decay       = 0.5f,               \
        .settings.sustain     = 0.5f,               \
        .settings.release     = 0.5f,               \
        .settings.noteOn      = false,              \
        .amplitude            = ADSR_MAX_AMPLITUDE, \
    }
// clang-format on

/* T Y P E D E F S */

typedef enum
{
    NOTE_CHANNEL_0,
    NOTE_CHANNEL_1,
    NOTE_CHANNEL_2,
    NOTE_CHANNEL_3,
    NOTE_CHANNEL_4,
    NOTE_CHANNEL_5,
    NOTE_CHANNEL_6,
    NOTE_CHANNEL_7,

    NOTE_CHANNEL_COUNT,
} Note_channel_E;

typedef struct
{
    ADSR_S adsr;
    Oscillator_E osc;
    WavetableType_E wavetableType;
} Note_config_S;

/* P U B L I C   F U N C T I O N S */

void Note_init(void);

float Note_update(const Note_channel_E channel);

void Note_setAdsrSettings(const Note_channel_E channel, const AdsrSettings_S adsrSettings);
void Note_setAdsrMode(const Note_channel_E channel, const AdsrMode_E adsrMode);
void Note_setAdsrModeSettings(const Note_channel_E channel, const ADSR_data_U modeSettings);
void Note_setAdsrAmplitude(const Note_channel_E channel, const float amplitude);

void Note_setActive(const Note_channel_E channel, bool active);
void Note_setFrequency(const Note_channel_E channel, float frequency);

bool Note_isActive(const Note_channel_E channel);

float Note_getFrequency(const Note_channel_E channel);

#endif  // FEATURE_NOTE
#endif  // NOTE_HPP_
