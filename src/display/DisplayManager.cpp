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

#if (FEATURE_DISPLAY)

using namespace Display;

/* D E F I N E S */

constexpr uint32_t BUFFER_SIZE = (DISPLAY_WIDTH * DISPLAY_HEIGHT / 10U);

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

/* T Y P E D E F S */

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* D A T A   D E F I N I T I O N S */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t displayBuffer1[BUFFER_SIZE];
static lv_color_t displayBuffer2[BUFFER_SIZE];
static lv_disp_t * disp;
static lv_disp_drv_t disp_drv;

static SemaphoreHandle_t lvglMutex = NULL;


/* D I S P L A Y   M A N A G E R */

static DisplayManager displayManager;

void DisplayManager::Init(void)
{
    ST7789_init();
    lvglMutex = xSemaphoreCreateMutex();

    // lv_disp_draw_buf_init(&draw_buf, displayBuffer1, NULL, BUFFER_SIZE);
    lv_disp_draw_buf_init(&draw_buf, displayBuffer1, displayBuffer2, BUFFER_SIZE);
    lv_disp_drv_init(&disp_drv);

    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = DISPLAY_WIDTH;
    disp_drv.ver_res  = DISPLAY_HEIGHT;
    disp              = lv_disp_drv_register(&disp_drv);
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
            display_homeScreenUpdate();
            break;

        case DisplayScreen::SETTINGS:
        case DisplayScreen::WAVEFORM:
        default:
            // nothing to update
            break;
    }


    // const bool buttonPressedA = Button_isPressed(BUTTON_CHANNEL_A);
    // const bool buttonPressedB = Button_isPressed(BUTTON_CHANNEL_B);

    // displayManager.buttonPressedRisingEdgeA = ((buttonPressedA) &&
    //                                            (displayManager.buttonPressedA == false));

    // displayManager.buttonPressedRisingEdgeB = ((buttonPressedB) &&
    //                                            (displayManager.buttonPressedB == false));

    // displayManager.buttonPressedA = buttonPressedA;
    // displayManager.buttonPressedB = buttonPressedB;
}

/* P R I V A T E   F U N C T I O N S */

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t width, height = 0U;

    width  = (area->x2 - area->x1) + 1U;
    height = (area->y2 - area->y1) + 1U;

    ST7789_drawBuffer(area->x1, area->y1, width, height, (uint8_t *)color_p, (width * height * 2U));
    lv_disp_flush_ready(disp);
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
        lv_theme_t *th = lv_theme_mono_init(disp, true, &lv_font_montserrat_24);
        lv_disp_set_theme(disp, th);
        displayManager.SetScreen(DisplayScreen::HOME);
        xSemaphoreGive(lvglMutex);
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(50U));

        if (ST7789_isInitialized())
        {
            display_update20Hz();

            if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
            {
                if (displayManager.buttonPressedRisingEdgeA)
                {
                    displayManager.SetScreen(DisplayScreen::HOME);
                }
                else if (displayManager.buttonPressedRisingEdgeB)
                {
                    displayManager.SetScreen(DisplayScreen::SETTINGS);
                }
                else
                {
                    // maintain screen
                }

                lv_task_handler();
                lv_timer_handler();
                xSemaphoreGive(lvglMutex);
            }
        }

        // vTaskDelay(pdMS_TO_TICKS(50U));
    }
}

#endif // FEATURE_DISPLAY
