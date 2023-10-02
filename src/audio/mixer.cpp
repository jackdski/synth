#include "mixer.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

using namespace Audio;

#define HALF_OF_MIXER_SAMPLES_PER_BLOCK (MIXER_SAMPLES_PER_BLOCK / 2U)

static Mixer mixer;

void Mixer::init(void)
{
    for (uint32_t wt = 0U; wt < static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT); wt++)
    {
        mixer.oscillators[wt].selectWavetable(((Wavetable::WavetableType)wt));
    }
}

void Mixer::updateSampleBlock(const uint32_t numberOfSamples)
{
    for (uint32_t i = 0U; i < numberOfSamples; i++)
    {
        float sample = 0.0f;

        for (uint32_t wt = 0U; wt < static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT); wt++)
        {
            Oscillator osc = mixer.oscillators[wt];

            if ((osc.wavetableType == hfoWavetable) || (osc.wavetableType == lfoWavetable))
            {
                sample += osc.getSample();
            }
        }
        sampleBlock[i] = (sample / static_cast<float>(Wavetable::WavetableType::WAVETABLE_COUNT));
    }
}

void Audio::mixerControl(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(100U);
    }
}

void I2SEx_TxRxDMAHalfCplt(DMA_HandleTypeDef *hdma)
{
}

void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    if (hi2s == mixer.i2sPeripheral)
    {
        mixer.updateSampleBlock(HALF_OF_MIXER_SAMPLES_PER_BLOCK);
        (void)HAL_I2SEx_TransmitReceive_DMA(mixer.i2sPeripheral, mixer.sampleBlock, NULL, MIXER_SAMPLES_PER_BLOCK);
    }

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}
