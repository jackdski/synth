#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include "features.h"

#include "keyboard.hpp"
#include "sequencer.hpp"
#include "osc.hpp"
#include "waveforms.hpp"

#include "AudioI2SInterface_hardwareSpecific.hpp"

#include "sgtl5000.h"

#if (FEATURE_AUDIO)

namespace Audio
{

constexpr uint32_t I2S_NUMBER_OF_CHANNELS = 2U;
constexpr uint32_t I2S_SAMPLES_PER_BLOCK = 256U;
constexpr uint32_t I2S_BUFFER_SIZE = (uint32_t)(I2S_SAMPLES_PER_BLOCK * I2S_NUMBER_OF_CHANNELS);

// #define I2S_DATA_FORMAT_MAX_VALUE       8388608.0f  // 24bit - 2^24 / 2
constexpr float I2S_DATA_FORMAT_MAX_VALUE = 32768.0f;  // 16bit - 2^16 / 2
constexpr uint32_t I2S_BUFFER_HALFWAY_INDEX = I2S_SAMPLES_PER_BLOCK;


enum class AudioMode
{
#if (FEATURE_KEYBOARD)
    Keyboard,
#endif
#if (FEATURE_SEQUENCER)
    Sequencer,
#endif
    Arpeggiator,
};

enum class AudioSettings
{
    Mode,
    BPM,
    Volume,
};

class AudioManager
{
public:
    AudioManager(Audio_I2SInterface * i2sInterface): i2sInterface(i2sInterface)
    {
    }

    Audio_I2SInterface * i2sInterface;

    AudioMode mode = AudioMode::Keyboard;

#if (FEATURE_SEQUENCER)
    SequencerManager sequencerManager;
#endif

#if FEATURE_KEYBOARD
    Keyboard         keyboard;
#endif

#if FEATURE_OSC
    Oscillator lfo = Oscillator(20.0F, 0.0F, WaveformType::SINE);
#endif

    bool active = false;
    float volume = 0.3F;

    void initalizeCodec(void);
    void setMode(const AudioMode newMode)
    {
        mode = newMode;
    }

    void updateIsActive(void);
    void update10Hz(void);
    void updateSampleBlock(const bool firstHalf);
};

}

void audio_incrementBpmTick(void);

#endif // FEATURE_AUDIO
#endif // AUDIO_MANAGER_H_
