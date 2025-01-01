#ifndef KNOB_CONTROLS_H_
#define KNOB_CONTROLS_H_

#include "features.h"

#if FEATURE_KNOB_CONTROLS

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "drv_encoder.h"

typedef enum
{
    KNOB_CONTROLS_VALUE_TYPE_UINT32,
    KNOB_CONTROLS_VALUE_TYPE_INT32,
    KNOB_CONTROLS_VALUE_TYPE_FLOAT,
} knobControls_valueType_E;

typedef enum
{
    KNOB_CONTROLS_CHANNEL_VOLUME,
    KNOB_CONTROLS_CHANNEL_COUNT
} KnobControls_channel_E;

typedef union
{
    uint32_t u32;
    int32_t i32;
    float f32;
} KnobControls_value_U;

typedef struct
{
    drv_encoder_channel_E encoderChannel;
    knobControls_valueType_E valueType;
    KnobControls_value_U initialValue;
    KnobControls_value_U minValue;
    KnobControls_value_U maxValue;
    KnobControls_value_U incrementValue;

    bool applyMinMax;
} KnobControls_channelConfig_S;

void knobControls_init(void);
void knobControls_update(void);

KnobControls_value_U knobControls_getValue(KnobControls_channel_E channel);
void knobControls_reset(KnobControls_channel_E channel);

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_KNOB_CONTROLS
#endif  // KNOB_CONTROLS_H_
