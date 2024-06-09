
/* I N C L U D E S */

#include "misc.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>

#include "LEDs.h"
#include "PCA9555.h"
#include "PCA9685.h"
#include "button.h"

#include "drv_encoder.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Utils.h"

/* D E F I N E S */

/* T Y P E D E F S */

static float brightness = 0.0f;
static bool direction   = false;
#define BRIGHTNESS_STEP 0.01F

/* P R I V A T E   D A T A   D E F I N I T I O N S */

void misc10HzTask(void *pvParameters)
{
    UNUSED(pvParameters);

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    while (1)
    {
#if FEATURE_PCA9555
        PCA9555_updateInputs();
#endif

        if (direction)
        {
            brightness += BRIGHTNESS_STEP;
            if (brightness >= LED_BRIGHTNESS_MAX_VALUE)
            {
                SATURATE_MAX(brightness, LED_BRIGHTNESS_MAX_VALUE);
                direction = false;
            }
        }
        else
        {
            brightness -= BRIGHTNESS_STEP;
            if (brightness <= LED_BRIGHTNESS_MIN_VALUE)
            {
                SATURATE_MIN(brightness, LED_BRIGHTNESS_MIN_VALUE);
                direction = true;
            }
        }
        LED_setBrightness(LED_CHANNEL_BUTTON_1, brightness);
        // LED_setBrightness(LED_CHANNEL_BUTTON_4, brightness);

        const float led2Brightness = Button_isPressed(BUTTON_CHANNEL_2) ? LED_BRIGHTNESS_MAX_VALUE : LED_BRIGHTNESS_MIN_VALUE;
        LED_setBrightness(LED_CHANNEL_BUTTON_2, led2Brightness);
        // const float led4Brightness = Button_isPressed(BUTTON_CHANNEL_4) ? LED_BRIGHTNESS_MAX_VALUE : LED_BRIGHTNESS_MIN_VALUE;

        // if (Button_isPressed(BUTTON_CHANNEL_1))
        // {
        //     LED_toggle(LED_CHANNEL_BUTTON_1);
        // }
        if (Button_isPressed(BUTTON_CHANNEL_9))
        {
            LED_toggle(LED_CHANNEL_BUTTON_9);
        }
        else
        {
            LED_setState(LED_CHANNEL_BUTTON_9, false);
        }

#if FEATURE_PCA9685
        PCA9685_updateOutputs();
#endif

#if FEATURE_GPIO
        drv_GPIO_update();
#endif

#if FEATURE_LEDS
        LED_toggle(LED_CHANNEL_BLINKY);
#endif

#if FEATURE_ENCODER
        ITM_SendChar(drv_encoder_getCount(DRV_ENCODER_CHANNEL_0));
        ITM_SendChar(drv_encoder_getCount(DRV_ENCODER_CHANNEL_1));
#endif

        vTaskDelay(100U);
    }
}

#endif  // FEATURE_MISC
