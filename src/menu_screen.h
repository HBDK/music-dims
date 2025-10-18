#pragma once
#include "screen_interface.h"
#include "screen_action.h"
#include "menu_item.h"
#include <TFT_eSPI.h>

class MenuScreen : public IScreen {
public:
    MenuScreen(MenuItem* items, int& count, int& index, TFT_eSPI& display);
    void handleEncoderInc() override;
    void handleEncoderDec() override;
    ScreenAction handleBackRelease(uint32_t pressLengthMs) override;
    ScreenAction handleDotRelease(uint32_t pressLengthMs) override;
    void drawCall() override;
    // Flicker reduction
    void forceRedraw();
private:
    MenuItem* menuItems;
    int backIndex;
    int& menuCount;
    int& menuIndex;
    TFT_eSPI& tft;
    void drawError();
    int lastMenuIndex = -1;
    int lastMenuCount = -1;
};
