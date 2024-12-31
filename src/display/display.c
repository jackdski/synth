#include "display.h"

/* I N C L U D E S */

#include <stdint.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "lv_themes.h"
#include "lvgl.h"

#include "ST7789.h"

#include "button.h"
#include "sgtl5000.h"

#ifdef __cplusplus
extern "C" {
#endif


#if (FEATURE_DISPLAY)

/* D E F I N E S */

#define BUFFER_SIZE     (DISPLAY_WIDTH * DISPLAY_HEIGHT / 10)

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

/* T Y P E D E F S */

typedef struct
{
    Display_screen_E screen;
    bool homeScreenPress;
    bool buttonPressedA;
    bool buttonPressedB;
    int32_t volume;
} Display_data_S;

/* P R I V A T E   F U N C T I O N   D E F I N I T I O N S */

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void display_setScreen(const Display_screen_E screen);
static void display_private_update_inputs(void);
static void update_volume(void);
static void volume_cb(lv_event_t *e);

/* D A T A   D E F I N I T I O N S */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t displayBuffer1[BUFFER_SIZE]; /*Declare a buffer for 1/10 screen size*/
static lv_color_t displayBuffer2[BUFFER_SIZE];
static lv_disp_t *disp;
static lv_disp_drv_t disp_drv;

static lv_obj_t *volumeBar;

static SemaphoreHandle_t lvglMutex = NULL;

static Display_data_S displayData =
{
    .screen = DISPLAY_SCREEN_HOME,
    .homeScreenPress = false,
    .buttonPressedA = false,
    .buttonPressedB = false,
};

/* P R I V A T E   F U N C T I O N S */

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void display_setScreen(const Display_screen_E screen);

static void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t width, height = 0U;

    width  = (area->x2 - area->x1) + 1U;
    height = (area->y2 - area->y1) + 1U;

    ST7789_drawBuffer(area->x1, area->y1, width, height, (uint8_t *)color_p, (width * height * 2U));
    lv_disp_flush_ready(disp);
}

static void display_setScreen(const Display_screen_E screen)
{
    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        displayData.screen = screen;
        switch (displayData.screen)
        {
            case DISPLAY_SCREEN_HOME:
            {
                lv_obj_clean(lv_scr_act());
                // display_homeScreen();
    lv_obj_t *obj = lv_label_create(lv_scr_act());
    // lv_obj_add_style(obj, &style, 0);
    lv_label_set_text(obj, "JD");
    lv_obj_center(obj);                
                break;
            }

            case DISPLAY_SCREEN_SETTINGS:
            {
                lv_obj_clean(lv_scr_act());
                // display_settingsSelection();
                break;
            }

            case DISPLAY_SCREEN_WAVEFORM:
            {
                lv_obj_clean(lv_scr_act());
                // display_waveformScreen();
                break;
            }

            case DISPLAY_SCREEN_COUNT:
            default:
                // do nothing
                break;
        }
        xSemaphoreGive(lvglMutex);
    }
}

static void display_private_update_inputs(void)
{
#if (FEATURE_BUTTON)
    const bool buttonPressedA = Button_isPressed(BUTTON_CHANNEL_A);
    const bool buttonPressedB = Button_isPressed(BUTTON_CHANNEL_B);
    displayData.buttonPressedA = (buttonPressedA && (displayData.buttonPressedA == false));
    displayData.buttonPressedB = (buttonPressedB && (displayData.buttonPressedB == false));
#endif

    displayData.homeScreenPress |= (displayData.buttonPressedA || displayData.buttonPressedB);
}

static void update_volume(void)
{
    const float volume = SGTL5000_getVolume();
    const int32_t vol  = (int32_t)(100.0f * (volume + SGTL5000_VOLUME_DB_MIN) / (SGTL5000_VOLUME_DB_MIN + 51.5f));
    if (vol != displayData.volume)
    {
        lv_event_send(volumeBar, LV_EVENT_VALUE_CHANGED, (void *)vol);
        displayData.volume = vol;
    }
}

static void volume_cb(lv_event_t *e)
{
    if (e != NULL)
    {
        lv_obj_t *volumeBar = lv_event_get_target(e);
        int32_t volume      = (int32_t)e->param;
        lv_bar_set_value(volumeBar, volume, LV_ANIM_OFF);
    }
}

/* P U B L I C   F U N C T I O N S */

void display_init(void)
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

void display_volume_bar(void)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    volumeBar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(volumeBar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(volumeBar, 20, 100);
    lv_obj_align(volumeBar, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_bar_set_range(volumeBar, 0, 100);
    lv_bar_set_value(volumeBar, 0, LV_ANIM_ON);

    lv_obj_add_event_cb(volumeBar, volume_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void displayControl(void *pvParameters)
{
    display_init();

    if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_theme_t *th = lv_theme_mono_init(disp, true, &lv_font_montserrat_24);
        lv_disp_set_theme(disp, th);
        xSemaphoreGive(lvglMutex);
    }

    display_setScreen(DISPLAY_SCREEN_HOME);
    while (1)
    {
        vTaskDelay(50U);

        display_private_update_inputs();

        if (ST7789_isInitialized())
        {
            if (displayData.buttonPressedA)
            {
                display_setScreen(DISPLAY_SCREEN_HOME);
            }
            else if (displayData.buttonPressedB)
            {
                // TODO: screenManager state machine
                display_setScreen(DISPLAY_SCREEN_SETTINGS);
            }
            else
            {
                // maintain screen
            }

            if (xSemaphoreTake(lvglMutex, portMAX_DELAY) == pdTRUE)
            {
                // update volume
                update_volume();

                lv_task_handler();
                lv_timer_handler();
                xSemaphoreGive(lvglMutex);
            }
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif  // FEATURE_DISPLAY
