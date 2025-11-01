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

    // Attempt to stop playback (previously in PlayerUtils::StopIfLongPress).
    // Returns true if the stop command succeeded.
    static bool StopPlayback();
    
    // Timestamp (millis) when the player state first became "idle" or "off".
    // Zero means we currently don't consider the player idle/off.
    static unsigned long lastIdleOrOffMillis();

    // Play media by link (delegates to ApiService). Returns true on success.
    static bool playMedia(const String& link);
    // Return pointer to the cached state (nullptr if not available).
    static const ApiService::PlayerState* getCachedStatePtr();

private:
    static ApiService::PlayerState cachedState;
    static unsigned long lastUpdate;
    static unsigned long lastPoll;
    static unsigned long pollIntervalMs;
    static unsigned long lastIdleOrOff;
    static bool hasState;
};
