#include "../include/main.hpp"

#define LED_SERVICE_TEST

void setup()
{
    Serial.begin(115200);
    Serial.println("LCD init...");

    Wire.begin(19, 20, 400000);

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

uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, hp = 0;

void loop()
{
    lv_task_handler();
    UI.hp_control_set(HP_max, HP_curr, HP_DEFAULT_TARGET_MS - (MS_curr = millis() - MS_old));
    LED.leds_task_handler();

    hp++;
    if (hp / 1000 % 2 == 0)
        LED.led_hp_inline(1000, hp % 1000);
    else
        LED.led_hp_mirror(1000, hp % 1000);

    t1 = millis() / 5000;
    if (t1 != t2)
    {
        t2 = t1;
        Serial.println("!!!");
        LED.leds_task_trigger_hit();
    }

    t3 = millis() / 60000;
    HP_curr = hp % 1000;
    if (t3 != t4)
    {
        HP_curr = hp = 1;
        t4 = t3;
        HP_max = 1000;
        MS_old = MS_curr;
    }
}