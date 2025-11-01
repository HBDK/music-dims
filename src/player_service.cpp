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
    // nothing to init for now
}

bool PlayerService::StopPlayback() {
    bool stopOk = ApiService::postPlayerStop();
    return stopOk;
}

void PlayerService::pollIfNeeded(unsigned long nowMillis) {
    if (nowMillis - lastPoll < pollIntervalMs) return;
    lastPoll = nowMillis;

    ApiService::PlayerState st;
    bool ok = ApiService::getPlayerState(st);
    if (!ok) return;

    // Update cached state and track lastUpdate
    // Detect transition into idle/off state and record when it first occurred
    if (!hasState) {
        // No previous state known
        if (st.isIdleOrOff()) {
            lastIdleOrOff = nowMillis;
        } else {
            lastIdleOrOff = 0;
        }
    } else {
        bool prevIdle = cachedState.isIdleOrOff();
        bool nowIdle = st.isIdleOrOff();
        if (!prevIdle && nowIdle) {
            // Just transitioned into idle/off
            lastIdleOrOff = nowMillis;
        } else if (!nowIdle) {
            // Not idle/off now — clear the idle timestamp
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
