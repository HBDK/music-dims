#include "detail_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

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

ScreenAction DetailScreen::handleBackRelease(uint32_t pressLengthMs) {
    if (PlayerUtils::StopIfLongPress(pressLengthMs)) {
        return ScreenAction::None;
    }
    return ScreenAction::SwitchToMenu;
}

ScreenAction DetailScreen::handleDotRelease(uint32_t pressLengthMs) {
    bool ok = ApiService::postPlayPause();
    return ScreenAction::None;
}

void DetailScreen::drawCall() {
    // Draw detail view: title, artist, album - only redraw when content changes
    String title = cachedState.title;
    String artist = cachedState.artist;
    String album = cachedState.album.length() ? cachedState.album : currentDetail.name;

    if (title != lastTitle) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        // smaller title font
        const int titleFont = 1;
        const int subFont = 1;
        int cx = tft.width() / 2;
        int y = tft.height() / 2 - 16;
        tft.drawCentreString(title.c_str(), cx, y, titleFont);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        // tighter spacing for artist/album
        tft.drawCentreString(artist.c_str(), cx, y + 22, subFont);
        tft.drawCentreString(album.c_str(), cx, y + 44, subFont);

        // Draw volume / muted state in top-right
        String volStr = cachedState.muted ? String("MUTE") : String("Vol:") + String(cachedState.volume);
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
    if (nowMillis - lastPoll < pollIntervalMs) return;
    lastPoll = nowMillis;

    ApiService::PlayerState st;
    bool ok = ApiService::getPlayerState(st);
    if (!ok) return; // keep previous cachedState

    // Update cached state and force redraw when title/artist/album/state/volume/muted changed
    bool changed = false;
    if (st.title != cachedState.title) changed = true;
    if (st.artist != cachedState.artist) changed = true;
    if (st.album != cachedState.album) changed = true;
    if (st.state != cachedState.state) changed = true;
    if (st.volume != cachedState.volume) changed = true;
    if (st.muted != cachedState.muted) changed = true;

    cachedState = st;
    if (changed) {
        forceRedraw();
    }
}

