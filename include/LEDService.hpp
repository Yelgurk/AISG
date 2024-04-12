#include <Arduino.h>
#include <vector>
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
    0, 1, 2, 3, 4, 5, 6, 7, 8,  // 9/10 in HP inline leds
    9, 10, 11, 12,              // LCD background x4 leds
    13, 14, 15                  // R,G,B led pins
};

class LEDService
{
public:
    vector<function<void(int32_t)>>     LedCornerSync;
    vector<function<void(int32_t)>>     LedInline;
    function<void(int32_t, int32_t)>    LedHPInline;
    function<void(int32_t, int32_t)>    LedHPMirror;
    function<void(int32_t)>             LedLCDBackground;
    function<void(int32_t, int32_t)>    LedRGB;

    void init_leds();
};

#endif