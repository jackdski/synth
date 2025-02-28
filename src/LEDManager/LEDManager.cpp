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
struct LEDButtonConfig_S
{
    Button_channel_E button;
    LED_channel_E led;

    static LEDButtonConfig_S create(Button_channel_E button, LED_channel_E led)
    {
        LEDButtonConfig_S config;
        config.button = button;
        config.led = led;
        return config;
    }
};

#endif

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

#if (FEATURE_BUTTON && FEATURE_LEDS)
static LEDButtonConfig_S ledButton1 = LEDButtonConfig_S::create(BUTTON_CHANNEL_1, LED_CHANNEL_BUTTON_1);
static LEDButtonConfig_S ledButton2 = LEDButtonConfig_S::create(BUTTON_CHANNEL_2, LED_CHANNEL_BUTTON_2);
static LEDButtonConfig_S ledButton3 = LEDButtonConfig_S::create(BUTTON_CHANNEL_3, LED_CHANNEL_BUTTON_3);
static LEDButtonConfig_S ledButton4 = LEDButtonConfig_S::create(BUTTON_CHANNEL_4, LED_CHANNEL_BUTTON_4);
static LEDButtonConfig_S ledButton5 = LEDButtonConfig_S::create(BUTTON_CHANNEL_5, LED_CHANNEL_BUTTON_5);
static LEDButtonConfig_S ledButton6 = LEDButtonConfig_S::create(BUTTON_CHANNEL_6, LED_CHANNEL_BUTTON_6);
static LEDButtonConfig_S ledButton7 = LEDButtonConfig_S::create(BUTTON_CHANNEL_7, LED_CHANNEL_BUTTON_7);
static LEDButtonConfig_S ledButton8 = LEDButtonConfig_S::create(BUTTON_CHANNEL_8, LED_CHANNEL_BUTTON_8);
static LEDButtonConfig_S ledButton9 = LEDButtonConfig_S::create(BUTTON_CHANNEL_9, LED_CHANNEL_BUTTON_9);
static LEDButtonConfig_S ledButton10 = LEDButtonConfig_S::create(BUTTON_CHANNEL_10, LED_CHANNEL_BUTTON_10);
static LEDButtonConfig_S ledButton11 = LEDButtonConfig_S::create(BUTTON_CHANNEL_11, LED_CHANNEL_BUTTON_11);
static LEDButtonConfig_S ledButton12 = LEDButtonConfig_S::create(BUTTON_CHANNEL_12, LED_CHANNEL_BUTTON_12);
static LEDButtonConfig_S ledButton13 = LEDButtonConfig_S::create(BUTTON_CHANNEL_13, LED_CHANNEL_BUTTON_13);
static LEDButtonConfig_S ledButton14 = LEDButtonConfig_S::create(BUTTON_CHANNEL_14, LED_CHANNEL_BUTTON_14);
static LEDButtonConfig_S ledButton15 = LEDButtonConfig_S::create(BUTTON_CHANNEL_15, LED_CHANNEL_BUTTON_15);
static LEDButtonConfig_S ledButton16 = LEDButtonConfig_S::create(BUTTON_CHANNEL_16, LED_CHANNEL_BUTTON_16);


static LEDButtonConfig_S ledButtonConfig[LED_BUTTON_PAIR_COUNT] =
{
    ledButton1,
    ledButton2,
    ledButton3,
    ledButton4,
    ledButton5,
    ledButton6,
    ledButton7,
    ledButton8,
    ledButton9,
    ledButton10,
    ledButton11,
    ledButton12,
    ledButton13,
    ledButton14,
    ledButton15,
    ledButton16,
};
#endif

/*  L E D   M A N A G E R */

LEDManager* LEDManager::instance = nullptr;

void LEDManager::init(void)
{
}

void LEDManager::setState(LEDManagerState newState)
{
    state = newState;
}

void LEDManager::update(void)
{
    switch (state)
    {
        case LEDManagerState::ON_PRESS:
            updateOnPress();
            break;

        case LEDManagerState::SEQUENCER:
            updateSequencer();
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

void LEDManager::updateOnPress(void)
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

void LEDManager::updateSequencer(void)
{

}

#endif // FEATURE_LED_MANAGER