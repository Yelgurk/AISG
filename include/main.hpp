#include <Arduino.h>
#include <Wire.h>
#include "./UIService.hpp"
#include "./LEDService.hpp"

#define HP_DEFAULT_TARGET_TASK  30
#define HP_DEFAULT_TARGET_MS    40 * 1000

UIService UI;
LEDService LED;

static int64_t
    HP_max  = 0,
    HP_curr = 0,
    MS_old  = 0,
    MS_curr = 0;
        