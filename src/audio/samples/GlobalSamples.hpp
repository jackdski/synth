#ifndef GLOBAL_SAMPLES_HPP_
#define GLOBAL_SAMPLES_HPP_

#include "features.h"

#if FEATURE_SAMPLE

#include "Sample.hpp"

#include "hi_hat_0.hpp"
#include "hi_hat_1.hpp"
#include "hi_hat_2.hpp"
#include "kick_0.hpp"

#include "constants.h"

extern float GlobalData::hi_hat_0_wavetable[7462U];
extern float GlobalData::hi_hat_1_wavetable[5545U];
extern float GlobalData::hi_hat_2_wavetable[6615U];
extern float GlobalData::kick_0_wavetable[20023U];

namespace Samples
{

enum class SampleType
{
    HI_HAT_0,
    HI_HAT_1,
    HI_HAT_2,
    KICK_0,
};

}

Samples::Sample globalSamples_getSample(Samples::SampleType sample);


#endif // FEATURE_SAMPLE
#endif // GLOBAL_SAMPLES_HPP_