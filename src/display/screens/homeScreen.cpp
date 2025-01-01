#include "features.h"

#include "DisplayManager.hpp"
#include "AudioManager.hpp"
#include "lvgl.h"

#include "volume_bar.hpp"

#include "sgtl5000.h"
#include "knobControls.h"
#include "drv_encoder.h"

#include <cstring>

#if (FEATURE_DISPLAY)

using namespace Display;

// Defines
constexpr char sequencer_str[]   = "Sequencer";
constexpr char keyboard_str[]    = "Keyboard";
constexpr char arpeggiator_str[] = "Arpeggiator";

constexpr uint32_t display_width  =  ST7789_LCD_HEIGHT;
constexpr uint32_t display_height = ST7789_LCD_WIDTH;

// Data Definitions
extern DisplayManager displayManager;
extern Audio::AudioManager audioManager;

static lv_group_t * g;
static lv_indev_t * encoderIndev;
static lv_obj_t * modeList;

// Static Function Declarations

static void mode_selection_cb(lv_event_t *e);
static void encoder_read(lv_indev_t * indev, lv_indev_data_t * data);

// Static Function Definitions

static void mode_selection_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        const char * btn_text = lv_list_get_button_text(modeList, obj);

        if (strcmp(btn_text, sequencer_str) == 0)
        {
            displayManager.SetScreen(DisplayScreen::SEQUENCER);
            audioManager.setMode(Audio::AudioMode::Sequencer);
        }
        else if (strcmp(btn_text, keyboard_str) == 0)
        {
            audioManager.setMode(Audio::AudioMode::Keyboard);
        }
        else if (strcmp(btn_text, arpeggiator_str)  == 0)
        {
            audioManager.setMode(Audio::AudioMode::Arpeggiator);
        }
        else
        {
            // unknown type
        }
    }
}

static void encoder_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    data->enc_diff = drv_encoder_updateAndGetDiff(DRV_ENCODER_CHANNEL_0);

    if (Button_isPressed(BUTTON_CHANNEL_A))
    {
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void mode_selection(void)
{
    g = lv_group_create();
    lv_group_set_default(g);

    encoderIndev = lv_indev_create();
    lv_indev_set_type(encoderIndev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(encoderIndev, encoder_read);

    lv_indev_set_group(encoderIndev, g);
    modeList = lv_list_create(lv_scr_act());

    lv_obj_set_size(modeList, lv_pct(95), lv_pct(99));
    lv_obj_set_align(modeList, LV_ALIGN_LEFT_MID);

    /*Add buttons to the list*/
    lv_obj_t * btn;
    lv_list_add_text(modeList, "Modes");

    /* SCREENS */
    btn = lv_list_add_button(modeList, LV_SYMBOL_AUDIO, sequencer_str);
    lv_obj_add_event_cb(btn, mode_selection_cb, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_button(modeList, LV_SYMBOL_AUDIO, keyboard_str);
    lv_obj_add_event_cb(btn, mode_selection_cb, LV_EVENT_CLICKED, NULL);

    btn = lv_list_add_button(modeList, LV_SYMBOL_AUDIO, arpeggiator_str);
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

#endif // FEATURE_DISPLAY
