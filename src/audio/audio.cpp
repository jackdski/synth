/* I N C L U D E S */

#include "features.h"

#include "audio.hpp"
#include "AudioManager.hpp"
#include "AudioI2SInterface.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "i2s.h"

#if FEATURE_AUDIO

using namespace Audio;

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void audioManager_i2sInterface_txHalfCompleteCallback(void);
static void audioManager_i2sInterface_txCompleteCallback(void);

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static TickType_t audioTaskLastWakeTime;

Audio_I2SInterface i2sInterface(audioManager_i2sInterface_txHalfCompleteCallback, audioManager_i2sInterface_txCompleteCallback);
AudioManager audioManager(&i2sInterface);

/* P R I V A T E   F U N C T I O N S */

static void audioManager_i2sInterface_txHalfCompleteCallback(void)
{
    audioManager.updateSampleBlock(true);
}

static void audioManager_i2sInterface_txCompleteCallback(void)
{
    audioManager.updateSampleBlock(false);
}

/* P U B L I C   F U N C T I O N S */

void audioTask(void *pvParameters)
{
    // I2S running is required to communicate with and initialize codec
    audioManager.initalizeCodec();

    audioTaskLastWakeTime = xTaskGetTickCount();
    xTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));

    while (1)
    {
        audioManager.update10Hz();

        xTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));
    }
}

void audio_incrementBpmTick(void)
{
    if (audioManager.mode == AudioMode::Sequencer)
    {
        audioManager.sequencerManager.incrementStep();
    }
}

void audio_updateSampleBlock(const bool firstHalf)
{
    audioManager.updateSampleBlock(firstHalf);
}


#endif  // FEATURE_AUDIO
