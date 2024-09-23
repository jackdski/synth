#include "display.h"
#include "features.h"
#include "lvgl.h"

#if (FEATURE_DISPLAY)

void display_homeScreen(void)
{
    /*Create an object with the new style*/
    lv_obj_t *obj = lv_label_create(lv_scr_act());
    // lv_obj_add_style(obj, &style, 0);
    lv_label_set_text(obj, "JD");
    lv_obj_center(obj);

    display_volume_bar();
}

#endif // FEATURE_DISPLAY
