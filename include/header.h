#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h> 
#include <AsyncJson.h>

#include "fileController.h"

// Credentials
#include "wifi_credentials.h"
#include "ota_credentials.h"

#define TURN_ON HIGH
#define TURN_OFF LOW

// Connect an Relay to each GPIO of your ESP8266
#define RELAY_1 5
#define RELAY_2 4
#define RELAY_3 14
#define RELAY_4 12
