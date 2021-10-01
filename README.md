# IOT Switch Board Controller
Program for controlling Switch board using **NodeMCU** via Home network **(WiFi)**, with **PlatformIO**.
Navigate to `http://<NodeMCU_IP>` for dashboard/homepage.

## Features
- [x] Connect to WiFi Access point
- [x] OTA update
- [ ] Working dashboard page
- [ ] Saving pin states in filesystem
- [ ] Settings page

## Connecting to WiFi
Put your **SSID** and **Password** in `src/wifi_credentials.h` file
./src/wifi_credentials.h

## OTA update [^1]
Navigate to `http://<NodeMCU_IP>/update`

### Securing your OTA update
Put your **Username** and **Password** in `src/ota_credentials.h` file


## References
[^1]: ElegantOTA: https://github.com/ayushsharma82/ElegantOTA
