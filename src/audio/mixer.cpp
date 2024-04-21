#include "mixer.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"

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
    codec.updateVolume(volume);
}

void Mixer::updateSampleBlock(const bool firstHalf)
{
    const uint32_t startIndex = (firstHalf) ? 0U : (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS / 2);
    const uint32_t endIndex   = (firstHalf) ? (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS / 2)
                                            : (MIXER_SAMPLES_PER_BLOCK * MIXER_NUMBER_OF_CHANNELS);
    for (uint32_t i = startIndex; i < endIndex; i += MIXER_NUMBER_OF_CHANNELS)
    {
        // update all notes
        float sample         = 0.0f;
        float numActiveNotes = 0.0f;

        for (uint32_t n = 0U; n < MIXER_MAX_NUMBER_NOTES; n++)
        {
            Note *note = &notes[n];
            if (note->getFrequency() > 0.0f)
            {
                sample += note->update();
                numActiveNotes += 1.0f;
            }
        }

        // apply affects
        if (lfoOsc.getFrequency() > 0.0f)
        {
            sample += lpf.update(sample);
            numActiveNotes += 1.0f;
        }

        // divide sample by number of active notes
        sample /= numActiveNotes;

        const uint16_t value = static_cast<uint16_t>(static_cast<int16_t>(sample * volume * I2S_DATA_FORMAT_MAX_VALUE));
        sampleBlock[i]       = value;  // left
        sampleBlock[i + 1]   = value;  // right
    }
}
