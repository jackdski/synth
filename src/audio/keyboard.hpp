#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "features.h"

#if FEATURE_KEYBOARD

#include "keyboardKey.hpp"
#include "waveforms.hpp"
#include "ChordInfo.hpp"

#include "button.h"

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "Utils.h"

namespace Audio
{

constexpr uint32_t KEYBOARD_NUM_KEYS = 16U;
constexpr uint32_t KEYBOARD_DEFAULT_STARTING_MIDI_NOTE = 60U; // C4 - middle C

enum class KeyboardScaleType
{
    MAJOR,
    MINOR,
};

enum class KeyboardMode
{
    NOTE,
    CHORD,
};


class Keyboard
{
private:
    KeyboardKey keys[KEYBOARD_NUM_KEYS] =
    {
        KeyboardKey(BUTTON_CHANNEL_1, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE),
        KeyboardKey(BUTTON_CHANNEL_2, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 1U),
        KeyboardKey(BUTTON_CHANNEL_3, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 2U, WaveformType::SAW),
        KeyboardKey(BUTTON_CHANNEL_4, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 3U, WaveformType::SAW),
        // KeyboardKey(BUTTON_CHANNEL_4, Samples::SampleType::HI_HAT_0),
        KeyboardKey(BUTTON_CHANNEL_5, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 4U, WaveformType::SAW),
        KeyboardKey(BUTTON_CHANNEL_6, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 5U, WaveformType::SQUARE),
        KeyboardKey(BUTTON_CHANNEL_7, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 6U, WaveformType::SQUARE),
        KeyboardKey(BUTTON_CHANNEL_8, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 7U, WaveformType::SQUARE),
        KeyboardKey(BUTTON_CHANNEL_9, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 8U, WaveformType::SQUARE),
        KeyboardKey(BUTTON_CHANNEL_10, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 9U),
        KeyboardKey(BUTTON_CHANNEL_11, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 10U),
        KeyboardKey(BUTTON_CHANNEL_12, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 11U),
        KeyboardKey(BUTTON_CHANNEL_13, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 12U),
        KeyboardKey(BUTTON_CHANNEL_14, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 13U),
        KeyboardKey(BUTTON_CHANNEL_15, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 14U),
        KeyboardKey(BUTTON_CHANNEL_16, KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 15U),
    };

    KeyboardMode mode = KeyboardMode::NOTE;
    uint32_t     baseNote = KEYBOARD_DEFAULT_STARTING_MIDI_NOTE;

    ChordType    chordType = ChordType::MAJOR;
    ChordData_S  chordData;

public:
    void updateInputs(void);
    float getSample(void);

    void setMode(KeyboardMode newMode);
    void setChordType(ChordType type)
    {
        chordType = type;
        chordData = chordInfo_getInfo(chordType);
    }

    uint32_t getNoteMidiNumber(void)
    {
        return baseNote;
    }
};

}

#endif  // FEATURE_KEYBOARD
#endif  // KEYBOARD_HPP_
