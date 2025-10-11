
#pragma once
#include <Arduino.h>
#include "screen_action.h"

class IScreen {
public:
    virtual ~IScreen() {}
    virtual void handleEncoderInc() = 0;
    virtual void handleEncoderDec() = 0;
    virtual ScreenAction handleBack(uint32_t pressLengthMs) = 0;
    virtual ScreenAction handleDotRelease(uint32_t pressLengthMs) = 0;
    virtual void drawCall() = 0;
};
