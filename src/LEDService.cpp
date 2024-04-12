#include "../include/LEDService.hpp"

void LEDService::init_leds()
{
    PCA_1.begin();
    PCA_2.begin();

    PCA_1.setFrequency(1526);
    PCA_2.setFrequency(1526);

    for (uint8_t i = 0; i < LED_CORNER_SYNC_CNT; i++)
        LedCornerSync.push_back([i](int32_t power_percentage) {
            power_percentage = power_percentage > 100 ? 100 : power_percentage;
            power_percentage = power_percentage < 0 ? 0 : power_percentage;

            power_percentage = 4095 / 100 * power_percentage;

            PCA_1.setPWM(PCA_1_ch[i], power_percentage);
            PCA_1.setPWM(PCA_1_ch[i + 5], power_percentage);
            PCA_1.setPWM(PCA_1_ch[i + 10], power_percentage);
        });

    for (uint8_t i = 0; i < LED_INLINE_CNT - 1; i++)
        LedInline.push_back([i](int32_t power_percentage) {
            power_percentage = power_percentage > 100 ? 100 : power_percentage;
            power_percentage = power_percentage < 0 ? 0 : power_percentage;

            power_percentage = 4095 / 100 * power_percentage;

            if (i == 0)
                PCA_1.setPWM(PCA_1_ch[15], power_percentage);
            else
                PCA_2.setPWM(PCA_2_ch[i - 1], power_percentage);
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
                LedInline[LED_INLINE_CNT / 2 - 1 + i](100);
            }
            else
            {
                LedInline[LED_INLINE_CNT / 2 - 1 - i](0);
                LedInline[LED_INLINE_CNT / 2 - 1 + i](0);
            }
    };

    //LedHPInline = [](int32_t max_val, int32_t curr_val){};
}