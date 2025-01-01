#ifndef SEQUENCER_HPP_
#define SEQUENCER_HPP_

#include "features.h"

#include "Voice.hpp"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "Utils.h"

#if FEATURE_SEQUENCER

constexpr uint32_t sequencer_max_num_voices = 4U;
constexpr uint32_t sequencer_max_steps_per_bar = 16U;
constexpr uint32_t sequencer_max_bars = 4U;

/*
TODO:
    - save configs to flash / external storage
*/

enum class SequencerStepCount
{
    SEQUENCER_STEP_COUNT_4  = 4U,
    SEQUENCER_STEP_COUNT_8  = 8U,
    SEQUENCER_STEP_COUNT_16 = 16U,
};

enum class SequencerBpm
{
    SEQUENCER_BPM_CUSTOM    = 0U,
    SEQUENCER_BPM_84        = 84U,
    SEQUENCER_BPM_100       = 100U,
    SEQUENCER_BPM_110       = 110U,
    SEQUENCER_BPM_120       = 120U,
    SEQUENCER_BPM_140       = 140U,
    SEQUENCER_BPM_180       = 180U,
};

enum class SequencerStepState
{
    DISABLED,
    AVAILABLE,
    PRESSED,
};

typedef enum
{
    SEQUENCER_SOURCE_0,
    SEQUENCER_SOURCE_1,
    SEQUENCER_SOURCE_2,
    SEQUENCER_SOURCE_3,

    SEQUENCER_SOURCE_COUNT,
} SequencerSource_E;

class SequencerTimerBase
{
public:

    void startTimer(void);
    void stopTimer(void);
    void tick(void);

    void updateFrequency(SequencerBpm bpmSetting);
};

class SequencerSettings
{
public:
    SequencerStepCount stepCount = SequencerStepCount::SEQUENCER_STEP_COUNT_16;
    SequencerBpm       bpm       = SequencerBpm::SEQUENCER_BPM_84;
    Voice              voices[sequencer_max_num_voices];

    uint8_t numberBars = 1U;
    bool    active[sequencer_max_num_voices][sequencer_max_bars][sequencer_max_steps_per_bar]; //  = { false };
};

class SequencerManager
{
private:
    SequencerSettings * config;

    uint32_t currentStep = 0U;
    uint32_t currentBar = 0U;

public:
    SequencerManager()
    {
        config = nullptr;
    }

    SequencerManager(SequencerSettings * config): config(config) { }

    bool setConfig(SequencerSettings * newConfig);
    void incrementStep(void);
    void increaseBarCount(void);
    void decreaseBarCount(void);

    float getSample(void);

    // void setStepActive(uint32_t bar, uint32_t step, bool isActive)
    // {
    //     if ((bar < sequencer_max_bars) &&
    //         (step < sequencer_max_steps_per_bar))
    //     {
    //         config->active[bar][step] = isActive;
    //     }
    // }
};

#endif // FEATURE_SEQUENCER
#endif  // SEQUENCER_HPP_
