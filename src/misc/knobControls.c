
#include "knobControls.h"

#if FEATURE_KNOB_CONTROLS

#include "sgtl5000.h"

#include "drv_encoder.h"

#include <stdio.h>

typedef struct
{
    uint32_t previousPosition;
    KnobControls_value_U value;
} KnobControls_channelData_S;

static KnobControls_channelData_S knobControls_channelData[KNOB_CONTROLS_CHANNEL_COUNT];

// clang-format off
static KnobControls_channelConfig_S knobControls_channelConfig[KNOB_CONTROLS_CHANNEL_COUNT] =
{
    [KNOB_CONTROLS_CHANNEL_SELECTOR] =
    {
        .encoderChannel     = DRV_ENCODER_CHANNEL_0,
        .valueType          = KNOB_CONTROLS_VALUE_TYPE_UINT32,
        .incrementValue.i32 = 1,
        .initialValue.i32   = 0,
        .applyMinMax        = false,
        .minValue.i32       = 0,
        .maxValue.i32       = 0,
    },
    [KNOB_CONTROLS_CHANNEL_VOLUME] =
    {
        .encoderChannel     = DRV_ENCODER_CHANNEL_1,
        .valueType          = KNOB_CONTROLS_VALUE_TYPE_FLOAT,
        .incrementValue.f32 = 0.5f, //(-1.0f * SGTL5000_HEADPHONE_VOLUME_DB_PER_STEP), // dB - lower is higher volume
        .initialValue.f32   = 0.5, // SGTL5000_INITIAL_VOLUME_DB,
        .applyMinMax        = true,
        .minValue.f32       = 1.0f, //SGTL5000_VOLUME_DB_MAX,
        .maxValue.f32       = 1.0f, //SGTL5000_VOLUME_DB_MIN,
    },
};
// clang-format on

void knobControls_init(void)
{
    memset(knobControls_channelData, 0U, sizeof(KnobControls_channelData_S));

    for (KnobControls_channel_E channel = (KnobControls_channel_E)0U; channel < KNOB_CONTROLS_CHANNEL_COUNT; channel++)
    {
        KnobControls_channelConfig_S *channelConfig = &knobControls_channelConfig[channel];
        KnobControls_channelData_S *channelData     = &knobControls_channelData[channel];

        channelData->value = channelConfig->initialValue;
    }
}

void knobControls_update(void)
{
    for (KnobControls_channel_E channel = (KnobControls_channel_E)0U; channel < KNOB_CONTROLS_CHANNEL_COUNT; channel++)
    {
        KnobControls_channelConfig_S *channelConfig = &knobControls_channelConfig[channel];
        KnobControls_channelData_S *channelData     = &knobControls_channelData[channel];

        const uint32_t currentPosition = drv_encoder_getCount(channelConfig->encoderChannel);
        printf("Channel %d: %lu\n", channel, currentPosition);

        const uint32_t diff            = (currentPosition - channelData->previousPosition);

        switch (channelConfig->valueType)
        {
            case KNOB_CONTROLS_VALUE_TYPE_UINT32:
                channelData->value.u32 += ((uint32_t)diff * channelConfig->incrementValue.u32);
                break;
            case KNOB_CONTROLS_VALUE_TYPE_INT32:
                channelData->value.i32 += ((int32_t)diff * channelConfig->incrementValue.i32);
                break;
            case KNOB_CONTROLS_VALUE_TYPE_FLOAT:
                channelData->value.f32 += ((float)diff * channelConfig->incrementValue.f32);
                break;
            default:
                // ignore
                break;
        }
    }
}

KnobControls_value_U knobControls_getValue(KnobControls_channel_E channel)
{
    const KnobControls_channelData_S *channelData = &knobControls_channelData[channel];
    return channelData->value;
}

void knobControls_reset(KnobControls_channel_E channel)
{
    const KnobControls_channelConfig_S *channelConfig = &knobControls_channelConfig[channel];
    KnobControls_channelData_S *channelData = &knobControls_channelData[channel];
    channelData->value = channelConfig->initialValue;
}

#endif  // FEATURE_KNOB_CONTROLS
