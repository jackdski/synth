#ifndef MIXER_HPP_
#define MIXER_HPP_

#include <stdint.h>

#include "lpf.hpp"
#include "note.hpp"
#include "oscillator.hpp"
#include "wavetables.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

#include "button.hpp"
#include "knob.hpp"
#include "sgtl5000.hpp"

#include "Utils.h"

namespace Audio
{

#define MIXER_SAMPLES_PER_BLOCK  200U
#define MIXER_NUMBER_OF_CHANNELS 2U
#define MIXER_MAX_NUMBER_NOTES   1U

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
    bool lpfActive     = true;
    float lpfFrequency = 400.0f;  // connect this up to a knob
    LPF<float> lpf{lpfFrequency, (1.0f / SYNTH_SAMPLE_FREQUENCY)};
    Note note = Note();

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

    Mixer(Devices::SGTL5000 &codec, I2S_HandleTypeDef *i2s, Devices::Knob &volumeKnob, Devices::Button &button)
        : codec(codec), i2s(i2s), volumeKnob(volumeKnob), button(button)
    {
    }

private:
    Devices::Knob &volumeKnob;
    Devices::Button &button;

    bool enabled     = true;
    bool initialized = false;

    bool middleCTestActive = false;

    float volume = 1.0F;
    float gain   = 1.0F;
};

}

#endif  // MIXER_HPP_
