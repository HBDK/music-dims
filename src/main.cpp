#include "screen_interface.h"
#include "menu_screen.h"
#include "detail_screen.h"
#include "screen_action.h"
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

MenuItem menuItems[400];
int menuCount = 0;
int menuIndex = 0;

IScreen* currentScreen = nullptr;
MenuScreen* menuScreen = nullptr;
DetailScreen* detailScreen = nullptr;


constexpr int MENU_VISIBLE = 5; // Number of items visible at once



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
  menuScreen = new MenuScreen(menuItems, menuCount, menuIndex, u8g2);
  // detailScreen = new DetailScreen(currentDetail, u8g2);
  currentScreen = menuScreen;
}


void handleEncoder()
{
  static int32_t lastTicks = 0;
  int32_t ticks = encoder.getCount();
  int32_t delta = ticks - lastTicks;
  lastTicks = ticks;

  static int32_t acc = 0;
  acc += delta;

  // Delegate encoder events to current screen
  while (acc >= 2) {
    acc -= 2;
    currentScreen->handleEncoderInc();
  }
  while (acc <= -2) {
    acc += 2;
    currentScreen->handleEncoderDec();
  }
}

void loop()
{
  handleEncoder();

  // Button: select menu item
  static bool lastDotState = false;
  static bool lastBackState = false;
  static uint32_t dotPressStart = 0;
  static uint32_t backPressStart = 0;

  // Dot button
  if (digitalRead(PIN_ENC_SW) == LOW) {
    if (!lastDotState) {
      dotPressStart = millis();
      lastDotState = true;
    }
  } else {
    if (lastDotState) {
      uint32_t pressLength = millis() - dotPressStart;
      ScreenAction action = currentScreen->handleDotRelease(pressLength);
      if (action == ScreenAction::SwitchToDetail) {
        delete detailScreen;
        detailScreen = new DetailScreen(menuItems[menuIndex], u8g2);
        currentScreen = detailScreen;
      } else if (action == ScreenAction::SwitchToMenu) {
        currentScreen = menuScreen;
      }
    }
    lastDotState = false;
    dotPressStart = 0;
  }

  // Back button
  if (digitalRead(PIN_BACK_BTN) == LOW) {
    if (!lastBackState) {
      backPressStart = millis();
      lastBackState = true;
    } 
  } else {
    if (lastBackState) {
      uint32_t pressLength = millis() - backPressStart;
      ScreenAction action = currentScreen->handleBackRelease(pressLength);
      if (action == ScreenAction::SwitchToDetail) {
        currentScreen = detailScreen;
      } else if (action == ScreenAction::SwitchToMenu) {
        currentScreen = menuScreen;
      }
    }
    lastBackState = false;
    backPressStart = 0;
  }

  currentScreen->drawCall();
  delay(10);
}
