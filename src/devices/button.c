/* I N C L U D E S */

#ifdef __cplusplus
extern "C" {
#endif

#include "button.h"
#include "PCA9555.h"
#include "Utils.h"
#include "drv_GPIO.h"

#if FEATURE_BUTTON

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    Button_config_S *config;
} Button_data_S;

/* D A T A   D E F I N I T I O N S */

static Button_data_S button_data;

/* P R I V A T E   F U N C T I O N S */

/* P U B L I C   F U N C T I O N S */

void Button_init(Button_config_S *config)
{
    if (config != NULL)
    {
        button_data.config = config;
    }
}

bool Button_isPressed(const Button_channel_E channel)
{
    const Button_channelConfig_S *channelConfig = &button_data.config->channelConfig[channel];

    bool ret = false;
    switch (channelConfig->inputType)
    {
        case BUTTON_INPUT_TYPE_GPIO:
        {
            bool pressed = drv_GPIO_getInput(channelConfig->gpioConfig.gpioChannel);
            if (channelConfig->gpioConfig.invert)
            {
                pressed = (pressed == false);
            }
            ret = pressed;
        }
            break;

#if FEATURE_PCA9555
        case BUTTON_INPUT_TYPE_PORT_EXPANDER:
            ret = PCA9555_getState(channelConfig->pca9555Channel);
            break;
#endif

        case BUTTON_INPUT_TYPE_COUNT:
        default:
            ret = false;
            break;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif


#endif  // FEATURE_BUTTON
