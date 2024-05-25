#ifndef MIXER_HPP_
#define MIXER_HPP_

#include <stdint.h>

#include "lpf.h"
#include "note.hpp"
#include "oscillator.h"
#include "wavetables.h"

#include "main.h"

#include "button.hpp"
#include "sgtl5000.hpp"

#include "Utils.h"

namespace Audio
{

#define MIXER_SAMPLES_PER_BLOCK  200U
#define MIXER_NUMBER_OF_CHANNELS 2U
#define MIXER_MAX_NUMBER_NOTES   20U

static Note mixerNotes[MIXER_MAX_NUMBER_NOTES];

class Mixer
{
public:
    friend class Oscillator;
    friend class Devices::SGTL5000;

    Devices::SGTL5000 &codec;

    I2S_HandleTypeDef *i2s;
    bool i2sActive = false;

    uint16_t sampleBlock[MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS];

    // effects
    float lpfFrequency = 1000.0f;  // connect this up to a knob
    LPF<float> lpf{lpfFrequency, (1.0f / SYNTH_SAMPLE_FREQUENCY)};
    Note *notes;

    // LFO
    Oscillator lfoOsc = Oscillator();

    void initCodec(void);
    void updateInputs(void);
    void updateSampleBlock(const bool firstHalf);

    bool isEnabled(void)
    {
        return enabled;
    }

    bool isInitialized(void)
    {
        return initialized;
    }

    Mixer(Devices::SGTL5000 &codec, I2S_HandleTypeDef *i2s) : codec(codec), i2s(i2s)
    {
        notes = mixerNotes;
        lfoOsc.setFrequency(20U);
    }

private:
    bool enabled     = true;
    bool initialized = false;

    bool middleCTestActive = false;

    float volume = 0.3F;
    float gain   = 0.5F;
};

}

#endif  // MIXER_HPP_
