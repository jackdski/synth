#ifndef SEQUENCER_HPP_
#define SEQUENCER_HPP_

#include "oscillator.h"
#include "wavetables.h"
#include "features.h"

#include <stdint.h>

#if FEATURE_SEQUENCER

constexpr uint32_t sequencer_max_num_voices = 4U;

typedef enum
{
    SEQUENCER_STEP_COUNT_4  = 4U,
    SEQUENCER_STEP_COUNT_8  = 8U,
    SEQUENCER_STEP_COUNT_16 = 16U,
} SequencerStepCount_E;

typedef enum
{
    SEQUENCER_BPM_84        = 84U,
    SEQUENCER_BPM_100       = 100U,
    SEQUENCER_BPM_110       = 110U,
    SEQUENCER_BPM_120       = 120U,
    SEQUENCER_BPM_140       = 140U,
    SEQUENCER_BPM_180       = 180U,
} SequencerBpm_E;

typedef enum
{
    SEQUENCER_SOURCE_0,
    SEQUENCER_SOURCE_1,
    SEQUENCER_SOURCE_2,
    SEQUENCER_SOURCE_3,

    SEQUENCER_SOURCE_COUNT,
} SequencerSource_E;

typedef struct
{
    bool              active[SEQUENCER_SOURCE_COUNT];
} SequencerStep_S;

typedef struct
{
    SequencerStepCount_E stepCount;
    SequencerBpm_E       bpm;
} SequencerSettings_S;


class SequencerStep
{
private:
public:
    bool active[sequencer_max_num_voices] = { false };
};

#endif // FEATURE_SEQUENCER
#endif  // SEQUENCER_HPP_
