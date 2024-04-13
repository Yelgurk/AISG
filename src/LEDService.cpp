#include "../include/LEDService.hpp"

int32_t * LEDService::_percentage_fix(int32_t * value)
{
    *value = *value > 100 ? 100 : *value;
    *value = *value < 0 ? 0 : *value;
    return value;
}

void LEDService::_percentage_to_pwm(int32_t * value, int32_t max_pwm)
{
    *value = max_pwm * *value / 100;
}

void LEDService::init_leds()
{
    PCA_1.begin();
    PCA_2.begin();

    PCA_1.setFrequency(1526);
    PCA_2.setFrequency(1526);

    for (uint8_t i = 0; i < LED_CORNER_SYNC_CNT; i++)
        LedCornerSync.push_back([i, this](int32_t power) {
            _percentage_to_pwm(
                _percentage_fix(&power)
            );

            PCA_1.setPWM(PCA_1_ch[i], power);
            PCA_1.setPWM(PCA_1_ch[i + 5], power);
            PCA_1.setPWM(PCA_1_ch[i + 10], power);
        });

    for (uint8_t i = 0; i < LED_INLINE_CNT; i++)
        LedInline.push_back([i, this](int32_t power) {
            _percentage_to_pwm(
                _percentage_fix(&power)
            );

            if (i == 0)
                PCA_1.setPWM(PCA_1_ch[15], power);
            else
                PCA_2.setPWM(PCA_2_ch[i - 1], power);
        });

    LedHPInline = [this](int32_t max_var, int32_t curr_var) {
        for (int32_t i = 0; i < LED_INLINE_CNT; i++)
            if (i * (max_var / LED_INLINE_CNT) < curr_var)
                LedInline[i](100);
            else
                LedInline[i](0);
    };

    LedHPMirror = [this](int32_t max_var, int32_t curr_var) {
        for (int32_t i = 0; i < LED_INLINE_CNT / 2; i++)
            if (i * (max_var / (LED_INLINE_CNT / 2)) < curr_var)
            {
                LedInline[LED_INLINE_CNT / 2 - 1 - i](100);
                LedInline[LED_INLINE_CNT / 2 + i](100);
            }
            else
            {
                LedInline[LED_INLINE_CNT / 2 - 1 - i](0);
                LedInline[LED_INLINE_CNT / 2 + i](0);
            }
    };

    LedLCDBackground = [this](int32_t power) {
        _percentage_to_pwm(
            _percentage_fix(&power)
        );

        PCA_2.setPWM(PCA_2_ch[9], power);
        PCA_2.setPWM(PCA_2_ch[10], power);
        PCA_2.setPWM(PCA_2_ch[11], power);
        PCA_2.setPWM(PCA_2_ch[12], power);
    };
    
    LedRGB = [this](int32_t rgb_hex, int32_t power) {
        int32_t red =    4095 / 255 * ((rgb_hex & 0xff0000) >> 16) / 100 * power;
        int32_t green =  4095 / 255 * ((rgb_hex & 0x00ff00) >> 8)  / 100 * power;
        int32_t blue =   4095 / 255 * (rgb_hex & 0x0000ff)         / 100 * power;

        PCA_2.setPWM(PCA_2_ch[13], red);
        PCA_2.setPWM(PCA_2_ch[14], green);
        PCA_2.setPWM(PCA_2_ch[15], blue);
    };
}

void LEDService::leds_task_clear() {
    while(!leds_task.empty())
        leds_task.pop();
}

void LEDService::leds_task_add(function<bool()> func) {
    leds_task.push(func);
    MS_old = MS_curr = millis();
}

void LEDService::leds_task_handler()
{
    MS_curr = millis();
    if (!leds_task.empty())
        if (leds_task.front()())
            leds_task.pop();
}

void LEDService::leds_task_trigger_hit()
{
    leds_task_clear();
    
    for (int8_t p = LED_CORNER_SYNC_CNT - 1; p >= 0; p--)
        for (uint8_t i = 0; i <= 100; i += 35)
            leds_task_add([this, p, i]() {
                led_corner_sync()[p](i);
                return true;
            });
    
    for (auto x : led_corner_sync())
        leds_task_add([this, x]() {
            x(0);
            return true;
        });
}