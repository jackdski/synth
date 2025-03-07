#include "features.h"

#include "DisplayManager.hpp"
#include "AudioManager.hpp"
#include "MIDI.hpp"
#include "lvgl.h"

#include <cstdio>

#if (FEATURE_DISPLAY && FEATURE_KEYBOARD)

using namespace Display;

/* D E F I N E S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

extern DisplayManager displayManager;
extern Audio::AudioManager audioManager;

static lv_group_t * g;
static lv_indev_t * selectionEncoderIndev;
static lv_indev_t * parameterSelectionEncoderIndev;

lv_obj_t * scaleNote;

/* P R I V A T E   F U N C T I O N S */


/* P U B L I C   F U N C T I O N S */

namespace Display
{

void display_keyboard(void)
{
    g = lv_group_create();
    lv_group_set_default(g);

    scaleNote = lv_label_create(lv_scr_act());
    const uint32_t noteMidiNumber = audioManager.keyboard.getNoteMidiNumber();
    char noteStr[3U]; // = "XX";
    Audio::MIDI::midi_getNoteName(noteMidiNumber, noteStr);
    lv_label_set_text(scaleNote, "JD");

    selectionEncoderIndev = lv_indev_create();
    lv_indev_set_type(selectionEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(selectionEncoderIndev, selection_encoder_read);

    parameterSelectionEncoderIndev = lv_indev_create();
    lv_indev_set_type(parameterSelectionEncoderIndev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(parameterSelectionEncoderIndev, parameter_encoder_read);
}

}

#endif // (FEATURE_DISPLAY)
