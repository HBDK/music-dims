#pragma once
#include <stdint.h>
#include "screen_interface.h"
#include "light_service.h"

class InputService {
public:
    static constexpr uint8_t PIN_ENC_A    = 32;
    static constexpr uint8_t PIN_ENC_B    = 33;
    static constexpr uint8_t PIN_ENC_SW   = 25;
    static constexpr uint8_t PIN_BACK_BTN = 27;

    InputService(IScreen *screen, LightService *light);
    void begin();
    ScreenAction poll();
    IScreen* currentScreen;
private:
    LightService* lightService;
    void handleEncoder();
    int32_t lastEncoder;
    int encoderDelta;
    bool lastDotState, lastBackState;
    uint32_t dotPressStart, backPressStart;
    bool dotReleased, backReleased;
};
