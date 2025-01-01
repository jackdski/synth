
#include "volume_bar.hpp"

static lv_obj_t *volumeBar;
static int32_t prevVolume = 0;

void display_volume_bar(void)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE_GREY));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    volumeBar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(volumeBar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(volumeBar, lv_pct(5), lv_pct(90));
    lv_obj_align(volumeBar, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_bar_set_range(volumeBar, 0, 100);
    lv_bar_set_value(volumeBar, 0, LV_ANIM_ON);

    lv_obj_add_event_cb(volumeBar, volume_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void volume_cb(lv_event_t *e)
{
    if (e != NULL)
    {
        lv_obj_t * bar = (lv_obj_t *)lv_event_get_target(e);
        int32_t volume = (int32_t)lv_event_get_param(e);
        lv_bar_set_value(bar, volume, LV_ANIM_OFF);
    }
}

void volume_bar_update(void)
{
    const float volume = SGTL5000_getVolume();
    int32_t vol  = (int32_t)(100.0f * (volume + SGTL5000_VOLUME_DB_MIN) / (SGTL5000_VOLUME_DB_MIN + 51.5f));
    if (prevVolume != vol)
    {
        lv_obj_send_event(volumeBar, LV_EVENT_VALUE_CHANGED, &vol);
    }
    prevVolume = vol;
}