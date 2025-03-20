#include "AudioI2SInterface_hardwareSpecific.hpp"
#include <stdint.h>

#include "audio.hpp"

#include "i2s.h"

#if FEATURE_AUDIO

using namespace Audio;

extern I2S_HandleTypeDef hi2s2;

void Audio_I2SInterface::i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples)
{
    (void)HAL_I2S_Transmit_DMA(&hi2s2, sampleBlock, numSamples);
}

void Audio_I2SInterface::i2sStop(uint16_t *sampleBlock, uint32_t numSamples)
{
    (void)HAL_I2S_DMAStop(&hi2s2);;
}

#endif // FEATURE_AUDIO
