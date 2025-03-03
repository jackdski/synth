#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include "features.h"

#if FEATURE_AUDIO

#include <stdbool.h>

void audioTask(void *pvParameters);

void audio_incrementBpmTick(void);
void audio_updateSampleBlock(const bool firstHalf);

#endif  // FEATURE_AUDIO
#endif  // AUDIO_HPP_
