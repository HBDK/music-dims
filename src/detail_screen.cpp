#include "detail_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

DetailScreen::DetailScreen(MenuItem& detail, TFT_eSPI& display)
    : currentDetail(detail), tft(display) {}

void DetailScreen::handleEncoderInc() {
    bool ok = ApiService::postVolumeUp();
    if (!ok) Serial.println("Failed to POST volume up");
}

void DetailScreen::handleEncoderDec() {
    bool ok = ApiService::postVolumeDown();
    if (!ok) Serial.println("Failed to POST volume down");
}

ScreenAction DetailScreen::handleBackRelease(uint32_t pressLengthMs) {
    if (PlayerUtils::StopIfLongPress(pressLengthMs)) {
        Serial.println("Playback stopped due to long press");
        return ScreenAction::None;
    }
    return ScreenAction::SwitchToMenu;
}

ScreenAction DetailScreen::handleDotRelease(uint32_t pressLengthMs) {
    Serial.println("POST to play/pause");
    bool ok = ApiService::postPlayPause();
    if (!ok) Serial.println("Failed to POST play/pause");
    return ScreenAction::None;
}

void DetailScreen::drawCall() {
    // Only redraw if name changed
    if (currentDetail.name != lastName) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        // Use display center so rotation / resolution doesn't place text off-screen
        int cx = tft.width() / 2;
        int cy = tft.height() / 2;
        // Use a medium built-in font for clear rendering
        const int fontNum = 2;
        tft.drawCentreString(currentDetail.name.c_str(), cx, cy - 10, fontNum);
        lastName = currentDetail.name;
    }
}

void DetailScreen::forceRedraw() {
    lastName = "";
    drawCall();
}
