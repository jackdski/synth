#ifndef DRV_ENCODER_HPP_
#define DRV_ENCODER_HPP_

/* I N C L U D E S */

#include "features.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include "drv_encoder_hardwareSpecific.h"
#include "main.h"

#if FEATURE_ENCODER

/* D E F I N E S */

/* T Y P E D E F S */

/* P U B L I C   F U N C T I O N S */

void drv_encoder_init(void);
uint32_t drv_encoder_getCount(const drv_encoder_channel_E channel);
int32_t drv_encoder_updateAndGetDiff(const drv_encoder_channel_E channel);

void drv_encoder_hardwareSpecific_init(void);

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_ENCODER
#endif  // DRV_ENCODER_HPP_
