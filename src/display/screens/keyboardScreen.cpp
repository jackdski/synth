#include "features.h"

#include "DisplayManager.hpp"
#include "AudioManager.hpp"
#include "MIDI.hpp"
#include "lvgl.h"

#include <cstdio>

#if (FEATURE_DISPLAY && FEATURE_KEYBOARD)

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static lv_obj_t * display_keyboardScreen_private_create_text(lv_obj_t * parent, const char * icon, const char * txt);
static lv_obj_t * display_keyboardScreen_private_create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk);
static lv_obj_t * display_keyboardScreen_private_create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max, int32_t val);

static void display_keyboardScreen_private_back_event_handler(lv_event_t * e);
static void display_keyboardScreen_private_switch_handler(lv_event_t * e);

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

extern Audio::AudioManager audioManager;

constexpr uint32_t display_width  =  ST7789_LCD_HEIGHT;
constexpr uint32_t display_height = ST7789_LCD_WIDTH;

constexpr int32_t rightScreenWidthPercentage = 33;
constexpr int32_t leftScreenWidthPercentage  = 67;


static lv_group_t * g;

static lv_indev_t * leftEncoderIndev;
static lv_indev_t * rightEncoderIndev;

// left side of screen
static lv_obj_t * menu;
static lv_obj_t * mode_selection_page;
static lv_obj_t * chord_mode_page;
static lv_obj_t * sounds_page;
static lv_obj_t * filters_page;
static lv_obj_t * effects_page;

/* P R I V A T E   F U N C T I O N S */

static lv_obj_t * display_keyboardScreen_private_create_text(lv_obj_t * parent, const char * icon, const char * txt)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if (icon)
    {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }

    if (txt)
    {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }
    return obj;
}

static lv_obj_t * display_keyboardScreen_private_create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool checked)
{
    lv_obj_t * obj = display_keyboardScreen_private_create_text(parent, icon, txt);
    lv_obj_t * sw = lv_switch_create(obj);

    lv_obj_add_state(sw, checked ? LV_STATE_CHECKED : 0);
    return obj;
}

static lv_obj_t * display_keyboardScreen_private_create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max, int32_t val)
{
    lv_obj_t * obj = display_keyboardScreen_private_create_text(parent, icon, txt);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

static void display_keyboardScreen_private_back_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target_obj(e);
    lv_obj_t * menu = (lv_obj_t *)lv_event_get_user_data(e);

    if(lv_menu_back_button_is_root(menu, obj)) {
        lv_obj_t * mbox1 = lv_msgbox_create(NULL);
        lv_msgbox_add_title(mbox1, "Hello");
        lv_msgbox_add_text(mbox1, "Root back btn click.");
        lv_msgbox_add_close_button(mbox1);
    }
}

static void display_keyboardScreen_private_switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = (lv_obj_t *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED)
    {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            lv_menu_set_page(menu, NULL);
            // lv_menu_set_sidebar_page(menu, root_page);
            lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);
        }
        else
        {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
            // lv_menu_set_page(menu, root_page);
        }
    }
}

/* P U B L I C   F U N C T I O N S */

namespace Display
{

#if 0
in top right:
    shows current base note if nothing pressed
    shows currently pressed note if one is pressed
right encoder is base note selector
left side:
    right button to select keyboard mode or setting from selection.
    Using left encoder to scroll:
        - back
        - note mode
        - chord mode
        - sound / effect selection
            - osc type
            - ADSR
            - hi / low pass filters
left button option is back to home screen
right button is select button
#endif

void display_keyboard(void)
{
    g = lv_group_create();
    lv_group_set_default(g);

    // left side of screen
    menu = lv_menu_create(lv_scr_act());

    lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
    lv_obj_add_event_cb(menu, display_keyboardScreen_private_back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /* Notes Mode Sub Page*/
    mode_selection_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(mode_selection_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(mode_selection_page);
    section = lv_menu_section_create(mode_selection_page);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Note Mode",   0, 150, 120);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Chord Mode",  0, 150, 50);

    /* Chord Mode Sub Page*/
    chord_mode_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(chord_mode_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(chord_mode_page);
    section = lv_menu_section_create(chord_mode_page);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Note Mode",   0, 150, 120);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Chord Mode",  0, 150, 50);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Sounds",      0, 150, 80);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Effects",     0, 150, 80);

    /* Sounds Mode Sub Page*/
    sounds_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sounds_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sounds_page);
    section = lv_menu_section_create(sounds_page);
    display_keyboardScreen_private_create_switch(section, LV_SYMBOL_AUDIO, "Sine",   false);
    display_keyboardScreen_private_create_switch(section, LV_SYMBOL_AUDIO, "Square", false);
    display_keyboardScreen_private_create_switch(section, LV_SYMBOL_AUDIO, "Saw",    false);

    /* Filters Sub Page*/
    filters_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(filters_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(filters_page);
    section = lv_menu_section_create(filters_page);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "LPF Hz",   0, 150, 120);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "HPF Hz",  0, 150, 50);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "",      0, 150, 80);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Effects",     0, 150, 80);

    /* Effects Sub Page*/
    effects_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(effects_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(effects_page);
    section = lv_menu_section_create(effects_page);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "LPF Hz",   0, 150, 120);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "HPF Hz",  0, 150, 50);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "",      0, 150, 80);
    display_keyboardScreen_private_create_slider(section, LV_SYMBOL_SETTINGS, "Effects",     0, 150, 80);

    /* SCREENS */
    // right side of screen
    // scaleNote = lv_label_create(lv_scr_act());
    // const uint32_t noteMidiNumber = audioManager.keyboard.getNoteMidiNumber();
    // char noteStr[3U]; // = "XX";
    // Audio::MIDI::midi_getNoteName(noteMidiNumber, noteStr);
    // lv_label_set_text(scaleNote, "JD");

    rightEncoderIndev = lv_indev_create();
    lv_indev_set_type(rightEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(rightEncoderIndev, selection_encoder_read);

    leftEncoderIndev = lv_indev_create();
    lv_indev_set_type(leftEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(leftEncoderIndev, parameter_encoder_read);
}

}

#endif // (FEATURE_DISPLAY)
