#include "tim.h"
#include "AudioManager.hpp"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }

    if (htim->Instance == TIM17)
    {
        Audio::audio_incrementBpmTick();
    }
}