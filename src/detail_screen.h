#pragma once
#include "screen_interface.h"
#include "screen_action.h"
#include "menu_item.h"
#include "api_service.h"
#include <TFT_eSPI.h>

class DetailScreen : public IScreen {
public:
    DetailScreen(MenuItem& detail, TFT_eSPI& display);
    void handleEncoderInc() override;
    void handleEncoderDec() override;
    
    ScreenAction handleBackShortPress() override;
    ScreenAction handleBackLongPress() override;
    ScreenAction handleDotShortPress() override;
    ScreenAction handleDotLongPress() override;
    void drawCall() override;
    void forceRedraw();
    
private:
    void drawHeader(const String& title, const String& artist, const String& album);
    void drawVolume(const String& volStr);
    void drawAndMoveCusror(const String& text, int& cursorY, int font, uint32_t color, int cx);
private:
    MenuItem& currentDetail;
    TFT_eSPI& tft;
    String lastTitle = "None";
    String lastVolStr = "None";
    unsigned long lastPoll = 5000;
    unsigned long pollIntervalMs = 5000; // default 5s
};
