#include <Arduino.h>
#include <Wire.h>
#include "./UIService.hpp"
#include "./LEDService.hpp"
#include "./TaskService.hpp"

#define HIT_SENSOR_DIGITAL_S_ESP_PIN    17

UIService UI;
LEDService LED;
TaskService TS;        

volatile bool hit_sensor_trigger = false;