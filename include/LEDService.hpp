#include <Arduino.h>
#include <queue>
#include "PCA9685.h"

#ifndef LEDService_hpp
#define LEDService_hpp

using namespace std;

#define LED_CORNER_SYNC_CNT     5
#define LED_INLINE_CNT      10
#define LED_BACKGROUND_CNT      4

static PCA9685 PCA_1(0x7F);
static PCA9685 PCA_2(0x40);

static const uint8_t PCA_1_ch[16]
{
    4, 3, 2, 1, 0,              // bottom-left leds
    14, 13, 12, 11, 10,         // top-center leds
    9, 8, 7, 6, 5,              // bottom-right leds
    15                          // first (1/10) in HP inline leds
};

static const uint8_t PCA_2_ch[16]
{
    7, 6, 5, 4, 3, 2, 1, 8, 0,  // 9/10 in HP inline leds
    9, 10, 11, 12,              // LCD background x4 leds
    13, 14, 15                  // R,G,B led pins
};

class LEDService
{
private:
    vector<function<void(int32_t)>>     LedCornerSync;
    vector<function<void(int32_t)>>     LedInline;
    function<void(int32_t, int32_t)>    LedHPInline;
    function<void(int32_t, int32_t)>    LedHPMirror;
    function<void(int32_t)>             LedLCDBackground;
    function<void(int32_t, int32_t)>    LedRGB;

    int32_t * _percentage_fix(int32_t * value);
    void _percentage_to_pwm(int32_t * value, int32_t max_pwm = 4095);

    queue<function<bool()>> leds_task;
    uint32_t MS_old = 0,
             MS_curr = 0;

public:

    void init_leds();
    vector<function<void(int32_t)>> led_corner_sync()           { return LedCornerSync; }
    vector<function<void(int32_t)>> led_inline()                { return LedInline; }
    void led_hp_inline(int32_t max_val, int32_t curr_val)       { LedHPInline(max_val, curr_val); }
    void led_hp_mirror(int32_t max_val, int32_t curr_val)       { LedHPMirror(max_val, curr_val); }
    void led_lcd_background(int32_t power_percentage)           { LedLCDBackground(power_percentage); }
    void led_r_g_b_power(int32_t rgb_hex, int32_t power)        { LedRGB(rgb_hex, power); }
    void leds_task_add(function<bool()>);
    void leds_task_clear();
    void leds_task_handler();
    void leds_task_trigger_hit();
};

#endif