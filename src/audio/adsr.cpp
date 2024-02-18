#include "adsr.hpp"

#include "Utils.h"

#include "math.h"
#include <stdint.h>

using namespace Audio;

ADSR::ADSR(void)
{
}

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

float ADSR::updateValue(void)
{
    switch (mode)
    {
        case ADSR_MODE_LINEAR:
            if (stage == AdsrStage_E::ADSR_STAGE_ATTACK)
            {
                amplitude += linearData.step;
                if (amplitude >= settings.attack)
                {
                    amplitude = settings.attack;
                    stage     = AdsrStage_E::ADSR_STAGE_DECAY;
                }
            }
            else if (stage == AdsrStage_E::ADSR_STAGE_DECAY)
            {
                amplitude -= linearData.step;
                if (amplitude < settings.decay)
                {
                    stage = AdsrStage_E::ADSR_STAGE_SUSTAIN;
                }
            }
            else if (stage == AdsrStage_E::ADSR_STAGE_SUSTAIN)
            {
                if (settings.sampleFrequency == 0.0f)
                {
                    stage = AdsrStage_E::ADSR_STAGE_RELEASE;
                }
            }
            else if (stage == AdsrStage_E::ADSR_STAGE_RELEASE)
            {
                amplitude = MAX(0.0f, amplitude - linearData.step);
            }
            else
            {
                // nothing
            }
            break;

        case ADSR_MODE_ASYMPTOTIC:
            if (stage == AdsrStage_E::ADSR_STAGE_ATTACK)
            {
                amplitude =
                    asymtoticData.asymtoticValue * amplitude + (1.0f - asymtoticData.asymtoticValue) * settings.attack;
            }
            else if ((stage == AdsrStage_E::ADSR_STAGE_DECAY) || (stage == AdsrStage_E::ADSR_STAGE_RELEASE))
            {
                amplitude =
                    asymtoticData.asymtoticValue * amplitude - (1.0f - asymtoticData.asymtoticValue) * settings.attack;
            }
            else
            {
                // nothing
            }
            break;

        default:
            // nothing
            break;
    }

    return amplitude;
}

void ADSR::update(const bool noteOff)
{
    const float newAmplitude = updateValue();

    switch (stage)
    {
        case AdsrStage_E::ADSR_STAGE_ATTACK:
        {
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
            amplitude = MAX(0.0f, newAmplitude);
            break;
        }

        default:
            break;
    }
}
