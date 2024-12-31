#include "LEDManager.hpp"

#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "LEDs.h"
#include "button.h"


#include "PCA9685.h"

#if (FEATURE_LED_MANAGER)

using namespace LED;

/* D E F I N E S */

#define LED_BUTTON_PAIR_COUNT   16U


/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */


/* T Y P E D E F S */

#if (FEATURE_BUTTON && FEATURE_LEDS)
typedef struct
{
    Button_channel_E button;
    LED_channel_E led;
} LEDButtonConfig_S;
#endif

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

#if (FEATURE_LEDS)
static LEDButtonConfig_S ledButtonConfig[LED_BUTTON_PAIR_COUNT] = {
    {.button = BUTTON_CHANNEL_1, .led = LED_CHANNEL_BUTTON_1},
    {.button = BUTTON_CHANNEL_2, .led = LED_CHANNEL_BUTTON_2},
    {.button = BUTTON_CHANNEL_3, .led = LED_CHANNEL_BUTTON_3},
    {.button = BUTTON_CHANNEL_4, .led = LED_CHANNEL_BUTTON_4},
    {.button = BUTTON_CHANNEL_5, .led = LED_CHANNEL_BUTTON_5},
    {.button = BUTTON_CHANNEL_6, .led = LED_CHANNEL_BUTTON_6},
    {.button = BUTTON_CHANNEL_7, .led = LED_CHANNEL_BUTTON_7},
    {.button = BUTTON_CHANNEL_8, .led = LED_CHANNEL_BUTTON_8},
    {.button = BUTTON_CHANNEL_9, .led = LED_CHANNEL_BUTTON_9},
    {.button = BUTTON_CHANNEL_10, .led = LED_CHANNEL_BUTTON_10},
    {.button = BUTTON_CHANNEL_11, .led = LED_CHANNEL_BUTTON_11},
    {.button = BUTTON_CHANNEL_12, .led = LED_CHANNEL_BUTTON_12},
    {.button = BUTTON_CHANNEL_13, .led = LED_CHANNEL_BUTTON_13},
    {.button = BUTTON_CHANNEL_14, .led = LED_CHANNEL_BUTTON_14},
    {.button = BUTTON_CHANNEL_15, .led = LED_CHANNEL_BUTTON_15},
    {.button = BUTTON_CHANNEL_16, .led = LED_CHANNEL_BUTTON_16},
};
#endif

/*  L E D   M A N A G E R */

void LEDManager::Init(void)
{

}

void LEDManager::Update(void)
{
    switch (state)
    {
        case LEDManagerState::ON_PRESS:
            UpdateOnPress();
            break;

        case LEDManagerState::SEQUENCER:
            UpdateSequencer();
            break;

        case LEDManagerState::INITIALIZING:
        default:
            // TODO
            break;
    }

#if FEATURE_PCA9685
    PCA9685_updateOutputs();
#endif
}

void LEDManager::UpdateOnPress(void)
{
#if (FEATURE_LEDS)
    for (uint8_t channel = 0U; channel < LED_BUTTON_PAIR_COUNT; channel++)
    {
        const LEDButtonConfig_S *config = &ledButtonConfig[channel];

        const float ledBrightness = Button_isPressed(config->button) ? LED_BRIGHTNESS_MAX_VALUE : LED_BRIGHTNESS_MIN_VALUE;
        LED_setBrightness(config->led, ledBrightness);
    }
#endif
}

void LEDManager::UpdateSequencer(void)
{

}

#endif // FEATURE_LED_MANAGER