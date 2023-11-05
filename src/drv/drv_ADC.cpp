#include "drv_ADC.hpp"
#include "stm32f4xx.h"

#define ADC_MAX

using namespace Drivers;

void drv_ADC::updateValue(void)
{
    ADC_ChannelConfTypeDef conf = {
        .Channel      = channel,
        .Rank         = 1U,
        .SamplingTime = ADC_SAMPLETIME_28CYCLES,
        .Offset       = 0U,
    };

    if (HAL_ADC_ConfigChannel(adc, &conf) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_ADC_Start(adc);
    HAL_ADC_PollForConversion(adc, HAL_MAX_DELAY);

    adcValueRaw = HAL_ADC_GetValue(adc);
    HAL_ADC_Stop(adc);
}

float drv_ADC::sampleValue(void)
{
    updateValue();
    return (float)(adcValueRaw / ADC_12_BIT_MAX_VALUE);
}

float drv_ADC::sampleValueRaw(void)
{
    updateValue();
    return adcValueRaw;
}
