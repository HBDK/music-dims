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

constexpr uint8_t PIN_BUZZER   = 26; // optional
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

// -------------------- UI State --------------------
int menuIndex = 0;
const char* items[] = {"Status", "Settings", "Info", "Beep", "test"};
constexpr int ITEM_COUNT = sizeof(items)/sizeof(items[0]);

void drawUI()
{
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x10_tf);

  // Menu
  for (int i = 0; i < ITEM_COUNT; ++i) {
    int y = 14 + i * 12;
    if (i == menuIndex) {
      u8g2.drawBox(0, y - 9, 128, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(4, y, items[i]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.drawStr(4, y, items[i]);
    }
  }

  u8g2.sendBuffer();
}

void setup()
{
  // Pins
  pinMode(PIN_ENC_SW, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // Encoder (ESP32Encoder)
  ESP32Encoder::useInternalWeakPullResistors = puType::none;
  encoder.attachHalfQuad(PIN_ENC_A, PIN_ENC_B);
  encoder.setCount(0);

  // Display
  // If you see nothing, try U8G2_R2 rotation or a different ST7567 constructor.
  u8g2.begin();
  u8g2.setContrast(180);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 12, "Booting...");
  u8g2.sendBuffer();

  // ...existing code...
}

void loop()
{
  // Handle encoder movement (coalesce ticks → steps)
  static int32_t lastTicks = 0;
  int32_t ticks = encoder.getCount();
  int32_t delta = ticks - lastTicks;
  lastTicks = ticks;

  // Many encoders yield 4 ticks per detent; adjust as needed
  static int32_t acc = 0;
  acc += delta;
  while (acc >= 2) { acc -= 2; menuIndex++; }
  while (acc <= -2){ acc += 2; menuIndex--; }
  if (menuIndex < 0) menuIndex = ITEM_COUNT - 1;
  if (menuIndex >= ITEM_COUNT) menuIndex = 0;

  // Button
  if (buttonPressed()) {
    if (menuIndex == 4) {
      // Beep
      digitalWrite(PIN_BUZZER, HIGH);
      delay(60);
      digitalWrite(PIN_BUZZER, LOW);
    }
  }

  drawUI();
  delay(10);
}
