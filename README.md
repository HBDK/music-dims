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
   ```
   In your code, build endpoint URLs like this:
   ```cpp
   String apiArtistsUrl = String(apiHost) + "/artists";
   ```
3. Make sure `secrets.h` is listed in `.gitignore` so it is not committed to git.
4. In your code, include it:
   ```cpp
   #include "secrets.h"
   ```

## Why?
- Keeps secrets out of version control
- Easy to change per developer or environment
- No extra build steps required

**Never commit your secrets.h file!**
