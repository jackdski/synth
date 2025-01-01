#include "features.h"

#include "DisplayManager.hpp"
#include "lvgl.h"

#if (FEATURE_DISPLAY)

using namespace Display;

#define NEW_SETTINGS 1

const char sequencer_str[] = "Sequencer";
const char keyboard_str[] = "Keyboard";
const char arpeggiator_str[] = "Arpeggiator";

#if (NEW_SETTINGS == 0)
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
#endif

void display_settingsSelection(void)
{
#if (NEW_SETTINGS == 0)
    /*Create a list*/
    settingsList = lv_list_create(lv_scr_act());
    lv_obj_set_size(settingsList, 180, 220);
    lv_obj_set_align(settingsList, LV_ALIGN_LEFT_MID);

    /*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(settingsList, "Screens");

    /* SCREENS */
    btn = lv_list_add_btn(settingsList, LV_SYMBOL_AUDIO, "Mode Selection");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

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
#else

    lv_obj_t * menu = lv_menu_create(lv_scr_act());
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    /*Modify the header*/
    lv_obj_t * back_btn = lv_menu_get_main_header_back_button(menu);
    lv_obj_t * back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label, "Back");

    lv_obj_t * cont;
    lv_obj_t * label;

    /*Sequencer Sub-Page*/
    lv_obj_t * sequencer_subpage = lv_menu_page_create(menu, (char*)"Sequencer");

    cont = lv_menu_cont_create(sequencer_subpage);
    label = lv_label_create(cont);
    lv_label_set_text(label, sequencer_str);
    // - Bar count
    // - step count
    // - write steps
    // - BPM

    /*Keyboard Sub-Page*/
    lv_obj_t * keyboard_subpage = lv_menu_page_create(menu, (char*)"Keyboard");

    cont = lv_menu_cont_create(keyboard_subpage);
    label = lv_label_create(cont);
    lv_label_set_text(label, keyboard_str);

    /*Create a main page*/
    lv_obj_t * main_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, sequencer_str);
    lv_menu_set_load_page_event(menu, cont, keyboard_subpage);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, keyboard_str);
    lv_menu_set_load_page_event(menu, cont, keyboard_subpage);

    lv_menu_set_page(menu, main_page);
#endif
}

#endif // FEATURE_DISPLAY
