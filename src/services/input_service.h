#pragma once
#include <stdint.h>
#include "screens/screen_interface.h"

class InputService {
public:
    static constexpr uint8_t PIN_ENC_A    = 21;
    static constexpr uint8_t PIN_ENC_B    = 22;
    static constexpr uint8_t PIN_ENC_SW   = 3;
    static constexpr uint8_t PIN_BACK_BTN = 1;
    static constexpr uint8_t IMPULSES_PER_CLICK = 1;

    InputService(IScreen *screen);
    void begin();
    ScreenAction poll();
    IScreen* currentScreen;
private:
    void handleEncoder();
    ScreenAction handleButton(uint8_t pin, bool &lastState, uint32_t &pressStart, bool &released,
                              ScreenAction (IScreen::*shortHandler)(), ScreenAction (IScreen::*longHandler)());
    int32_t lastEncoder;
    int encoderDelta;
    bool lastDotState, lastBackState;
    uint32_t dotPressStart, backPressStart;
    bool dotReleased, backReleased;
};
