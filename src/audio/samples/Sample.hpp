#ifndef SAMPLE_HPP_
#define SAMPLE_HPP_

#include "features.h"

#if FEATURE_SAMPLE

#include "Utils.h"

#include <stdint.h>

namespace Samples
{

class Sample
{
public:
    float * sampleArray;

    uint32_t sampleRate = 0U;
    uint32_t length = 0U;

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

        // do not cycle around samples, only play once
        if (currentPosition <= length)
        {
            currentPosition++;
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

        // do not cycle around samples, only play once
        if (currentPosition <= length)
        {
            currentPosition++;
        }
        return ret;
    }
};

}

#endif // FEATURE_SAMPLE
#endif // SAMPLE_HPP_