#ifndef WAVETABLES_H_
#define WAVETABLES_H_

#include "sine_wavetable.h"
#include "saw_wavetable.h"
#include "square_wavetable.h"

namespace Wavetable
{
    enum class WavetableType
    {
        SINE_WAVETABLE,
        SAW_WAVETABLE,
        SQUARE_WAVETABLE,

        WAVETABLE_COUNT,
    };
}

#endif