
/* I N C L U D E S */

#include "LEDs.h"

#if FEATURE_LEDS

#ifdef __cplusplus
extern "C" {
#endif

#include "PCA9685.h"
#include "drv_GPIO.h"

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    LED_config_S *config;
} LED_data_S;

/* D A T A   D E F I N I T I O N S */

static LED_data_S ledData;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void LED_init(LED_config_S *config)
{
    if (config != NULL)
    {
        ledData.config = config;
        for (LED_channel_E channel = (LED_channel_E)0U; channel < LED_CHANNEL_COUNT; channel++)
        {
            LED_setState(channel, false);
        }
    }
}

void LED_setState(LED_channel_E channel, bool enable)
{
    LED_channelConfig_S *channelConfig = &ledData.config->channelConfig[channel];

    switch (channelConfig->type)
    {
        case LED_TYPE_GPIO:
        {
            drv_GPIO_setOutput(channelConfig->gpioConfig.gpio, (drv_GPIO_state_E)enable);
            break;
        }

#if FEATURE_PCA9685
        case LED_TYPE_PCA9685:
        {
            PCA9685_setState(channelConfig->PCA9685Config.channel, enable);
            break;
        }
#endif
        default:
            break;
    }
}

void LED_toggle(LED_channel_E channel)
{
    LED_channelConfig_S *channelConfig = &ledData.config->channelConfig[channel];
    switch (channelConfig->type)
    {
        case LED_TYPE_GPIO:
            drv_GPIO_toggleOutput(channelConfig->gpioConfig.gpio);
            break;
#if FEATURE_PCA9685
        case LED_TYPE_PCA9685:
            PCA9685_toggle(channelConfig->PCA9685Config.channel);
            break;
#endif
        default:
            // do nothing
            break;
    }
}

bool LED_getState(LED_channel_E channel)
{
    LED_channelConfig_S *channelConfig = &ledData.config->channelConfig[channel];
    return (drv_GPIO_getInput(channelConfig->gpioConfig.gpio) == DRV_GPIO_HIGH);
}

void LED_setBrightness(LED_channel_E channel, float brightness)
{
    LED_channelConfig_S *channelConfig = &ledData.config->channelConfig[channel];
    switch (channelConfig->type)
    {
        case LED_TYPE_GPIO:
            drv_GPIO_setOutput(channelConfig->gpioConfig.gpio, (brightness > 0.0f));
            break;

#if FEATURE_PCA9685
        case LED_TYPE_PCA9685:
            PCA9685_setBrightness(channelConfig->PCA9685Config.channel, brightness);
            break;
#endif
        default:
            // do nothing
            break;
    }
}

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_LEDS
