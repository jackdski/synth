#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <stdint.h>

void audioTask(void *pvParameters);

void audio_hardwareSpecific_i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples);
void audio_hardwareSpecific_i2sStop(void);

void audio_txHalfCompleteCallback(void);
void audio_txCompleteCallback(void);

#endif  // AUDIO_HPP_
