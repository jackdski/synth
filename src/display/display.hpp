#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"

#include "ssd1306.hpp"

namespace Display
{

class display
{
private:
public:
    Devices::SSD1306 &ssd1306;

    display(Devices::SSD1306 &ssd1306) : ssd1306(ssd1306)
    {
    }

    bool initialized = false;

    void init(void);
    void homeScreen(void);
};

void displayControl(void *pvParameters);

}

#endif  // DISPLAY_HPP_
