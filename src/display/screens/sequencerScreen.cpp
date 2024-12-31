#include "features.h"

#include "DisplayManager.hpp"
#include "lvgl.h"

#if (FEATURE_DISPLAY)

using namespace Display;

void display_sequencer(void)
{
    /* top layer showing:
     *  - playing active or paused
     */

    /* tabview with spinboxes (https://docs.lvgl.io/8.3/widgets/extra/spinbox.html):
        - step count (using SequencerStepCount enum values)
        - bar count
     */
}

#endif // FEATURE_DISPLAY