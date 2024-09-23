#include "display.h"
#include "features.h"
#include "lvgl.h"

#if (FEATURE_DISPLAY)

static lv_obj_t * settingsList;

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_UNUSED(obj);
    }
}
void display_settingsSelection(void)
{
    /*Create a list*/
    settingsList = lv_list_create(lv_scr_act());
    lv_obj_set_size(settingsList, 180, 220);
    lv_obj_set_align(settingsList, LV_ALIGN_LEFT_MID);

    /*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(settingsList, "Screens");

    /* SCREENS */
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_AUDIO, "Waveform");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(settingsList, LV_SYMBOL_NEW_LINE, "Back");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    /* SETTINGS */
    lv_list_add_text(settingsList, "Settings");
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_VOLUME_MID, "Volume");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_SETTINGS, "ADSR");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(settingsList, "Exit");
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_OK, "Apply");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
}

#endif // FEATURE_DISPLAY
