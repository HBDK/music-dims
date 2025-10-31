#pragma once
#include <stdint.h>
#include "screen_interface.h"

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
    int32_t lastEncoder;
    int encoderDelta;
    bool lastDotState, lastBackState;
    uint32_t dotPressStart, backPressStart;
    bool dotReleased, backReleased;
};
