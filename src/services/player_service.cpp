#include "player_service.h"
#include <Arduino.h>
#include "api_service.h"

PlayerState PlayerService::cachedState;
unsigned long PlayerService::lastUpdate = 0;
unsigned long PlayerService::lastPoll = 0;
unsigned long PlayerService::pollIntervalMs = 5000;
bool PlayerService::hasState = false;
unsigned long PlayerService::lastIdleOrOff = 0;

void PlayerService::begin() {
    // nothing to init for now
}

bool PlayerService::StopPlayback() {
    bool stopOk = ApiService::postPlayerStop();
    return stopOk;
}

bool PlayerService::playMedia(const String& link) {
    bool ok = ApiService::postPlayMedia(link);
    if (ok) {
        lastIdleOrOff = 0;
    }
    return ok;
}

void PlayerService::pollIfNeeded(unsigned long nowMillis) {
    if (nowMillis - lastPoll < pollIntervalMs) return;
    lastPoll = nowMillis;

    PlayerState st;
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

bool PlayerService::getState(PlayerState& out) {
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

const PlayerState* PlayerService::getCachedStatePtr() {
    if (!hasState) return nullptr;
    return &cachedState;
}
