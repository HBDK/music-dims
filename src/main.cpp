#include <Arduino.h>
#include <U8g2lib.h>

// -------------------- Pin Map (change if you rewired) --------------------
constexpr uint8_t PIN_LCD_SCK  = 18; // ESP32 VSPI SCK
constexpr uint8_t PIN_LCD_MOSI = 23; // ESP32 VSPI MOSI
constexpr uint8_t PIN_LCD_CS   = 5;
constexpr uint8_t PIN_LCD_DC   = 4;  // A0
constexpr uint8_t PIN_LCD_RST  = 2;

constexpr uint8_t PIN_ENC_A    = 32;
constexpr uint8_t PIN_ENC_B    = 33;
constexpr uint8_t PIN_ENC_SW   = 25;
constexpr uint8_t PIN_BACK_BTN = 27;
// -------------------- Display: ST7567 (BTT Mini12864 v2.0) --------------------
// Try this first (JLX glass is common). If nothing shows, try the SH1107-style
// or a different ST7567 constructor from U8g2’s list.
U8G2_ST7567_JLX12864_F_4W_HW_SPI u8g2(
  U8G2_R2,
  /* cs=*/ PIN_LCD_CS,
  /* dc=*/ PIN_LCD_DC,
  /* reset=*/ PIN_LCD_RST
);
// Note: HW_SPI uses the board’s default MOSI/SCK (we wired to VSPI defaults).

// ...existing code...

// -------------------- Encoder (ESP32Encoder) --------------------
#include <ESP32Encoder.h>
ESP32Encoder encoder;

bool debouncedButtonPressed(uint8_t pin) {
  static uint32_t tLast[32] = {0}; // Support up to 32 pins
  static bool last[32] = {HIGH};
  bool now = digitalRead(pin);
  if (now != last[pin]) {
    last[pin] = now;
    tLast[pin] = millis();
  }
  if (millis() - tLast[pin] > 20 && now == LOW) return true;
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
bool showingAlbums = false;

// Album detail page state
bool showingAlbumDetail = false;
MenuItem currentAlbum;
MenuItem currentArtist;


constexpr int MENU_VISIBLE = 5; // Number of items visible at once

void drawMenu()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  if (showingAlbumDetail) {
    // Album detail page
    u8g2.drawStr(0, 16, currentArtist.name.c_str());
    u8g2.drawStr(0, 32, currentAlbum.name.c_str());
    u8g2.sendBuffer();
    return;
  }

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
  }
  u8g2.sendBuffer();
}

void handleMenuSelect(int id) {
  if (!showingAlbums) {
    // Fetch albums and overwrite menuItems
  bool ok = ApiService::fetchAlbums(menuItems, menuCount, id);
    menuIndex = 0;
    showingAlbums = true;
    // Save current artist info for detail page
    currentArtist.id = id;
    currentArtist.name = menuItems[menuIndex].name;
    Serial.print("Selected artist id: ");
    Serial.print(id);
    Serial.print(", name: ");
    Serial.println(menuItems[menuIndex].name);
    if (!ok) Serial.println("Failed to fetch albums!");
  } else {
    // Show album detail page
    currentAlbum.id = menuItems[menuIndex].id;
    currentAlbum.name = menuItems[menuIndex].name;
    showingAlbumDetail = true;
    Serial.print("Selected album id: ");
    Serial.print(currentAlbum.id);
    Serial.print(", name: ");
    Serial.println(currentAlbum.name);
  }
}

void setup()
{
  Serial.begin(115200);

  // Pins
  pinMode(PIN_ENC_SW, INPUT_PULLUP);
  pinMode(PIN_BACK_BTN, INPUT_PULLUP);

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

  ApiService::fetchArtists(menuItems, menuCount);
}

void handleBack() {
  if (showingAlbumDetail) {
    // Go back to album list for current artist
  bool ok = ApiService::fetchAlbums(menuItems, menuCount, currentArtist.id);
    menuIndex = 0;
    showingAlbumDetail = false;
    showingAlbums = true;
    Serial.println("Back to album list");
    if (!ok) Serial.println("Failed to fetch albums!");
    return;
  }
  // Default: go back to artist menu
  ApiService::fetchArtists(menuItems, menuCount);
  menuIndex = 0;
  showingAlbums = false;
  Serial.println("Back to artist menu");
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
  static bool lastBackState = false;
  if (debouncedButtonPressed(PIN_ENC_SW)) {
    if (!lastDotState) {
      if (!showingAlbumDetail) {
        handleMenuSelect(menuItems[menuIndex].id);
      }
      lastDotState = true;
    }
  } else {
    lastDotState = false;
  }

  // Back button: go back to previous menu
  if (debouncedButtonPressed(PIN_BACK_BTN)) {
    if (!lastBackState && (showingAlbums || showingAlbumDetail)) {
      handleBack();
      lastBackState = true;
    }
  } else {
    lastBackState = false;
  }

  drawMenu();
  delay(10);
}
