#include "audio.hpp"
#include "mixer.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

#include "i2c.h"
#include "i2s.h"
#include "sgtl5000.hpp"

extern Audio::Mixer mixer;

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == mixer.i2s)
    {
        mixer.updateSampleBlock(true);
    }
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == mixer.i2s)
    {
        mixer.updateSampleBlock(false);
    }
}

namespace Audio
{

void mixerControl(void *pvParameters)
{
    mixer.updateSampleBlock(true);
    (void)HAL_I2S_Transmit_DMA(mixer.i2s, mixer.sampleBlock, MIXER_SAMPLES_PER_BLOCK * 2U);
    mixer.initCodec();
    (void)HAL_I2S_DMAStop(mixer.i2s);

    while (1)
    {
        mixer.updateInputs();

        if (mixer.isEnabled() && mixer.isInitialized())
        {
            if (mixer.i2sActive == false)
            {
                // add first block of samples to buffer
                mixer.updateSampleBlock(true);

                // kick off I2S transaction
                (void)HAL_I2S_Transmit_DMA(mixer.i2s, mixer.sampleBlock, MIXER_SAMPLES_PER_BLOCK * 2);
                mixer.i2sActive = true;
            }
        }

        vTaskDelay(100U);
    }
}

}
