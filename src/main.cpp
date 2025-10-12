#include "screen_interface.h"
#include "menu_screen.h"
#include "detail_screen.h"
#include "screen_action.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "input_service.h"
#include "light_service.h"

// -------------------- Pin Map (change if you rewired) --------------------
constexpr uint8_t PIN_LCD_SCK  = 18; // ESP32 VSPI SCK
constexpr uint8_t PIN_LCD_MOSI = 23; // ESP32 VSPI MOSI
constexpr uint8_t PIN_LCD_CS   = 5;
constexpr uint8_t PIN_LCD_DC   = 4;  // A0
constexpr uint8_t PIN_LCD_RST  = 2;
// -------------------- Display: ST7567 (BTT Mini12864 v2.0) --------------------
// Try this first (JLX glass is common). If nothing shows, try the SH1107-style
// or a different ST7567 constructor from U8g2’s list.
// U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2(
//   U8G2_R2,
//   /* cs=*/ PIN_LCD_CS,
//   /* dc=*/ PIN_LCD_DC,
//   /* reset=*/ PIN_LCD_RST
// );
// Note: HW_SPI uses the board’s default MOSI/SCK (we wired to VSPI defaults).

// -------------------- WiFi & API Integration --------------------
// API service and MenuItem struct
#include "menu_item.h"
#include "api_service.h"

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
LightService* lightService = nullptr;

constexpr int MENU_VISIBLE = 5; // Number of items visible at once

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);

  // LightService setup
  lightService = new LightService();
  lightService->begin();

  // Display
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("Connecting WiFi...", 160, 30, 2);

  // InputService setup
  menuScreen = new MenuScreen(menuItems, menuCount, menuIndex, tft);
  currentScreen = menuScreen;
  inputService = new InputService(currentScreen, lightService);
  inputService->begin();

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
  } else if (action == ScreenAction::SwitchToMenu) {
    currentScreen = menuScreen;
    inputService->currentScreen = currentScreen;
  }

  currentScreen->drawCall();
  delay(10);
}
