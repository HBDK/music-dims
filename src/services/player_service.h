#pragma once
#include "api_service.h"

class PlayerService {
public:
    static void begin();
    static void pollIfNeeded(unsigned long nowMillis);
    static bool getState(PlayerState& out);
    static unsigned long lastUpdatedMillis();
    static bool StopPlayback();
    static unsigned long lastIdleOrOffMillis();
    static bool playMedia(const String& link);
    static const PlayerState* getCachedStatePtr();

private:
    static PlayerState cachedState;
    static unsigned long lastUpdate;
    static unsigned long lastPoll;
    static unsigned long pollIntervalMs;
    static unsigned long lastIdleOrOff;
    static bool hasState;
};
