#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "features.h"

#if (FEATURE_DISPLAY)

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"

#include "ST7789.h"

#define DISPLAY_WIDTH  ST7789_LCD_HEIGHT
#define DISPLAY_HEIGHT ST7789_LCD_WIDTH

typedef enum
{
    DISPLAY_SCREEN_HOME,
    DISPLAY_SCREEN_SETTINGS,
    DISPLAY_SCREEN_WAVEFORM,

    DISPLAY_SCREEN_COUNT
} Display_screen_E;

void displayControl(void *pvParameters);

// SCREENS
void display_homeScreen(void);
void display_settingsSelection(void);
void display_waveformScreen(void);

// ADD ONS
void display_volume_bar(void);

#endif  // FEATURE_DISPLAY
#endif  // DISPLAY_H_
