#ifndef ADSR_HPP_
#define ADSR_HPP_

#include <stdint.h>

#include "oscillator.hpp"
#include "wavetables.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2s_ex.h"

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
    float attack;
    float decay;
    float sustain;
    float release;
    float sampleFrequency;
} AdsrSettings_S;

class ADSR
{
private:
public:
    AdsrMode_E mode;
    AdsrStage_E stage;
    float amplitude;
    AdsrSettings_S settings;

    struct
    {
        float step;
    } linearData;

    struct
    {
        float asymtoticValue;  // a = e^(-T/tau)   T = sample period, tau = time constant
    } asymtoticData;

    ADSR(AdsrMode_E mode, AdsrSettings_S AdsrSettings);
    ~ADSR();

    void updateLinear(void);
    void updateAsymptotic(const bool noteOff);
};

}

#endif
