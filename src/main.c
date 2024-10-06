#include "main.h"
#include "features.h"

/* I N C L U D E S */
#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "task.h"

#include "hardwareSpecific.h"

#include "audio.hpp"
#include "display.h"
#include "misc.h"

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void initTask(void *pvParameters);

/* M A I N */
int main(void)
{
     hardwareSpecificInit();

    // traceSTART();

    (void)xTaskCreate(initTask, "init", configMINIMAL_STACK_SIZE, (void *)NULL, 0U, NULL);
    // initTask(NULL);
    vTaskStartScheduler();
    while (1)
    {
        // loop
    }
}

/* P R I V A T E    F U N C T I O N S */

static void initTask(void *pvParameters)
{
    // (void)xTaskCreate(misc100HzTask, "misc100Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 2U, NULL);
    (void)xTaskCreate(misc10HzTask,  "misc10Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);
    // (void)xTaskCreate(misc1HzTask,  "misc1Hz", configMINIMAL_STACK_SIZE, (void *)NULL, 3U, NULL);

    // (void)xTaskCreate(audioTask, "audioTask", configMINIMAL_STACK_SIZE, (void *)NULL, 1U, NULL);

#if (FEATURE_DISPLAY)
    // (void)xTaskCreate(displayControl, "displayControl", 512 * 2, (void *)NULL, 4U, NULL);
#endif
    // vTaskDelete(NULL);
}

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
