#include "menu_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

MenuScreen::MenuScreen(MenuItem* items, int& count, int& index, TFT_eSPI& display)
    : menuItems(items), menuCount(count), menuIndex(index), tft(display) {}

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
        Serial.println("Playback stopped due to long press");
        return ScreenAction::None;
    }
    bool ok = ApiService::fetchMenuItems(menuItems, menuCount, ApiService::backLink);
    menuIndex = 0;
    if (!ok) Serial.println("Failed to fetch previous menu!");
    return ScreenAction::SwitchToMenu;
}

ScreenAction MenuScreen::handleDotRelease(uint32_t pressLengthMs) {
    MenuItem& selected = menuItems[menuIndex];
    if (selected.link.startsWith("player:")) {
        Serial.print("Selected for playback: ");
        Serial.print(selected.id);
        Serial.print(", name: ");
        Serial.println(selected.name);
        bool playOk = ApiService::postPlayMedia(selected.link);
        if (!playOk) Serial.println("Failed to POST play to media endpoint");
        return ScreenAction::SwitchToDetail;
    } else {
        bool ok = ApiService::fetchMenuItems(menuItems, menuCount, selected.link);
        menuIndex = 0;
        Serial.print("Selected id: ");
        Serial.print(selected.id);
        Serial.print(", name: ");
        Serial.println(selected.name);
        if (!ok) Serial.println("Failed to fetch next menu!");
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

void MenuScreen::drawCall() {
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
            if (itemIdx == menuIndex) {
                tft.fillRect(0, y - 8, 320, 32, TFT_BLUE);
                tft.setTextColor(TFT_WHITE, TFT_BLUE);
            } else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.setCursor(10, y);
            tft.print(menuItems[itemIdx].name.c_str());
        }
    }
}
