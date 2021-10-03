#include <ArduinoJson.h>
#include <FS.h>

// Get Json file from file system using filepath
DynamicJsonDocument getJsonFile(String filepath, DynamicJsonDocument& doc) {
    if (SPIFFS.exists(filepath)) {
        File jsonFile = SPIFFS.open(filepath, "r");
        if (jsonFile) {
            deserializeJson(doc, jsonFile);
            jsonFile.close();
            // Logging file data
            // Serial.print("File data: ");
            // serializeJson(doc, Serial);
            // Serial.println("");
            return doc;
        }
        Serial.print("Could not read ");
        Serial.println(filepath);
        return doc;
    }
    Serial.print(filepath);
    Serial.println(" file doesn't exist.");
    return doc;
}

String updateJsonFile(String filepath, DynamicJsonDocument& doc) {
    File jsonFile = SPIFFS.open(filepath, "w");
    serializeJson(doc, jsonFile);
    jsonFile.close();
    // Logging data to be written to file
    // Serial.print("Data to file: ");
    // serializeJson(doc, Serial);
    // Serial.println("");
    return "Updated " + filepath;
}
