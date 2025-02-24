#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include "features.h"

#if FEATURE_AUDIO

#include <stdbool.h>

// #ifdef __cplusplus
// extern "C" {
// #endif
void audioTask(void *pvParameters);
// #ifdef __cplusplus
// }
// #endif

void audio_incrementBpmTick(void);
void audio_updateSampleBlock(const bool firstHalf);

#endif  // FEATURE_AUDIO
#endif  // AUDIO_HPP_
