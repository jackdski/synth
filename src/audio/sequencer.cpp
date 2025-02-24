/* I N C L U D E S */

#include "features.h"
#include "sequencer.hpp"

#if FEATURE_SEQUENCER

#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"

#include "sequencer_hardwareSpecific.hpp"


using namespace Audio;

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
    // update which voices from SELECTED to PLAYING
    for (uint32_t voice = 0U; voice < sequencer_max_num_voices; voice++)
    {
        if (config->stepState[voice][currentStep] == SequencerStepState::PLAYING)
        {
            config->stepState[voice][currentStep] = SequencerStepState::SELECTED;
        }
    }

    // increment step, update SELECTED voices to PLAYING
    currentStep++;
    if (currentStep > (uint32_t)config->stepCount)
    {
        currentBar++;
        if (currentBar > config->numberBars)
        {
            currentBar = 0U;
        }

        for (uint32_t voice = 0U; voice < sequencer_max_num_voices; voice++)
        {
            if (config->stepState[voice][currentStep] == SequencerStepState::SELECTED)
            {
                config->stepState[voice][currentStep] = SequencerStepState::PLAYING;
            }
        }
    }
}

void SequencerManager::increaseBarCount(void)
{
    SATURATE_INC(config->numberBars, sequencer_max_bars, 1U);
}

void SequencerManager::decreaseBarCount(void)
{
    SATURATE_DEC(config->numberBars, 1U, 1U);
}

void SequencerManager::startSequencer(void)
{
    currentStep = 0U;
    currentBar = 0U;
    state = SequencerState::ACTIVE;
    sequencerTickTimer.startTimer();
}

void SequencerManager::stopSequencer(void)
{
    state = SequencerState::INACTIVE;
    sequencerTickTimer.stopTimer();
}

float SequencerManager::getSample(void)
{
    portDISABLE_INTERRUPTS();

    float sample = 0.0f;

    if (config != nullptr)
    {
        float sample = 0.0f;
        float sampleCount = 0.0f;

        for (uint32_t voice = 0U; voice < sequencer_max_num_voices; voice++)
        {
            if (config->stepState[voice][currentStep] == SequencerStepState::PLAYING)
            {
#if (FEATURE_VOICE)
                sample += sample += config->voices[voice].getSample(false);
                sampleCount += 1.0f;
#endif
            }
        }

        if (sampleCount > 0.0f)
        {
            sample /= sampleCount;
        }
    }

    portENABLE_INTERRUPTS();

    return sample;
}

#endif  // FEATURE_SEQUENCER
