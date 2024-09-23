#include "adsr.h"

/* I N C L U D E S */

#include "Utils.h"

#include "math.h"
#include <stdint.h>

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static float ADSR_private_updateAmplitude(ADSR_S *adsr, const bool noteOff);

/* D A T A   D E F I N I T I O N S */

/* P R I V A T E   F U N C T I O N S */

static float ADSR_private_updateAmplitude(ADSR_S *adsr, const bool noteOff)
{
    float amplitude = adsr->amplitude;

    switch (adsr->mode)
    {
        case ADSR_MODE_LINEAR:
            if (adsr->stage == ADSR_STAGE_ATTACK)
            {
                amplitude += adsr->data.linearData.step;
                if (amplitude >= (1.0f - adsr->settings.attack))
                {
                    amplitude = (1.0f - adsr->settings.attack);
                }
            }
            else if (adsr->stage == ADSR_STAGE_DECAY)
            {
                amplitude -= adsr->data.linearData.step;
            }
            else if (adsr->stage == ADSR_STAGE_RELEASE)
            {
                // clang-format off
                amplitude = MAX(0.0f, amplitude - adsr->data.linearData.step);
                // clang-format on
            }
            else if (adsr->stage == ADSR_STAGE_SUSTAIN)
            {
                // no change
            }
            else
            {
                // nothing
            }
            break;

        case ADSR_MODE_ASYMPTOTIC:
            if (adsr->stage == ADSR_STAGE_ATTACK)
            {
                // clang-format off
                amplitude = adsr->data.asymptoticData.asymtoticValue * amplitude + (1.0f - adsr->data.asymptoticData.asymtoticValue) * adsr->settings.attack;
                // clang-format on
            }
            else if ((adsr->stage == ADSR_STAGE_DECAY) || (adsr->stage == ADSR_STAGE_RELEASE))
            {
                // clang-format off
                amplitude = adsr->data.asymptoticData.asymtoticValue * amplitude - (1.0f - adsr->data.asymptoticData.asymtoticValue) * adsr->settings.attack;
                // clang-format on
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

/* P U B L I C   F U N C T I O N S */

float ADSR_update(ADSR_S *adsr, const bool noteOff)
{
    float newAmplitude = ADSR_private_updateAmplitude(adsr, noteOff);

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
    return adsr->amplitude;
}

void ADSR_setNoteOn(ADSR_S *adsr, bool noteOn)
{
    adsr->settings.noteOn = noteOn;
}
