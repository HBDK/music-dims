#include "player_service.h"
#include <Arduino.h>

ApiService::PlayerState PlayerService::cachedState;
unsigned long PlayerService::lastUpdate = 0;
unsigned long PlayerService::lastPoll = 0;
unsigned long PlayerService::pollIntervalMs = 5000; // 2s default
bool PlayerService::hasState = false;
unsigned long PlayerService::lastIdleOrOff = 0;

const ApiService::PlayerState* PlayerService::getCachedStatePtr() {
    if (!hasState) return nullptr;
    return &cachedState;
}

void PlayerService::begin() {
}

bool PlayerService::StopPlayback() {
    bool stopOk = ApiService::postPlayerStop();
    return stopOk;
}

bool PlayerService::playMedia(const String& link) {
    bool ok = ApiService::postPlayMedia(link);
    if (ok) {
        lastIdleOrOff = millis();
    }
    return ok;
}

void PlayerService::pollIfNeeded(unsigned long nowMillis) {
    if (nowMillis - lastPoll < pollIntervalMs) return;
    lastPoll = nowMillis;

    ApiService::PlayerState st;
    bool ok = ApiService::getPlayerState(st);
    if (!ok) return;

    if (!hasState) {
        if (st.isIdleOrOff()) {
            lastIdleOrOff = nowMillis;
        } else {
            lastIdleOrOff = 0;
        }
    } else {
        bool prevIdle = cachedState.isIdleOrOff();
        bool nowIdle = st.isIdleOrOff();
        if (!prevIdle && nowIdle) {
            lastIdleOrOff = nowMillis;
        } else if (!nowIdle) {
            lastIdleOrOff = 0;
        }
    }

    cachedState = st;
    lastUpdate = nowMillis;
    hasState = true;
}

bool PlayerService::getState(ApiService::PlayerState& out) {
    if (!hasState) return false;
    out = cachedState;
    return true;
}

unsigned long PlayerService::lastUpdatedMillis() {
    return lastUpdate;
}

unsigned long PlayerService::lastIdleOrOffMillis() {
    return lastIdleOrOff;
}
