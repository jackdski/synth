#ifndef MIXER_HPP_
#define MIXER_HPP_

#include <stdint.h>

#include "oscillator.hpp"
#include "wavetables.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

#include "sgtl5000.hpp"

namespace Audio
{

#define MIXER_SAMPLES_PER_BLOCK 100U

class Mixer
{
public:
    friend class Oscillator;
    friend class Devices::SGTL5000;

    Oscillator oscillators[static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT)];
    Devices::SGTL5000 &codec;

    typedef enum
    {
        SAMPLE_BLOCK_A,
        SAMPLE_BLOCK_B,
        SAMPLE_BLOCK_COUNT,
    } mixerSampleBlock_E;

    I2C_HandleTypeDef *i2c;

    Wavetable::WavetableType hfoWavetable = Wavetable::WavetableType::SINE_WAVETABLE;
    Wavetable::WavetableType lfoWavetable = Wavetable::WavetableType::SINE_WAVETABLE;

    float gain                           = 1.0F;
    mixerSampleBlock_E activeSampleBlock = SAMPLE_BLOCK_A;

    uint16_t sampleBlock[SAMPLE_BLOCK_COUNT][MIXER_SAMPLES_PER_BLOCK];

    bool enabled = false;

    void init(void);
    void updateSampleBlock(const uint32_t numberOfSamples);
    void isrCallback(void);

    Mixer(Devices::SGTL5000 &codec) : codec(codec)
    {
    }

private:
};

}

#endif  // MIXER_HPP_
