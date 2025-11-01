#include <Arduino.h>
#include <ESP32Encoder.h>
#include "input_service.h"

static ESP32Encoder encoder;


InputService::InputService(IScreen* screen)
        : lastEncoder(0), encoderDelta(0), lastDotState(false), lastBackState(false),
            dotPressStart(0), backPressStart(0),
            dotReleased(false), backReleased(false), currentScreen(screen) {}

void InputService::begin() {
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BACK_BTN, INPUT_PULLUP);
    ESP32Encoder::useInternalWeakPullResistors = puType::none;
    encoder.attachHalfQuad(PIN_ENC_A, PIN_ENC_B);
    encoder.setCount(0);
    lastEncoder = encoder.getCount();
}

void InputService::handleEncoder()
{
  int32_t current = encoder.getCount();
  int32_t delta = current - lastEncoder;
  lastEncoder = current;

  static int32_t acc = 0;
  acc += delta;

  // Delegate encoder events to current screen
  while (acc >= IMPULSES_PER_CLICK) {
    acc -= IMPULSES_PER_CLICK;
    currentScreen->handleEncoderInc();
  }
  while (acc <= -IMPULSES_PER_CLICK) {
    acc += IMPULSES_PER_CLICK;
    currentScreen->handleEncoderDec();
  }
}

ScreenAction InputService::handleButton(uint8_t pin, bool &lastState, uint32_t &pressStart, bool &released,
                                        ScreenAction (IScreen::*shortHandler)(), ScreenAction (IScreen::*longHandler)()) {

    const int longPressThresholdMs = 1500;
    ScreenAction action = ScreenAction::None;
    bool state = digitalRead(pin) == LOW;
    if (state && !lastState) {
        pressStart = millis();
        released = false;
    }

    if (state && lastState) {
        uint32_t pressLength = millis() - pressStart;
        if (!released && pressLength >= longPressThresholdMs) {
            action = (currentScreen->*longHandler)();
            released = true;
        }
    }

    if (!state && lastState) {
        if (!released) {
            action = (currentScreen->*shortHandler)();
        }
        released = false;
    }

    lastState = state;
    return action;
}

ScreenAction InputService::poll() {

    ScreenAction action = ScreenAction::None;
    handleEncoder();

    action = handleButton(PIN_ENC_SW, lastDotState, dotPressStart, dotReleased,
                          &IScreen::handleDotShortPress, &IScreen::handleDotLongPress);

    if (action != ScreenAction::None) {
        return action;
    }

    action = handleButton(PIN_BACK_BTN, lastBackState, backPressStart, backReleased,
                          &IScreen::handleBackShortPress, &IScreen::handleBackLongPress);

    return action;
}
