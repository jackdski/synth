#ifndef SAMPLE_HPP_
#define SAMPLE_HPP

#include <features.h>

#include <stdint.h>

#if FEATURE_SAMPLE

class Sample
{
private:
public:
    float * sampleArray;

    uint32_t sampleRate;
    uint32_t length;

    uint32_t currentPosition = 0U;

    Sample(float * sampleArray, uint32_t sampleRate, uint32_t length) : sampleArray(sampleArray), sampleRate(sampleRate), length(length)
    {
    }

    Sample(void)
    {
        sampleArray = nullptr;
        sampleRate = 0U;
        length = 0U;
    }

    float getSample(void)
    {
        float ret = sampleArray[currentPosition];

        currentPosition++;
        if (currentPosition >= length)
        {
            currentPosition = 0U;
        }
        return ret;
    }

    float getSample(const bool restart)
    {
        if (restart)
        {
            currentPosition = 0U;
        }

        float ret = sampleArray[currentPosition];

        currentPosition++;
        if (currentPosition >= length)
        {
            currentPosition = 0U;
        }
        return ret;
    }
};

#endif // FEATURE_SAMPLE
#endif // SAMPLE_HPP