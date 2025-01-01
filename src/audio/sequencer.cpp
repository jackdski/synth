/* I N C L U D E S */

#include "sequencer.hpp"
#include "features.h"

#include "FreeRTOS.h"
#include "task.h"

#include "sequencer_hardwareSpecific.hpp"

#if FEATURE_SEQUENCER

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

SequencerTimer sequencerTickTimer;

SequencerSettings defaultSettings;
SequencerManager sequencerManager(&defaultSettings);

/* P U B L I C   F U N C T I O N S */

bool SequencerManager::setConfig(SequencerSettings * newConfig)
{
    bool ret = false;
    if (newConfig != nullptr)
    {
        config      = newConfig;
        currentBar  = 0U;
        currentStep = 0U;
        ret = true;
    }

    return ret;
}

void SequencerManager::incrementStep(void)
{
    if (config == nullptr)
    {
        return;
    }

    currentStep++;
    if (currentStep > (uint32_t)config->stepCount)
    {
        currentBar++;
        if (currentBar > config->numberBars)
        {
            currentBar = 0U;
        }
    }
}

void SequencerManager::increaseBarCount(void)
{
    if (config == nullptr)
    {
        return;
    }
    SATURATE_INC(config->numberBars, sequencer_max_bars, 1U);
}

void SequencerManager::decreaseBarCount(void)
{
    if (config == nullptr)
    {
        return;
    }
    SATURATE_DEC(config->numberBars, 1U, 1U);
}

float SequencerManager::getSample(void)
{
    float sample = 0.0f;

    if (config != nullptr)
    {
        float sampleCount = 0.0f;


        for (uint32_t voice = 0U; voice < sequencer_max_num_voices; voice++)
        {
            for (uint8_t bar = 0U; bar < config->numberBars; bar++)
            {
                for (uint32_t step = 0U; step < (uint32_t)config->stepCount; step++)
                {
                    const bool restart = config->active[voice][bar][step];
                    sample += config->voices[voice].getSample(restart);
                    sampleCount += 1.0f;
                }
            }
        }
    }

    return sample;
}

#endif  // FEATURE_SEQUENCER
