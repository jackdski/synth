
/* I N C L U D E S */

#include "misc.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "LEDs.h"
#include "PCA9555.h"
#include "button.h"
#include "sgtl5000.h"

#include "knobControls.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Utils.h"
#include "cpu_utils.h"
#include "hardwareSpecific.h"

#ifdef __cplusplus
extern "C" {
#endif

/* D E F I N E S */

#define MISC_BRIGHTNESS_PULSE_DEMO 0

/* T Y P E D E F S */

#define BRIGHTNESS_STEP            0.01F


extern TaskHandle_t misc100HzTaskHandle;
extern TaskHandle_t misc10HzTaskHandle;
extern TaskHandle_t audioTaskHandle;
extern TaskHandle_t displayTaskHandle;

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static TickType_t misc10HzTaskLastWakeTime;
static TickType_t misc100HzTaskLastWakeTime;

static char buffer[40U * 6U];

/* P U B L I C   F U N C T I O N S */

void misc1HzTask(void *pvParameters)
{
    UNUSED(pvParameters);

    UBaseType_t displayTaskHighWaterMark;
    UBaseType_t audioTaskHighWaterMark;
    UBaseType_t misc10HzTaskHighWaterMark;
    UBaseType_t misc100HzTaskHighWaterMark;

    while(1)
    {
#if (FEATURE_SGTL5000)
        SGTL5000_pollRegisters();
#endif

        // vTaskGetRunTimeStats(buffer);
        displayTaskHighWaterMark   = uxTaskGetStackHighWaterMark( displayTaskHandle );
        audioTaskHighWaterMark     = uxTaskGetStackHighWaterMark( audioTaskHandle );
        misc100HzTaskHighWaterMark = uxTaskGetStackHighWaterMark( misc100HzTaskHandle );
        misc10HzTaskHighWaterMark  = uxTaskGetStackHighWaterMark( misc10HzTaskHandle );

        printf("Display Task watermark: %ld\n", (uint32_t)displayTaskHighWaterMark);
        printf("Audio Task watermark: %ld\n", (uint32_t)audioTaskHighWaterMark);
        printf("Misc 100Hz Task watermark: %ld\n", (uint32_t)misc100HzTaskHighWaterMark);
        printf("Misc 10Hz Task watermark: %ld\n", (uint32_t)misc10HzTaskHighWaterMark);

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

    while (1)
    {
#if FEATURE_PCA9555
        PCA9555_updateInputs();
#endif

#if FEATURE_GPIO
        drv_GPIO_update();
#endif

#if FEATURE_LEDS
        LED_toggle(LED_CHANNEL_BLINKY);
#endif

#if FEATURE_ENCODER
        knobControls_update();
        const KnobControls_value_U volumeValue = knobControls_getValue(KNOB_CONTROLS_CHANNEL_VOLUME);
        // printf("Volume: %f\n", volumeValue.f32);
#endif

#if FEATURE_SGTL5000
        SGTL5000_updateVolume(volumeValue.f32);
#endif

        xTaskDelayUntil(&misc10HzTaskLastWakeTime, pdMS_TO_TICKS(100U));
    }
}

void misc100HzTask(void *pvParameters)
{
    UNUSED(pvParameters);

    misc100HzTaskLastWakeTime = xTaskGetTickCount();

    while (1)
    {
// #if FEATURE_PCA9555
//         PCA9555_updateInputs();
// #endif
        xTaskDelayUntil(&misc100HzTaskLastWakeTime, pdMS_TO_TICKS(10U));
    }
}

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_MISC
