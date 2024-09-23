#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "features.h"

#if FEATURE_AUDIO

#ifdef __cplusplus
extern "C" {
#endif
void audioTask(void *pvParameters);
#ifdef __cplusplus
}
#endif


void audio_txHalfCompleteCallback(void);
void audio_txCompleteCallback(void);

#endif  // FEATURE_AUDIO
#endif  // AUDIO_HPP_
