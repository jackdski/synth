/* I N C L U D E S */

#include "LEDs.h"

#if (FEATURE_LEDS)

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */


/* D A T A   D E F I N I T I O N S */

static LED_channelConfig_S ledChannelConfig[LED_CHANNEL_COUNT] =
{
    [LED_CHANNEL_BLINKY] =
    {
        .type = LED_TYPE_GPIO,
        .gpioConfig = { .gpio = DRV_GPIO_CHANNEL_LED_BLINKY, },
    }
};

extern LED_config_S ledConfig;
LED_config_S ledConfig =
{
    .channelConfig = ledChannelConfig,
    .channelCount  = LED_CHANNEL_COUNT,
};

/* P U B L I C   F U N C T I O N S */


#endif // FEATURE_LEDS