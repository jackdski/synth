#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "adsr.hpp"
#include "oscillator.hpp"

namespace Audio
{

class Note
{
private:
    ADSR adsr = ADSR();

public:
    Note(void)
    {
        frequency = 0.0f;
    }

    Note(float frequency, AdsrSettings_S adsrSettings) : frequency(frequency)
    {
        adsr = ADSR(AdsrMode_E::ADSR_MODE_LINEAR, adsrSettings);
    }

    float frequency = 0.0f;
    Oscillator hfo;
    Oscillator lfo;

    void updateFrequency(float frequency);
    float update(bool noteOff);
};

}  // namespace Audio

#endif  // NOTE_HPP_
