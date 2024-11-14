#include "../include/fall_and_temperature_sensor.h"
#include "../include/bma250.h"
#include "../include/display.h"
#include "../include/serial.h"
#include <math.h>

/**
 * @brief Checks for fall detection and temperature status.
 *
 * This function reads the acceleration values from the BMA250 sensor and calculates the magnitude.
 * It then compares the magnitude to predefined thresholds to determine if a fall has occurred.
 * If a fall is detected, the function sets the activity status to the appropriate message and initiates the fall display.
 *
 */
void checkFallDetectionAndTemperature()
{
    // Read accelerometer data
    accel_sensor.read();
    double x = accel_sensor.X;
    double y = accel_sensor.Y;
    double z = accel_sensor.Z;
    temp = ((accel_sensor.rawTemp * 0.5) + 24.0);

    // Check for sensor error
    if (x == -1 && y == -1 && z == -1)
    {
        SerialMonitorInterface.println("ERROR! NO BMA250 DETECTED!");
        return;
    }

    // Time calculations for jerk
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();
    double deltaTime = (currentTime - lastTime) / 1000.0; // Convert ms to seconds
    if (deltaTime == 0)
    {
        return;
    }
    lastTime = currentTime;

    // Previous acceleration values
    static double prevX = x, prevY = y, prevZ = z;

    // Calculate acceleration magnitude
    double magnitude = sqrt(x * x + y * y + z * z);

    // Calculate jerk (rate of change of acceleration)
    double jerkX = (x - prevX) / deltaTime;
    double jerkY = (y - prevY) / deltaTime;
    double jerkZ = (z - prevZ) / deltaTime;
    double totalJerk = sqrt(jerkX * jerkX + jerkY * jerkY + jerkZ * jerkZ);

    // Update previous acceleration values
    prevX = x;
    prevY = y;
    prevZ = z;

    // Define thresholds with consideration of rest magnitude (~33)
    const double REST_MAGNITUDE = 33.0;                                  // Average magnitude at rest
    const double FREE_FALL_DELTA = 10.0;                                 // Reduced drop from rest for sensitive detection
    const double FREE_FALL_THRESHOLD = REST_MAGNITUDE - FREE_FALL_DELTA; // 23.0
    const double IMPACT_THRESHOLD_MINOR = 30.0;                          // Increased for minor impacts
    const double IMPACT_THRESHOLD_MODERATE = 150.0;                      // Increased for moderate impacts
    const double IMPACT_THRESHOLD_SEVERE = 400.0;                        // Increased for severe impacts
    const double JERK_THRESHOLD = 70.0;

    SerialMonitorInterface.print("Magnitude: ");
    SerialMonitorInterface.println(magnitude);
    SerialMonitorInterface.print("Total jerk: ");
    SerialMonitorInterface.println(totalJerk);
    SerialMonitorInterface.print("X: ");
    SerialMonitorInterface.print(x);
    SerialMonitorInterface.print(", Y: ");
    SerialMonitorInterface.print(y);
    SerialMonitorInterface.print(", Z: ");
    SerialMonitorInterface.println(z);

    // Fall detection logic
    static bool freeFallDetected = false;
    if (magnitude < FREE_FALL_THRESHOLD)
    {
        freeFallDetected = true;
    }
    else if (freeFallDetected && magnitude > IMPACT_THRESHOLD_MINOR && totalJerk > JERK_THRESHOLD)
    {
        if (magnitude <= IMPACT_THRESHOLD_MODERATE)
        {
            activityStatus = "MINOR FALL DETECTED!";
            SerialMonitorInterface.println("Minor fall detected!");
            sendFallStatus("MINOR FALL DETECTED\n");
        }
        else if (magnitude <= IMPACT_THRESHOLD_SEVERE)
        {
            activityStatus = "MODERATE FALL DETECTED!";
            SerialMonitorInterface.println("Moderate fall detected!");
            sendFallStatus("MODERATE FALL DETECTED\n");
        }
        else
        {
            activityStatus = "SEVERE FALL DETECTED!";
            SerialMonitorInterface.println("Severe fall detected!");
            sendFallStatus("SEVERE FALL DETECTED\n");
        }
        initiateFallDisplay();
        freeFallDetected = false;
    }
    else
    {
        freeFallDetected = false;
    }

    // Update activity status based on magnitude
    if (!fallDetectedFlag)
    {
        // Calculate the change in magnitude
        static double prevMagnitude = magnitude;
        double deltaMagnitude = fabs(magnitude - prevMagnitude);
        prevMagnitude = magnitude;

        SerialMonitorInterface.print("Delta Magnitude: ");
        SerialMonitorInterface.println(deltaMagnitude);

        // Define thresholds for activity detection based on delta magnitude
        const double RESTING_MAX_DELTA = 3.0;
        const double WALKING_MAX_DELTA = 25.0;

        if (deltaMagnitude < RESTING_MAX_DELTA)
        {
            activityStatus = "RESTING";
        }
        else if (deltaMagnitude < WALKING_MAX_DELTA)
        {
            activityStatus = "WALKING";
        }
        else
        {
            activityStatus = "RUNNING";
        }

        // Show serial debug information
        showSerial();
    }
}

/**
 * @brief Initiates the fall display by setting flags and timestamps.
 *
 */
void initiateFallDisplay()
{
    if (!fallDetectedFlag)
    {
        fallDetectedFlag = true;
        fallDetectedTime = millis();
    }
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
