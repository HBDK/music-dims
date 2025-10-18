#include "screen_interface.h"
#include "menu_screen.h"
#include "detail_screen.h"
#include "screen_action.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "input_service.h"

// -------------------- WiFi & API Integration --------------------
// API service and MenuItem struct
#include "menu_item.h"
#include "api_service.h"
#include "player_service.h"

// Secrets config
#include "secrets.h"

// Dynamic menu storage

MenuItem menuItems[400];
int menuCount = 0;
int menuIndex = 0;

IScreen* currentScreen = nullptr;
MenuScreen* menuScreen = nullptr;
DetailScreen* detailScreen = nullptr;

InputService* inputService = nullptr;

constexpr int MENU_VISIBLE = 5; // Number of items visible at once

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  // Display
  tft.init();
  tft.setRotation(3); // Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("Connecting WiFi...", 160, 30, 2);

  // InputService setup
  menuScreen = new MenuScreen(menuItems, menuCount, menuIndex, tft);
  currentScreen = menuScreen;
  inputService = new InputService(currentScreen);
  inputService->begin();

  // Start player service which will poll the API periodically in the main loop
  PlayerService::begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("WiFi connected!", 160, 30, 2);
  delay(500);

  ApiService::fetchMenuItems(menuItems, menuCount, "/");
}


void loop()
{
  ScreenAction action = inputService->poll();

  if (action == ScreenAction::SwitchToDetail) {
    delete detailScreen;
    detailScreen = new DetailScreen(menuItems[menuIndex], tft);
    currentScreen = detailScreen;
    inputService->currentScreen = currentScreen;
    detailScreen->forceRedraw();
  } else if (action == ScreenAction::SwitchToMenu) {
    currentScreen = menuScreen;
    inputService->currentScreen = currentScreen;
    menuScreen->forceRedraw();
  }

  unsigned long now = millis();
  // Poll global PlayerService (will update cached player state)
  PlayerService::pollIfNeeded(now);
  // Let the detail screen react to the cached state when active
  if (currentScreen == detailScreen && detailScreen != nullptr) {
    detailScreen->pollIfNeeded(now);
  }

  currentScreen->drawCall();
  delay(10);
}
