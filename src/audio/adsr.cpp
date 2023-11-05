#include "adsr.hpp"

#include "Utils.h"

#include "math.h"
#include <stdint.h>

using namespace Audio;

ADSR::ADSR(AdsrMode_E AdsrMode, AdsrSettings_S AdsrSettings)
{
    mode  = AdsrMode;
    stage = AdsrStage_E::ADSR_STAGE_ATTACK;

    settings.attack          = MAX(0.0f, MIN(1.0f, AdsrSettings.attack));
    settings.decay           = MAX(0.0f, MIN(1.0f, AdsrSettings.decay));
    settings.sustain         = MAX(0.0f, MIN(1.0f, AdsrSettings.sustain));
    settings.release         = MAX(0.0f, MIN(1.0f, AdsrSettings.release));
    settings.sampleFrequency = MAX(0.0f, AdsrSettings.sampleFrequency);

    if (mode == AdsrMode_E::ADSR_MODE_ASYMPTOTIC and (settings.sampleFrequency > 0.0f))
    {
        const float tau              = 2.0e-1;  // make configurable
        const float sample_period    = (1.0f / settings.sampleFrequency);
        const float exponent         = ((-1.0f * sample_period) / tau);
        asymtoticData.asymtoticValue = exp(exponent);
    }
}

void ADSR::updateAsymptotic(const bool noteOff)
{
    switch (stage)
    {
        case AdsrStage_E::ADSR_STAGE_ATTACK:
        {
            const float newAmplitude =
                asymtoticData.asymtoticValue * amplitude + (1.0f - asymtoticData.asymtoticValue) * settings.attack;
            amplitude = MIN(newAmplitude, settings.attack);

            if (noteOff)
            {
                stage = AdsrStage_E::ADSR_STAGE_RELEASE;
            }
            else if (amplitude >= settings.attack)
            {
                stage = AdsrStage_E::ADSR_STAGE_DECAY;
            }
            else
            {
                // remain in ATTACK
            }
            break;
        }

        case AdsrStage_E::ADSR_STAGE_DECAY:
        {
            const float newAmplitude =
                asymtoticData.asymtoticValue * amplitude + (-1.0f - asymtoticData.asymtoticValue) * settings.decay;
            amplitude = MIN(newAmplitude, settings.decay);

            if (amplitude <= settings.decay)
            {
                stage = AdsrStage_E::ADSR_STAGE_SUSTAIN;
            }
            break;
        }

        case AdsrStage_E::ADSR_STAGE_SUSTAIN:
        {
            if (noteOff)
            {
                stage = AdsrStage_E::ADSR_STAGE_RELEASE;
            }
            break;
        }

        case AdsrStage_E::ADSR_STAGE_RELEASE:
        {
            const float newAmplitude =
                asymtoticData.asymtoticValue * amplitude + (-1.0f - asymtoticData.asymtoticValue) * settings.release;
            amplitude = MAX(0.0f, newAmplitude);
        }
        default:
            break;
    }
}
