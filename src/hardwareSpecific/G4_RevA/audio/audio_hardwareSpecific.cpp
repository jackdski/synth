#include "audio.hpp"
#include "AudioManager.hpp"
#include "audio_hardwareSpecific.hpp"

#include "i2s.h"

#if FEATURE_AUDIO

using namespace Audio;

extern I2S_HandleTypeDef hi2s2;


void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == &hi2s2)
    {
        taskENTER_CRITICAL();
        audio_txHalfCompleteCallback();
        taskEXIT_CRITICAL();
    }
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == &hi2s2)
    {
        taskENTER_CRITICAL();
        audio_txCompleteCallback();
        taskEXIT_CRITICAL();
    }
}

void Audio_I2SInterface::i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples)
{
    (void)HAL_I2S_Transmit_DMA(&hi2s2, sampleBlock, numSamples);
}

void Audio_I2SInterface::i2sStop(uint16_t *sampleBlock, uint32_t numSamples)
{
    (void)HAL_I2S_DMAStop(&hi2s2);;
}

#endif // FEATURE_AUDIO
