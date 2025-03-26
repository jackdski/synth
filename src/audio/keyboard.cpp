
/* I N C L U D E S */

#include "keyboard.hpp"

#if FEATURE_KEYBOARD

using namespace Audio;

/* D E F I N E S */

/* T Y P E D E F S */

/* P R I V A T E   D A T A   D E F I N I T I O N S */

void Keyboard::updateInputs(void)
{
    for (uint32_t i = 0; i < KEYBOARD_NUM_KEYS; i++)
    {
        keys[i].update();
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
            for (uint32_t i = 0; i < KEYBOARD_NUM_KEYS; i++)
            {
                if (keys[i].active)
                {
                    sample += keys[i].getSample();
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
