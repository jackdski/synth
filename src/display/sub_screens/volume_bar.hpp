#ifndef VOLUME_BAR_HPP_
#define VOLUME_BAR_HPP_

#include "features.h"

#include "lvgl.h"

#if (FEATURE_DISPLAY)

namespace Display
{

void display_volume_bar(void);
void volume_cb(lv_event_t *e);
void volume_bar_update(void);

}

#endif // FEATURE_DISPLAY
#endif // VOLUME_BAR_HPP_