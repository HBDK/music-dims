#pragma once
#include <Adafruit_NeoPixel.h>
#include <stdint.h>

class LightService {
public:
    static constexpr uint8_t PIN_LCD_RGB = 21;
    static constexpr uint8_t NUM_PIXELS = 3;

    LightService();
    void begin();
    void setButtonLed(uint8_t r, uint8_t g, uint8_t b); // Set both button LEDs (0,1)
    void setBacklight(uint8_t r, uint8_t g, uint8_t b); // Set backlight LED (2)
    void show(); // Push changes to hardware

private:
    Adafruit_NeoPixel rgbBacklight;
    uint32_t buttonColor;
    uint32_t backlightColor;
};
