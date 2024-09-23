#ifndef AUDIO_HPP_
#define AUDIO_HPP_

/* I N C L U D E S */

#include "features.h"
#include <stdint.h>

#if FEATURE_MIXER

/* D E F I N E S */

/* T Y P E D E F S */

typedef enum {
    AUDIO_MODE_KEYBOARD,
    AUDIO_MODE_KEYBOARD_SEQUENCER,
    AUDIO_MODE_KEYBOARD_CHORDS,

    AUDIO_MODE_COUNT,
} AudioMode_E;

/* P U B L I C   F U N C T I O N S */

void audioTask(void *pvParameters);

void audio_hardwareSpecific_i2sTransmit(uint16_t *sampleBlock, uint32_t numSamples);
void audio_hardwareSpecific_i2sStop(void);

void audio_txHalfCompleteCallback(void);
void audio_txCompleteCallback(void);

#endif  // FEATURE_MIXER
#endif  // AUDIO_HPP_
