#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include "menu_item.h"

class ApiService {
public:
  static String backLink;
  static bool fetchMenuItems(MenuItem* items, int& count, const String& path) {
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

  static bool fetchArtists(MenuItem* items, int& count) {
    return fetchMenuItems(items, count, "/artists");
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

// Define static member
String ApiService::backLink = "";
