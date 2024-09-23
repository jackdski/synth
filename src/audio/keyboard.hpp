#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "features.h"

#if FEATURE_KEYBOARD


#include "osc.hpp"
#include "button.h"

#include <cmath>

namespace Audio {

constexpr uint32_t KEYBOARD_NUM_KEYS = 16U;
constexpr uint32_t KEYBOARD_DEFAULT_STARTING_MIDI_NOTE = 60U; // C4 - middle C

class KeyboardKey
{
private:
    Button_channel_E buttonChannel = BUTTON_CHANNEL_UNDEFINED;
    Oscillator osc = Oscillator();

public:
    bool active = false;

    KeyboardKey(const uint32_t midiNumber, Button_channel_E button)
    {
        buttonChannel = button;
        assignNote(midiNumber);
    }

    void assignNote(const uint32_t midiNumber)
    {
        const float midiConversionPower = ((float)midiNumber - 69.0F) / 12.0F;
        const float oscFrequency = 440.0F * pow(2.0F, midiConversionPower);
        osc.setFrequency(oscFrequency);
    }

    void update(void)
    {
        active = Button_isPressed(buttonChannel);
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (active)
        {
            sample = osc.getSample();
        }
        return sample;
    }
};

class Keyboard
{
private:
    KeyboardKey keys[KEYBOARD_NUM_KEYS] =
    {
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE, BUTTON_CHANNEL_1),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 1U, BUTTON_CHANNEL_2),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 2U, BUTTON_CHANNEL_3),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 3U, BUTTON_CHANNEL_4),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 4U, BUTTON_CHANNEL_5),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 5U, BUTTON_CHANNEL_6),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 6U, BUTTON_CHANNEL_7),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 7U, BUTTON_CHANNEL_8),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 8U, BUTTON_CHANNEL_9),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 9U, BUTTON_CHANNEL_10),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 10U, BUTTON_CHANNEL_11),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 11U, BUTTON_CHANNEL_12),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 12U, BUTTON_CHANNEL_13),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 13U, BUTTON_CHANNEL_14),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 14U, BUTTON_CHANNEL_15),
        KeyboardKey(KEYBOARD_DEFAULT_STARTING_MIDI_NOTE + 15U, BUTTON_CHANNEL_16),
    };

public:
    void updateInputs(void);
    float getSample(void);
};

}

#endif  // FEATURE_KEYBOARD
#endif  // KEYBOARD_H_
