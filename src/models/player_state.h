#include <WString.h>

class PlayerState {
  public:
    String state;
    int volume = 0;
    bool muted = false;
    String title;
    String artist;
    String album;
    String pictureUrl;

    bool isIdleOrOff() const {
      return state == "idle" || state == "off";
    }
};