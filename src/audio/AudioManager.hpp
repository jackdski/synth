#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include "keyboard.hpp"
#include "sgtl5000.h"

#include "audio_hardwareSpecific.hpp"

namespace Audio
{

constexpr uint32_t I2S_NUMBER_OF_CHANNELS = 2U;
constexpr uint32_t I2S_SAMPLES_PER_BLOCK = 256U;
constexpr uint32_t I2S_BUFFER_SIZE = (uint32_t)(I2S_SAMPLES_PER_BLOCK * I2S_NUMBER_OF_CHANNELS);

// #define I2S_DATA_FORMAT_MAX_VALUE       8388608.0f  // 24bit - 2^24 / 2
constexpr float I2S_DATA_FORMAT_MAX_VALUE = 32768.0f;  // 16bit
constexpr uint32_t I2S_BUFFER_HALFWAY_INDEX = I2S_SAMPLES_PER_BLOCK;


enum class AudioMode
{
    Keyboard,
    Sequencer,
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
private:
    /* data */
public:
    AudioMode mode = AudioMode::Keyboard;

    Keyboard keyboard;

    Oscillator lfo = Oscillator(20.0F, 0.0F, WAVETABLE_TYPE_SINE);

    Audio_I2SInterface i2sInterface;

    bool active = false;
    float volume = 0.3F;

    void initalizeCodec(uint16_t * sampleBlock);
    void updateIsActive(void);

    void update10Hz(void);

    void updateSampleBlock(uint16_t *sampleBlock, const bool firstHalf);
};

}


#endif // AUDIO_MANAGER_H_