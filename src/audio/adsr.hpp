#ifndef ADSR_HPP_
#define ADSR_HPP_

#include <stdint.h>

#include "oscillator.hpp"
#include "wavetables.hpp"

#include "main.h"

#include "sgtl5000.hpp"

namespace Audio
{

typedef enum
{
    ADSR_STAGE_ATTACK,
    ADSR_STAGE_DECAY,
    ADSR_STAGE_SUSTAIN,
    ADSR_STAGE_RELEASE,
} AdsrStage_E;

typedef enum
{
    ADSR_MODE_LINEAR,
    ADSR_MODE_ASYMPTOTIC,
} AdsrMode_E;

typedef struct
{
    float attack          = 0.5f;
    float decay           = 0.5f;
    float sustain         = 0.5f;
    float release         = 0.5f;
    float sampleFrequency = 0.0f;
} AdsrSettings_S;

class ADSR
{
private:
public:
    AdsrMode_E mode   = AdsrMode_E::ADSR_MODE_LINEAR;
    AdsrStage_E stage = AdsrStage_E::ADSR_STAGE_ATTACK;
    float amplitude   = 0.0f;
    AdsrSettings_S settings;

    struct
    {
        float step = 0.00001f;
    } linearData;

    // T = sample period, tau = time constant
    // a = e^(-T/tau)
    struct
    {
        float asymtoticValue = 1.0f;
    } asymtoticData;

    ADSR(void);
    ADSR(AdsrMode_E mode, AdsrSettings_S AdsrSettings);

    float updateValue(void);
    void update(const bool noteOff);

    void setSampleFrequency(float frequency)
    {
        settings.sampleFrequency = frequency;
    }
};

}

#endif
