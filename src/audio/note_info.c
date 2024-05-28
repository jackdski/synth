
/* I N C L U D E S */

#include "note_info.h"

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

// clang-format off
static NoteInfo_S noteInfo[NOTE_INFO_COUNT] = {
    [NOTE_INFO_INDEX_C4] =
    {
        .frequency = 261.6f,
    },
    [NOTE_INFO_INDEX_D4] =
    {
        .frequency = 293.67f,
    },
    [NOTE_INFO_INDEX_E4] =
    {
        .frequency = 329.63f,
    },
    [NOTE_INFO_INDEX_F4] =
    {
        .frequency = 349.23f,
    },
};
// clang-format on

/* P U B L I C   F U N C T I O N S */

float NoteInfo_getFrequency(const NoteInfo_index_E index)
{
    return noteInfo[index].frequency;
}

// get direction: __HAL_TIM_IS_TIM_COUNTING_DOWN
