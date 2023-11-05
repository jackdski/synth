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

using namespace Audio;

void Mixer::init(void)
{
    codec.init(i2c);

    for (uint32_t wt = 0U; wt < static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT); wt++)
    {
        oscillators[wt].selectWavetable(((Wavetable::WavetableType)wt));
    }
}

void Mixer::updateSampleBlock(const uint32_t numberOfSamples)
{
    for (uint32_t i = 0U; i < numberOfSamples; i++)
    {
        float sample = 0.0f;

        for (uint32_t wt = 0U; wt < static_cast<int>(Wavetable::WavetableType::WAVETABLE_COUNT); wt++)
        {
            Oscillator osc = oscillators[wt];

            if ((osc.wavetableType == hfoWavetable) || (osc.wavetableType == lfoWavetable))
            {
                sample += osc.getSample();
            }
        }
        sampleBlock[activeSampleBlock][i] = (sample / static_cast<float>(Wavetable::WavetableType::WAVETABLE_COUNT));
    }
}

void Mixer::isrCallback(void)
{
    updateSampleBlock(MIXER_SAMPLES_PER_BLOCK);
}
