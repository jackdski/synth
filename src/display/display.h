#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "features.h"

#if (FEATURE_DISPLAY)

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"

#include "ST7789.h"

#define DISPLAY_WIDTH  ST7789_LCD_WIDTH
#define DISPLAY_HEIGHT ST7789_LCD_HEIGHT

void displayControl(void *pvParameters);

#endif  // FEATURE_DISPLAY
#endif  // DISPLAY_H_
