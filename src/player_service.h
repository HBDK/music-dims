#pragma once
#include "api_service.h"

class PlayerService {
public:
    // Initialize service (no-op for now)
    static void begin();

    // Poll periodically; should be called from main loop with millis()
    static void pollIfNeeded(unsigned long nowMillis);

    // Get cached state; returns false if no valid state yet
    static bool getState(ApiService::PlayerState& out);

    // Last time the state was updated (millis), or 0 if never
    static unsigned long lastUpdatedMillis();

private:
    static ApiService::PlayerState cachedState;
    static unsigned long lastUpdate;
    static unsigned long lastPoll;
    static unsigned long pollIntervalMs;
    static bool hasState;
};
