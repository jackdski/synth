#ifndef DISPLAY_MANGER_HPP_
#define DISPLAY_MANGER_HPP_

#include <stdint.h>
#include <stdbool.h>

#include "sgtl5000.h"
#include "ST7789.hpp"

#include "button.h"

#if (FEATURE_DISPLAY)

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

class DisplayManager
{
private:
    DisplayScreen screen = DisplayScreen::HOME;

    int32_t volume = 0;

public:
    DisplayManager()
    {
    }

    bool buttonPressedA = false;
    bool buttonPressedB = false;
    bool buttonPressedRisingEdgeA = false;
    bool buttonPressedRisingEdgeB = false;

    void Init(void);
    void update20Hz(void);

    void SetScreen(const DisplayScreen screen);
};

}

void displayControl(void *pvParameters);

// SCREENS
void display_homeScreen(void);
void display_homeScreenUpdate(void);

void display_settingsSelection(void);
void display_waveformScreen(void);

#endif // FEATURE_DISPLAY
#endif // DISPLAY_MANGER_HPP_