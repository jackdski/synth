#include "audio.h"
#include "keyboard.h"
#include "mixer.h"

#include "FreeRTOS.h"
#include "task.h"

#include "sgtl5000.h"

#if FEATURE_MIXER

static uint16_t sampleBlock[MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS];

void audioTask(void *pvParameters)
{
    Note_init();
    Keyboard_init();

    // I2S running is required to communicate with and initialize codec
    memset(sampleBlock, 0U, sizeof(sampleBlock));
    audio_hardwareSpecific_i2sTransmit(sampleBlock, (MIXER_SAMPLES_PER_BLOCK * 2U));
    Mixer_init();
    audio_hardwareSpecific_i2sStop();

    while (1)
    {
        Keyboard_updateInputs();
        Mixer_updateInputs();

        if (Mixer_isEnabled())
        {
            // if (mixer.i2sActive == false)
            {
                // add first block of samples to buffer
                Mixer_updateSampleBlock(sampleBlock, true);

                // kick off fist I2S transaction
                (void)audio_hardwareSpecific_i2sTransmit(sampleBlock, MIXER_SAMPLES_PER_BLOCK * 2);
                // mixer.i2sActive = true;
            }
        }
        else
        {
            audio_hardwareSpecific_i2sStop();
        }

        vTaskDelay(100U);
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
