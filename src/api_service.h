#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "menu_item.h"

class ApiService {
public:
  struct PlayerState {
    String state;
    int volume = 0;
    bool muted = false;
    String title;
    String artist;
    String album;
    String pictureUrl;
  };

  // Fetch current player state for the configured player.
  // NOTE: This uses the `playerName` value from `secrets.h` (your configured
  // player in secrets). Do NOT change this signature to accept an arbitrary
  // id unless you intentionally want callers to pass different player ids.
  // If you do need to query other player ids, use the explicit helper
  // `getPlayerStateById(playerId, out)` below.
  static bool getPlayerState(PlayerState& out) {
    String url = String(apiHost) + "/players/" + String(playerName);
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<2048> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err) {
        out.state = doc.containsKey("state") && !doc["state"].isNull() ? doc["state"].as<String>() : String();
        out.volume = doc.containsKey("volume") ? doc["volume"].as<int>() : 0;
        out.muted = doc.containsKey("muted") ? doc["muted"].as<bool>() : false;
        out.title = doc.containsKey("title") && !doc["title"].isNull() ? doc["title"].as<String>() : String();
        out.artist = doc.containsKey("artist") && !doc["artist"].isNull() ? doc["artist"].as<String>() : String();
        out.album = doc.containsKey("album") && !doc["album"].isNull() ? doc["album"].as<String>() : String();
        out.pictureUrl = doc.containsKey("picture-url") && !doc["picture-url"].isNull() ? doc["picture-url"].as<String>() : String();
        http.end();
        return true;
      }
    }
    http.end();
    return false;
  }

  // Fetch current player state for an explicit player id. This is a
  // non-breaking convenience wrapper for multi-player scenarios — it is
  // intentionally separate from `getPlayerState` so the default behavior
  // continues to use the `playerName` from `secrets.h`.
  static bool getPlayerStateById(const String& playerId, PlayerState& out) {
    String url = String(apiHost) + "/players/" + playerId;
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<2048> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err) {
        out.state = doc.containsKey("state") && !doc["state"].isNull() ? doc["state"].as<String>() : String();
        out.volume = doc.containsKey("volume") ? doc["volume"].as<int>() : 0;
        out.muted = doc.containsKey("muted") ? doc["muted"].as<bool>() : false;
        out.title = doc.containsKey("title") && !doc["title"].isNull() ? doc["title"].as<String>() : String();
        out.artist = doc.containsKey("artist") && !doc["artist"].isNull() ? doc["artist"].as<String>() : String();
        out.album = doc.containsKey("album") && !doc["album"].isNull() ? doc["album"].as<String>() : String();
        out.pictureUrl = doc.containsKey("picture-url") && !doc["picture-url"].isNull() ? doc["picture-url"].as<String>() : String();
        http.end();
        return true;
      }
    }
    http.end();
    return false;
  }

  static bool postPlayPause() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/play-pause";
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.POST(""); // Empty body
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static String backLink;

  static bool fetchMenuItems(MenuItem* items, int& count, const String& maybePath) {
    String path = maybePath.isEmpty() ? "/" : maybePath;
    String url = String(apiHost) + path;
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<16384> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err) {
        count = 0;
        // Only parse object with 'items' array
        if (doc.containsKey("items")) {
          JsonArray arr = doc["items"].as<JsonArray>();
          for (JsonObject obj : arr) {
            if (count < 400) {
              items[count].id = obj["id"].as<String>();
              items[count].name = obj["name"].as<String>();
              items[count].link = obj["link"].as<String>();
              count++;
            }
          }
        }
        // Save back_link if present
        if (doc.containsKey("back_link")) {
          backLink = doc["back_link"].as<String>();
        } else {
          backLink = "";
        }
        http.end();
        return true;
      }
    }
    http.end();
    return false;
  }

  static bool postPlayMedia(const String& link) {
    String url = String(apiHost) + "/players/" + String(playerName) + "/play";
    HTTPClient http;
    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/json");
    String body = String("{\"link\":\"") + link + "\"}";
    int httpCode = http.POST(body);
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postAlbumPlay(const String& albumId) {
    String url = String(apiHost) + "/players/" + String(playerName) + "/play-album";
    HTTPClient http;
    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/json");
    String body = String("{\"album_id\":\"") + albumId + "\"}";
    int httpCode = http.POST(body);
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postPlayerStop() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/stop";
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.POST(""); // Empty body
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postVolumeUp() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/volume-up";
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postVolumeDown() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/volume-down";
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }
};

