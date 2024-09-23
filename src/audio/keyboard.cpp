
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

    for (uint32_t i = 0; i < KEYBOARD_NUM_KEYS; i++)
    {
        if (keys[i].active)
        {
            sample += keys[i].getSample();
            activeSamples += 1.0F;
        }
    }

    if (activeSamples > 0.0F)
    {
        sample /= activeSamples;
    }
    return sample;
}

#endif  // FEATURE_KEYBOARD
