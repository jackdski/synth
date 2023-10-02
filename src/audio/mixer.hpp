#ifndef MIXER_HPP_
#define MIXER_HPP_

#include <stdint.h>

#include "oscillator.hpp"
#include "wavetables.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

namespace Audio
{

#define MIXER_SAMPLES_PER_BLOCK 100U

class Mixer
{
public:
    Wavetable::WavetableType hfoWavetable = Wavetable::WavetableType::SINE_WAVETABLE;
    Wavetable::WavetableType lfoWavetable = Wavetable::WavetableType::SINE_WAVETABLE;

    Oscillator oscillators[static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT)];

    I2S_HandleTypeDef *i2sPeripheral;

    float gain = 1.0F;

    uint16_t sampleBlock[MIXER_SAMPLES_PER_BLOCK];

    void init(void);
    void updateSampleBlock(const uint32_t numberOfSamples);

private:
};

void mixerControl(void *pvParameters);

}

#endif  // MIXER_HPP_
