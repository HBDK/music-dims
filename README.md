# Music DIMS ESP32 Project

This project runs on an ESP32 and drives a BTT Mini12864 v2.0 display with rotary encoder input, WiFi connectivity, and a dynamic menu system for browsing artists and albums from an API server.

## Features
- Dynamic menu for artists and albums
- Rotary encoder navigation
- Back button for menu navigation
- WiFi connection and API data fetching
- Album detail page
- Modular API service

## Hardware
- ESP32 microcontroller
- BTT Mini12864 v2.0 display (ST7567 controller)
- Rotary encoder
- Back button

## Usage
1. Clone the repo and open in VS Code with PlatformIO.
2. Edit `src/secrets.h` with your WiFi, API host, and player name info.
3. Wire up hardware as described above.
4. Build and upload firmware:
   ```
   pio run --target upload
   ```
5. Use the encoder and buttons to navigate artists and albums.

---

# Project Secrets

This project uses a `secrets.h` file to store sensitive configuration such as WiFi credentials and API URLs. This file is **not** tracked in version control and should be created manually by each developer.

## Usage
1. Create a file named `secrets.h` in the `src/` directory.
2. Add your WiFi credentials and API host, for example:
   ```cpp
   // src/secrets.h
   #pragma once
   const char* ssid = "your_wifi";
   const char* password = "your_password";
   const char* apiHost = "http://your_host:5000";
   const char* playerName = "bathroom_speaker"; // Used for player control endpoints
   ```
3. Make sure `secrets.h` is listed in `.gitignore` so it is not committed to git.
4. In your code, include it:
   ```cpp
   #include "secrets.h"
   ```

**Never commit your secrets.h file!**
