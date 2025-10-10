#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

struct MenuItem {
  int id;
  String name;
};

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
            items[count].id = obj["id"].as<int>();
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

  static bool fetchAlbums(MenuItem* items, int& count, int artistId) {
    String url = String(apiHost) + "/artists/" + String(artistId) + "/albums";
    return fetchMenuItems(items, count, url);
  }
};
