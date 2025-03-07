#ifndef KEYBOARD_KEY_HPP_
#define KEYBOARD_KEY_HPP_

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

}


#endif // FEAETURE_KEYBOARD
#endif // KEYBOARD_KEY_HPP_