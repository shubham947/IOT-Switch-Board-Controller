#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <ArduinoJson.h> 

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

AsyncWebServer server(80);

void setup()
{
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  SPIFFS.begin();

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  // Handing POST requests to turn on/off sockets
  // server.addHandler(new AsyncCallbackJsonWebHandler("/switch", [](AsyncWebServerRequest *request, JsonVariant &json) {
  //   const JsonObject &jsonObj = json.as<JsonObject>();
  //   if (jsonObj["on"]) {
  //     Serial.println("Turn on");
  //     digitalWrite(RELAY_1, TURN_ON);
  //   } else {
  //     Serial.println("Turn off");
  //     digitalWrite(RELAY_1, TURN_OFF);
  //   }
  //   request->send(200, "OK");
  // }));

  // Serving static content: HTML/CSS/JS files
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Handling not found addresses
  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      Serial.println("Not found - " + request->url());
      request->send(404, "Not found");
    }
  });
  
  // Starting Server
  AsyncElegantOTA.begin(&server, OTA_USERNAME, OTA_PASSWORD);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // put your main code here, to run repeatedly:
}