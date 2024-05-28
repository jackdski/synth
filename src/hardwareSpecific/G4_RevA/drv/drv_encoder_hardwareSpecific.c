#include "drv_encoder.h"
#include "tim.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void drv_encoder_hardwareSpecific_init(void)
{
    HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
}

uint32_t drv_encoder_getCount(const drv_encoder_channel_E channel)
{
    uint32_t count = 0U;
    switch (channel)
    {
        case DRV_ENCODER_CHANNEL_0:
            count = TIM2->CNT;
            break;

        case DRV_ENCODER_CHANNEL_1:
            count = TIM3->CNT;
            break;

        case DRV_ENCODER_CHANNEL_COUNT:
        default:
            break;
    }
    return count;
}
