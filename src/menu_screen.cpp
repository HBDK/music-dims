#include "menu_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"
// Use display dimensions defined in User_Setup.h (TFT_eSPI)
#include "User_Setup.h"

MenuScreen::MenuScreen(MenuItem* items, int& count, int& index, TFT_eSPI& display)
    : menuItems(items), menuCount(count), menuIndex(index), tft(display), lastMenuIndex(-1), lastMenuCount(-1), backIndex(0) {}

void MenuScreen::handleEncoderInc() {
    menuIndex++;
    if (menuIndex >= menuCount) menuIndex = 0;
}

void MenuScreen::handleEncoderDec() {
    menuIndex--;
    if (menuIndex < 0) menuIndex = menuCount - 1;
}

ScreenAction MenuScreen::handleBackRelease(uint32_t pressLengthMs) {
    if (PlayerUtils::StopIfLongPress(pressLengthMs)) {
        return ScreenAction::None;
    }
    bool ok = ApiService::fetchMenuItems(menuItems, menuCount, ApiService::backLink);
    menuIndex = menuCount > backIndex ? backIndex : 0;
    return ScreenAction::SwitchToMenu;
}

ScreenAction MenuScreen::handleDotRelease(uint32_t pressLengthMs) {
    MenuItem& selected = menuItems[menuIndex];
    if (selected.link.startsWith("player:")) {
        bool playOk = ApiService::postPlayMedia(selected.link);
        backIndex = menuIndex;
        return ScreenAction::SwitchToDetail;
    } else {
        bool ok = ApiService::fetchMenuItems(menuItems, menuCount, selected.link);
        backIndex = menuIndex;
        menuIndex = 0;
        return ScreenAction::None;
    }
}

void MenuScreen::drawError() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    // swapped: use TFT_HEIGHT where code previously used TFT_WIDTH, and vice-versa
    int cx = TFT_HEIGHT / 2;
    int y1 = TFT_WIDTH / 8;
    tft.drawCentreString("No items found!", cx, y1, 2);
    tft.drawCentreString("Check API & WiFi", cx, y1 + (TFT_WIDTH / 16), 2);
}

// Helper: Truncate long names for menu display
String fitMenuName(const String& name, int maxChars) {
    if (name.length() > maxChars) {
        return name.substring(0, maxChars - 3) + "...";
    }
    return name;
}

void MenuScreen::drawCall() {
    // Only redraw if menuIndex or menuCount changed
    if (menuIndex != lastMenuIndex || menuCount != lastMenuCount) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        if (menuCount == 0) {
            drawError();
        } else {
            // Layout computed from screen size so it adapts to different displays
            // swapped: use width/height macros oppositely to match orientation mapping
            const int topMargin = TFT_WIDTH / 12; // ~40 on 480px high when swapped
            const int itemHeight = TFT_WIDTH / 12; // spacing between items
            const int visibleCount = min(5, max(1, TFT_WIDTH / itemHeight));
            int scrollStart = menuIndex - (visibleCount / 2);
            if (scrollStart < 0) scrollStart = 0;
            if (scrollStart > menuCount - visibleCount) scrollStart = menuCount - visibleCount;
            if (scrollStart < 0) scrollStart = 0;
            int maxChars = max(10, TFT_HEIGHT / 14); // rough estimate of chars per line (swapped)
            int rectHeight = max(8, itemHeight - 8);
            for (int i = 0; i < visibleCount && (scrollStart + i) < menuCount; ++i) {
                int y = topMargin + i * itemHeight;
                int itemIdx = scrollStart + i;
                String displayName = fitMenuName(menuItems[itemIdx].name, maxChars);
                if (itemIdx == menuIndex) {
                    tft.fillRect(0, y - 8, TFT_HEIGHT, rectHeight, TFT_BLUE);
                    tft.setTextColor(TFT_WHITE, TFT_BLUE);
                } else {
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                }
                tft.setCursor(10, y);
                tft.print(displayName.c_str());
            }
        }
        lastMenuIndex = menuIndex;
        lastMenuCount = menuCount;
    }
}

void MenuScreen::forceRedraw() {
    lastMenuIndex = -1;
    lastMenuCount = -1;
    drawCall();
}
