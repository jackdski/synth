/* I N C L U D E S */

#include "lib_timer.hpp"
#include "features.h"

#if FEATURE_LIB_TIMER

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

static uint32_t lib_timer_tickMs = 0U;

/* P U B L I C   F U N C T I O N S */

void lib_timer_incrementTick(void)
{
    lib_timer_tickMs++;
}

void lib_timer::startTimer(void)
{
    state = LibTimerState::RUNNING;
    startMs = lib_timer_tickMs;
}

void lib_timer::stopTimer(void)
{
    state = LibTimerState::IDLE;
}

LibTimerState lib_timer::updateTimer(void)
{
    switch (state)
    {
        case LibTimerState::RUNNING:
        {
            if (lib_timer_tickMs - startMs >= lengthMs)
            {
                state = LibTimerState::EXPIRED;
            }
            break;
        }

        case LibTimerState::IDLE:
        case LibTimerState::EXPIRED:
        default:
            // do nothing
            break;
    }
    return state;
}

#endif // FEATURE_LIB_TIMER
