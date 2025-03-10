#ifndef LIB_TIMER_BASE_HPP_
#define LIB_TIMER_BASE_HPP_

/* I N C L U D E S */

#include "features.h"

#include "Utils.h"
#include "main.h"

#if (FEATURE_LIB_TIMER)

/* D E F I N E S */

/* T Y P E D E F S */

enum class LibTimerState
{
    IDLE,
    RUNNING,
    EXPIRED,
};

class lib_timer
{
private:
    uint32_t lengthMs;
    uint32_t startMs = 0U;
    LibTimerState state = LibTimerState::IDLE;

public:
    lib_timer(uint32_t lengthMs) : lengthMs(lengthMs)
    {
    }

    void startTimer(void);
    void stopTimer(void);

    LibTimerState updateTimer(void);
};

/* P U B L I C   F U N C T I O N S */

void lib_timer_incrementTick(void);

#endif  // FEATURE_LIB_TIMER
#endif  // LIB_TIMER_BASE_HPP_
