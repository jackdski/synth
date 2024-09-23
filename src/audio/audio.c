/* I N C L U D E S */

#include "audio.h"
// #include "keyboard.hpp"
#include "mixer.h"

#include "FreeRTOS.h"
#include "task.h"

#include "sgtl5000.h"
#include "features.h"

#if FEATURE_MIXER

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static uint16_t sampleBlock[MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS];

static TickType_t audioTaskLastWakeTime;


/* P U B L I C   F U N C T I O N S */

void audioTask(void *pvParameters)
{
    Note_init();
    // Keyboard_init();

    // I2S running is required to communicate with and initialize codec
    memset(sampleBlock, 0U, sizeof(sampleBlock));
    audio_hardwareSpecific_i2sTransmit(sampleBlock, (MIXER_SAMPLES_PER_BLOCK * 2U));
    Mixer_init();
    audio_hardwareSpecific_i2sStop();

    audioTaskLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));

    while (1)
    {
        // Keyboard_updateInputs();
        Mixer_updateInputs();

        if (Mixer_isEnabled())
        {
            // add first block of samples to buffer
            Mixer_updateSampleBlock(sampleBlock, true);

            // kick off fist I2S
            (void)audio_hardwareSpecific_i2sTransmit(sampleBlock, MIXER_SAMPLES_PER_BLOCK * 2);
        }
        else
        {
            audio_hardwareSpecific_i2sStop();
        }

        vTaskDelayUntil(&audioTaskLastWakeTime, pdMS_TO_TICKS(10U));
    }
}

void audio_txHalfCompleteCallback(void)
{
    Mixer_updateSampleBlock(sampleBlock, true);
}

void audio_txCompleteCallback(void)
{
    Mixer_updateSampleBlock(sampleBlock, false);
}

#endif  // FEATURE_MIXER
