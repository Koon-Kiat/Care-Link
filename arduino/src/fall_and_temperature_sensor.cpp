/*
// src/SensorHandler.cpp
#include "../include/fall_and_temperature_sensor.h"
#include <math.h>
#include "../include/bma250.h"
#include "../include/display.h"
#include "../include/serial.h"

/**
 * @brief Checks for fall detection and temperature.
 *
 * This function reads the acceleration values from the BMA250 sensor and calculates the magnitude.
 * It then categorizes the activity status based on the magnitude and checks for fall detection.
 * If a fall is detected, it sends the fall status to the connected device and displays the activity status and temperature.
 *
 
void checkFallDetectionAndTemperature()
{
    accel_sensor.read(); // Update acceleration values
    x = accel_sensor.X;
    y = accel_sensor.Y;
    z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0); // Convert raw temperature to Celsius

    // Define fall thresholds for categorization
    const int16_t FALL_MINOR_THRESHOLD_LOW = -400;
    const int16_t FALL_MODERATE_THRESHOLD_LOW = -1000;
    const int16_t FALL_SEVERE_THRESHOLD = -1500;

    const int16_t FALL_MINOR_THRESHOLD_HIGH = 400;     // Positive threshold for minor rise
    const int16_t FALL_MODERATE_THRESHOLD_HIGH = 1000; // Positive threshold for moderate rise
    const int16_t FALL_SEVERE_THRESHOLD_HIGH = 1500;   // Positive threshold for severe rise

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100)
    {           // Check every 100ms
        return; // Skip this loop if within the sampling period
    }
    lastCheckTime = currentTime; // Update the last check time

    // Define thresholds for activity detection
    const double RESTING_THRESHOLD = 248.0; // Adjusted based on observed resting magnitude
    const double WALKING_THRESHOLD = 260.0; // Adjusted based on testing
    const double RUNNING_THRESHOLD = 300.0; // Adjusted based on testing

    // Calculate magnitude
    double magnitude = sqrt(x * x + y * y + z * z);
    String activityStatus;

    if (magnitude < RESTING_THRESHOLD)
    {
        activityStatus = "RESTING"; // Indicates rest
    }
    else if (magnitude >= RESTING_THRESHOLD && magnitude < WALKING_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else if (magnitude >= WALKING_THRESHOLD)
    {
        activityStatus = "RUNNING";
    }

    // Check thresholds for fall detection
    if (z <= FALL_SEVERE_THRESHOLD || z >= FALL_SEVERE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Severe fall detected!");
        sendFallStatus("SEVERE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MODERATE_THRESHOLD_LOW || z >= FALL_MODERATE_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Moderate fall detected!");
        sendFallStatus("MODERATE FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else if (z <= FALL_MINOR_THRESHOLD_LOW || z >= FALL_MINOR_THRESHOLD_HIGH)
    {
        SerialMonitorInterface.println("Minor fall detected!");
        sendFallStatus("MINOR FALL DETECTED\n");
        displayActivityStatus("FALL DETECTED!", temp);
        delay(10000); // Display for 10 seconds
    }
    else
    {
        sendFallStatus("SAFE\n");
    }

    // Display activity status and temperature on TinyScreen
    displayActivityStatus(activityStatus.c_str(), temp);
    sendTemperatureStatus(temp);

    if (x == -1 && y == -1 && z == -1)
    {
        // Print error message to Serial Monitor
        SerialMonitorInterface.print("ERROR! NO BMA250 DETECTED!");
    }
    else
    {
        showSerial();
    }

    delay(1000); // Delay for stability
}

*/

#include "../include/fall_and_temperature_sensor.h"
#include <ArduinoJson.h>        // Include ArduinoJson for JSON formatting
#include <math.h>               // For calculations
#include "../include/bma250.h"  // Include BMA250 accelerometer module
#include "../include/display.h" // Include display module
#include "../include/serial.h"  // Include serial module
#include "../include/WiFiModule.h" // Include WiFi module for server communication
#include "../include/WiFiConfig.h" // Include the WiFi and server configuration
#include "../include/serial.h" // Include the header to use the global temp

