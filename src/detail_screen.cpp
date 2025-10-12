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
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(3);
    tft.drawCentreString(currentDetail.name.c_str(), 160, 100, 3);
}
