#include "audio.hpp"
#include "mixer.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

#include "i2c.h"
#include "i2s.h"
#include "sgtl5000.hpp"

extern Audio::Mixer mixer;

namespace Audio
{

void mixerControl(void *pvParameters)
{
    mixer.init();

    while (1)
    {
        if (mixer.enabled)
        {
            // add first block of samples to buffer
            // kick off I2S transaction
        }

        vTaskDelay(100U);
    }
}

void I2SEx_TxRxDMAHalfCplt(DMA_HandleTypeDef *hdma)
{
}

void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    if (hi2s == (I2S_HandleTypeDef *)mixer.codec.i2c)
    {
        mixer.isrCallback();
        (void)HAL_I2SEx_TransmitReceive_DMA((I2S_HandleTypeDef *)mixer.codec.i2c,
                                            mixer.sampleBlock[mixer.activeSampleBlock],
                                            NULL,
                                            MIXER_SAMPLES_PER_BLOCK);
    }

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

}
