#include <lgfx_user/LGFX_ESP32_sample.hpp>
#include <lvgl.h>

#ifndef UIService_hpp
#define UIService_hpp

#define LGFX_USE_V1
#define SCREEN_WIDTH    320U
#define SCREEN_HEIGHT   170U
#define SCREEN_BUFFER   (SCREEN_WIDTH * SCREEN_HEIGHT) / 2 * (LV_COLOR_DEPTH / 8)

static LGFX lcd;
static lv_obj_t * ui_Screen1;
static uint8_t lvBuffer[SCREEN_BUFFER];

class UIService
{
public:
    void init_screen();
    void init_controls();

    //el_battery Battery;
    //el_dateTime_c DateTimeC;
    //el_task_c TaskC;
    //el_stateInfo StateInfoC;
    //el_blowHelp BlowHelpC;
    //el_blowSelect BlowSelectC;
    //el_blowProg BlowProgC;
    //el_awaitInfo AwaitInfoC;
    //el_settings SettingsC;
    //el_notification NotificationC;

    void show_scr_task();
    void show_scr(uint8_t page);
    void data_refresh_task();
};

#endif