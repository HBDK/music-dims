
#pragma once
#include "screen_interface.h"
#include "screen_action.h"
#include "menu_item.h"
#include <U8g2lib.h>

class DetailScreen : public IScreen {
public:
    DetailScreen(MenuItem& detail, U8G2& display);
    void handleEncoderInc() override;
    void handleEncoderDec() override;
    ScreenAction handleBack(uint32_t pressLengthMs) override;
    ScreenAction handleDotRelease(uint32_t pressLengthMs) override;
    void drawCall() override;
private:
    MenuItem& currentDetail;
    U8G2& u8g2;
};
