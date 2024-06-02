
/* I N C L U D E S */

#include "keyboard.h"

#if FEATURE_KEYBOARD

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

// clang-format off
static Keyboard_channelConfig_S channelConfig[KEYBOARD_CHANNEL_COUNT] =
{
    [KEYBOARD_CHANNEL_0] =
    {
        .buttonChannel  = BUTTON_CHANNEL_1,
        .noteChannel    = NOTE_CHANNEL_0,
        .noteInfoIndex  = NOTE_INFO_INDEX_C4,
    },
    [KEYBOARD_CHANNEL_1] =
    {
        .buttonChannel  = BUTTON_CHANNEL_2,
        .noteChannel    = NOTE_CHANNEL_1,
        .noteInfoIndex  = NOTE_INFO_INDEX_D4,
    },
    [KEYBOARD_CHANNEL_2] =
    {
        .buttonChannel  = BUTTON_CHANNEL_3,
        .noteChannel    = NOTE_CHANNEL_2,
        .noteInfoIndex  = NOTE_INFO_INDEX_E4,
    },
    [KEYBOARD_CHANNEL_3] =
    {
        .buttonChannel  = BUTTON_CHANNEL_4,
        .noteChannel    = NOTE_CHANNEL_3,
        .noteInfoIndex  = NOTE_INFO_INDEX_F4,
    },
};
// clang-format on

/* P U B L I C   F U N C T I O N S */

void Keyboard_init(void)
{
    for (Keyboard_channel_E channel = (Keyboard_channel_E)0U; channel < KEYBOARD_CHANNEL_COUNT; channel++)
    {
        Note_init();

        const float freq = NoteInfo_getFrequency(channelConfig[channel].noteInfoIndex);
        Note_setFrequency(channelConfig[channel].noteChannel, freq);
    }
}

void Keyboard_updateInputs(void)
{
    for (Keyboard_channel_E channel = (Keyboard_channel_E)0U; channel < KEYBOARD_CHANNEL_COUNT; channel++)
    {
        const bool buttonActive = Button_isPressed(channelConfig[channel].buttonChannel);
        Note_setActive(channelConfig[channel].noteChannel, buttonActive);
    }
}

#endif  // FEATURE_KEYBOARD
