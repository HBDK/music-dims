
#pragma once
#include <Arduino.h>
#include "screen_action.h"

class IScreen {
public:
    virtual ~IScreen() {}
    virtual void handleEncoderInc() = 0;
    virtual void handleEncoderDec() = 0;
    virtual ScreenAction handleBackShortPress() { return ScreenAction::None; }
    virtual ScreenAction handleBackLongPress() { return ScreenAction::None; }

    virtual ScreenAction handleDotShortPress() { return ScreenAction::None; }
    virtual ScreenAction handleDotLongPress() { return ScreenAction::None; }
    virtual void drawCall() = 0;
};
