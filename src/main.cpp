#include "main.h"
#include "features.h"

/* I N C L U D E S */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"

#include "hardwareSpecific.h"

#include "audio.hpp"
#include "DisplayManager.hpp"
// #include "display.h"
#include "misc.h"

using namespace std;

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

// static void initTask(void *pvParameters);


TaskHandle_t misc100HzTaskHandle;
TaskHandle_t misc10HzTaskHandle;
TaskHandle_t audioTaskHandle;
TaskHandle_t displayTaskHandle;

/* M A I N */
int main(void)
{
     hardwareSpecificInit();

    // traceSTART();

#if 0
    // (void)xTaskCreate(initTask, "init", configMINIMAL_STACK_SIZE, (void *)NULL, 0U, NULL);
#else
    (void)xTaskCreate(misc100HzTask, "misc100Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 2U, &misc100HzTaskHandle);
    (void)xTaskCreate(misc10HzTask,  "misc10Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, &misc10HzTaskHandle);
    // (void)xTaskCreate(misc1HzTask,  "misc1Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);

# if (FEATURE_AUDIO)
    (void)xTaskCreate(audioTask, "audioTask", configMINIMAL_STACK_SIZE, (void *)NULL, 1U, &audioTaskHandle);
# endif

# if (FEATURE_DISPLAY)
    (void)xTaskCreate(displayControl, "displayControl", 1024U, (void *)NULL, 4U, &displayTaskHandle);
# endif
#endif

    vTaskStartScheduler();
    while (1)
    {
        // loop
    }
}

/* P R I V A T E    F U N C T I O N S */

// static void initTask(void *pvParameters)
// {
//     // (void)xTaskCreate(misc100HzTask, "misc100Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 2U, NULL);
//     (void)xTaskCreate(misc10HzTask,  "misc10Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);
//     // (void)xTaskCreate(misc1HzTask,  "misc1Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);

// #if (FEATURE_AUDIO)
//     (void)xTaskCreate(audioTask, "audioTask", configMINIMAL_STACK_SIZE, (void *)NULL, 1U, NULL);
// #endif

// #if (FEATURE_DISPLAY)
//     (void)xTaskCreate(displayControl, "displayControl", 512U, (void *)NULL, 4U, NULL);
// #endif

//     vTaskDelete(NULL);
// }

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
