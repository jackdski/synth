#include "display.h"

#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "lvgl.h"

#include "ST7789.h"

#if (FEATURE_DISPLAY)

#define BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT / 10)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t displayBuffer1[BUFFER_SIZE]; /*Declare a buffer for 1/10 screen size*/
static lv_color_t displayBuffer2[BUFFER_SIZE];
static lv_disp_t *disp;
static lv_disp_drv_t disp_drv;

SemaphoreHandle_t xGuiSemaphore = NULL;

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t width, height = 0U;

    width  = (area->x2 - area->x1) + 1U;
    height = (area->y2 - area->y1) + 1U;

    ST7789_drawBuffer(area->x1, area->y1, width, height, (uint8_t *)color_p, (width * height * 2U));
    lv_disp_flush_ready(disp);
}

void display_init(void)
{
    ST7789_init();
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_disp_draw_buf_init(&draw_buf, displayBuffer1, displayBuffer2, BUFFER_SIZE);
    lv_disp_drv_init(&disp_drv);

    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res  = DISPLAY_WIDTH;
    disp_drv.ver_res  = DISPLAY_HEIGHT;
    disp              = lv_disp_drv_register(&disp_drv);
}

void display_homeScreen(void)
{
    // lv_obj_t *label1 = lv_label_create(lv_scr_act());
    // // lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    // lv_label_set_text(label1, "Home Screen");
    // // lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    // // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    // // lv_obj_align(label1, LV_ALIGN_CENTER, 0, 40);

    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 5);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_RED, 2));
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_pad_all(&style, 10);

    lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_text_letter_space(&style, 5);
    lv_style_set_text_line_space(&style, 20);
    lv_style_set_text_decor(&style, LV_TEXT_DECOR_UNDERLINE);

    /*Create an object with the new style*/
    lv_obj_t *obj = lv_label_create(lv_scr_act());
    lv_obj_add_style(obj, &style, 0);
    lv_label_set_text(obj,
                      "Text of\n"
                      "a label");

    lv_obj_center(obj);
}

void displayControl(void *pvParameters)
{
    display_init();

    while (1)
    {
        vTaskDelay(50U);

        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
        {
            display_homeScreen();
            xSemaphoreGive(xGuiSemaphore);
        }

        if (ST7789_isInitialized())
        {
            if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdTRUE)
            {
                lv_task_handler();
                lv_timer_handler();
                xSemaphoreGive(xGuiSemaphore);
            }
        }
    }
}

#endif  // FEATURE_DISPLAY
