#include "features.h"

#include "sequencerScreen.hpp"
// #include "sequencer.hpp"

#include "DisplayManager.hpp"
#include "AudioManager.hpp"
#include "lvgl.h"

#include "volume_bar.hpp"

#include "sgtl5000.h"

#include "drv_encoder.h"

#include <cstdio>

#if (FEATURE_DISPLAY)

using namespace Display;

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void back_button_event_handler(lv_event_t * e);
static void bpm_selection_event_handler(lv_event_t * e);
static void selection_encoder_read(lv_indev_t * indev, lv_indev_data_t * data);
static void parameter_encoder_read(lv_indev_t * indev, lv_indev_data_t * data);

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

// Data Definitions
extern DisplayManager displayManager;
extern Audio::AudioManager audioManager;

static lv_group_t * g;
static lv_indev_t * selectionEncoderIndev;
static lv_indev_t * parameterSelectionEncoderIndev;

static lv_obj_t * tileview;
static lv_obj_t * backBtn;

// static lv_obj_t * stepTile;
// static lv_obj_t * currentStepSelection = NULL;

static lv_obj_t * bpmTile;
static lv_obj_t * currentBpmSelection = NULL;

// static lv_obj_t * barCountTile;
// static lv_obj_t * currentBarCountSelection = NULL;

/* P R I V A T E   F U N C T I O N S */

static void back_button_event_handler(lv_event_t * e)
{
    if (e != NULL)
    {
        lv_event_code_t code = lv_event_get_code(e);
        if (code == LV_EVENT_CLICKED)
        {
            displayManager.SetScreen(DisplayScreen::HOME);
        }
    }
}

static void bpm_selection_event_handler(lv_event_t * e)
{
    if (e != NULL)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);

        if (code == LV_EVENT_CLICKED)
        {
            currentBpmSelection = obj;
            // const Audio::SequencerBpm * bpmValue = (Audio::SequencerBpm *)lv_event_get_user_data(e);
            // audioManager.sequencerManager.setBpm(*bpmValue);
        }
    }
}

static void selection_encoder_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    data->enc_diff = drv_encoder_updateAndGetDiff(DRV_ENCODER_CHANNEL_0);

    // if (Button_isPressed(BUTTON_CHANNEL_A))
    // {
    //     data->state = LV_INDEV_STATE_PRESSED;
    // }
    // else
    // {
    //     data->state = LV_INDEV_STATE_RELEASED;
    // }
}

static void parameter_encoder_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    data->enc_diff = drv_encoder_updateAndGetDiff(DRV_ENCODER_CHANNEL_1);

    // if (Button_isPressed(BUTTON_CHANNEL_B))
    // {
    //     data->state = LV_INDEV_STATE_PRESSED;
    // }
    // else
    // {
    //     data->state = LV_INDEV_STATE_RELEASED;
    // }
}

/* P U B L I C   F U N C T I O N S */

void Display::display_sequencer(void)
{
    g = lv_group_create();
    lv_group_set_default(g);

    selectionEncoderIndev = lv_indev_create();
    lv_indev_set_type(selectionEncoderIndev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(selectionEncoderIndev, selection_encoder_read);

    parameterSelectionEncoderIndev = lv_indev_create();
    lv_indev_set_type(parameterSelectionEncoderIndev, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(parameterSelectionEncoderIndev, parameter_encoder_read);


    tileview = lv_tileview_create(lv_screen_active());
    lv_obj_set_size(tileview, lv_pct(95), lv_pct(100));
    lv_obj_set_pos(tileview, lv_pct(15), 0);
    lv_obj_align(tileview, LV_ALIGN_LEFT_MID, 0, 0);

    /* main tile showing:
     *  - sequencer and current settings
     *  - playing active or paused
     */
    lv_obj_t * mainTile = lv_tileview_add_tile(tileview, 0, 0, LV_DIR_BOTTOM);
    lv_obj_t * mainTileLabel = lv_label_create(mainTile);
    lv_label_set_text(mainTileLabel, "Sequencer");
    lv_obj_align(mainTileLabel, LV_ALIGN_TOP_MID, 0, 0);

    backBtn = lv_win_add_button(mainTile, LV_SYMBOL_LEFT, lv_pct(10));
    lv_obj_add_event_cb(backBtn, back_button_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_set_size(backBtn, lv_pct(10), lv_pct(25));
    lv_obj_set_pos(backBtn, 0, 0);
    lv_obj_align(backBtn, LV_ALIGN_LEFT_MID, 0, 0);

    // - bpm selection
    {
        bpmTile = lv_spinbox_create(lv_screen_active());
        lv_obj_t * stepList = lv_list_create(bpmTile);
        lv_obj_set_size(stepList, lv_pct(100), lv_pct(100));

        Audio::SequencerBpm bpmValues[6U] =
        {
            Audio::SequencerBpm::SEQUENCER_BPM_84,
            Audio::SequencerBpm::SEQUENCER_BPM_100,
            Audio::SequencerBpm::SEQUENCER_BPM_110,
            Audio::SequencerBpm::SEQUENCER_BPM_120,
            Audio::SequencerBpm::SEQUENCER_BPM_140,
            Audio::SequencerBpm::SEQUENCER_BPM_180,
        };

        for (auto& bpm : bpmValues)
        {
            char bpmStr[3U];
            sprintf(bpmStr, "%ld", (uint32_t)bpm);

            lv_obj_t * bpmButton = lv_button_create(stepList);

            lv_obj_t * lab = lv_label_create(bpmButton);
            lv_label_set_text_fmt(lab, bpmStr);
            lv_obj_add_event_cb(bpmButton, bpm_selection_event_handler, LV_EVENT_CLICKED, &bpm);

            lv_list_add_button(stepList, NULL, bpmStr);

            if (audioManager.sequencerManager.getBpmSetting() == bpm)
            {
                currentBpmSelection = bpmButton;
                lv_obj_add_state(currentBpmSelection, LV_STATE_CHECKED);
            }
        }
    }

    // TODO
    // - bar count
    // - step count
}

#endif // FEATURE_DISPLAY