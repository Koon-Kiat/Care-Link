#include <Arduino.h> // Include Arduino definitions
#undef min           // Prevent macro conflicts
#undef max
#include "../include/jsonHandler.h"
#include <ArduinoJson.h>

String createJsonPayload(const String &fallStatus, const String &activityStatus, float temperature) {
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["fall_status"] = fallStatus.c_str();
    jsonDoc["activity_status"] = activityStatus.c_str();
    jsonDoc["temperature"] = temperature;

    // Serialize JSON to a string
    String sensorData;
    serializeJson(jsonDoc, sensorData);

    return sensorData; // Return the JSON string
}
