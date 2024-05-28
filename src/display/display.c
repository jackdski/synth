#include "display.h"

#include <stdint.h>

#include "FreeRTOS.h"
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

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    /* Divide by 8 */
    const uint8_t row1 = area->y1 >> 3;
    const uint8_t row2 = area->y2 >> 3;

    // uint8_t conf[] = {
    //     SSD1306_CONTROL_BYTE_CMD_STREAM,
    //     SSD1306_CMD_SET_MEMORY_ADDR_MODE,
    //     0x00,
    //     SSD1306_CMD_SET_COLUMN_RANGE,
    //     (uint8_t)area->x1,
    //     (uint8_t)area->x2,
    //     SSD1306_CMD_SET_PAGE_RANGE,
    //     row1,
    //     row2,
    // };

    // bool err = mDisplay.ssd1306.writeBytes(conf, sizeof(conf));
    // err |= mDisplay.ssd1306.writeBytes((uint8_t *)color_p, SSD1306_COLUMNS * (1 + row2 - row1));

    lv_disp_flush_ready(disp);
}

void display_init(void)
{
    // ST7789_init();

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
    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    // lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    lv_label_set_text(label1, "Home Screen");
    // lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    // lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(label1, LV_ALIGN_CENTER, 0, 40);
}

void displayControl(void *pvParameters)
{
    display_init();

    while (1)
    {
        vTaskDelay(10U);
        // if (mDisplay.initialized)
        {
            lv_task_handler();
            display_homeScreen();
        }
    }
}

#endif  // FEATURE_DISPLAY
