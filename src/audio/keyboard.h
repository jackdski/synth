#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "button.h"
#include "note.h"
#include "note_info.h"

typedef enum
{
    KEYBOARD_CHANNEL_0,
    KEYBOARD_CHANNEL_1,
    KEYBOARD_CHANNEL_2,
    KEYBOARD_CHANNEL_3,

    KEYBOARD_CHANNEL_COUNT,
} Keyboard_channel_E;

typedef struct
{
    Button_channel_E buttonChannel;
    Note_channel_E noteChannel;
    NoteInfo_index_E noteInfoIndex;
} Keyboard_channelConfig_S;

void Keyboard_init(void);
void Keyboard_updateInputs(void);

#endif  // KEYBOARD_H_
