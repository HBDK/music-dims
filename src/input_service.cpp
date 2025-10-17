#include "input_service.h"
#include <Arduino.h>
#include <ESP32Encoder.h>

static ESP32Encoder encoder;


InputService::InputService(IScreen* screen, LightService* light)
        : lastEncoder(0), encoderDelta(0), lastDotState(false), lastBackState(false),
            dotPressStart(0), backPressStart(0),
            dotReleased(false), backReleased(false), currentScreen(screen), lightService(light){}

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

ScreenAction InputService::poll() {

    ScreenAction action = ScreenAction::None;
    handleEncoder();

    // Dot button
    bool dotState = digitalRead(PIN_ENC_SW) == LOW;
    dotReleased = false;
    if (dotState && !lastDotState) {
        dotPressStart = millis();
    } else if (!dotState && lastDotState) {
        uint32_t dotPressLength = millis() - dotPressStart;
        action = currentScreen->handleDotRelease(dotPressLength);
    }

    lastDotState = dotState;

    if (action != ScreenAction::None) {
        return action;
    }

    // Back button
    bool backState = digitalRead(PIN_BACK_BTN) == LOW;
    backReleased = false;
    if (backState && !lastBackState) {
        backPressStart = millis();
    } else if (!backState && lastBackState) {
        uint32_t backPressLength = millis() - backPressStart;
        action = currentScreen->handleBackRelease(backPressLength);
        lightService->setButtonLed(32, 0, 32);
        lightService->show();
    }

    if (backState)
    {
        uint32_t pressDuration = millis() - backPressStart;
        if (pressDuration > 1000) {
            lightService->setButtonLed(0, 0, 0);
            lightService->show();
        }
    }

    lastBackState = backState;
    return action;
}
