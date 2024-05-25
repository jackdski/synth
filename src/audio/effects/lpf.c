#include "lpf.h"

float LPF_update(LPF_S * lpf, float newValue)
{
    float retValue = (((float)1.0f - lpf->smoothingFactor) * lpf->oldValue + (newValue * lpf->smoothingFactor));
    lpf->oldValue   = newValue;
    return retValue;
}

void LPF_reset(LPF_S * lpf)
{
    lpf->oldValue = (float)(0.0f);
}