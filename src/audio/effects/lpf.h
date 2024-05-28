#ifndef LPF_H_
#define LPF_H_

#include <float.h>

typedef struct
{
    float cutoffFrequency;
    float oldValue;
    float smoothingFactor;
} LPF_S;

void LPF_init(LPF_S *lpf, float cutoffFrequency, float samplePeriod);
float LPF_update(LPF_S *lpf, float newValue);
void LPF_reset(LPF_S *lpf);

#endif  // LPF_H_
