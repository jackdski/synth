#ifndef LEDS_H_
#define LEDS_H_

/* I N C L U D E S */

#include "features.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (FEATURE_LEDS)

#include "LEDs_hardwareSpecific.h"

#include "PCA9685.h"
#include "drv_GPIO.h"

#include <stdbool.h>
#include <stdint.h>

/* D E F I N E S */

#define LED_BRIGHTNESS_MIN_VALUE 0.0f
#define LED_BRIGHTNESS_MAX_VALUE 1.0f

/* T Y P E D E F S */

typedef enum
{
    LED_TYPE_GPIO,
    LED_TYPE_PCA9685,
} LED_type_E;

typedef struct
{
    drv_GPIO_channel_E gpio;
} LED_GpioConfig_S;

typedef struct
{
    uint8_t channel;  // TODO: PCA9685PW driver
} LED_PCA9685Config_S;

typedef struct
{
    LED_type_E type;

    union
    {
        LED_GpioConfig_S gpioConfig;
        LED_PCA9685Config_S PCA9685Config;
    };
} LED_channelConfig_S;

typedef struct
{
    LED_channelConfig_S *channelConfig;
    uint32_t channelCount;
} LED_config_S;

/* P U B L I C   F U N C T I O N S */

void LED_init(LED_config_S *config);
void LED_setState(LED_channel_E channel, bool enable);
void LED_toggle(LED_channel_E channel);
bool LED_getState(LED_channel_E channel);
void LED_setBrightness(LED_channel_E channel, float brightness);


#ifdef __cplusplus
}
#endif

#endif  // FEATURE_LEDS
#endif  // LEDS_H_
