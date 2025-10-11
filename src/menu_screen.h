
#pragma once
#include "screen_interface.h"
#include "screen_action.h"
#include "menu_item.h"
#include <U8g2lib.h>

class MenuScreen : public IScreen {
public:
    MenuScreen(MenuItem* items, int& count, int& index, U8G2& display);
    void handleEncoderInc() override;
    void handleEncoderDec() override;
    ScreenAction handleBack(uint32_t pressLengthMs) override;
    ScreenAction handleDotRelease(uint32_t pressLengthMs) override;
    void drawCall() override;
private:
    MenuItem* menuItems;
    int& menuCount;
    int& menuIndex;
    U8G2& u8g2;
};
