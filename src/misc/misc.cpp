
/* I N C L U D E S */

#include "misc.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "LEDManager.hpp"

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

// #ifdef __cplusplus
// extern "C" {
// #endif

/* D E F I N E S */

#define PRINT_HIGHWATER_MARKS 0

/* T Y P E D E F S */

#if PRINT_HIGHWATER_MARKS
extern TaskHandle_t misc100HzTaskHandle;
extern TaskHandle_t misc10HzTaskHandle;
extern TaskHandle_t audioTaskHandle;
extern TaskHandle_t displayTaskHandle;
#endif

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static TickType_t misc10HzTaskLastWakeTime;
static TickType_t misc100HzTaskLastWakeTime;

static char buffer[40U * 8U];

#if (FEATURE_LED_MANAGER)
static LED::LEDManager* LEDManager = LED::LEDManager::getInstance();
#endif

/* P U B L I C   F U N C T I O N S */

void misc1HzTask(void *pvParameters)
{
    UNUSED(pvParameters);

#if PRINT_HIGHWATER_MARKS
    UBaseType_t displayTaskHighWaterMark;
    UBaseType_t audioTaskHighWaterMark;
    UBaseType_t misc10HzTaskHighWaterMark;
    UBaseType_t misc100HzTaskHighWaterMark;
#endif

    while(1)
    {
#if (FEATURE_SGTL5000)
        SGTL5000_pollRegisters();
#endif

#if (FEATURE_CPU_STATS)
        vTaskGetRunTimeStats(buffer);
        printf(buffer, 40U * 6U);
#endif

#if PRINT_HIGHWATER_MARKS
        displayTaskHighWaterMark   = uxTaskGetStackHighWaterMark( displayTaskHandle );
        audioTaskHighWaterMark     = uxTaskGetStackHighWaterMark( audioTaskHandle );
        misc100HzTaskHighWaterMark = uxTaskGetStackHighWaterMark( misc100HzTaskHandle );
        misc10HzTaskHighWaterMark  = uxTaskGetStackHighWaterMark( misc10HzTaskHandle );

        // printf("Display Task watermark: %ld\n", (uint32_t)displayTaskHighWaterMark);
        // printf("Audio Task watermark: %ld\n", (uint32_t)audioTaskHighWaterMark);
        // printf("Misc 100Hz Task watermark: %ld\n", (uint32_t)misc100HzTaskHighWaterMark);
        // printf("Misc 10Hz Task watermark: %ld\n", (uint32_t)misc10HzTaskHighWaterMark);
#endif // PRINT_HIGHWATER_MARKS

        // const uint16_t cpuUsage = osGetCPUUsage();
        // sprintf(buffer, "CPU Usage: %d\n", cpuUsage);
        // printf(buffer, 40U * 6U);
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

#if (FEATURE_LED_MANAGER)
    LEDManager->update();
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
        xTaskDelayUntil(&misc100HzTaskLastWakeTime, pdMS_TO_TICKS(10U));
    }
}

// #ifdef __cplusplus
// }
// #endif

#endif  // FEATURE_MISC
