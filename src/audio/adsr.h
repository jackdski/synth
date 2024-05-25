#ifndef ADSR_HPP_
#define ADSR_HPP_

/* I N C L U D E S */

#include <stdint.h>

#include "wavetables.h"
#include "oscillator.h"

#include "main.h"

#include "sgtl5000.h"


/* D E F I N E S */

/* T Y P E D E F S */

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
    float attack;   //         = 0.5f;
    float decay;    //         = 0.5f;
    float sustain;  //         = 0.5f;
    float release;  //         = 0.5f;
    float sampleFrequency; //  = 0.0f;
} AdsrSettings_S;


typedef struct
{
    float step; // 0.00001f;
} ADSR_linearData_S;


typedef struct
{
    // T = sample period, tau = time constant
    // a = e^(-T/tau)
    float asymtoticValue; // = 1.0f;
} ADSR_asymtoticData_S;

typedef union
{
    ADSR_linearData_S    linearData;
    ADSR_asymtoticData_S asymptoticData;
} ADSR_data_U;


typedef struct
{
    AdsrMode_E      mode;
    ADSR_data_U     data;

    AdsrStage_E     stage;
    AdsrSettings_S  settings;
    float           amplitude;
} ADSR_S;

/* P U B L I C   F U N C T I O N S */

float ADSR_updateValue(ADSR_S * adsr);
void ADSR_update(ADSR_S * adsr, const bool noteOff);
void ADSR_setSampleFrequency(ADSR_S * adsr, float frequency);

#endif
