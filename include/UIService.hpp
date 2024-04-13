#include <lgfx_user/LGFX_ESP32_sample.hpp>
#include <lvgl.h>
#include <vector>
#include "./ui_font_RussoOne96.hpp"

#ifndef UIService_hpp
#define UIService_hpp

using namespace std;

#define LGFX_USE_V1
#define SCREEN_WIDTH    320U
#define SCREEN_HEIGHT   170U
#define SCREEN_BUFFER   (SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_DEPTH) / 10 / 2

static LGFX lcd;
static uint8_t lvBuffer_1[SCREEN_BUFFER];
static uint8_t lvBuffer_2[SCREEN_BUFFER];

class UIService
{
private:
    void _init_controls();
    void _hide_hp_control();
    void _stopwatchs_set_text();
    bool _is_await_control_act = false;

public:
    void init_screen();

    lv_obj_t * screen;
    lv_obj_t * ellipse;
    lv_obj_t * hp_dark_back;
    lv_obj_t * hp_light_back;

    lv_obj_t * stopwatch_dark;
    lv_obj_t * stopwatch_light;

    lv_anim_t * a_ellipse;

    uint16_t anim_curr_val();
    void show_await_control();
    void show_hp_control();
    bool hp_control_set(int32_t max_val, int32_t curr_val, int32_t time_left);
};

#endif