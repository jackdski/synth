#include "sequencer.h"

void SequencerChannel::setCycleSampleSetting(bool setting)
{
    cycleSample = setting;
}

void SequencerChannel::setWavetable(Wavetables::Wavetable *newWavetable)
{
    if (newWavetable != nullptr)
    {
        wavetable = newWavetable;
    }
}

void Sequencer::setStepCount(SequencerStepCount newStepCountSetting)
{
    stepCount = newStepCountSetting;
}

void Sequencer::setStepActive(uint32_t step, bool active)
{
    stepActive[step] = active;
}

float Sequencer::getSample(void)
{
    float sample   = 0.0f;
    float numSteps = 0.0f;

    for (auto &channels : this->steps)
    {
        const uint32_t currentWavetableIndex = step.wavetable->getIndex();
        const uint32_t numSamplesInWavetable = step.wavetable->getNumberOfSamples();

        bool applySample = cycleSample;
        applySample |= ((cycleSample == false) && (currentWavetableIndex == numSamplesInWavetable));

        if (applySample && step.active)
        {
            sample += step.wavetable->getNextSample();
        }

        numSteps += 1.0f;
    }

    return (sample / numSteps);
}
