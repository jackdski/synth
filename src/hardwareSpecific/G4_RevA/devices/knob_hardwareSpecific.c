
/* I N C L U D E S */

#include "knob.h"

#if (FEATURE_KNOB)

#include "drv_encoder.h"

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

// clang-format off
static Knob_channelConfig_S Knob_channelConfig[KNOB_CHANNEL_COUNT] =
{
    [KNOB_CHANNEL_0] = { .encoderChannel = DRV_ENCODER_CHANNEL_0, },
    [KNOB_CHANNEL_1] = { .encoderChannel = DRV_ENCODER_CHANNEL_1, },
};
// clang-format on

Knob_config_S Knob_config = {
    .channelConfig = Knob_channelConfig,
    .channelCount  = (uint32_t)(KNOB_CHANNEL_COUNT),
};

/* P U B L I C   F U N C T I O N S */

#endif  // FEATURE_KNOB
