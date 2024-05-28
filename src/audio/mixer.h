#ifndef MIXER_H_
#define MIXER_H_

#include "features.h"

#if (FEATURE_MIXER)

/* I N C L U D E S */

#include <stdint.h>

#include "lpf.h"
#include "note.h"
#include "oscillator.h"
#include "wavetables.h"

#include "main.h"

// #include "button.h"
#include "sgtl5000.h"

#include "Utils.h"

/* D E F I N E S */

#define MIXER_SAMPLES_PER_BLOCK  200U
#define MIXER_NUMBER_OF_CHANNELS 2U

/* T Y P E D E F S */

/* P U B L I C   F U N C T I O N S */

void Mixer_init(void);
void Mixer_initCodec(void);
void Mixer_updateInputs(void);

void Mixer_updateVolume(float volume);

void Mixer_updateSampleBlock(uint16_t *sampleBlock, const bool firstHalf);

void Mixer_setEnable(const bool enable);
bool Mixer_isEnabled(void);
bool Mixer_isInitialized(void);

#endif  // FEATURE_MIXER
#endif  // MIXER_H_
