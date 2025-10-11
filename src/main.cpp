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

#include <Adafruit_NeoPixel.h>

constexpr uint8_t PIN_LCD_RGB = 21; // Change to your actual NeoPixel data pin
constexpr uint8_t NUM_PIXELS = 3;   // Usually 1 for backlight

Adafruit_NeoPixel rgbBacklight(NUM_PIXELS, PIN_LCD_RGB, NEO_GRB + NEO_KHZ800);

// -------------------- WiFi & API Integration --------------------
// API service and MenuItem struct
#include "menu_item.h"
#include "api_service.h"

// Secrets config
#include "secrets.h"

// Dynamic menu storage
MenuItem menuItems[400]; // used for all menu levels
int menuCount = 0;
int menuIndex = 0;
String currentBackLink = "";
MenuItem currentDetail;


constexpr int MENU_VISIBLE = 5; // Number of items visible at once

void drawMenu()
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  if (!currentDetail.id.isEmpty()) {
    // Detail page
    u8g2.drawStr(0, 16, currentDetail.name.c_str());
    u8g2.sendBuffer();
    return;
  }

  if (menuCount == 0) {
    u8g2.drawStr(0, 24, "No items found!");
    u8g2.drawStr(0, 40, "Check API & WiFi");
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

void handleMenuSelect(String id) {
  MenuItem& selected = menuItems[menuIndex];
  if (selected.link.startsWith("player:")) {
    currentDetail = selected;
    Serial.print("Selected for playback: ");
    Serial.print(selected.id);
    Serial.print(", name: ");
    Serial.println(selected.name);
    bool playOk = ApiService::postPlayMedia(selected.link);
    if (!playOk) Serial.println("Failed to POST play to media endpoint");
  } else {
    // Fetch next menu level using link
    bool ok = ApiService::fetchMenuItems(menuItems, menuCount, selected.link);
    menuIndex = 0;
    currentBackLink = ApiService::backLink;
    currentDetail = MenuItem();
    Serial.print("Selected id: ");
    Serial.print(selected.id);
    Serial.print("Selected link: ");
    Serial.print(selected.link);
    Serial.print(", name: ");
    Serial.println(selected.name);
    if (!ok) Serial.println("Failed to fetch next menu!");
  }
}

void setup()
{
  Serial.begin(115200);

  rgbBacklight.begin();
  rgbBacklight.setBrightness(100); // Optional: 0-255
  rgbBacklight.setPixelColor(0, rgbBacklight.Color(32, 0, 32)); // Purple (R,G,B)
  rgbBacklight.setPixelColor(1, rgbBacklight.Color(32, 0, 32)); // Purple (R,G,B)
  rgbBacklight.setPixelColor(2, rgbBacklight.Color(128, 0, 128)); // Purple (R,G,B)
  rgbBacklight.show();

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

  ApiService::fetchMenuItems(menuItems, menuCount, "/");
}

void handleBack() {
  bool ok = ApiService::fetchMenuItems(menuItems, menuCount, currentBackLink);
  menuIndex = 0;
  currentBackLink = "";
  currentDetail = MenuItem();
  Serial.println("Back to previous menu");
  if (!ok) Serial.println("Failed to fetch previous menu!");
  return;
}

void handleEncoder()
{
  static int32_t lastTicks = 0;
  int32_t ticks = encoder.getCount();
  int32_t delta = ticks - lastTicks;
  lastTicks = ticks;

  static int32_t acc = 0;
  acc += delta;

  if (!currentDetail.id.isEmpty()) {
    // On detail page, use encoder for volume
    while (acc >= 2) {
      acc -= 2;
      bool ok = ApiService::postVolumeUp();
      if (!ok) Serial.println("Failed to POST volume up");
    }
    while (acc <= -2) {
      acc += 2;
      bool ok = ApiService::postVolumeDown();
      if (!ok) Serial.println("Failed to POST volume down");
    }
  } else {
    // Normal menu navigation
    while (acc >= 2) { acc -= 2; menuIndex++; }
    while (acc <= -2){ acc += 2; menuIndex--; }
    if (menuIndex < 0) menuIndex = menuCount - 1;
    if (menuIndex >= menuCount) menuIndex = 0;
  }
}

void loop()
{
  handleEncoder();

  // Button: select menu item
  static bool lastDotState = false;
  static bool lastBackState = false;
  static uint32_t backPressStart = 0;
  static bool longBackSent = false;

  if (debouncedButtonPressed(PIN_ENC_SW)) {
    if (!lastDotState) {
      if (currentDetail.id.isEmpty()) {
        handleMenuSelect(menuItems[menuIndex].id);
      }
      lastDotState = true;
    }
  } else {
    lastDotState = false;
  }

  // Back button: short and long press
  if (digitalRead(PIN_BACK_BTN) == LOW) {
    if (!lastBackState) {
      backPressStart = millis();
      longBackSent = false;
      lastBackState = true;
    } else {
      // If held for >1s, send stop
      if (!longBackSent && millis() - backPressStart > 1000) {
        bool stopOk = ApiService::postPlayerStop();
        if (!stopOk) Serial.println("Failed to POST stop to player endpoint");
        longBackSent = true;
      }
    }
  } else {
    if (lastBackState && !longBackSent) {
      handleBack();
    }
    lastBackState = false;
    backPressStart = 0;
    longBackSent = false;
  }

  drawMenu();
  delay(10);
}
