#include "../include/UIService.hpp"

void UIService::init_screen()
{
    lcd.init();
    lcd.setRotation(3);

    lv_init();
    
    static auto *lvDisplay = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
            uint32_t w = lv_area_get_width(area);
            uint32_t h = lv_area_get_height(area);
            lv_draw_sw_rgb565_swap(data, w*h);
            lcd.pushImage(area->x1, area->y1, w, h, (uint16_t*)data);
            lv_display_flush_ready(display);
        });
    lv_display_set_buffers(lvDisplay, lvBuffer, nullptr, SCREEN_BUFFER, LV_DISPLAY_RENDER_MODE_PARTIAL);

    //static auto *lvInput = lv_indev_create();
    //lv_indev_set_type(lvInput, LV_INDEV_TYPE_POINTER);
    //lv_indev_set_read_cb(lvInput, [](lv_indev_t* dev, lv_indev_data_t* data) {
    //  uint16_t touchX, touchY;
    //
    //  data->state = LV_INDEV_STATE_REL;
    //
    //  if (lcd.getTouch(&touchX, &touchY))
    //  {
    //      data->state = LV_INDEV_STATE_PR;
    //      data->point.x = touchX;
    //      data->point.y = touchY;
    //  }
    //});

    int16_t f = -20, s = 7, x = f * s / 2 + f / 2;
    int colors_hex[s] = {
        0x5D8AA8,
        0xE32636,
        0xFFBF00,
        0x3CAA3C,
        0x79553D,
        0xDA3287,
        0x000000
    };

    for (uint8_t i = 0; i < s; i++)
    {
        lv_obj_t *label = lv_label_create( lv_scr_act() );
        lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
        lv_obj_align( label, LV_ALIGN_CENTER, 0, x -= f );
        lv_obj_set_style_text_color(label, lv_color_hex(colors_hex[i]), LV_STATE_DEFAULT);
    }
}

void UIService::init_controls()
{
    
}

void UIService::show_scr_task()
{
    
}

void UIService::show_scr(uint8_t page)
{
    
}

void UIService::data_refresh_task()
{

}