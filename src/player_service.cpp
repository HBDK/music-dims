#include "player_service.h"
#include <Arduino.h>

ApiService::PlayerState PlayerService::cachedState;
unsigned long PlayerService::lastUpdate = 0;
unsigned long PlayerService::lastPoll = 0;
unsigned long PlayerService::pollIntervalMs = 5000; // 2s default
bool PlayerService::hasState = false;

void PlayerService::begin() {
    // nothing to init for now
}

void PlayerService::pollIfNeeded(unsigned long nowMillis) {
    if (nowMillis - lastPoll < pollIntervalMs) return;
    lastPoll = nowMillis;

    ApiService::PlayerState st;
    bool ok = ApiService::getPlayerState(st);
    if (!ok) return;

    // Update cached state and track lastUpdate
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
