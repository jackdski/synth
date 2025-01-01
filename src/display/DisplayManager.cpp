#include "DisplayManager.hpp"

/* I N C L U D E S */

#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "lvgl.h"

#include "ST7789.hpp"

#include "button.h"
#include "sgtl5000.h"
#include "drv_encoder.h"

#include "volume_bar.hpp"

#if (FEATURE_DISPLAY)

using namespace Display;

/* D E F I N E S */

constexpr uint32_t BYTES_PER_PIXEL = (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565));
constexpr uint32_t BUFFER_SIZE = ((DISPLAY_WIDTH * DISPLAY_HEIGHT / 10U) * BYTES_PER_PIXEL);

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void display_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);

/* T Y P E D E F S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

static uint8_t displayBuffer1[BUFFER_SIZE];
static uint8_t displayBuffer2[BUFFER_SIZE];

static lv_display_t * lv_display;

static SemaphoreHandle_t lvglMutex = NULL;


/* D I S P L A Y   M A N A G E R */

DisplayManager displayManager;

void DisplayManager::Init(void)
{
    ST7789_init();
    lvglMutex = xSemaphoreCreateMutex();

    lv_display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_display_set_buffers(lv_display, displayBuffer1, displayBuffer2, sizeof(displayBuffer1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(lv_display, display_flush);
}

void DisplayManager::SetScreen(const DisplayScreen screen)
{
    lv_obj_clean(lv_scr_act());

    switch (screen)
    {
        case DisplayScreen::HOME:
        default:
            display_homeScreen();
            break;

        case DisplayScreen::SETTINGS:
            display_settingsSelection();
            break;

        case DisplayScreen::SEQUENCER:
            display_sequencer();
            break;

        case DisplayScreen::WAVEFORM:
            display_waveformScreen();
            break;
    }
}

void DisplayManager::update20Hz(void)
{
    switch (screen)
    {
        case DisplayScreen::HOME:
        case DisplayScreen::SEQUENCER:
            volume_bar_update();
            break;

        case DisplayScreen::SETTINGS:
        case DisplayScreen::WAVEFORM:
        default:
            // nothing to update
            break;
    }
}

/* P R I V A T E   F U N C T I O N S */

static void display_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    uint16_t width, height = 0U;

    width  = (area->x2 - area->x1) + 1U;
    height = (area->y2 - area->y1) + 1U;

    ST7789_drawBuffer(area->x1, area->y1, width, height, px_map, (width * height * 2U));
    lv_display_flush_ready(display);
}

static void display_update20Hz(void)
{
    displayManager.update20Hz();
}

/* P U B L I C   F U N C T I O N S */

void displayControl(void *pvParameters)
{
    displayManager.Init();

    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_theme_t *th = lv_theme_mono_init(lv_display, true, &lv_font_montserrat_14);
        lv_display_set_theme(lv_display, th);
        displayManager.SetScreen(DisplayScreen::HOME);
        xSemaphoreGive(lvglMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(50U));

    while (1)
    {
        if (ST7789_isInitialized())
        {
            display_update20Hz();

            if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
            {
                // if (displayManager.buttonPressedRisingEdgeA)
                // {
                //     displayManager.SetScreen(DisplayScreen::HOME);
                // }
                // else if (displayManager.buttonPressedRisingEdgeB)
                // {
                //     displayManager.SetScreen(DisplayScreen::SETTINGS);
                // }
                // else
                // {
                //     // maintain screen
                // }

                lv_task_handler();
                lv_timer_handler();
                xSemaphoreGive(lvglMutex);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50U));
    }
}

#endif // FEATURE_DISPLAY
