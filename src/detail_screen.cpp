#include "detail_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"
#include "player_service.h"

DetailScreen::DetailScreen(MenuItem& detail, TFT_eSPI& display)
    : currentDetail(detail), tft(display) {}

void DetailScreen::handleEncoderInc() {
    bool ok = ApiService::postVolumeUp();
    lastPoll = 4000;
}

void DetailScreen::handleEncoderDec() {
    bool ok = ApiService::postVolumeDown();
    lastPoll = 4000;
}

ScreenAction DetailScreen::handleBackShortPress() {
    return ScreenAction::SwitchToMenu;
}

ScreenAction DetailScreen::handleBackLongPress() {
    PlayerUtils::StopIfLongPress(1000);
    return ScreenAction::None;
}

ScreenAction DetailScreen::handleDotShortPress() {
    bool ok = ApiService::postPlayPause();
    return ScreenAction::None;
}

ScreenAction DetailScreen::handleDotLongPress() {
    return ScreenAction::None;
}

void DetailScreen::drawCall() {
    ApiService::PlayerState st;
    bool ok = PlayerService::getState(st);

    String title = ok ? st.title : String("None");
    String artist = ok ? st.artist : String("");
    String album = (ok && st.album.length()) ? st.album : currentDetail.name;

    if (title != lastTitle) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        const int titleFont = 1;
        const int subFont = 1;
        int cx = tft.width() / 2;
        int y = tft.height() / 2 - 16;
        tft.drawCentreString(title.c_str(), cx, y, titleFont);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawCentreString(artist.c_str(), cx, y + 22, subFont);
        tft.drawCentreString(album.c_str(), cx, y + 44, subFont);

        String volStr = ok && st.muted ? String("MUTE") : (ok ? String("Vol:") + String(st.volume) : String("Vol:?"));
        int volFont = 1;
        int volW = tft.textWidth(volStr);
        int volX = tft.width() - 10 - volW;
        tft.setTextColor(TFT_CYAN, TFT_BLACK);
        tft.drawString(volStr.c_str(), volX, 8, volFont);

        lastTitle = title;
    }
}

void DetailScreen::forceRedraw() {
    lastTitle = "None";
    drawCall();
}

void DetailScreen::pollIfNeeded(unsigned long nowMillis) {
    (void)nowMillis;
    drawCall();
}

