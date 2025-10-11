#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "menu_item.h"

class ApiService {
public:
  static bool fetchMenuItems(MenuItem* items, int& count, const String& url) {
    if (WiFi.status() != WL_CONNECTED) return false;
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<16384> doc;
      DeserializationError err = deserializeJson(doc, payload);
      if (!err) {
        JsonArray arr = doc.as<JsonArray>();
        count = 0;
        for (JsonObject obj : arr) {
          if (count < 400) {
            items[count].id = obj["id"].as<String>();
            items[count].name = obj["name"].as<String>();
            count++;
          }
        }
        http.end();
        return true;
      }
    }
    http.end();
    return false;
  }

  static bool fetchArtists(MenuItem* items, int& count) {
    String url = String(apiHost) + "/artists";
    return fetchMenuItems(items, count, url);
  }

  static bool fetchAlbums(MenuItem* items, int& count, String artistId) {
    String url = String(apiHost) + "/artists/" + artistId + "/albums";
    return fetchMenuItems(items, count, url);
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
};
