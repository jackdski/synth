#ifndef SEQUENCER_HPP_
#define SEQUENCER_HPP_

#include "oscillator.h"
#include "wavetables.h"

#include <stdint.h>

typedef enum
{
    SEQUENCER_STEP_COUNT_4  = 4U,
    SEQUENCER_STEP_COUNT_8  = 8U,
    SEQUENCER_STEP_COUNT_16 = 16U,
} SequencerStepCount_E;

class SequencerChannel
{
public:
    bool cycleSample                 = true;
    Wavetables::Wavetable *wavetable = nullptr;

    void setCycleSampleSetting(bool setting);
    void setWavetable(Wavetables::Wavetable *newWavetable);
};

class SequencerStep
{
private:
public:
    bool active = false;
};

class Sequencer
{
public:
    void setStepCount(SequencerStepCount newStepCountSetting);
    void setStepActive(uint32_t step, bool active);

    float getSample(void);

private:
    static constexpr uint32_t maxSteps = (uint32_t)(SequencerStepCount::SEQUENCER_STEP_COUNT_16);

    // settings
    SequencerStepCount stepCount = SequencerStepCount::SEQUENCER_STEP_COUNT_16;

    static constexpr uint32_t maxConcurrentChannelsPerStep = 2U;

    bool stepActive[maxSteps] = {false};
    SequencerChannel channels[maxConcurrentChannelsPerStep];
};
}

#endif  // SEQUENCER_HPP_
