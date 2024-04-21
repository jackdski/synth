#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#include "sine_wavetable.h"
#include "saw_wavetable.h"
#include "square_wavetable.h"

#include <stdint.h>

#define WAVETABLE_NUM_SAMPLES 2048U

namespace Wavetables
{

enum class WavetableType
{
    SINE_WAVETABLE,
    SAW_WAVETABLE,
    SQUARE_WAVETABLE,

    WAVETABLE_COUNT,
};

class Wavetable
{
private:
    uint32_t numSamples = 0U;

public:
    WavetableType type;
    const float * wavetableData = nullptr;

    uint32_t index = 0U;

    Wavetable(WavetableType type, const float * wavetableData): type(type), wavetableData(wavetableData)
    {
        numSamples = sizeof(wavetableData) / sizeof(float);
    }

    float getNextSample(void);
    uint32_t getNumberOfSamples(void);
    uint32_t getIndex(void);
};

Wavetable * getWavetable(const WavetableType type);

}



#endif // WAVETABLES_H_
