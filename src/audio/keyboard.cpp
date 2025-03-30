
/* I N C L U D E S */

#include "keyboard.hpp"

#if FEATURE_KEYBOARD

using namespace Audio;

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

void Keyboard::assignAvailableVoiceBank(const Button_channel_E button, const uint32_t midiNumber)
{
    for (auto& currentVoice : voiceBank)
    {
        if (currentVoice.active == false)
        {
            currentVoice.buttonChannel = button;
            currentVoice.active = true;

            const float oscFrequency = CONVERT_MIDI_TO_FREQUENCY(midiNumber);

            // TODO: assign waveform type
            currentVoice.voice = Voice(oscFrequency);
            break;
        }
    }
}

void Keyboard::freeVoiceBank(const Button_channel_E button)
{
    // probably a more efficient way of doing this
    for (auto& currentVoice : voiceBank)
    {
        if (currentVoice.buttonChannel == button)
        {
            currentVoice.active = false;
        }
    }
}

void Keyboard::updateInputs(void)
{
    for (uint32_t i = 0; i < KEYBOARD_NUM_KEYS; i++)
    {
        const bool _buttonPressed = Button_isPressed(buttonChannels[i]);
        buttonPressedRisingEdge[KEYBOARD_NUM_KEYS] = (_buttonPressed && (buttonPressed[i] == false));
        buttonPressedFallingEdge[KEYBOARD_NUM_KEYS] = ((_buttonPressed == false) && buttonPressed[i]);
        buttonPressed[i] = _buttonPressed;

        switch (mode)
        {
            case KeyboardMode::CHORD:
            {
                if (buttonPressedRisingEdge[i])
                {
                    const uint32_t midiNumber = (baseNote + getStepsFromBaseNote((Button_channel_E)i));
                    assignAvailableVoiceBank((Button_channel_E)i, midiNumber);
                }
                else if (buttonPressedFallingEdge[i])
                {
                    freeVoiceBank((Button_channel_E)i);
                }
                else
                {
                    // no change
                }
                break;
            }

            case KeyboardMode::NOTE:
            default:
            {
                if (buttonPressedRisingEdge[i])
                {
                    const uint32_t midiNumber = (baseNote + getStepsFromBaseNote((Button_channel_E)i));
                    assignAvailableVoiceBank((Button_channel_E)i, midiNumber);
                }
                else if (buttonPressedFallingEdge[i])
                {
                    freeVoiceBank((Button_channel_E)i);
                }
                else
                {
                    // no change
                }
                break;
            }
        }

        // if chord mode --> assign multiple voice banks to chord notes on rising edge
    }
}

float Keyboard::getSample(void)
{
    float sample = 0.0F;
    float activeSamples = 0.0F;

    switch (mode)
    {
        case KeyboardMode::CHORD:
        {
            // TODO: initialize new Oscillators for chord on rising edge of
            //       button press. Support just one button press at a time
            // for (uint32_t i = 0U; i < 3U; i++)
            // {
            // }

            break;
        }

        case KeyboardMode::NOTE:
        default:
        {
            for (auto& currentVoice : voiceBank)
            {
                if (currentVoice.active)
                {
                    sample += currentVoice.voice.getSample();
                    activeSamples += 1.0F;
                }
            }
            break;
        }
    }

    if (activeSamples > 0.0F)
    {
        sample /= activeSamples;
    }
    return sample;
}

void Keyboard::setMode(KeyboardMode newMode)
{
    mode = newMode;
}

#endif  // FEATURE_KEYBOARD
