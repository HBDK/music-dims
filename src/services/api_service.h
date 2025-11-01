// Moved to services folder
#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "models/menu_item.h"
#include <models/player_state.h>

class ApiService {
public:


  static WiFiClientSecure& tlsClient() {
    static WiFiClientSecure client;
    static bool inited = false;
    if (!inited) {
      inited = true;
      client.setInsecure();
    }
    return client;
  }

  static HTTPClient& sharedHttp() {
    static HTTPClient http;
    static bool inited = false;
    if (!inited) {
      inited = true;
#if defined(HTTP_TCP_CLIENT_H) || defined(HTTP_CLIENT_IMPL_H)
      // http.setReuse(true);
#endif
      http.setTimeout(5000);
    }
    return http;
  }

  static bool beginRequest(HTTPClient& http, const String& url) {
    if (url.startsWith("https://")) {
      WiFiClientSecure& client = tlsClient();
      http.begin(client, url.c_str());
    } else {
      http.begin(url.c_str());
    }
    return true;
  }

  static bool getPlayerState(PlayerState& out) {
    String url = String(apiHost) + "/players/" + String(playerName);
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
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

  static bool getPlayerStateById(const String& playerId, PlayerState& out) {
    String url = String(apiHost) + "/players/" + playerId;
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
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
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static String backLink;

  static bool fetchMenuItems(MenuItem* items, int& count, const String& maybePath) {
    String path = maybePath.isEmpty() ? "/" : maybePath;
    String url = String(apiHost) + path;
    if (WiFi.status() != WL_CONNECTED) return false;
    static unsigned long lastMenuFetch = 0;
    static String lastMenuPath = "";
    const unsigned long MENU_CACHE_MS = 2000;
    unsigned long now = millis();
    if (url == lastMenuPath && (now - lastMenuFetch) < MENU_CACHE_MS) {
      return false;
    }

    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<16384> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err) {
        count = 0;
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
        if (doc.containsKey("back_link")) {
          backLink = doc["back_link"].as<String>();
        } else {
          backLink = "";
        }
        http.end();
        lastMenuFetch = now;
        lastMenuPath = url;
        return true;
      }
    }
    http.end();
    return false;
  }

  static bool postPlayMedia(const String& link) {
    String url = String(apiHost) + "/players/" + String(playerName) + "/play";
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    http.addHeader("Content-Type", "application/json");
    String body = String("{\"link\":\"") + link + "\"}";
    int httpCode = http.POST(body);
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postAlbumPlay(const String& albumId) {
    String url = String(apiHost) + "/players/" + String(playerName) + "/play-album";
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    http.addHeader("Content-Type", "application/json");
    String body = String("{\"album_id\":\"") + albumId + "\"}";
    int httpCode = http.POST(body);
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postPlayerStop() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/stop";
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postVolumeUp() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/volume-up";
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }

  static bool postVolumeDown() {
    String url = String(apiHost) + "/players/" + String(playerName) + "/volume-down";
    HTTPClient& http = sharedHttp();
    beginRequest(http, url);
    int httpCode = http.POST("");
    http.end();
    return httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT;
  }
};
