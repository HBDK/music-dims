#include "detail_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

DetailScreen::DetailScreen(MenuItem& detail, U8G2& display)
    : currentDetail(detail), u8g2(display) {}

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
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 16, currentDetail.name.c_str());
    u8g2.sendBuffer();
}
