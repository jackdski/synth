#include "PCA9555.h"

#if FEATURE_PCA9555

static PCA9555_pinConfig_S PCA9555_pinConfig[PCA9555_CHANNEL_COUNT] = {
    [PCA9555_CHANNEL_0] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_1] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_2] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_3] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_4] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_5] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_6] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_7] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_8] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_9] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_10] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_11] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_12] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_13] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_14] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
    [PCA9555_CHANNEL_15] =
        {
                             .pinType = PCA9555_PIN_TYPE_INPUT,
                             .invert  = true,
                             },
};

PCA9555_config_S PCA9555_config = {
    .i2cDevice      = DRV_I2C_DEVICE_PORT_EXPANDER,
    .pinConfig      = PCA9555_pinConfig,
    .pinConfigCount = (uint32_t)(PCA9555_CHANNEL_COUNT),
};

#endif  // FEATURE_PCA9555
