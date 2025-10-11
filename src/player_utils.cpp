#include "player_utils.h"
#include "api_service.h"

bool PlayerUtils::StopIfLongPress(uint32_t pressLengthMs) {
    if (pressLengthMs < 1000) {
        return false;
    }
    bool stopOk = ApiService::postPlayerStop();
    return stopOk;
}
