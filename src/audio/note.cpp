#include "note.hpp"

#include "float.h"
#include "stdint.h"

using namespace Audio;

void Note::updateFrequency(float frequency)
{
    hfo.setFrequency(frequency);
}

float Note::update(bool noteOff)
{
    float sample = hfo.getSample();

    // add lfo if active
    // if ((lfo.frequency != 0U) && (lfo.amplitude != 0.0f))
    // {
    //     const float lfoSample = lfo.getSample();
    //     sample                = ((sample + lfoSample) / 2.0f);
    //     adsr.update(noteOff);
    // }

    return sample;
}
