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
    // Read state from PlayerService; if none available, show placeholder
    ApiService::PlayerState st;
    bool ok = PlayerService::getState(st);

    String title = ok ? st.title : String("None");
    String artist = ok ? st.artist : String("");
    String album = (ok && st.album.length()) ? st.album : currentDetail.name;

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
    // DetailScreen no longer polls the API directly; PlayerService polls
    // globally. However, the screen can trigger a redraw if the global
    // state has changed since last draw.
    // We simply call drawCall() which will fetch the latest cached state
    // and redraw if needed.
    (void)nowMillis;
    drawCall();
}

