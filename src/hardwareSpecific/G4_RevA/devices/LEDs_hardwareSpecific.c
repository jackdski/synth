/* I N C L U D E S */

#include "LEDs.h"

#if (FEATURE_LEDS)

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

// clang-format off
static LED_channelConfig_S ledChannelConfig[LED_CHANNEL_COUNT] =
{
    [LED_CHANNEL_BLINKY] =
    {
        .type = LED_TYPE_GPIO,
        .gpioConfig = { .gpio = DRV_GPIO_CHANNEL_LED_BLINKY, },
    },
    [LED_CHANNEL_BUTTON_1] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_7 },
    },
    [LED_CHANNEL_BUTTON_2] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_6 },
    },
    [LED_CHANNEL_BUTTON_3] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_5 },
    },
    [LED_CHANNEL_BUTTON_4] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_4 },
    },
    [LED_CHANNEL_BUTTON_5] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_11 },
    },
    [LED_CHANNEL_BUTTON_6] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_10 },
    },
    [LED_CHANNEL_BUTTON_7] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_9 },
    },
    [LED_CHANNEL_BUTTON_8] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_8 },
    },
    [LED_CHANNEL_BUTTON_9] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_3 },
    },
    [LED_CHANNEL_BUTTON_10] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_2 },
    },
    [LED_CHANNEL_BUTTON_11] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_1 },
    },
    [LED_CHANNEL_BUTTON_12] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_0 },
    },
    [LED_CHANNEL_BUTTON_13] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_15 },
    },
    [LED_CHANNEL_BUTTON_14] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_14 },
    },
    [LED_CHANNEL_BUTTON_15] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_13 },
    },
    [LED_CHANNEL_BUTTON_16] =
    {
        .type = LED_TYPE_PCA9685,
        .PCA9685Config = { .channel = PCA9685_CHANNEL_12 },
    },
};
// clang-format on

// extern LED_config_S ledConfig;
LED_config_S ledConfig = {
    .channelConfig = ledChannelConfig,
    .channelCount  = LED_CHANNEL_COUNT,
};

/* P U B L I C   F U N C T I O N S */

#endif  // FEATURE_LEDS
