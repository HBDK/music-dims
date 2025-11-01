#include "detail_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
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
    PlayerService::StopPlayback();
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
    const ApiService::PlayerState* pst = PlayerService::getCachedStatePtr();

    String title = pst ? pst->title : String("None");
    String artist = pst ? pst->artist : String("");
    String album = (pst && pst->album.length()) ? pst->album : currentDetail.name;

    String volStr = (pst && pst->muted) ? String("MUTE") : (pst ? String("Vol:") + String(pst->volume) : String("Vol:?"));

    if (title != lastTitle) {
        drawHeader(title, artist, album);
        drawVolume(volStr);
    }

    if (volStr != lastVolStr) {
        drawVolume(volStr);
    }
}

void DetailScreen::drawHeader(const String& title, const String& artist, const String& album) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    const int titleFont = 1;
    const int subFont = 1;
    int cx = tft.width() / 2;
    int y = tft.height() / 2 - 16;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int cursor = tft.height() - 24;
    drawAndMoveCusror(album, cursor, subFont, TFT_WHITE, cx);
    drawAndMoveCusror(artist, cursor, subFont, TFT_WHITE, cx);
    drawAndMoveCusror(title, cursor, titleFont, TFT_YELLOW, cx);

    lastTitle = title;
}
void DetailScreen::drawAndMoveCusror(const String& text, int& cursorY, int font, uint32_t color, int cx) {
    if (text.length() == 0) return;

    tft.setTextColor(color, TFT_BLACK);
    tft.drawCentreString(text.c_str(), cx, cursorY, font);
    cursorY -= (tft.fontHeight(font) + 4);
}

void DetailScreen::drawVolume(const String& volStr) {
    int volFont = 1;
    int volW = tft.textWidth(volStr);
    int volX = tft.width() - 10 - volW;
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString(volStr.c_str(), volX, 8, volFont);

    lastVolStr = volStr;
}

void DetailScreen::forceRedraw() {
    lastTitle = "None";
    lastVolStr = "None";
    drawCall();
}

ScreenAction DetailScreen::poll() {
    const int thresholdMs = 300000;
    const ApiService::PlayerState* pst = PlayerService::getCachedStatePtr();
    unsigned long idleAt = PlayerService::lastIdleOrOffMillis();
    if (pst && pst->isIdleOrOff() && idleAt != 0 && (millis() - idleAt) >= thresholdMs) {
        return ScreenAction::SwitchToMenu;
    }
    return ScreenAction::None;
}