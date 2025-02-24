#include "audio.hpp"

#include "tim.h"
#include "i2s.h"

extern I2S_HandleTypeDef hi2s2;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }

    if (htim->Instance == TIM17)
    {
        audio_incrementBpmTick();
    }
}

// Interrupts
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == &hi2s2)
    {
        audio_updateSampleBlock(true);
    }
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s == &hi2s2)
    {
        audio_updateSampleBlock(false);
    }
}