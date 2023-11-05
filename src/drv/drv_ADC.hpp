#ifndef DRV_ADC_HPP_
#define DRV_ADC_HPP_

#include "adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

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

    uint32_t adcValueRaw = 0U;
    float adcValue       = 0.0f;  // [0.0-1.0]

    float sampleValue(void);
    float sampleValueRaw(void);

private:
    ADC_HandleTypeDef *adc;
    uint32_t channel;

    void updateValue(void);
};

}

#endif  //  DRV_ADC_HPP_
