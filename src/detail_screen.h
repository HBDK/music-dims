#pragma once
#include "screen_interface.h"
#include "screen_action.h"
#include "menu_item.h"
#include <TFT_eSPI.h>

class DetailScreen : public IScreen {
public:
    DetailScreen(MenuItem& detail, TFT_eSPI& display);
    void handleEncoderInc() override;
    void handleEncoderDec() override;
    ScreenAction handleBackRelease(uint32_t pressLengthMs) override;
    ScreenAction handleDotRelease(uint32_t pressLengthMs) override;
    void drawCall() override;
private:
    MenuItem& currentDetail;
    TFT_eSPI& tft;
};
