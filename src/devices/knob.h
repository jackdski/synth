#ifndef KNOB_HPP_
#define KNOB_HPP_

#include "features.h"

#if (FEATURE_KNOB)

/* I N C L U D E S */

#include "drv_encoder.h"
#include "knob_hardwareSpecific.h"

#include <stdint.h>

/* D E F I N E S */

/* T Y P E D E F S */

typedef struct
{
    drv_encoder_channel_E encoderChannel;
} Knob_channelConfig_S;

typedef struct
{
    Knob_channelConfig_S *channelConfig;
    uint32_t channelCount;
} Knob_config_S;

/* P U B L I C   F U N C T I O N S */

void Knob_init(Knob_config_S *config);

uint32_t Knob_getCount(const Knob_channel_E channel);

#endif  // FEATURE_KNOB
#endif  // KNOB_HPP_
