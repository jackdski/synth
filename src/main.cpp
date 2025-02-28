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
#include "misc.h"

using namespace std;

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

TaskHandle_t misc100HzTaskHandle;
TaskHandle_t misc10HzTaskHandle;
TaskHandle_t audioTaskHandle;
TaskHandle_t displayTaskHandle;

/* M A I N */
int main(void)
{
     hardwareSpecificInit();

    // traceSTART();

    (void)xTaskCreate(misc100HzTask, "misc100Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 2U, &misc100HzTaskHandle);
    (void)xTaskCreate(misc10HzTask,  "misc10Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, &misc10HzTaskHandle);
    (void)xTaskCreate(misc1HzTask,   "misc1Hz", configMINIMAL_STACK_SIZE * 4, (void *)NULL, 3U, NULL);

#if (FEATURE_AUDIO)
    (void)xTaskCreate(audioTask, "audioTask", configMINIMAL_STACK_SIZE, (void *)NULL, 1U, &audioTaskHandle);
#endif

#if (FEATURE_DISPLAY)
    (void)xTaskCreate(displayControl, "displayControl", 1024U, (void *)NULL, 4U, &displayTaskHandle);
#endif

    vTaskStartScheduler();
    while (1)
    {
        // loop
    }
}

/* P R I V A T E    F U N C T I O N S */

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
