# Music DIMS ESP32 Project

This project consists of an ESP32 firmware and a .NET Web API backend that together create a music browsing and control system. The ESP32 drives a TFT display with rotary encoder input, connects via WiFi to the backend API, and provides a dynamic menu system for browsing music libraries and controlling Home Assistant media players.

## Architecture

### ESP32 Firmware
- Dynamic hierarchical menu system for music browsing  
- Rotary encoder navigation with press/long-press functionality
- WiFi connectivity and REST API integration
- Media player control (play/pause, stop, volume, album playback)
- RGB LED lighting control (button LEDs and backlight)
- Modular screen system with TFT display support

### .NET Web API Backend
- ASP.NET Core minimal API with Home Assistant integration
- RESTful endpoints for music library browsing (artists, albums, radio channels)
- Media player control endpoints (play/pause, stop, volume control)
- Docker containerized deployment with Traefik reverse proxy
- Encrypted secrets management using SOPS
- Swagger API documentation

## Hardware
- ESP32 microcontroller
- TFT display (configured via TFT_eSPI library) 
- Rotary encoder with push button
- RGB NeoPixel LEDs (3 pixels: 2 for buttons, 1 for backlight)

## Project Structure
```
├── src/                    # ESP32 Arduino firmware source code
├── backend/MusicDimsApi/   # .NET 10 Web API backend
├── docker-compose.yml      # Production deployment configuration  
├── justfile               # Automation commands for deployment
└── swagger.json           # OpenAPI specification
```

## Setup

### ESP32 Firmware
1. Open project in VS Code with PlatformIO extension
2. Create `src/secrets.h` with your WiFi and API configuration:
   ```cpp
   #pragma once
   const char* ssid = "your_wifi_network";
   const char* password = "your_wifi_password";
   const char* apiHost = "http://your_api_host:5000";
   const char* playerName = "your_home_assistant_media_player";
   ```
3. Build and upload to ESP32:
   ```bash
   pio run --target upload
   ```

### Backend API
1. Set up Home Assistant integration secrets in `backend/MusicDimsApi/Api/appsettings.secrets.json`
2. Deploy using Docker Compose:
   ```bash
   just deploy  # Handles decryption and deployment
   ```
3. API will be available at configured host with Swagger UI at `/swagger`

## Usage
- **Encoder Rotation**: Navigate up/down through menu items
- **Short Press**: Select item or enter submenu  
- **Long Press**: Stop playback and return to parent menu
- **RGB LEDs**: Provide visual feedback for navigation and status

---

## Configuration

### ESP32 Secrets
Create a `src/secrets.h` file with your WiFi and API configuration. This file is not tracked in version control.

**Example `src/secrets.h`:**
```cpp
#pragma once
const char* ssid = "your_wifi_network";
const char* password = "your_wifi_password"; 
const char* apiHost = "http://your_api_host:5000";
const char* playerName = "your_home_assistant_media_player";
```

### Backend Secrets
The backend API uses encrypted configuration managed with SOPS. See `justfile` for encryption/decryption commands.

**Never commit unencrypted secrets!**
