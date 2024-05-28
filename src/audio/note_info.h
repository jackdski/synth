#ifndef NOTE_INFO_H_
#define NOTE_INFO_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

// https://newt.phys.unsw.edu.au/jw/notes.html
typedef struct
{
    float frequency;  // ex. 261.6 Hz
    // uint8_t midiNumber;    // ex. 60
} NoteInfo_S;

typedef enum
{
    NOTE_INFO_INDEX_C4,
    NOTE_INFO_INDEX_D4,
    NOTE_INFO_INDEX_E4,
    NOTE_INFO_INDEX_F4,

    NOTE_INFO_COUNT
} NoteInfo_index_E;

float NoteInfo_getFrequency(const NoteInfo_index_E index);

#endif  // NOTE_INFO_H_
