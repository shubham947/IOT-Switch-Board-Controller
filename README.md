# IOT Switch Board Controller
Program for controlling Switch board using **NodeMCU** via Home network **(WiFi)**, with **PlatformIO**.
Navigate to `http://<NodeMCU_IP>` for dashboard/homepage.

## Features
- [x] Connect to WiFi Access point
- [x] OTA update
- [x] Working dashboard page
- [x] Get current states of devices on page load
- [x] Saving pin states in filesystem, to retain pin states over restart
- [ ] Switching to AcessPoint/Hotspot mode, to change SSID & Password of WiFi, when not able to connect to WiFi
- [ ] Password protecting device state
- [ ] Settings page

## Connecting to WiFi
Put your **SSID** and **Password** in `src/wifi_credentials.h` file

## OTA update [^1]
Navigate to `http://<NodeMCU_IP>/update`

### Securing your OTA update
Put your **Username** and **Password** in `src/ota_credentials.h` file


## References
[^1]: ElegantOTA: https://github.com/ayushsharma82/ElegantOTA
