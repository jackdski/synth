
/* I N C L U D E S */

#include "misc.h"

#if (FEATURE_MISC)

#include <stdbool.h>
#include <stdint.h>

#include "LEDs.h"

#include "FreeRTOS.h"
#include "task.h"

/* D E F I N E S */

/* T Y P E D E F S */

typedef struct
{
    LED_channel_E   blinkyLEDChannel;
} MiscConfig_S;

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static MiscConfig_S miscConfig =
{
    .blinkyLEDChannel = LED_CHANNEL_BLINKY,
};

void misc10HzTask(void *pvParameters)
{
    while (1)
    {
        LED_toggle(miscConfig.blinkyLEDChannel);
        vTaskDelay(100);
    }
}

#endif // FEATURE_MISC