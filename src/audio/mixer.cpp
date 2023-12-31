#include "mixer.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

#include "i2c.h"
#include "sgtl5000.hpp"

#define HALF_OF_MIXER_SAMPLES_PER_BLOCK (MIXER_SAMPLES_PER_BLOCK / 2U)

// #define I2S_DATA_FORMAT_MAX_VALUE       8388608.0f  // 24bit - 2^24 / 2
#define I2S_DATA_FORMAT_MAX_VALUE       32768.0f  // 16bit

using namespace Audio;

Note _notes[MIXER_MAX_NUMBER_NOTES];

void Mixer::initCodec(void)
{
    initialized = codec.init();
}

void Mixer::updateInputs(void)
{
    codec.updateRegisters();
    volumeKnob.update();
    // volume = volumeKnob.getPosition();

    // const bool newMiddleCActive        = button.isPressed();
    // const bool middleCActiveRisingEdge = (newMiddleCActive && (middleCTestActive == false));
    // middleCTestActive ^= newMiddleCActive;

    // note.updateFrequency(440.0f);

    // if (middleCTestActive)
    // {
    //     if (middleCActiveRisingEdge)
    //     {
    //         notes[0U].updateFrequency(440.0f);
    //     }
    // }
    // else
    // {
    //     notes[0U].updateFrequency(0.0f);
    // }
}

// void Mixer::updateSampleBlock(const mixerSampleBlock_E block)
void Mixer::updateSampleBlock(const bool firstHalf)
{
    const uint32_t startIndex = (firstHalf) ? 0U : (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS / 2);
    const uint32_t endIndex   = (firstHalf) ? (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS / 2)
                                            : (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS);
    for (uint32_t i = startIndex; i < endIndex; i += MIXER_NUMBER_OF_CHANNELS)
    {
        // update all notes
        float sample       = 0.0f;
        const bool noteOff = (note.frequency == 0.0f);
        sample += note.update(noteOff);

        if (lpfActive)
        {
            sample = lpf.update(sample);
        }
        else
        {
            lpf.reset();
        }

        // TODO: divide sample by number of active notes
        const uint16_t value = static_cast<uint16_t>(static_cast<int16_t>(sample * volume * I2S_DATA_FORMAT_MAX_VALUE));
        sampleBlock[i]       = value;  // left
        sampleBlock[i + 1]   = value;  // right
    }
}
