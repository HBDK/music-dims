#include "light_service.h"

LightService::LightService()
    : rgbBacklight(NUM_PIXELS, PIN_LCD_RGB, NEO_GRB + NEO_KHZ800),
      buttonColor(rgbBacklight.Color(0,0,0)),
      backlightColor(rgbBacklight.Color(0,0,0)) {}

void LightService::begin() {
    rgbBacklight.begin();
    rgbBacklight.setBrightness(100); // Default brightness
    setButtonLed(32, 0, 32); // Default purple
    setBacklight(128, 0, 128); // Default purple
    show();
}

void LightService::setButtonLed(uint8_t r, uint8_t g, uint8_t b) {
    buttonColor = rgbBacklight.Color(r, g, b);
    rgbBacklight.setPixelColor(0, buttonColor);
    rgbBacklight.setPixelColor(1, buttonColor);
}

void LightService::setBacklight(uint8_t r, uint8_t g, uint8_t b) {
    backlightColor = rgbBacklight.Color(r, g, b);
    rgbBacklight.setPixelColor(2, backlightColor);
}

void LightService::show() {
    rgbBacklight.show();
}
