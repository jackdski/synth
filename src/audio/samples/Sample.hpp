#ifndef SAMPLE_HPP_
#define SAMPLE_HPP

#include <features.h>

#include <stdint.h>


#if FEATURE_SAMPLES

class Sample
{
private:
public:
    float * sampleWavetable;

    uint32_t sampleRate;
    uint32_t length;

    uint32_t currentPosition = 0U;

    Sample(float * sampleWavetable, uint32_t sampleRate, uint32_t length) : sampleWavetable(sampleWavetable), sampleRate(sampleRate), length(length)
    {
    }

    float getSample(void)
    {
        float ret = sampleWavetable[currentPosition];
        currentPosition++;
        if (currentPosition >= length)
        {
            currentPosition = 0U;
        }
        return ret;
    }
};

#endif // FEATURE_SAMPLES
#endif // SAMPLE_HPP