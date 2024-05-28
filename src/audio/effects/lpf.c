#include "lpf.h"

void LPF_init(LPF_S *lpf, float cutoffFrequency, float samplePeriod)
{
    lpf->cutoffFrequency = cutoffFrequency;
    lpf->smoothingFactor = (samplePeriod / ((1.0f / cutoffFrequency) + samplePeriod));
    lpf->oldValue        = 0.0f;
}

float LPF_update(LPF_S *lpf, float newValue)
{
    float retValue = (((float)1.0f - lpf->smoothingFactor) * lpf->oldValue + (newValue * lpf->smoothingFactor));
    lpf->oldValue  = newValue;
    return retValue;
}

void LPF_reset(LPF_S *lpf)
{
    lpf->oldValue = (float)(0.0f);
}
