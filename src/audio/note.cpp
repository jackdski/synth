#include "note.hpp"

#include "float.h"
#include "stdint.h"

using namespace Audio;

void Note::updateFrequency(float frequency)
{
    osc.setFrequency(frequency);
    adsr.setSampleFrequency(frequency);
}

float Note::update(void)
{
    float sample = osc.getSample();

    sample *= adsr.updateValue();

    return sample;
}
