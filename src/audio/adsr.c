#include "adsr.h"

#include "Utils.h"

#include "math.h"
#include <stdint.h>


// ADSR::ADSR(AdsrMode_E AdsrMode, AdsrSettings_S AdsrSettings)
// {
//     mode  = AdsrMode;
//     stage = ADSR_STAGE_ATTACK;

//     settings.attack          = MAX(0.0f, MIN(1.0f, AdsrSettings.attack));
//     settings.decay           = MAX(0.0f, MIN(1.0f, AdsrSettings.decay));
//     settings.sustain         = MAX(0.0f, MIN(1.0f, AdsrSettings.sustain));
//     settings.release         = MAX(0.0f, MIN(1.0f, AdsrSettings.release));
//     settings.sampleFrequency = MAX(0.0f, AdsrSettings.sampleFrequency);

//     if (mode == AdsrMode_E::ADSR_MODE_ASYMPTOTIC and (settings.sampleFrequency > 0.0f))
//     {
//         const float tau              = 2.0e-1;  // make configurable
//         const float sample_period    = (1.0f / settings.sampleFrequency);
//         const float exponent         = ((-1.0f * sample_period) / tau);
//         asymtoticData.asymtoticValue = exp(exponent);
//     }
// }

float ADSR_updateValue(ADSR_S * adsr)
{
    float amplitude = adsr->amplitude;

    switch (adsr->mode)
    {
        case ADSR_MODE_LINEAR:
            if (adsr->stage == ADSR_STAGE_ATTACK)
            {
                amplitude += adsr->data.linearData.step;
                if (amplitude >= adsr->settings.attack)
                {
                    amplitude = adsr->settings.attack;
                    adsr->stage     = ADSR_STAGE_DECAY;
                }
            }
            else if (adsr->stage == ADSR_STAGE_DECAY)
            {
                amplitude -= adsr->data.linearData.step;
                if (amplitude < adsr->settings.decay)
                {
                    adsr->stage = ADSR_STAGE_SUSTAIN;
                }
            }
            else if (adsr->stage == ADSR_STAGE_SUSTAIN)
            {
                if (adsr->settings.sampleFrequency == 0.0f)
                {
                    adsr->stage = ADSR_STAGE_RELEASE;
                }
            }
            else if (adsr->stage == ADSR_STAGE_RELEASE)
            {
                amplitude = MAX(0.0f, amplitude - adsr->data.linearData.step);
            }
            else
            {
                // nothing
            }
            break;

        case ADSR_MODE_ASYMPTOTIC:
            if (adsr->stage == ADSR_STAGE_ATTACK)
            {
                amplitude =
                    adsr->data.asymptoticData.asymtoticValue * amplitude + (1.0f - adsr->data.asymptoticData.asymtoticValue) * adsr->settings.attack;
            }
            else if ((adsr->stage == ADSR_STAGE_DECAY) || (adsr->stage == ADSR_STAGE_RELEASE))
            {
                amplitude =
                    adsr->data.asymptoticData.asymtoticValue * amplitude - (1.0f - adsr->data.asymptoticData.asymtoticValue) * adsr->settings.attack;
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

void ADSR_update(ADSR_S * adsr, const bool noteOff)
{
    float newAmplitude = ADSR_updateValue(adsr);

    switch (adsr->stage)
    {
        case ADSR_STAGE_ATTACK:
        {
            newAmplitude = MIN(newAmplitude, adsr->settings.attack);

            if (noteOff)
            {
                adsr->stage = ADSR_STAGE_RELEASE;
            }
            else if (newAmplitude >= adsr->settings.attack)
            {
                adsr->stage = ADSR_STAGE_DECAY;
            }
            else
            {
                // remain in ATTACK
            }
            break;
        }

        case ADSR_STAGE_DECAY:
        {
            newAmplitude = MIN(newAmplitude, adsr->settings.decay);

            if (newAmplitude <= adsr->settings.decay)
            {
                adsr->stage = ADSR_STAGE_SUSTAIN;
            }
            break;
        }

        case ADSR_STAGE_SUSTAIN:
        {
            if (noteOff)
            {
                adsr->stage = ADSR_STAGE_RELEASE;
            }
            break;
        }

        case ADSR_STAGE_RELEASE:
        {
            newAmplitude = MAX(0.0f, newAmplitude);
            break;
        }

        default:
            break;
    }

    adsr->amplitude = newAmplitude;
}

void ADSR_setSampleFrequency(ADSR_S * adsr, float frequency)
{
    adsr->settings.sampleFrequency = frequency;
}
