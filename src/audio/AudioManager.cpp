#include "AudioManager.hpp"

#include "sgtl5000.h"
#include "LEDs.h"

#include <stdio.h>

#if FEATURE_AUDIO

using namespace Audio;

static uint16_t sampleBlock[I2S_BUFFER_SIZE];

#if (defined(DEBUG) && (DEBUG) && 0)
static bool     waveformRising = false;
static uint16_t lastSample = 0U;
#endif

void AudioManager::initalizeCodec(void)
{
    memset(sampleBlock, 0U, sizeof(sampleBlock));

    // need to transmit on I2S to wake up codec
    i2sInterface->i2sTransmit(sampleBlock, I2S_BUFFER_SIZE);
    SGTL5000_init();
}

void AudioManager::update10Hz(void)
{
#if (FEATURE_KEYBOARD)
    keyboard.updateInputs();
#endif

    SGTL5000_updateVolume(volume);
}

void AudioManager::updateSampleBlock(const bool firstHalf)
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
#if (FEATURE_KEYBOARD)
                sample = keyboard.getSample();
#endif
                break;
            }

            case AudioMode::Sequencer:
            {
                sample = sequencerManager.getSample();
                break;
            }

            case AudioMode::Arpeggiator:
            default:
            {
                // TODO
                break;
            }
        }

        // apply LFO
// #if FEATURE_OSC
//         const float lfoSample = lfo.getSample() * lfoVolume;
//         sample = (lfoSample + sample) / 2.0f;
// #endif

        // sample as a 16bit value
        const uint16_t value = static_cast<uint16_t>(static_cast<int16_t>(sample * I2S_DATA_FORMAT_MAX_VALUE));

        sampleBlock[i]       = value;  // left
        sampleBlock[i + 1U]  = value; // right

#if (defined(DEBUG) && (DEBUG) && 0)
        // TODO: if debug active, print newest sample if there was a change in direction
        if (i == startIndex)
        {
            printf("%i\n", value);
        }
        else if (i > startIndex)
        {
            if ((waveformRising) &&
                (lastSample > sample))
            {
                printf("%i\n", value);
            }
            else if ((waveformRising == false) &&
                     (lastSample < sample))
            {
                printf("%i\n", value);
            }
            else
            {
                // do noting
            }
        }
        else
        {
            // nothing
        }

        lastSample = sample;
#endif
    }
}

#endif // FEATURE_AUDIO
