#include "header.h"


AsyncWebServer server(80);

// Get Relay based on deviceId
int getRelay(String deviceId) {
  if (deviceId == "R1D1") {
    return RELAY_1;
  } else if (deviceId == "R1D2") {
    return RELAY_2;
  } else if (deviceId == "R1D3") {
    return RELAY_3;
  } else if (deviceId == "R1D4") {
    return RELAY_4;
  }
  return -1;
}

// Function for changing device state based on "deviceId" and "state"
bool switchDeviceState(String deviceId, bool state) {
  int relay = getRelay(deviceId);
  if (relay == -1){
    return false;
  }
  
  if (state) {
    digitalWrite(relay, TURN_ON);
    Serial.print("Turned on device ");
  } else {
    digitalWrite(relay, TURN_OFF);
    Serial.print("Turned off device ");
  }
  Serial.println(deviceId);
  return true;
}

// Initiaze Pins/devices
String initializePins() {
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  // Getting devices.json
  DynamicJsonDocument devicesDoc(1024);
  const String filepath = "/devices.json";
  getJsonFile(filepath, devicesDoc);
  JsonObject jObj = devicesDoc.as<JsonObject>();

  // Looping over devices data from file
  for(JsonPair p: jObj) {
    String key = p.key().c_str();
    switchDeviceState(key, devicesDoc[key]["state"]);
  }
  return "Initialized Pins.";
}

void setup() {
  SPIFFS.begin();

  // Initiaze Pins/devices
  initializePins();

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

  // Handing POST requests to get sockets' states
  server.addHandler(new AsyncCallbackJsonWebHandler("/getSwitchStates", [](AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject &jsonObj = json.as<JsonObject>();
    JsonArray deviceIds = jsonObj["deviceIds"];
    const int deviceIdArrSize = deviceIds.size();
    bool success = true;
    String res = "{\"";

    // Looping through deviceIds and fetching their states
    for (int i=0; i < deviceIdArrSize; i++) {
      int state = digitalRead(getRelay(deviceIds[i]));
      res += deviceIds[i] + "\":"+ state;
      res += i < deviceIdArrSize - 1 ? ",\"" : "}";
      if (state == -1) {
        success = false;
      }
    }

    Serial.print("Current device states: ");
    Serial.println(res);

    if (success) {
      request->send(200, "OK", res);
    } else {
      request->send(500, "Something went wrong.", res);
    }
  }));

  // Handing POST requests to turn on/off sockets
  server.addHandler(new AsyncCallbackJsonWebHandler("/switch", [](AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject jsonObj = json.as<JsonObject>();
    const String deviceId = jsonObj["deviceId"];
    const bool state = jsonObj["state"];
    bool success = false;

    // Getting devices.json
    DynamicJsonDocument devicesDoc(1024);
    const String filepath = "/devices.json";
    getJsonFile(filepath, devicesDoc);

    // Checking whether device is locked
    if (!devicesDoc[deviceId]["locked"]) {
      success = switchDeviceState(deviceId, state);
    } else if (devicesDoc["password"] == jsonObj["password"]) {
      request->send(401, "UNAUTHORIZED");
    } else {
      request->send(403, "FORBIDDEN");
    }
    
    // Sending response
    if (success) {
      request->send(200, "OK");
    } else {
      request->send(200, "FAIL");
    }
    
    // Updating devices.json
    devicesDoc[deviceId]["state"] = state;
    Serial.println(updateJsonFile(filepath, devicesDoc));
  }));

  // Serving static contents - HTML/CSS/JS files
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