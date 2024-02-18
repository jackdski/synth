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
    }

    Note(float frequency, AdsrSettings_S adsrSettings)
    {
        osc.setFrequency(frequency);
        adsr = ADSR(AdsrMode_E::ADSR_MODE_LINEAR, adsrSettings);
    }

    Oscillator osc;

    void updateFrequency(float frequency);
    float update(void);

    float getFrequency(void)
    {
        return osc.frequency;
    }
};

}  // namespace Audio

#endif  // NOTE_HPP_
