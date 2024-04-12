#include "../include/main.hpp"

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

    for (auto x : LED.LedCornerSync)
        for (uint8_t i = 0; i <= 100; i++)
        {
            delayMicroseconds(100);
            x(i);
        }

    delay(500);

    for (auto x : LED.LedCornerSync)
        for (uint8_t i = 0; i <= 100; i++)
        {
            delayMicroseconds(100);
            x(0);
        }

    Serial.println("LED inited!");
}

void loop()
{
    //Serial.println("frame");
    lv_task_handler(); 
    lv_tick_inc(5);
    delay(5);
}