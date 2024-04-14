#include "../include/UIService.hpp"

static uint32_t my_tick_get_cb(void) {
  return millis();
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}

static void anim_opa_cb(void * var, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t*)var, v, 0);
    UIService::anim_set_current_val(v);
}

void UIService::init_screen()
{
    lcd.init();
    lcd.setRotation(3);

    lv_init();
    lv_tick_set_cb(my_tick_get_cb);
    
    static auto *lvDisplay = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
            uint32_t w = lv_area_get_width(area);
            uint32_t h = lv_area_get_height(area);
            lv_draw_sw_rgb565_swap(data, w*h);
            lcd.pushImage(area->x1, area->y1, w, h, (uint16_t*)data);
            lv_display_flush_ready(display);
        });
    lv_display_set_buffers(lvDisplay, lvBuffer_1, lvBuffer_2, SCREEN_BUFFER, LV_DISPLAY_RENDER_MODE_PARTIAL);

    screen = lv_obj_create(NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_disp_load_scr(screen);

    _init_controls();
}

void UIService::_init_controls()
{
    ellipse = lv_obj_create(screen);
    lv_obj_set_size(ellipse, 0, 0);
    lv_obj_set_style_opa(ellipse, 255, 0);
    lv_obj_set_style_radius(ellipse, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ellipse, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ellipse, lv_color_hex(0xFF0800), LV_STATE_DEFAULT);
    lv_obj_align(ellipse, LV_ALIGN_CENTER, 0, 0);

    lv_anim_init(a_ellipse = new lv_anim_t());
    lv_anim_set_var(a_ellipse, ellipse);
    lv_anim_set_values(a_ellipse, 0, 600);
    lv_anim_set_time(a_ellipse, 900);
    lv_anim_set_repeat_delay(a_ellipse, 100);
    lv_anim_set_repeat_count(a_ellipse, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(a_ellipse, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(a_ellipse, anim_size_cb);
    lv_anim_start(a_ellipse);

    lv_anim_set_values(a_ellipse, 255, 0);
    lv_anim_set_exec_cb(a_ellipse, anim_opa_cb);
    lv_anim_start(a_ellipse);



    hp_dark_back = lv_obj_create(screen);
    lv_obj_set_size(hp_dark_back, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_align(hp_dark_back, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_radius(hp_dark_back, 0, 0);
    lv_obj_set_style_border_width(hp_dark_back, 0, 0);
    lv_obj_set_style_bg_color(hp_dark_back, lv_color_hex(0x000000), 0);

    stopwatch_dark = lv_label_create(hp_dark_back);
    lv_obj_set_style_text_font(stopwatch_dark, &ui_font_RussoOne96, 0);
    lv_obj_align(stopwatch_dark, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(stopwatch_dark, lv_color_hex(0xFFFFFF), 0);



    hp_light_back = lv_obj_create(screen);
    lv_obj_set_size(hp_light_back, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_clear_flag(hp_light_back, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(hp_light_back, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_radius(hp_light_back, 0, 0);
    lv_obj_set_style_border_width(hp_light_back, 0, 0);
    lv_obj_set_style_bg_color(hp_light_back, lv_color_hex(0xFF6800), 0);

    stopwatch_light = lv_label_create(hp_light_back);
    lv_obj_set_style_text_font(stopwatch_light, &ui_font_RussoOne96, 0);
    lv_obj_align(stopwatch_light, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(stopwatch_light, lv_color_hex(0x000000), 0);
}

void UIService::show_await_control()
{
    lv_obj_set_style_opa(hp_dark_back, 0, 0);
    lv_obj_set_style_opa(hp_light_back, 0, 0);
    _is_await_control_act = true;
}

void UIService::show_hp_control()
{
    lv_obj_set_style_opa(hp_dark_back, 255, 0);
    lv_obj_set_style_opa(hp_light_back, 255, 0);
    lv_obj_set_width(hp_light_back, SCREEN_WIDTH);
    lv_obj_align(stopwatch_light, LV_ALIGN_CENTER, 0, 0);
    _is_await_control_act = false;

}

void UIService::show_win_control()
{
    lv_obj_set_width(hp_light_back, SCREEN_WIDTH);
    lv_obj_align(stopwatch_light, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(hp_light_back, lv_color_hex(0x00FF00), 0);
}

bool UIService::hp_control_set(bool is_await, int32_t max_val, int32_t curr_val, int32_t time_left)
{
    if (is_await)
    {
        show_await_control();
        return false;
    }
    else
        show_hp_control();  

    lv_obj_set_style_bg_color(hp_light_back, lv_color_hex(0xFF6800), 0);
    lv_obj_set_width(hp_light_back, SCREEN_WIDTH * curr_val / max_val);
    lv_obj_align(stopwatch_light, LV_ALIGN_CENTER, (SCREEN_WIDTH - (SCREEN_WIDTH * curr_val / max_val)) / 2, 0);
    
    std::string millis_str = std::to_string(static_cast<double_t>(time_left) / 1000.0);
    millis_str = millis_str.substr(0, millis_str.find(".") + 2);

    lv_label_set_text(stopwatch_dark, millis_str.c_str());
    lv_label_set_text(stopwatch_light, millis_str.c_str());

    return true;
}

uint16_t UIService::_current_anim_val = 0;

void UIService::anim_set_current_val(uint16_t val)
{
    UIService::_current_anim_val = val;
}

uint16_t UIService::anim_get_current_val()
{
    return UIService::_current_anim_val;
}