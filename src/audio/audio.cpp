/* I N C L U D E S */

#include "audio.hpp"
#include "AudioManager.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "features.h"

#include "i2s.h"

#if FEATURE_AUDIO

using namespace Audio;

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static uint16_t sampleBlock[I2S_BUFFER_SIZE];
static TickType_t audioTaskLastWakeTime;

static AudioManager audioManager;

/* P U B L I C   F U N C T I O N S */

void audioTask(void *pvParameters)
{
    // I2S running is required to communicate with and initialize codec
    memset(sampleBlock, 0U, sizeof(sampleBlock));
    audioManager.initalizeCodec(sampleBlock);

    audioTaskLastWakeTime = xTaskGetTickCount();
    xTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));

    while (1)
    {
        audioManager.update10Hz();

        xTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));
    }
}

void audio_txHalfCompleteCallback(void)
{
    audioManager.updateSampleBlock(sampleBlock, true);
}

void audio_txCompleteCallback(void)
{
    audioManager.updateSampleBlock(sampleBlock, false);
}


#endif  // FEATURE_AUDIO
