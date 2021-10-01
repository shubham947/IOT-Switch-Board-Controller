#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Credentials
const char *SSID = "SSID";
const char *WIFI_PASSWORD = "Password";
const char *OTA_USERNAME = "admin";
const char *OTA_PASSWORD = "OTAPASSWORD";

// Connect an Relay to each GPIO of your ESP8266
const int relay1 = 5;
const int relay2 = 4;
const int relay3 = 14;
const int relay4 = 12;

AsyncWebServer server(80);

void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

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

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(SPIFFS, "/index.html", "text/html", false);
  // });

  // server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(SPIFFS, "/style.css", "text/css", false);
  // });

  AsyncElegantOTA.begin(&server, OTA_USERNAME, OTA_PASSWORD);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  // put your main code here, to run repeatedly:
}