// Define global variables for sensor data
String activityStatus = "UNKNOWN"; // Stores the current activity status
String fallStatus = "SAFE";        // Stores the current fall status

void checkFallDetectionAndTemperature()
{
    // Read accelerometer data
    accel_sensor.read();
    int x = accel_sensor.X;
    int y = accel_sensor.Y;
    int z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0); // Convert raw temperature to Celsius

    // Define fall thresholds
    const int16_t FALL_MINOR_THRESHOLD_LOW = -400;
    const int16_t FALL_MODERATE_THRESHOLD_LOW = -1000;
    const int16_t FALL_SEVERE_THRESHOLD = -1500;

    const int16_t FALL_MINOR_THRESHOLD_HIGH = 400;
    const int16_t FALL_MODERATE_THRESHOLD_HIGH = 1000;
    const int16_t FALL_SEVERE_THRESHOLD_HIGH = 1500;

    // Define activity thresholds
    const double RESTING_THRESHOLD = 248.0;
    const double WALKING_THRESHOLD = 260.0;
    const double RUNNING_THRESHOLD = 300.0;

    // Introduce a short delay between readings for better sampling
    static unsigned long lastCheckTime = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime < 100) return;
    lastCheckTime = currentTime;

    // Calculate magnitude for activity classification
    double magnitude = sqrt(x * x + y * y + z * z);

    // Classify activity status
    if (magnitude < RESTING_THRESHOLD)
    {
        activityStatus = "RESTING";
    }
    else if (magnitude >= RESTING_THRESHOLD && magnitude < WALKING_THRESHOLD)
    {
        activityStatus = "WALKING";
    }
    else if (magnitude >= WALKING_THRESHOLD)
    {
        activityStatus = "RUNNING";
    }

    // Check for fall detection and severity
    fallStatus = "SAFE"; // Default fall status
    if (z <= FALL_SEVERE_THRESHOLD || z >= FALL_SEVERE_THRESHOLD_HIGH)
    {
        fallStatus = "SEVERE FALL DETECTED";
        SerialMonitorInterface.println("Severe fall detected!");
        displayActivityStatus("SEVERE FALL", temp);
        delay(5000); // Display for 5 seconds
    }
    else if (z <= FALL_MODERATE_THRESHOLD_LOW || z >= FALL_MODERATE_THRESHOLD_HIGH)
    {
        fallStatus = "MODERATE FALL DETECTED";
        SerialMonitorInterface.println("Moderate fall detected!");
        displayActivityStatus("MODERATE FALL", temp);
        delay(3000); // Display for 3 seconds
    }
    else if (z <= FALL_MINOR_THRESHOLD_LOW || z >= FALL_MINOR_THRESHOLD_HIGH)
    {
        fallStatus = "MINOR FALL DETECTED";
        SerialMonitorInterface.println("Minor fall detected!");
        displayActivityStatus("MINOR FALL", temp);
        delay(2000); // Display for 2 seconds
    }

    // Create JSON object to send data
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["fall_status"] = fallStatus.c_str();        // Add fall status
    jsonDoc["activity_status"] = activityStatus.c_str(); // Add activity status
    jsonDoc["temperature"] = temp;                     // Add temperature

    // Serialize JSON to a string
    String jsonData;
    serializeJson(jsonDoc, jsonData);

    // Send JSON data to the server
    sendSensorData(SERVER_ADDRESS, SERVER_PORT, jsonData);

    // Display activity status and temperature
    displayActivityStatus(activityStatus.c_str(), temp);
    sendTemperatureStatus(temp);

    // Check for sensor errors
    if (x == -1 && y == -1 && z == -1)
    {
        SerialMonitorInterface.println("ERROR! NO BMA250 DETECTED!");
    }
    else
    {
        showSerial(); // Output sensor values to Serial Monitor
    }

    delay(1000); // Stability delay
}

String getFallStatus()
{
    return fallStatus;
}

String getActivityStatus()
{
    return activityStatus;
}

float getTemperature()
{
    return temp;
}
