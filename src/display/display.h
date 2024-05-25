#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "features.h"

# if (FEATURE_DISPLAY)

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"

// #include "ssd1306.hpp"

#define DISPLAY_WIDTH   240U
#define DISPLAY_HEIGHT  135U

void displayControl(void *pvParameters);

# endif // FEATURE_DISPLAY
#endif  // DISPLAY_H_
