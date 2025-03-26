#ifndef DISPLAY_MANGER_HPP_
#define DISPLAY_MANGER_HPP_

#include "features.h"

#if (FEATURE_DISPLAY)

#include <stdint.h>
#include <stdbool.h>

#include "sgtl5000.h"
#include "ST7789.hpp"

#include "button.h"


namespace Display
{

#define DISPLAY_WIDTH  ST7789_LCD_HEIGHT
#define DISPLAY_HEIGHT ST7789_LCD_WIDTH

enum class DisplayScreen
{
    HOME,
    SETTINGS,
    WAVEFORM,
    AUDIO_SELECTION,

#if (FEATURE_KEYBOARD)
    KEYBOARD,
#endif

#if (FEATURE_SEQUENCER)
    SEQUENCER,
#endif

    COUNT,
    NONE
};

class DisplayButton
{
private:
    Button_channel_E buttonChannel;
    DisplayScreen requestedScreen = DisplayScreen::NONE;

public:
    bool pressed = false;

    void update(void)
    {
        pressed = Button_isPressed(buttonChannel);
    }

    DisplayButton(Button_channel_E buttonChannel): buttonChannel(buttonChannel)
    {
    }
};

struct DisplayManagerData_S
{
#if (FEATURE_KEYBOARD)
    DisplayScreen screen = DisplayScreen::KEYBOARD;
#else
    DisplayScreen screen = DisplayScreen::HOME;
#endif

    int32_t volume = 0;
};

void displayManager_init(void);
void displayManager_setScreen(void);
void displayManager_update20Hz(void);

}

void displayTask(void *pvParameters);

#endif // FEATURE_DISPLAY
#endif // DISPLAY_MANGER_HPP_