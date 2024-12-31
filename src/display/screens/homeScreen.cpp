#include "features.h"

#include "DisplayManager.hpp"
#include "AudioManager.hpp"
#include "lvgl.h"

#include "sgtl5000.h"

#if (FEATURE_DISPLAY)

using namespace Display;

// Defines
constexpr char sequencer_str[]   = "Sequencer";
constexpr char keyboard_str[]    = "Keyboard";
constexpr char arpeggiator_str[] = "Arpeggiator";

// Data Definitions
extern Audio::AudioManager audioManager;


static lv_obj_t *volumeBar;
static int32_t prevVolume = 0;

static lv_obj_t * modeList;

// Static Function Declarations

static void display_volume_bar(void);
static void volume_cb(lv_event_t *e);
static void mode_selection_cb(lv_event_t *e);

// Static Function Definitions

static void display_volume_bar(void)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    volumeBar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(volumeBar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(volumeBar, 20, 100);
    lv_obj_align(volumeBar, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_bar_set_range(volumeBar, 0, 100);
    lv_bar_set_value(volumeBar, 0, LV_ANIM_ON);

    lv_obj_add_event_cb(volumeBar, volume_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

static void volume_cb(lv_event_t *e)
{
    if (e != NULL)
    {
        lv_obj_t *volumeBar = lv_event_get_target(e);
        int32_t volume      = (int32_t)e->param;
        lv_bar_set_value(volumeBar, volume, LV_ANIM_OFF);
    }
}

static void mode_selection_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        const char * btn_text = lv_list_get_btn_text(modeList, obj);

        if (btn_text == sequencer_str)
        {
            audioManager.setMode(Audio::AudioMode::Sequencer);
        }
        else if (btn_text == keyboard_str)
        {
            audioManager.setMode(Audio::AudioMode::Keyboard);
        }
        else if (btn_text == arpeggiator_str)
        {
            audioManager.setMode(Audio::AudioMode::Arpeggiator);
        }
        else
        {
            // unknown type
        }
    }
}

static void mode_selection(void)
{
    modeList = lv_list_create(lv_scr_act());
    lv_obj_set_size(modeList, 180, 220);
    lv_obj_set_align(modeList, LV_ALIGN_LEFT_MID);

    /*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(modeList, "Modes");

    /* SCREENS */
    btn = lv_list_add_btn(modeList, LV_SYMBOL_AUDIO, sequencer_str);
    lv_obj_add_event_cb(btn, mode_selection_cb, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(modeList, LV_SYMBOL_AUDIO, keyboard_str);
    lv_obj_add_event_cb(btn, mode_selection_cb, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_btn(modeList, LV_SYMBOL_NEW_LINE, arpeggiator_str);
    lv_obj_add_event_cb(btn, mode_selection_cb, LV_EVENT_CLICKED, NULL);
}

void display_homeScreen(void)
{
    /*Create an object with the new style*/
    lv_obj_t *obj = lv_label_create(lv_scr_act());
    lv_label_set_text(obj, "JD");
    lv_obj_center(obj);

    /*Mode Selection*/
    mode_selection();

    display_volume_bar();
}

void display_homeScreenUpdate(void)
{
    const float volume = SGTL5000_getVolume();
    const int32_t vol  = (int32_t)(100.0f * (volume + SGTL5000_VOLUME_DB_MIN) / (SGTL5000_VOLUME_DB_MIN + 51.5f));
    if (prevVolume != vol)
    {
        lv_event_send(volumeBar, LV_EVENT_VALUE_CHANGED, (void *)vol);
    }
    prevVolume = vol;
}

#endif // FEATURE_DISPLAY
