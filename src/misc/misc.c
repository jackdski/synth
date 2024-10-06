
/* I N C L U D E S */

#include "misc.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "LEDs.h"
#include "PCA9555.h"
#include "PCA9685.h"
#include "button.h"
#include "sgtl5000.h"

#include "knobControls.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Utils.h"
#include "cpu_utils.h"
#include "hardwareSpecific.h"

/* D E F I N E S */

#define MISC_BRIGHTNESS_PULSE_DEMO 0

/* T Y P E D E F S */

#define BRIGHTNESS_STEP            0.01F

#if (FEATURE_BUTTON || FEATURE_LED)
typedef struct
{
#if FEATURE_BUTTON
    Button_channel_E button;
#endif
#if FEATURE_LED
    LED_channel_E led;
#endif
} MiscLedButtonConfig_S;
#endif

/* P R I V A T E   D A T A   D E F I N I T I O N S */

#if (FEATURE_LED)
static MiscLedButtonConfig_S ledButtonConfig[16U] = {
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

static TickType_t misc10HzTaskLastWakeTime;
// static TickType_t misc100HzTaskLastWakeTime;

static char buffer[40U * 6U];

/* P U B L I C   F U N C T I O N S */

void misc1HzTask(void *pvParameters)
{
    UNUSED(pvParameters);
    while(1)
    {
#if (FEATURE_SGTL5000)
        SGTL5000_pollRegisters();
#endif

        // vTaskGetRunTimeStats(buffer);
        const uint16_t cpuUsage = osGetCPUUsage();
        sprintf(buffer, "CPU Usage: %d\n", cpuUsage);
        printf(buffer, 40U * 6U);
        vTaskDelay(1000U);
    }
}

void misc10HzTask(void *pvParameters)
{
    UNUSED(pvParameters);

    misc10HzTaskLastWakeTime = xTaskGetTickCount();
    // vTaskDelay(pdMS_TO_TICKS(100U));

    while (1)
    {
#if FEATURE_PCA9555
        PCA9555_updateInputs();
#endif

#if (FEATURE_LED)
        for (uint8_t channel = 0U; channel < 16U; channel++)
        {
            const MiscLedButtonConfig_S *config = &ledButtonConfig[channel];

            const float ledBrightness = Button_isPressed(config->button) ? LED_BRIGHTNESS_MAX_VALUE : LED_BRIGHTNESS_MIN_VALUE;
            LED_setBrightness(config->led, ledBrightness);
        }
#endif

// #if FEATURE_PCA9685
//         PCA9685_updateOutputs();
// #endif

#if FEATURE_GPIO
        drv_GPIO_update();
#endif

// #if FEATURE_LEDS
//         LED_toggle(LED_CHANNEL_BLINKY);
// #endif

#if FEATURE_ENCODER
        knobControls_update();
        const KnobControls_value_U volumeValue = knobControls_getValue(KNOB_CONTROLS_CHANNEL_VOLUME);
        // printf("Volume: %f\n", volumeValue.f32);
#endif
#if FEATURE_SGTL5000
        SGTL5000_updateVolume(volumeValue.f32);
#endif

        // vTaskDelay(pdMS_TO_TICKS(100U));
        xTaskDelayUntil(&misc10HzTaskLastWakeTime, pdMS_TO_TICKS(100U));
    }
}

// void misc100HzTask(void *pvParameters)
// {
//     UNUSED(pvParameters);

//     misc100HzTaskLastWakeTime = xTaskGetTickCount();

//     while (1)
//     {
// // #if FEATURE_PCA9555
// //         PCA9555_updateInputs();
// // #endif
//         xTaskDelayUntil(&misc10HzTaskLastWakeTime, pdMS_TO_TICKS(10U));
//     }
// }

#endif  // FEATURE_MISC
