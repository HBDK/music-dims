#include "menu_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

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
    tft.drawCentreString("No items found!", 160, 60, 2);
    tft.drawCentreString("Check API & WiFi", 160, 100, 2);
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
            int scrollStart = menuIndex - 2;
            if (scrollStart < 0) scrollStart = 0;
            if (scrollStart > menuCount - 5) scrollStart = menuCount - 5;
            if (scrollStart < 0) scrollStart = 0;
            for (int i = 0; i < 5 && (scrollStart + i) < menuCount; ++i) {
                int y = 40 + i * 40;
                int itemIdx = scrollStart + i;
                String displayName = fitMenuName(menuItems[itemIdx].name, 22); // 22 chars max
                if (itemIdx == menuIndex) {
                    tft.fillRect(0, y - 8, 320, 32, TFT_BLUE);
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
