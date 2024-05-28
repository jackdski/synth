#ifndef DRV_ENCODER_HARDWARE_SPECIFIC_HPP_
#define DRV_ENCODER_HARDWARE_SPECIFIC_HPP_

/* I N C L U D E S */

#include "features.h"

#if FEATURE_ENCODER

#include "stm32g4xx.h"

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum
{
    DRV_ENCODER_CHANNEL_0,
    DRV_ENCODER_CHANNEL_1,

    DRV_ENCODER_CHANNEL_COUNT
} drv_encoder_channel_E;

/* P U B L I C   F U N C T I O N S */

void drv_encoder_hardwareSpecific_init(void);

#endif  // FEATURE_ENCODER
#endif  // DRV_ENCODER_HARDWARE_SPECIFIC_HPP_
