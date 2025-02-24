
#include "GlobalSamples.hpp"

// namespace Samples
// {

Samples::Sample globalSamples_getSample(Samples::SampleType sample)
{
    Samples::Sample ret;
    switch (sample)
    {
        case Samples::SampleType::HI_HAT_0:
            ret = Samples::Sample(GlobalData::hi_hat_1_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::hi_hat_1_wavetable));
            break;

        // case Samples::SampleType::HI_HAT_1:
        //     ret = Samples::Sample(GlobalData::hi_hat_1_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::hi_hat_1_wavetable));
        //     break;

        // case Samples::SampleType::HI_HAT_2:
        //     ret = Samples::Sample(GlobalData::hi_hat_2_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::hi_hat_2_wavetable));
        //     break;

        // case Samples::SampleType::KICK_0:
        //     ret = Samples::Sample(GlobalData::kick_0_wavetable, SYNTH_SAMPLE_FREQUENCY, NUM_ELEMENTS_IN_ARRAY(GlobalData::kick_0_wavetable));
        //     break;

        default:
            // nothing
            break;
    }
    return ret;
}

// }
