
#include "menu_screen.h"
#include "screen_action.h"
#include "api_service.h"
#include <Arduino.h>
#include "player_utils.h"

MenuScreen::MenuScreen(MenuItem* items, int& count, int& index, U8G2& display)
    : menuItems(items), menuCount(count), menuIndex(index), u8g2(display) {}

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
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 24, "No items found!");
    u8g2.drawStr(0, 40, "Check API & WiFi");
    u8g2.sendBuffer();
}

void MenuScreen::drawCall() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    if (menuCount == 0) {
        drawError();
    } else {
        int scrollStart = menuIndex - 2;
        if (scrollStart < 0) scrollStart = 0;
        if (scrollStart > menuCount - 5) scrollStart = menuCount - 5;
        if (scrollStart < 0) scrollStart = 0;
        for (int i = 0; i < 5 && (scrollStart + i) < menuCount; ++i) {
            int y = 14 + i * 12;
            int itemIdx = scrollStart + i;
            if (itemIdx == menuIndex) {
                u8g2.drawBox(0, y - 9, 128, 11);
                u8g2.setDrawColor(0);
                u8g2.drawStr(4, y, menuItems[itemIdx].name.c_str());
                u8g2.setDrawColor(1);
            } else {
                u8g2.drawStr(4, y, menuItems[itemIdx].name.c_str());
            }
        }
    }
    u8g2.sendBuffer();
}
