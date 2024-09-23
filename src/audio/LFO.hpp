#ifndef LFO_HPP_
#define LFO_HPP_

#include "osc.hpp"

namespace Audio
{

class LFO
{
private:
    // Oscillator osc = Oscillator();
public:
    LFO(float frequency);
    float getSample(void);
};

// LFO::LFO(float frequency)
// {
//     // osc.setFrequency(frequency);
//     // osc = Oscillator(frequency, 0.0F, WAVETABLE_TYPE_SINE);
// }

float LFO::getSample(void)
{
    // return osc.getSample();
    return 0.0f;
}

}

#endif // LFO_HPP_