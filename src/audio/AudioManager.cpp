#include "AudioManager.hpp"

#include "sgtl5000.h"
#include "audio.hpp"
#include "LEDs.h"

#include <stdio.h>

#include "hardwareSpecific.h"

#if FEATURE_AUDIO

using namespace Audio;

AudioManager audioManager;

void AudioManager::initalizeCodec(uint16_t * sampleBlock)
{
    // memset(sampleBlock, 0U, sizeof(sampleBlock));

    // need to transmit on I2S to wake up codec
    i2sInterface.i2sTransmit(sampleBlock, I2S_BUFFER_SIZE);
    SGTL5000_init();
}

void AudioManager::update10Hz(void)
{
    keyboard.updateInputs();
    SGTL5000_updateVolume(volume);
}

void AudioManager::updateSampleBlock(uint16_t *sampleBlock, const bool firstHalf)
{
    const uint32_t startIndex = (firstHalf) ? 0U : (I2S_BUFFER_HALFWAY_INDEX);
    const uint32_t endIndex   = (firstHalf) ? (I2S_BUFFER_HALFWAY_INDEX) : (I2S_BUFFER_SIZE);

    for (uint32_t i = startIndex; i < endIndex; i += I2S_NUMBER_OF_CHANNELS)
    {
        // update all notes
        float sample = 0.0F;

        switch (mode)
        {
            case AudioMode::Keyboard:
            {
                sample = keyboard.getSample();
                break;
            }

            case AudioMode::Sequencer:
            case AudioMode::Arpeggiator:
            default:
            {
                // TODO
                break;
            }
        }

        // apply LFO
        const float lfoSample = lfo.getSample() * lfoVolume;
        sample = (lfoSample + sample) / 2.0f;

        // sample as a 16bit value
        // uint16_t value = (uint16_t)((int16_t)(sample * I2S_DATA_FORMAT_MAX_VALUE));
        uint16_t value = static_cast<uint16_t>(static_cast<int16_t>(sample * I2S_DATA_FORMAT_MAX_VALUE));
        // value *= volume;

        sampleBlock[i]       = value;  // left
        sampleBlock[i + 1U]  = value; // right
    }
}

#endif // FEATURE_AUDIO
