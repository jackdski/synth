#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "features.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (FEATURE_BUTTON)

/* I N C L U D E S */

#include "PCA9555.h"
#include "drv_GPIO.h"

#include "button_hardwareSpecific.h"

#include <stdint.h>

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum
{
    BUTTON_INPUT_TYPE_GPIO,
#if FEATURE_PCA9555
    BUTTON_INPUT_TYPE_PORT_EXPANDER,
#endif
    BUTTON_INPUT_TYPE_COUNT
} Button_inputType_E;

typedef struct
{
    drv_GPIO_channel_E gpioChannel;
    bool               invert;
} Button_gpioConfig_S;

typedef struct
{
    Button_inputType_E inputType;
    union
    {
#if FEATURE_PCA9555
        PCA9555_channel_E pca9555Channel;
#endif
        Button_gpioConfig_S gpioConfig;
    };

    // uint32_t debounceMs;
} Button_channelConfig_S;

typedef struct
{
    Button_channelConfig_S *channelConfig;
    uint32_t channelCount;
} Button_config_S;

/* P U B L I C   F U N C T I O N S */

void Button_init(Button_config_S *config);

bool Button_isPressed(const Button_channel_E channel);

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_BUTTON
#endif  // BUTTON_HPP_
