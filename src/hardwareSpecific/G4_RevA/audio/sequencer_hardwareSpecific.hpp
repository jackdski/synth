#ifndef AUDIO_HARDWARE_SPECIFIC_HPP_
#define AUDIO_HARDWARE_SPECIFIC_HPP_

#include "sequencer.hpp"

#include <stdint.h>

#include "tim.h"

extern TIM_HandleTypeDef htim17;

constexpr uint32_t system_clock_frequency = 16000000U;
constexpr uint32_t clock_divider = 1600U;
constexpr uint32_t timer_clock_frequency = (system_clock_frequency / clock_divider);

constexpr uint32_t sequencer_180bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 180U);
constexpr uint32_t sequencer_140bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 140U);
constexpr uint32_t sequencer_120bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 120U);
constexpr uint32_t sequencer_110bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 110U);
constexpr uint32_t sequencer_100bpm_ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 100U);
constexpr uint32_t sequencer_84bpm_ticks  = CONVERT_BPM_TO_TICKS(timer_clock_frequency, 84U);


class SequencerTimer: public SequencerTimerBase
{
public:
    void startTimer(void)
    {
        HAL_TIM_Base_Start(&htim17);
    }

    void stopTimer(void)
    {
        HAL_TIM_Base_Stop(&htim17);
    }

    void tick(void);

    void updateFrequency(SequencerBpm bpmSetting, uint32_t customBpm)
    {
        uint32_t ticks = 0U;
        switch (bpmSetting)
        {
            case SequencerBpm::SEQUENCER_BPM_84:
                ticks = sequencer_84bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_100:
                ticks = sequencer_100bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_110:
                ticks = sequencer_110bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_120:
                ticks = sequencer_120bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_140:
                ticks = sequencer_140bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_180:
                ticks = sequencer_180bpm_ticks;
                break;
            case SequencerBpm::SEQUENCER_BPM_CUSTOM:
            default:
                ticks = CONVERT_BPM_TO_TICKS(timer_clock_frequency, customBpm);
                break;
        }

        __HAL_TIM_SET_COUNTER(&htim17, ticks);
    }
};

#endif
