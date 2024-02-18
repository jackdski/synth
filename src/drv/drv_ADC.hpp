#ifndef DRV_ADC_HPP_
#define DRV_ADC_HPP_

#include "adc.h"
#include "main.h"

#define ADC_10_BIT_MAX_VALUE  1023U
#define ADC_12_BIT_MAX_VALUE  4095U

#define ADC_REFERENCE_VOLTAGE 3.3F

namespace Drivers
{

class drv_ADC
{
public:
    drv_ADC(ADC_HandleTypeDef *adcHandle, uint32_t adcChannel) : adc(adcHandle), channel(adcChannel)
    {
    }

    void sample(void);
    float getSampleValue(void);
    uint32_t getSampleValueRaw(void);

private:
    ADC_HandleTypeDef *adc;
    uint32_t channel;

    uint32_t adcValueRaw = 0U;
    float adcValue       = 0.0f;  // [0.0-1.0]

    void updateValue(void);
};

}

#endif  //  DRV_ADC_HPP_
