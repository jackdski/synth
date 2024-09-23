#include "display.h"
#include "features.h"
#include "lvgl.h"

#if (FEATURE_DISPLAY)

#include "lvgl.h"

#define NUM_WAVEFORM_POINTS 3U

void display_waveformScreen(void)
{
    /*Create an array for the points of the line*/
    static lv_point_t line_points[NUM_WAVEFORM_POINTS] = {
        {                      (lv_coord_t)0, DISPLAY_HEIGHT / 4},
        {(lv_coord_t)(DISPLAY_WIDTH * 2 / 5), DISPLAY_HEIGHT / 2},
        {(lv_coord_t)(DISPLAY_WIDTH * 3 / 5), DISPLAY_HEIGHT / 4},
    };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 3);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    /*Create a line and apply the new style*/
    lv_obj_t *line1;
    line1 = lv_line_create(lv_scr_act());
    lv_line_set_points(line1, line_points, NUM_WAVEFORM_POINTS); /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);
    lv_obj_center(line1);

    display_volume_bar();
}

#endif // FEATURE_DISPLAY
