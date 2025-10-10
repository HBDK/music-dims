#include <Arduino.h>
#include <U8g2lib.h>
// ...existing code...

// -------------------- Pin Map (change if you rewired) --------------------
constexpr uint8_t PIN_LCD_SCK  = 18; // ESP32 VSPI SCK
constexpr uint8_t PIN_LCD_MOSI = 23; // ESP32 VSPI MOSI
constexpr uint8_t PIN_LCD_CS   = 5;
constexpr uint8_t PIN_LCD_DC   = 4;  // A0
constexpr uint8_t PIN_LCD_RST  = 2;

constexpr uint8_t PIN_ENC_A    = 32;
constexpr uint8_t PIN_ENC_B    = 33;
constexpr uint8_t PIN_ENC_SW   = 25;

// ...existing code...
// ...existing code...

// -------------------- Display: ST7567 (BTT Mini12864 v2.0) --------------------
// Try this first (JLX glass is common). If nothing shows, try the SH1107-style
// or a different ST7567 constructor from U8g2’s list.
U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2(
  U8G2_R2,          // rotation
  /* cs=*/ PIN_LCD_CS,
  /* dc=*/ PIN_LCD_DC,
  /* reset=*/ PIN_LCD_RST
);
// Note: HW_SPI uses the board’s default MOSI/SCK (we wired to VSPI defaults).

// ...existing code...

// -------------------- Encoder (ESP32Encoder) --------------------
#include <ESP32Encoder.h>
ESP32Encoder encoder;

bool buttonPressed()
{
  static uint32_t tLast = 0;
  static bool last = HIGH;
  bool now = digitalRead(PIN_ENC_SW);
  if (now != last) {
    last = now;
    tLast = millis();
  }
  // crude debounce
  if (millis() - tLast > 20 && now == LOW) return true;
  return false;
}

// -------------------- WiFi & API Integration --------------------
// API service and MenuItem struct
#include "api_service.h"

// Secrets config
#include "secrets.h"

String apiArtistsUrl = String(apiHost) + "/artists";

// Dynamic menu storage
MenuItem menuItems[400]; // used for both artists and albums
int menuCount = 0;
int menuIndex = 0;
bool dotVisible = false;
bool showingAlbums = false;


constexpr int MENU_VISIBLE = 5; // Number of items visible at once

void drawMenu()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  if (menuCount == 0) {
    u8g2.drawStr(0, 24, showingAlbums ? "No albums found!" : "No artists found!");
    if (!showingAlbums) u8g2.drawStr(0, 40, "Check API & WiFi");
  } else {
    int scrollStart = menuIndex - MENU_VISIBLE/2;
    if (scrollStart < 0) scrollStart = 0;
    if (scrollStart > menuCount - MENU_VISIBLE) scrollStart = menuCount - MENU_VISIBLE;
    if (scrollStart < 0) scrollStart = 0;
    for (int i = 0; i < MENU_VISIBLE && (scrollStart + i) < menuCount; ++i) {
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
    if (dotVisible && !showingAlbums) {
      u8g2.drawDisc(124, 6, 3, U8G2_DRAW_ALL);
    }
  }
  u8g2.sendBuffer();
}

void handleMenuSelect(int id) {
  if (!showingAlbums) {
    // Fetch albums and overwrite menuItems
    String albumsUrl = String(apiHost) + "/artists/" + String(id) + "/albums";
    bool ok = ApiService::fetchAlbums(menuItems, menuCount, albumsUrl);
    menuIndex = 0;
    showingAlbums = true;
    Serial.print("Selected artist id: ");
    Serial.print(id);
    Serial.print(", name: ");
    Serial.println(menuItems[menuIndex].name);
    if (!ok) Serial.println("Failed to fetch albums!");
  } else {
    // Return to artist list
    ApiService::fetchArtists(menuItems, menuCount, apiArtistsUrl);
    menuIndex = 0;
    showingAlbums = false;
    Serial.print("Selected album id: ");
    Serial.print(menuItems[menuIndex].id);
    Serial.print(", name: ");
    Serial.println(menuItems[menuIndex].name);
  }
}

void setup()
{
  Serial.begin(115200);

  // Pins
  pinMode(PIN_ENC_SW, INPUT_PULLUP);

  // Encoder (ESP32Encoder)
  ESP32Encoder::useInternalWeakPullResistors = puType::none;
  encoder.attachHalfQuad(PIN_ENC_A, PIN_ENC_B);
  encoder.setCount(0);

  // Display
  u8g2.begin();
  u8g2.setContrast(180);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 12, "Connecting WiFi...");
  u8g2.sendBuffer();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 12, "WiFi connected!");
  u8g2.sendBuffer();
  delay(500);

  ApiService::fetchArtists(menuItems, menuCount, apiArtistsUrl);
}


void handleEncoder()
{
  static int32_t lastTicks = 0;
  int32_t ticks = encoder.getCount();
  int32_t delta = ticks - lastTicks;
  lastTicks = ticks;

  static int32_t acc = 0;
  acc += delta;
  while (acc >= 2) { acc -= 2; menuIndex++; }
  while (acc <= -2){ acc += 2; menuIndex--; }
  if (menuIndex < 0) menuIndex = menuCount - 1;
  if (menuIndex >= menuCount) menuIndex = 0;
}

void loop()
{
  handleEncoder();

  // Button: select menu item
  static bool lastDotState = false;
  if (buttonPressed()) {
    if (!lastDotState) {
      handleMenuSelect(menuItems[menuIndex].id);
      lastDotState = true;
    }
  } else {
    lastDotState = false;
  }

  drawMenu();
  delay(10);
}
