#include "features.h"

#include "DisplayManager.hpp"
#include "sequencer.hpp"

#include "lvgl.h"

#include "volume_bar.hpp"

#include "sgtl5000.h"

#include "drv_encoder.h"

#if (FEATURE_DISPLAY)

using namespace Display;

// Data Definitions
extern DisplayManager displayManager;
extern Audio::AudioManager audioManager;

static lv_group_t * g;
static lv_indev_t * stepCountEncoderIndev;

static lv_obj_t * window;
static lv_obj_t * backBtn;

#if 0
static lv_obj_t * stepSpinbox;
static lv_obj_t * bpmSpinbox;
static lv_obj_t * barCountSpinbox;


static void button_event_handler(lv_event_t * e)
{
    if (e != NULL)
    {
        lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
        UNUSED(obj);
    }
}

static void stepSpinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if((code == LV_EVENT_SHORT_CLICKED) ||
       (code  == LV_EVENT_LONG_PRESSED_REPEAT))
    {
        lv_spinbox_increment(stepSpinbox);
    }
}

static void stepSpinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if((code == LV_EVENT_SHORT_CLICKED) ||
       (code  == LV_EVENT_LONG_PRESSED_REPEAT))
    {
        lv_spinbox_decrement(stepSpinbox);
    }
}

static void bpmSpinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if((code == LV_EVENT_SHORT_CLICKED) ||
       (code  == LV_EVENT_LONG_PRESSED_REPEAT))
    {
        lv_spinbox_increment(bpmSpinbox);
    }
}

static void bpmSpinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if((code == LV_EVENT_SHORT_CLICKED) ||
       (code  == LV_EVENT_LONG_PRESSED_REPEAT))
    {
        lv_spinbox_decrement(bpmSpinbox);
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
#endif

void display_sequencer(void)
{
    g = lv_group_create();
    lv_group_set_default(g);

    stepCountEncoderIndev = lv_indev_create();
    lv_indev_set_type(stepCountEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(stepCountEncoderIndev, encoder_read);

    window = lv_win_create(lv_screen_active());
    lv_obj_set_size(window, lv_pct(95), lv_pct(100));
    lv_obj_set_pos(window, lv_pct(15), 0);
    lv_obj_align(window, LV_ALIGN_LEFT_MID, 0, 0);

    /* top layer showing:
     *  - playing active or paused
     */

    lv_obj_t * header = lv_win_get_header(window);
    lv_obj_set_size(header, lv_pct(95), lv_pct(25));
    lv_obj_set_pos(header, 0, 0);
    lv_obj_align(header, LV_ALIGN_TOP_LEFT, 0, 0);

    backBtn = lv_win_add_button(window, LV_SYMBOL_LEFT, lv_pct(10));
    // lv_obj_add_event_cb(backBtn, button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(backBtn, lv_pct(10), lv_pct(25));
    lv_obj_set_pos(backBtn, 0, 0);
    lv_obj_align(backBtn, LV_ALIGN_LEFT_MID, 0, 0);

    lv_win_add_title(window, "Sequencer");

#if 0
    /* tabview with spinboxes or rollers:
        - step count (using SequencerStepCount enum values)
        - bar count
        - bpm count
     */
    stepSpinbox = lv_spinbox_create(lv_screen_active());
    lv_spinbox_set_range(stepSpinbox, -1000, 25000);
    lv_spinbox_set_digit_format(stepSpinbox, 5, 2);
    lv_spinbox_step_prev(stepSpinbox);
    lv_obj_set_width(stepSpinbox, 100);
    lv_obj_center(stepSpinbox);

    int32_t h = lv_obj_get_height(stepSpinbox);

    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, stepSpinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, stepSpinbox_increment_event_cb, LV_EVENT_ALL,  NULL);

    btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, stepSpinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, stepSpinbox_decrement_event_cb, LV_EVENT_ALL, NULL);
#endif

    display_volume_bar();
}

#endif // FEATURE_DISPLAY