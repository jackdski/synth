#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "features.h"

#if FEATURE_KEYBOARD

#include "Voice.hpp"
#include "GlobalSamples.hpp"
#include "waveforms.hpp"

#include "button.h"

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

#include "Utils.h"

namespace Audio
{

constexpr uint32_t KEYBOARD_NUM_KEYS = 16U;
constexpr uint32_t KEYBOARD_DEFAULT_STARTING_MIDI_NOTE = 60U; // C4 - middle C

class KeyboardKey
{
private:
    Button_channel_E buttonChannel = BUTTON_CHANNEL_UNDEFINED;
    Voice            voice;

public:
    bool active = false;
    bool buttonRisingEdge = false;

    KeyboardKey(Button_channel_E button, const uint32_t midiNumber)
    {
        buttonChannel = button;
        assignNote(midiNumber);
    }

    KeyboardKey(Button_channel_E button, const uint32_t midiNumber, WaveformType waveformType)
    {
        buttonChannel = button;
        assignNote(midiNumber, waveformType);
    }

#if FEATURE_SAMPLE
    KeyboardKey(Button_channel_E button, Samples::SampleType sampleType)
    {
        buttonChannel = button;
        voice = Voice(sampleType);
    }
#endif

    void assignNote(const uint32_t midiNumber, const WaveformType waveformType = WaveformType::SINE)
    {
        const float oscFrequency = CONVERT_MIDI_TO_FREQUENCY(midiNumber);
        voice = Voice(waveformType, oscFrequency);
    }

#if FEATURE_SAMPLE
    void assignSample(const Samples::SampleType sample)
    {
        voice = Voice(sample);
    }
#endif

    void update(void)
    {
        const bool isPressed = Button_isPressed(buttonChannel);
        buttonRisingEdge = isPressed && (active == false);
        active = isPressed;
    }

    float getSample(void)
    {
        float sample = 0.0F;
        if (active)
        {
            sample = voice.getSample(buttonRisingEdge);
        }
        return sample;
    }
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

public:
    void updateInputs(void);
    float getSample(void);
};

}

#endif  // FEATURE_KEYBOARD
#endif  // KEYBOARD_HPP_
