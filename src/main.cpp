#include "../include/main.hpp"

//#define LED_SERVICE_TEST

void setup()
{
    Serial.begin(115200);
    Serial.println("LCD init...");

    Wire.begin(19, 20, 400000);

    pinMode(HIT_SENSOR_DIGITAL_S_ESP_PIN, INPUT);
    attachInterrupt(
        HIT_SENSOR_DIGITAL_S_ESP_PIN,
        [](){ hit_sensor_trigger = true; },
        RISING
    );

    UI.init_screen();
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);

    Serial.println("LCD inited!");
    Serial.println("LED init...");

    LED.init_leds();

#ifdef LED_SERVICE_TEST

    for (auto x : LED.led_corner_sync())
        for (uint8_t i = 0; i <= 200; i++)
        {
            delayMicroseconds(200);
            x(i <= 100 ? i : 200 - i);
        }

    delay(100);

    for (int32_t hp = 0, max_hp = 345; hp < max_hp; hp += 2)
    {
        LED.led_hp_inline(max_hp, hp);
        delayMicroseconds(100);
    }
    LED.led_hp_inline(1, 0);

    delay(100);

    for (int hp = 0, max_hp = 345; hp < max_hp; hp += 2)
    {
        LED.led_hp_mirror(max_hp, hp);
        delayMicroseconds(100);
    }
    LED.led_hp_inline(1, 0);

    delay(100);

    for (uint8_t i = 0; i <= 100; i++)
        { delayMicroseconds(2000); LED.led_lcd_background(i); }
    LED.led_lcd_background(0);

    delay(100);

    for (uint8_t i = 0; i <= 100; i++)
        { delayMicroseconds(2000); LED.led_r_g_b_power(0xC3490A, i); }
    LED.led_r_g_b_power(0x000000, 0);

#endif

    Serial.println("LED inited!");
    UI.show_hp_control();
}

void loop()
{
    lv_task_handler();
    LED.leds_task_handler();
    
    if (hit_sensor_trigger)
    {
        TS.hit_detected();
        LED.leds_task_trigger_hit();
        hit_sensor_trigger = false;
    }
    
    if (TS.aim_task_handler())
    {
        UI.show_win_control();
        LED.led_r_g_b_power(0x00FF00, 100);
        LED.led_hp_mirror(TS.aim_max_hp(), 0);

        for (uint32_t ms = millis(); millis() - ms < 5000;)
            lv_task_handler();

        LED.led_r_g_b_power(0x000000, 0);
        hit_sensor_trigger = false;
    }

    if (TS.is_await())
        LED.led_r_g_b_power(0xFF0800, 100 * UI.anim_get_current_val() / 255);
    else
        LED.led_r_g_b_power(0x000000, 0);

    UI.hp_control_set(TS.is_await(), TS.aim_max_hp(), TS.aim_curr_hp(), TS.time_left_ms());
    LED.led_hp_mirror(TS.aim_max_hp(), TS.is_await() ? 0 : TS.aim_curr_hp());
}