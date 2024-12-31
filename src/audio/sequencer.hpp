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


constexpr uint32_t system_clock_frequency = 16000000U;
constexpr uint32_t clock_divider = 1600U;
constexpr uint32_t timer_clock_frequency = (system_clock_frequency / clock_divider);

constexpr uint32_t sequencer_180bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 180U);
constexpr uint32_t sequencer_140bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 140U);
constexpr uint32_t sequencer_120bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 120U);
constexpr uint32_t sequencer_110bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 110U);
constexpr uint32_t sequencer_100bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 100U);
constexpr uint32_t sequencer_84bpm_ticks  = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 84U);

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

typedef enum
{
    SEQUENCER_SOURCE_0,
    SEQUENCER_SOURCE_1,
    SEQUENCER_SOURCE_2,
    SEQUENCER_SOURCE_3,

    SEQUENCER_SOURCE_COUNT,
} SequencerSource_E;

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
